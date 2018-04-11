// IRPmodel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CustomerDB.h"
#include "CustomerVRPDB.h"
#include "IRP.h"
#include "VRPmodel.h"
#include <vector>
#include <cmath>
#include "IRPmodel.h"
#include <stdlib.h>
#include <iomanip>
#include "DataSets.h"
#include "Calculations.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream
//#include <boost/tuple/tuple.hpp>

//#include "gnuplot-iostream.h"


using namespace DataSets;
using namespace std;
//Solution struct to hold solutions



/*The construction heurestic solves an IRP with relaxed arc variables(x_ijt).
The solution provides information about which customers are visited in each period
and how much that are picked up/delivered at each customer.
For each time period a VRP is solved exactly for the visited nodes. */


//Forward decleration
void solveVRP(vector<IRP::NodeIRP *> &nodes, int t, IRP & instance, IRP::Solution* sol);
IRP::RouteProblem * getRouteProblem(IRP::Solution * sol);
string createInstance(int nCustomers, int nPeriods, int version);
string getFilename(int nCustomers, int nPeriods, int version);



void conHeurestic(CustomerIRPDB & db) {

	bool relaxArcVariables = true;

	//Initialize the model with relaxed arc variables
	IRP modelIRP(db, relaxArcVariables, false);

	//Allocate solution
	IRP::Solution * solIRP = 0;
	IRP::Solution * prevSol = 0;

	//Allocate tabu information
	double ** VisitMatrix;
	double ** changeMatrix;

	//Nodes to store solution information
	vector <IRP::NodeIRP*> nodes;

	//Diversification iteration
	for (int iteration = 0; iteration <= 10; iteration++) {

		//Solve the relaxed IRP model
		solIRP = modelIRP.solveModel();

		//Allocate a vrp solution
		IRP::Solution * sol = new IRP::Solution(*solIRP);

		//Solve a VRP for each period
		for (int t = 1; t <= modelIRP.getNumOfPeriods(); t++){
			nodes = sol->getVisitedNodes(t);
			solveVRP(nodes, t, modelIRP, sol);
		}
	
		sol->printSolution();

		//Solve routing problem		
		IRP::RouteProblem * routeProb = getRouteProblem(sol);
		IRP::Solution * solRoute = routeProb->solveProblem();

		solRoute->printSolution();

		//Get visited nodes in the shift period and solve a VRP
		auto nodes = solRoute->getVisitedNodes(routeProb->getShiftPeriod());
		solveVRP(nodes, routeProb->getShiftPeriod(), modelIRP, solRoute);

		solRoute->printSolution();

		//Update tabu information
		VisitMatrix = sol->getVisitedMatrix();

		if (prevSol != 0) {
			changeMatrix  = modelIRP.getVisitDifference(sol, prevSol);
			modelIRP.updateTabuMatrix(changeMatrix);
		}

		//Add visit constraint
		modelIRP.addVisitConstraint(VisitMatrix, ModelParameters::ForceChanges);

		cout << "\n\n Objective:" << sol->getObjective() << "\n";

		//Store solution from present iteration
		prevSol = sol;
	}
	

	for (int t = 1; t <= modelIRP.getNumOfPeriods(); t++)
		for (auto route : solIRP->getRoutes(t)) {
			cout << "\n";
			cout<<"Cap left in route "<<route->getId()<<" in period "<< t<< " : "<<route->getResidualCapacity()<<"\n";
	}
}//End heurestic


int main()
{

			int nCustomers = 3;
			int nPeriods = 3;
			int version = 1;
			
			//string filename = createInstance(nCustomers, nPeriods, version);
			string filename = getFilename(nCustomers, nPeriods, version);

			//Initialize database
			CustomerIRPDB db(filename, true);

			//Run heurestic
			conHeurestic(db);

			//Run exact model model
			IRP model(db, false);
			model.addValidIneq(ModelParameters::MinimumNodeVisit);
			model.addValidIneq(ModelParameters::MinimumInventory);
			model.addValidIneq(ModelParameters::MinimumFlow);

			IRP::Solution *sol = model.solveModel();
			sol->printSolution();
		//	sol->print("graphSolutions/IRPexact" + to_string(i));
			//((sol->printSolution(model);

	
		return 0;
}



void solveVRP(vector<IRP::NodeIRP *> &nodes, int t, IRP & instance, IRP::Solution* sol) {
	//modelIRP.getVisitedCustomers(t, customers);

	//Clear the nodes edges
	for (auto node : nodes)
		node->deleteEdges();


	//modelIRP.getDemand(t, demand, customers);
	if (nodes.size() >= 1) {
		//Initialize a VRP database with the demand and customers
		auto i = instance.getMap();
		CustomerVRPDB VRPdatabase(nodes, *instance.getMap());

		//CustomerVRPDB VRPdatabase(demand, customers);
		VRPmodel modelVRP(VRPdatabase, *instance.getMap(), instance.getCapacity());
		modelVRP.solveModel();

		//Add partial solution to IRP
		modelVRP.addToIRPSolution(t, sol, instance);

	}
}

IRP::RouteProblem * getRouteProblem(IRP::Solution * sol) {

	vector <IRP::Route *> routes;
	//Solve a routing problem
	for (auto t : sol->Instance.Periods) {
		for (auto r : sol->getRoutes(t))
			routes.push_back(r);
	}

	IRP::RouteProblem * routeProb = new IRP::RouteProblem(sol->Instance, routes);
	int period = sol->selectPeriod(ModelParameters::HIGHEST_RESIDUAL_CAPACITY);
	period = 2;
	routeProb->ShiftPeriod = period;
	routeProb->formulateRouteProblem(ModelParameters::MIN_SERVICE);
	routeProb->lockRoutes(sol->getRoutes());

	return routeProb;
};


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
			CustomerIRPDB db(filename, true);

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
			conHeurestic(db);

			//Exact model
			IRP model(db, false);

			IRP::Solution *sol = model.solveModel();
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

string createInstance(int nCustomers, int nPeriods, int version) {
	//Sleep(1000);
	CustomerIRPDB *db = new CustomerIRPDB(nCustomers, nPeriods);

	ofstream instanceFile;

	string filename = getFilename(nCustomers, nPeriods, version);
	db->writeInstanceToFile(instanceFile, filename);
	return filename;
}

string getFilename(int nCustomers, int nPeriods, int version) {

	string extension(".txt");
	return  "3 vehicles/C" + to_string(nCustomers) + "T" + to_string(nPeriods) + "_v" + to_string(version) + extension;
}



//Removed code


//int period = sol->shiftQuantity(ModelParameters::HIGHEST_RESIDUAL_CAPACITY, ModelParameters::MIN_SERVICE);

//routeProb.shiftQuantityCtr(sol->getTotalDelivery(period) + sol->getTotalPickup(period))

//Do localSearch
/*
IRP::LocalSearch LocalSearch(modelIRP, sol);
IRP::Solution * localSearchSol = modelIRP.allocateSolution();
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



