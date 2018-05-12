// IRPmodel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "IRP.h"
#include "VRPmodel.h"
#include <boost/tuple/tuple.hpp>
#include "gnuplot-iostream.h"
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <iomanip>
#include "DataSets.h"
#include "NodeInstanceDB.h"
#include "Calculations.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream
//#include <boost/tuple/tuple.hpp>

//#include "gnuplot-iostream.h"



using namespace DataSets;
using namespace std;
//Solution struct to hold solutions

//Forward decleration
void Heurestic(const NodeInstanceDB &db);

int main()
{
	int nCustomers = 6;
	int nPeriods = 5;
	for (int version = 1; version <= 6; version++) {

	//	const NodeInstanceDB *db = NodeInstanceDB::createInstance(nCustomers, nPeriods, version);
		const NodeInstanceDB *db = NodeInstanceDB::openInstance(nCustomers, nPeriods, version);

		//Run heurestic
		Heurestic(*db);

		//Run exact model model
		IRP model(*db, false);
		model.useIPSubtourElimination();
		//model.addValidIneq(ModelParameters::MinimumNodeVisit);
	//	model.addValidIneq(ModelParameters::MinimumInventory);
		//model.addValidIneq(ModelParameters::MinimumFlow);
		Solution *solLP = model.solveLPModel();
		Solution *sol = model.solveModel();
		sol->plot("Solution/exact");
		sol->printSolution();
		double lpObj = solLP->getObjective();
		double lpTrans = solLP->getTransportationCost();
		double lpHold = solLP->getHoldingCost();
		model.printSolutionToFile(lpObj, lpTrans, lpHold, version);
	}
	return 0;
}


/*The construction heurestic solves an IRP with relaxed arc variables(x_ijt).
The solution provides information about which customers are visited in each period
and how much that are picked up/delivered at each customer.
For each time period a VRP is solved exactly for the visited nodes. */


void Heurestic(const NodeInstanceDB &db) {

	bool relaxArcVariables = true;

	//Initialize the model with relaxed arc variables
	IRP modelIRP(db, relaxArcVariables);
	//Use subtour elimination in relaxed IRP
	modelIRP.useIPSubtourElimination();
	vector<Solution *> solutionHolder;
	vector<Solution*> constructionSolHolder;
	//Allocate solution
	Solution solIRP(db);
	Solution solVRP(db);
	Solution prevSol(db);
	Solution tempSol(db);
	Solution bestSol(db);
	time_t tic;
	int counter;


	bool add;
	vector<Route*> intensificationRoutes;
	vector<Route*> tempRoutes;

	//Allocate tabu information
	double ** VisitMatrixConstruction;
	double ** changeMatrixVRP;
	double ** VisitMatrixBest;

	bool breakIntensification;

	//Diversification iteration
	for (int iteration = 1; iteration <= 10; iteration++) {

		//Solve the relaxed IRP model
		solIRP = *modelIRP.solveModel();
	
		//Allocate a vrp solution
		solVRP = solIRP;
		solIRP.printSolution();


		//Solve a VRP for each 		
		for (int t = 1; t <= modelIRP.getNumOfPeriods(); t++) {
			solVRP.solveVRP(t);
		}

		solVRP.printSolution();
		//Check feasibility
		int i = 0;
		while (!solVRP.isFeasible() && i <= 5) {
			solVRP.routeSearch();
			solVRP.printSolution();
			i++;
		}
	

		if (!solVRP.isFeasible()) {
			cout << "Error: Not able to construct feasible solution";
			exit(101);
		}
		
		solVRP.solveInventoryProblem();
		constructionSolHolder.push_back(new Solution(solVRP));

		bestSol = solVRP;

		//Save the routes found in the construction
		for (Route * r : bestSol.getAllRoutes()) {
			intensificationRoutes.push_back(new Route(*r));
		};

		//start clock to measure intensification time
		tic = time(NULL);
		counter = 1;
		tempSol = bestSol;
		//Start intensification search

		breakIntensification = false;

		do {

			//Shift quantity operator
			tempSol.printSolution();
			tempSol.shiftQuantityMIP();
			tempSol.printSolution();
			//Update best solution if improved
			if (tempSol.getObjective() < bestSol.getObjective()) {
				bestSol = tempSol;
			}
			
			//Save any new routes
			tempRoutes = tempSol.getAllRoutes();
			for (Route* r : tempRoutes) {
				add = true;
				for (Route * r2 : intensificationRoutes)
					if (r->isDuplicate(r2)) {
						add = false;
					}
				if (add)
					intensificationRoutes.push_back(new Route(*r));
			}

			cout << "Iteration: " << iteration << ",  counter: " << counter << "\n";
			//Route search operator which require at least one change	
			tempSol.routeSearch(ModelParameters::REQUIRE_CHANGE);

			//terminate intensification if tempSol already found in the intensification
			if(tempSol == bestSol)
				breakIntensification = true;


			//Update best solution if improved
			if (tempSol.getObjective() < bestSol.getObjective()) {
				bestSol = tempSol;
			}

			//Save any new routes
			tempRoutes = tempSol.getAllRoutes();
			for (Route* r : tempRoutes) {
				add = true;
				for (Route * r2 : intensificationRoutes)
					if (r->isDuplicate(r2)) {
						add = false;
					}
				if (add)
					intensificationRoutes.push_back(new Route(*r));
			}

			//terminate intensification if exisiting solution found in previous devirsification iteration
			for (Solution * r : solutionHolder) {
				if (tempSol == *r)
					breakIntensification = true;	
			}


			//Update iteration counter, check termination condition
			counter++;

		} while (counter <= 4 && tic-time(NULL) <= ModelParameters::INTENSIFICATION_MAX_TIME && !breakIntensification);
	
		
		//Solve a routing problem for the routes found in the intensification search
		RouteProblem routeProb(db, intensificationRoutes);
		routeProb.formulateRouteProblem(ModelParameters::HIGHEST_TOTALCOST);
		bestSol = *routeProb.solveProblem();


		//Clear routes
		for (Route * r : intensificationRoutes) {
			delete r;
		}
		intensificationRoutes.clear();

		solutionHolder.push_back(new Solution(bestSol));

		//Diversification steps

		//Update tabu information
		VisitMatrixConstruction = solVRP.getVisitedMatrix();
		VisitMatrixBest = bestSol.getVisitedMatrix();


		if (iteration >= 2) {
			changeMatrixVRP = Solution::getVisitDifference(solVRP, prevSol);
			modelIRP.updateTabuMatrix(changeMatrixVRP);
		}

		//Add holding cost constraint
		else {
			//modelIRP.addHoldingCostCtr(sol->getHoldingCost());
		}

		//Add visit constraint
		//modelIRP.addVisitConstraint(VisitMatrixConstruction, ModelParameters::ForceVisits);
		modelIRP.addRouteCtr(solVRP.getAllRoutes());
		//Store construction solution from present iteration
		prevSol = solVRP;

	}

	//Final step solve route problem
	//Routeholder
	vector <Route *> AllRoutes;
	for (Solution * s : solutionHolder)
		for (Route * r : s->getAllRoutes())
			AllRoutes.push_back(r);


	//Solve a routing problem
	RouteProblem routeProb(db, AllRoutes);
	routeProb.formulateRouteProblem(ModelParameters::HIGHEST_TOTALCOST);
	Solution * solFinal = routeProb.solveProblem();

	//Print solution
	solFinal->printSolution();


	//Diversification information

	vector<double> objectiveValues;
	vector<vector<double>> similarity;

	int i = 1;
	vector<vector<NodeIRP*>> graphHolder;
	for (auto solution : constructionSolHolder) {
		vector <NodeIRP*> graph;
		solution->buildGraph(graph);
		//solution->plot("Solution/ConSol" + to_string(i++));
		graphHolder.push_back(graph);
		objectiveValues.push_back(solution->getObjective());
	}


	similarity.resize(graphHolder.size());
	//Find similarity for all pair of solutions
	cout << "\n" << "Similarity Matrix between solution i and j\t\t\t\t\t\tObjective value solution i";
	for (int i = 0; i < graphHolder.size(); i++) {
		similarity[i].resize(graphHolder.size());
		cout << "\n";
		for (int j = 0; j < graphHolder.size(); j++) {
			double sim = graphAlgorithm::getSimiliarity(graphHolder[i], graphHolder[j]);
			similarity[i][j] = sim;
			cout <<setprecision(2)<< sim << "\t";
		}

		cout << setprecision(5)<< objectiveValues[i];

	}

	objectiveValues.clear();
	similarity.clear();
	graphHolder.clear();

	for (auto solution : solutionHolder) {
		vector <NodeIRP*> graph;
		//solution->plot("Solution/sol" + to_string(i++));
		solution->buildGraph(graph);
		graphHolder.push_back(graph);
		objectiveValues.push_back(solution->getObjective());
	}


	cout << "\n";
	similarity.resize(graphHolder.size());
	//Find similarity for all pair of solutions
	cout << "\n" << "Similarity Matrix between solution i and j\t\t\t\t\t\tObjective value solution i";
	for (int i = 0; i < graphHolder.size(); i++) {
		similarity[i].resize(graphHolder.size());
		cout << "\n";
		for (int j = 0; j < graphHolder.size(); j++) {
			double sim = graphAlgorithm::getSimiliarity(graphHolder[i], graphHolder[j]);
			similarity[i][j] = sim;
			cout << setprecision(2) << sim << "\t";
		}

		cout << setprecision(5) << objectiveValues[i];

	}

}//End heurestic



void solveVRP(vector<NodeIRP *> &nodes, int t, IRP & instance, Solution* sol) {
	//modelIRP.getVisitedCustomers(t, customers);




	//modelIRP.getDemand(t, demand, customers);
	if (nodes.size() >= 1) {
		//Initialize a VRP database with the demand and customers

	}
	
}



void analyzeSolutions() {

	const int size = 5;
	const int modelTypes = 3;

	//Holds IRP rel, construction and Exact for size instances. Ex 3 types * 5 instances


	int Customers[] = { 6, 3, 6 };
	int Periods[] = { 5, 3, 7 };

	vector<double>* ConstructionSolutions;
	vector<double>* RelaxedSolutions;
	vector<double>* ExactSol;
	vector <double> *** SolutionHolder;


	//Number of instance types
	ofstream outputFile;
	outputFile.open("ConstructionVsExact");

	for (int p = 0; p < 3; p++) {

		int nCustomers = Customers[p];
		int nPeriods = Periods[p];
		double nRoutesEx = 0;
		double nRoutesCon = 0;

		SolutionHolder = new vector<double> **[ModelList.size()];
		for (auto model : ModelList) {
			SolutionHolder[model] = new vector<double> *[ContentList.size()];
			for (auto field : ContentList) {
				SolutionHolder[model][field] = new vector<double>[nPeriods + 1];
				for (int period = 1; period <= nPeriods; period++)
					SolutionHolder[model][field][period].resize(size);
			}	
		}

				
		//Instance of same type
		for (int i = 0; i < size; i++) {


			//Sleep(1000);
			//CustomerIRPDB *db = new CustomerIRPDB(nCustomers, nPeriods);

			string extension(".txt");
			string filename = "3 vehicles/C" + to_string(nCustomers) + "T" + to_string(nPeriods) + "_v" + to_string(i + 1) + extension;
			ofstream instanceFile;
			NodeInstanceDB db(filename);

			vector <double> val;
			//db->writeInstanceToFile(instanceFile, filename);

			//Initialize solutions
			ConstructionSolutions = new vector<double>[ContentList.size()];
			RelaxedSolutions = new vector<double>[ContentList.size()];
			ExactSol = new vector<double>[ContentList.size()];

			for (auto field : ContentList) {
				ConstructionSolutions[field].resize(nPeriods+1);
				RelaxedSolutions[field].resize(nPeriods+1);
				ExactSol[field].resize(nPeriods+1);
			}

			//heurestic
			Heurestic(db);

			//Exact model
			IRP model(db, false);

			Solution *sol = model.solveModel();
			sol->printSolution();


			//Save data from solution
			for (int t = 1; t <= model.getNumOfPeriods(); t++) {
				ExactSol[TransCost][t] = sol->getTransportationCost(t);
				ExactSol[HoldingCost][t] = sol->getHoldingCost(t);
				ExactSol[Routes][t] = sol->getNumberOfRoutes(t);
				ExactSol[Objective][t] = sol->getHoldingCost(t) + sol->getTransportationCost(t);
				ExactSol[nNodeVisits][t] = sol->getNodeVisits(t);
				ExactSol[Delivery][t] = sol->getDelivery(t);
				ExactSol[Pickup][t] = sol->getPickup(t);
			}



			for (auto field : ContentList) {
				for (int period = 1; period <= nPeriods; period++)
				{
					SolutionHolder[Construction][field][period][i] = ConstructionSolutions[field][period];
					SolutionHolder[Exact][field][period][i] = ExactSol[field][period];
				}
			}

		}//End instance type


		Calculations CalcObject(SolutionHolder, size, nPeriods);
		double *** CostData = CalcObject.getCostDistribution();
		double *** RouteData = CalcObject.getRouteInformation();
		ofstream textFile;
		textFile.open("Results/DataFromInstances.txt");
		string row = "$C_" + to_string(nCustomers) + "V_" + to_string(ModelParameters::nVehicles) + "T_" + to_string(nPeriods) + "$ ";

		vector<string> colTitle(CostDataList.size());

		colTitle[TransExact] = "$C^T$ &";
		colTitle[HoldExact] = "$C^H$ &";
		colTitle[deltaTrans] = "$\Delta C^T$ &";
		colTitle[deltaHold] = "$\Delta C^H$ &";
		colTitle[deltaObjective] = "$\Delta Z &";
		colTitle[nVehicles] = "$\Delta K &";
		colTitle[nNodeVisits] = "$\Delta N &";
		colTitle[dDelivery] = "$\Delta del &";
		colTitle[dPickup] = "$\Delta pick &";

		vector<string> rowTitle(nPeriods + 1);

		stringstream stream;


		textFile << "\\tab\\tab\\tab\\tab";
		for (auto field : CostDataList) {
			textFile << "&$" << colTitle[field] << "&";
		}
		textFile << "\\" << "\n" << "lashline \\" << "\n";

		for (int t = 1; t <= nPeriods + 1; t++) {
			if (t < nPeriods + 1)
				textFile << "Period " << to_string(t) << "&";
			else
				textFile << "Total&";

			for (auto field : CostDataList)
			{
				stream << fixed << setprecision(2) << CostData[field][t][Average];
				textFile << stream.str() << "\\%\ &";
				stream.str(string());
			}

			for (auto field : RouteDataList) {
				stream << fixed << setprecision(2) << RouteData[field][t][Average];
				if (field == nVehicles)
					textFile << stream.str() << "\&";
				else
					textFile << stream.str() << "\\%\ &";
				stream.str(string());
			}

			textFile << "\n";

			if (t == nPeriods) {
				textFile << "lashline\\";
			}
		}

		textFile.close();
	} //end periods


	  //sol->print("graphSolutions/VRP");
	  //model.addRoutesToVector();


	  /*for (int t = 1; t <= modelIRP.getNumOfPeriods(); t++) {
	  VRPSol[TransCost][t] = sol->getTransportationCost(t);
	  VRPSol[HoldingCost][t]= sol->getHoldingCost(t);
	  VRPSol[Routes][t]= sol->getNumberOfRoutes(t);
	  VRPSol[Objective][t] = sol->getTransportationCost(t) + sol->getHoldingCost(t);;
	  VRPSol[nNodeVisits][t] = sol->getNodeVisits(t);
	  VRPSol[Delivery][t] = sol->getDelivery(t);
	  VRPSol[Pickup][t] = sol->getPickup(t);

	  RelSol[TransCost][t] = solIRP->getTransportationCost(t);
	  RelSol[HoldingCost][t] = solIRP->getHoldingCost(t);
	  RelSol[Routes][t] = solIRP->getNumberOfRoutes(t);
	  RelSol[Objective][t] = solIRP->getTransportationCost(t) + solIRP->getHoldingCost(t);;
	  RelSol[nNodeVisits][t] = solIRP->getNodeVisits(t);
	  RelSol[Delivery][t] = solIRP->getDelivery(t);
	  RelSol[Pickup][t] = solIRP->getPickup(t);
	  }

	  static int i = 1;
	  //sol->print(model, "IRPconstruction"+to_string(i));
	  i++;*/
	 //Arguments: vector<double> * &VRPSol, vector<double>* &RelSol

}







//Removed code


//int period = sol->shiftQuantity(ModelParameters::HIGHEST_RESIDUAL_CAPACITY, ModelParameters::MIN_SERVICE);

//routeProb.shiftQuantityCtr(sol->getTotalDelivery(period) + sol->getTotalPickup(period))

//Do localSearch
/*
IRP::LocalSearch LocalSearch(modelIRP, sol);
Solution * localSearchSol = modelIRP.allocateSolution();
LocalSearch.ShiftQuantity(localSearchSol);

//Solve a VRP for each period
for (int t = 1; t <= modelIRP.getNumOfPeriods(); t++) {
	nodes = localSearchSol->getVisitedNodes(t);
	solveVRP(nodes, t, modelIRP, localSearchSol);

}
localSearchSol->printSolution();


*/


/*string s[] = { "IRP relaxed:\t","\nConstr obj:\t", "\nExact obj:\t" };

for (int j = 0; j < 3; j++) {
printf("\n");
cout << s[j] << ":\t" << ": ";
for (int i = 0; i < size; i++) {
printf("%0.f\t", SolutionHolder[i][j].Objective);
}

printf("\n");
printf("Trans:\t\t");
for (int i = 0; i < size; i++) {
printf("%0.f\t", SolutionHolder[i][j].HoldingCost);
}

printf("\n");
printf("Hold:\t\t");
for (int i = 0; i < size; i++) {
printf("%0.f\t", SolutionHolder[i][j].TransportationCost);
}

}
}
outputFile.close();*/



