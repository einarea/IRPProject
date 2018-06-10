/*
This  is  the  source  code  employed  in  the  
computational  study  in  the master thesis; A matheuristic for the 
inventory routing problem with divisible pickup and delivery, at the Norwegian
University of Science and Technology by Einar Aastveit, Simen Vadseth and Tuva Toftdahl Staver.
*/

#include "stdafx.h"
#include "IRP.h"
#include "SolutionInfo.h"
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

using namespace DataSets;
using namespace std;

//Forward decleration
SolutionInfo::InstanceInfo * Heurestic(const NodeInstanceDB &db, SolutionInfo::InstanceInfo * = nullptr, RouteAnalyzer::Instance * = nullptr);
SolutionInfo::InstanceInfo * ExactMethod(const NodeInstanceDB &db, int version, SolutionInfo::InstanceInfo * = nullptr);

int main()
{
	RouteAnalyzer routeInfo;
	SolutionInfo solutionData;
	int index;

	//Used to calculate dual gap in the heurestic
	vector<double> bestBound = { 28114.88622,	32020.03663,	29311.66501,	31159.64979,	28668.85732 };

	vector<int> n = {24, 30};

		for (int nCustomers : n) {
		
			int nPeriods = 5;
			SolutionInfo data;

			for (int version =1; version <= 5; version++) {

				//Functions to open and create instances
				index = version - 1;
				//const NodeInstanceDB *db = NodeInstanceDB::createInstance(nCustomers, nPeriods, version);
				//const NodeInstanceDB *db = NodeInstanceDB::createPDInstance(nCustomers, nPeriods, version);
				//const NodeInstanceDB *db = NodeInstanceDB::createDelInstance(nCustomers, nPeriods, version);
				//const NodeInstanceDB *db = NodeInstanceDB::createClosenessToDepotInstance(nCustomers, nPeriods, version);
				//const NodeInstanceDB *db = NodeInstanceDB::createClusterInstance(nCustomers, 5, nPeriods, version);
				const NodeInstanceDB *db = NodeInstanceDB::openInstance(nCustomers, nPeriods, version);
				
				//Run heurestic
				//auto problemInstance = routeInfo.addInstance(version);
				auto instanceData = solutionData.newInstance("C" + to_string(nCustomers) + "v_" + to_string(version));
				//instanceData->bestBound = bestBound[index];
				Heurestic(*db, instanceData)->printInstanceToFile()->printInstanceTimeSeriesToFile();
				
				//To get percentage away from the best known solution or the lower bound.
				//instanceData->bestExactSolution = exactVal[version-1];

				//Run exact model model
				ExactMethod(*db, version, instanceData)->printInstanceToFile();
				
			}
		}


		solutionData.printAverageInstancesToFile("cumulativeData");
		//routeInfo.printAverageToFile("RouteInfoo"+to_string(nCustomers)+".txt");
	return 0;
}


SolutionInfo::InstanceInfo * ExactMethod(const NodeInstanceDB &db, int version, SolutionInfo::InstanceInfo * instanceData) {
	IRP model(db, false, instanceData);

	if (ModelParameters::SUBTOUR_ELIMINATION) {
		model.useIPSubtourElimination(ModelParameters::GLOBAL_CUTS);
	}

	//model.addValidIneq(ModelParameters::MinimumNodeVisit);
	//	model.addValidIneq(ModelParameters::MinimumInventory);
	//model.addValidIneq(ModelParameters::MinimumFlow);
	Solution *solLP = model.solveLPModel();
	Solution *sol = model.solveModel();
	//Store final solution in instance data
	instanceData->addSolutionPoint(ModelParameters::FINAL_SOL, model.bestSol, difftime(time(NULL), model.startTime));
	instanceData->fillInfo();
	instanceData->bestObjective = sol->getObjective();
	instanceData->nDivisible = sol->getnDivisible();
	instanceData->nSingleService = sol->getnSingleService();
	instanceData->nTotalNodesServed = sol->getTotalNodesServed();
	instanceData->nSimultanouesService = sol->getnSimultaneousVisits();
	instanceData->nRoutes = sol->getnRoutes();
	instanceData->solTime = difftime(time(NULL), model.startTime);
	instanceData->printInstanceToFile(false);

	sol->printSolution();

	double lpObj = solLP->getObjective();
	double lpTrans = solLP->getTransportationCost();
	double lpHold = solLP->getHoldingCost();
	model.printSolutionToFile(lpObj, lpTrans, lpHold, version, 1);

	return instanceData;
}

/*The construction heurestic solves an IRP with relaxed arc variables(x_ijt).
The solution provides information about which customers are visited in each period
and how much that are picked up/delivered at each customer and the routes.
For each time period a VRP is solved exactly for the visited nodes. 

From the construction solution an intensification search is performed with two operators;
shift quantity and route search. There are several configuations for the shift quantity operator.
*/

//forward declarations
void addRoutes(Solution&, vector<Route*>&);
void printDiversificationInformation(vector<Solution*> constHolder, vector<Solution*> solHolder, string filename);

SolutionInfo::InstanceInfo* Heurestic(const NodeInstanceDB &db, SolutionInfo::InstanceInfo * instance,  RouteAnalyzer::Instance * problemInstance) {
	static int i = 1;
	bool relaxArcVariables = true;
	SolutionInfo solutionPoints;

	//Initialize simultanous model if that is the solution approach
	if (ModelParameters::Simultanoues_RelaxedIRP == true)
		ModelParameters::Simultaneous = true;

	//Initialize the model with relaxed arc variables
	IRP modelIRP(db, relaxArcVariables);

	//Use subtour elimination in relaxed IRP
	if (ModelParameters::SUBTOUR_ELIMINATION) {
		modelIRP.useIPSubtourElimination(ModelParameters::GLOBAL_CUTS);
	}




	vector<Solution *> solutionHolder;
	vector<Solution*> constructionSolHolder;
	//Solution holders
	Solution solIRP(db);
	Solution solVRP(db);
	Solution prevSol(db);
	Solution tempSol(db);
	Solution bestSol(db);
	time_t tic; //Time of intensification
	time_t ticSQ; //Time of shift quantity
	double sqTime; 
	time_t ticRS; //Time of route search
	time_t totalTime; //Running time Heurestic
	time_t startTime; //Start time heurestic
	int counter;

	//Objective value holders
	double tempObj;
	double bestObj = 1000000;
	double itrBestObj;
	double afterConst;

	bool skipIteration;
	double relaxed = 1;


	//Route containers
	vector<Route*> intensificationRoutes;
	vector<Route*> bestRoutes;
	vector<Route*> tempRoutes;

	//Matrices for tabu information
	double ** VisitMatrixConstruction;
	double ** changeMatrixVRP;
	double ** VisitMatrixBest;

	//Indicator = true if intensification iterations are to be exited
	bool breakIntensification;


	//Start total time clock
	totalTime = 0;
	startTime = time(NULL);
	int iteration = 1;
	int intCounter = 0;
	RouteProblem::clearRouteCounter();
	//Add instance to store sol information
	
	//Diversification iteration
	while (totalTime <= ModelParameters::HEURESTIC_TIME) {
		//Turn on simultanoues if that is the solution approach
		if (ModelParameters::Simultanoues_RelaxedIRP == true)
			ModelParameters::Simultaneous = true;

		//Solve the relaxed IRP model
		solIRP = *modelIRP.solveModel();
		//solIRP.plot("Solution/plo");
		skipIteration = false;
		//Break iteration if not feasible
		if (solIRP.getObjective() <= 1) {
			skipIteration = true;
		}

		if (!skipIteration) {

			if (iteration == 1) {
				instance->relaxedObj = modelIRP.bestSol;
				instance->irpRel = modelIRP.bestBound;
			}
			//Allocate a vrp solution
			solVRP = solIRP;


			//Turn off any simultaneous constraints
			ModelParameters::Simultaneous = false;

			//Solve a VRP for each  period			
			for (int t = 1; t <= modelIRP.getNumOfPeriods(); t++) {
				solVRP.solveVRP(t);
			}

			itrBestObj = solVRP.getObjective();


			//If continuous improvement is to be recorded; Record improvement = true
			if (solVRP.getObjective() <= bestObj) {
				bestObj = solVRP.getObjective();
				if (ModelParameters::RecordImprovement) {
					instance->addSolutionPoint(ModelParameters::VRP, solVRP.getObjective(), difftime(time(NULL), startTime));
				}
			}

			//If the current solution is to be recorded; RecordImprovement = false
			else if (!ModelParameters::RecordImprovement) instance->addSolutionPoint(ModelParameters::VRP, solVRP.getObjective(), difftime(time(NULL), startTime));


			//If not feasible, shift quantity from infeasible period
			if (!solVRP.isFeasible()) {
				solVRP.shiftQuantityMIP(ModelParameters::INFEASIBLE);
			}

			//If not succeeded to find a feasible solution, perform a route search
			int i = 1;
			while (!solVRP.isFeasible() && i <= 5) {
				solVRP.routeSearch();
				i++;
			}

			//Exit program if no feasible solution
			if (!solVRP.isFeasible()) {
				cout << "Error: Not able to construct feasible solution";
				exit(101);
			}

			//Minimize the inventory for the present routes
			solVRP.solveInventoryProblem();

			//Record the construction objective value
			afterConst = solVRP.getObjective();

			//Save the construction solution
			constructionSolHolder.push_back(new Solution(solVRP));

			//Update the current best solution
			bestSol = solVRP;

			//Save the routes found in the construction
			addRoutes(bestSol, intensificationRoutes);

			//Save routes to best if selection problem is to select between all routes 
			if (ModelParameters::SelectBetweenAllRoutes)
				addRoutes(bestSol, bestRoutes);




			//start clock to measure intensification time
			tic = time(NULL);
			counter = 1;
			tempSol = bestSol;

			//Indicator = true if intensification is to be terminated, false otherwise 
			breakIntensification = false;

			//If routeinformation is stored
			if (problemInstance != nullptr)
				problemInstance->addIteration();

			//Start intensification search
			do {

				//Start clock for shift quantity
				ticSQ = time(NULL);
				//Shift quantity operator, perform while solution improved
				do {
					tempObj = tempSol.getObjective();
					tempSol.shiftQuantityMIP(ModelParameters::RESTRICTED_SHIFT);

					//Save any new routes
					addRoutes(tempSol, intensificationRoutes);

					if (tempSol.getObjective() <= itrBestObj) {
						itrBestObj = tempSol.getObjective();
						if (ModelParameters::SelectBetweenAllRoutes) {
							addRoutes(bestSol, bestRoutes);
						}

						if (tempSol.getObjective() <= bestObj) {
							bestObj = tempSol.getObjective();
							if (ModelParameters::RecordImprovement) {
								instance->addSolutionPoint(ModelParameters::SHIFT_QUANTITY, tempSol.getObjective(), difftime(time(NULL), startTime));
							}
						}
					}
					else if (!ModelParameters::RecordImprovement) instance->addSolutionPoint(ModelParameters::SHIFT_QUANTITY, tempSol.getObjective(), difftime(time(NULL), startTime));
				} while (tempSol.getObjective() <= tempObj - 1);

				sqTime = difftime(time(NULL), ticSQ);


				//Start route search clock
				ticRS = time(NULL);
				int iii = 0;
				//Perform as long as shift quantity
				while (difftime(time(NULL), ticRS) < sqTime || iii < 1) {
					//Route search operator which require at least one change
					tempSol.routeSearch(ModelParameters::REQUIRE_CHANGE);

					//Save any new routes
					addRoutes(tempSol, intensificationRoutes);
					iii++;

					//Get route information
					if (problemInstance != nullptr) {
						problemInstance->getLatestPopulation()->addGeneration(tempSol.GeneratedRoutes);
						problemInstance->getLatestPopulation()->getLatestGeneration()->addSelectedGeneration(tempSol.SelectedRoutes);
						problemInstance->getLatestPopulation()->getLatestGeneration()->ImprovementSel = tempSol.getObjective();
					}

					//Record improvent or current solution value
					if (tempSol.getObjective() <= itrBestObj) {
						itrBestObj = tempSol.getObjective();

						if (ModelParameters::SelectBetweenAllRoutes) {
							addRoutes(bestSol, bestRoutes);
						}

						if (ModelParameters::RecordImprovement && itrBestObj <= bestObj) {
							bestObj = itrBestObj;
							instance->addSolutionPoint(ModelParameters::ROUTE_SEARCH, tempSol.getObjective(), difftime(time(NULL), startTime));
						}
					}
					else if (!ModelParameters::RecordImprovement) instance->addSolutionPoint(ModelParameters::ROUTE_SEARCH, tempSol.getObjective(), difftime(time(NULL), startTime));
				}


				//terminate intensification if tempSol already found in the intensification
				if (tempSol == bestSol)
					breakIntensification = true;


				//terminate intensification if exisiting solution found in previous devirsification iteration
				for (Solution * r : solutionHolder) {
					if (tempSol == *r)
						breakIntensification = true;
				}

				//Update iteration counter, check termination condition
				counter++;

			} while (counter < 100 && difftime(time(NULL), tic) <= ModelParameters::INTENSIFICATION_TIME && !breakIntensification);

			intCounter += counter - 1;

			//Solve a routing problem for the routes found in the intensification search
			RouteProblem routeProb(db, intensificationRoutes);
			routeProb.formulateRouteProblem(ModelParameters::HIGHEST_TOTALCOST);
			bestSol = *routeProb.solveProblem();

			if (ModelParameters::SelectBetweenAllRoutes)
				addRoutes(bestSol, bestRoutes);

			if (problemInstance != nullptr)
				problemInstance->getLatestPopulation()->Improvement = (afterConst - bestSol.getObjective()) / afterConst;

			if (ModelParameters::RecordImprovement) {
				if (bestSol.getObjective() <= bestObj) {
					instance->addSolutionPoint(ModelParameters::INTENSIFICATAION_END, bestSol.getObjective(), difftime(time(NULL), startTime));
					bestObj = bestSol.getObjective();
				}
			}
			else if (!ModelParameters::RecordImprovement) instance->addSolutionPoint(ModelParameters::INTENSIFICATAION_END, bestSol.getObjective(), difftime(time(NULL), startTime));

			//Clear routes
			for (Route * r : intensificationRoutes) {
				delete r;
			}

			intensificationRoutes.clear();

			//Push back solution from intensification
			solutionHolder.push_back(new Solution(bestSol));

			//If continually improve, solve a route problem for the current routes found
			if (ModelParameters::RecordImprovement) {

				//Solve a routing problem
				RouteProblem routeProb(db, bestRoutes);
				routeProb.formulateRouteProblem(ModelParameters::HIGHEST_TOTALCOST);
				Solution * solFinal = routeProb.solveProblem();

				double aa = solFinal->getObjective();
				if (solFinal->getObjective() <= bestObj) {
					instance->addSolutionPoint(ModelParameters::INTENSIFICATAION_END, solFinal->getObjective(), difftime(time(NULL), startTime));
					bestObj = solFinal->getObjective();
				};

			}
		}


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
			//modelIRP.addHoldingCostCtr(solVRP.getHoldingCost());
		}
		

		//Relax constraints if skipped iteration due to infeasible IRP
		if (skipIteration){
			//Add visit constraint

			double minChange = (int)ceil((double) relaxed * ModelParameters::MIN_CHANGE);
			relaxed = 0.8*relaxed;
			if (ModelParameters::MinChanges)
				modelIRP.addVisitConstraint(VisitMatrixConstruction, ModelParameters::ForceChanges, minChange);
			if (ModelParameters::RouteChange)
				modelIRP.addRouteCtr(solVRP.getAllRoutes(), relaxed);
		}

		else {
			//Add visit constraint
			relaxed = 1;
			if (ModelParameters::MinChanges)
				modelIRP.addVisitConstraint(VisitMatrixConstruction, ModelParameters::ForceChanges);
			if (ModelParameters::RouteChange)
				modelIRP.addRouteCtr(solVRP.getAllRoutes());
			//Store construction solution from present iteration
			prevSol = solVRP;
		}
		iteration++;
		totalTime = difftime(time(NULL), startTime);

	}

	//Print all routes

	//Final step solve route problem
	//Routeholder
	if (!ModelParameters::SelectBetweenAllRoutes) {
		for (Solution * s : solutionHolder)
			for (Route * r : s->getAllRoutes())
				bestRoutes.push_back(r);
	}


	//Solve a routing problem
	RouteProblem routeProb(db, bestRoutes);
	routeProb.formulateRouteProblem(ModelParameters::HIGHEST_TOTALCOST);
	Solution * solFinal = routeProb.solveProblem();

	instance->addSolutionPoint(ModelParameters::FINAL_SOL, solFinal->getObjective(), difftime(time(NULL), startTime));
	instance->fillInfo();
	instance->nRoutePool = bestRoutes.size();
	instance->nIterations = iteration - 1;
	instance->nIntIterations = intCounter;
	instance->bestObjective = solFinal->getObjective();
	instance->nDivisible = solFinal->getnDivisible();
	instance->nSingleService = solFinal->getnSingleService();
	instance->nTotalNodesServed = solFinal->getTotalNodesServed();
	instance->nSimultanouesService = solFinal->getnSimultaneousVisits();
	instance->nRoutes = solFinal->getnRoutes();
	instance->solTime = difftime(time(NULL), startTime);

	//Diversification information
	//printDiversificationInformation(constructionSolHolder, solutionHolder, db.getName());
	
	return instance;
}//End heurestic


void addRoutes(Solution& sol, vector<Route*>& routePool)
{
	bool add;
	vector<Route*> routes = sol.getAllRoutes();
	for (Route* r : routes) {
		add = true;
		for (Route * r2 : routePool)
			if (r->isDuplicate(r2)) {
				add = false;
			}
		if (add) {
			routePool.push_back(new Route(*r));
		}
		delete r;
	}

}

void printDiversificationInformation(vector<Solution*> constructionSolHolder, vector<Solution*> solutionHolder, string filename)
{
	vector<double> objectiveValues;
	vector<vector<double>> similarity;

	vector<vector<NodeIRP*>> graphHolder;
	for (auto solution : constructionSolHolder) {
		vector <NodeIRP*> graph;
		solution->buildGraph(graph);
		//solution->plot("Solution/ConSol" + to_string(i++));
		graphHolder.push_back(graph);
		objectiveValues.push_back(solution->getObjective());
	}

	ofstream ins;
	ins.open("Solution/" + filename);


	similarity.resize(graphHolder.size());
	//Find similarity for all pair of solutions
	ins << "\n" << "Similarity Matrix between solution i and j\t\t\t\t\t\tObjective value solution i";
	for (int i = 0; i < graphHolder.size(); i++) {
		similarity[i].resize(graphHolder.size());
		ins << "\n";
		for (int j = 0; j < graphHolder.size(); j++) {
			double sim = graphAlgorithm::getSimiliarity(graphHolder[i], graphHolder[j]);
			similarity[i][j] = sim;
			ins << setprecision(2) << sim << "\t";
		}

		ins << setprecision(5) << objectiveValues[i];

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


	ins << "\n";
	similarity.resize(graphHolder.size());
	//Find similarity for all pair of solutions
	ins << "\n" << "Similarity Matrix between solution i and j\t\t\t\t\t\tObjective value solution i";
	for (int i = 0; i < graphHolder.size(); i++) {
		similarity[i].resize(graphHolder.size());
		ins << "\n";
		for (int j = 0; j < graphHolder.size(); j++) {
			double sim = graphAlgorithm::getSimiliarity(graphHolder[i], graphHolder[j]);
			similarity[i][j] = sim;
			ins << setprecision(2) << sim << "\t";
		}

		ins << setprecision(5) << objectiveValues[i];

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
				ConstructionSolutions[field].resize(nPeriods + 1);
				RelaxedSolutions[field].resize(nPeriods + 1);
				ExactSol[field].resize(nPeriods + 1);
			}

			//heurestic
			//Heurestic(db);

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
}


