// IRPmodel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CustomerDB.h"
#include "CustomerVRPDB.h"
#include "IRP.h"
#include "VRPmodel.h"
#include <boost/tuple/tuple.hpp>
#include "gnuplot-iostream.h"
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

using namespace std;
using namespace DataSets;

//Solution struct to hold solutions



/*The construction heurestic solves an IRP with relaxed arc variables(x_ijt).
The solution provides information about which customers are visited in each period
and how much that are picked up/delivered at each customer.
For each time period a VRP is solved exactly for the visited nodes. */


	
	

void constructionHeurestic(CustomerIRPDB & db, vector<double> * &VRPSol, vector<double>* &RelSol) {

	
	bool relaxArcVariables = true;
	IRP modelIRP(db, relaxArcVariables, false);

	IRP::Solution * prevSol = 0;
	IRP::Solution *solIRP = 0;
	IRP::Solution *localSearchSol = 0;
	double ** VisitMatrix;
	double ** changeMatrix;
	int numOfNodes = modelIRP.getNumOfNodes();
	int nPeriods = modelIRP.getNumOfPeriods();
	vector <IRP::NodeIRP*> nodes;


	for (int iteration = 0; iteration <= 10; iteration++) {

		solIRP = modelIRP.solveModel();
	
		solIRP->printSolution();
		//Vectors to hold visited customers and their demand
		vector<Customer *> customers;
		vector <vector<double>> demand;


		//solIRP->print("IRPSolution/irp", ModelParameters::X);
		//vector<XPRBcut> subtourCut;
		int idVRP = modelIRP.allocateSolution();
		IRP::Solution *sol = modelIRP.getSolution(idVRP);
		//bool addedCuts = model.sepStrongComponents(subtourCut);

		//Solve a VRP for each period
		for (int t = 1; t <= modelIRP.getNumOfPeriods(); t++) {
			//modelIRP.getVisitedCustomers(t, customers);
			nodes = sol->getVisitedNodes(t);
			//modelIRP.getDemand(t, demand, customers);
			if (nodes.size() >= 1) {
				//Initialize a VRP database with the demand and customers
				
				CustomerVRPDB VRPdatabase(nodes, *modelIRP.getMap());

				//CustomerVRPDB VRPdatabase(demand, customers);
				VRPmodel modelVRP(VRPdatabase, *modelIRP.getMap(), modelIRP.getCapacity());
				modelVRP.solveModel();

				//Add partial solution to IRP
				modelVRP.addToIRPSolution(t, sol, modelIRP);
				customers.clear();
			}
		}
		//bool feasible = sol->isFeasible();
		sol->printSolution();

		//Create set of nodes visited
		
		//Do localSearch
		IRP::LocalSearch LocalSearch(modelIRP, sol);
		localSearchSol = LocalSearch.ShiftQuantity();
		
		//Solve a VRP for each period
		for (int t = 1; t <= modelIRP.getNumOfPeriods(); t++) {
			//modelIRP.getVisitedCustomers(t, customers);
			nodes = localSearchSol->getVisitedNodes(t);
			//modelIRP.getDemand(t, demand, customers);
			if (nodes.size() >= 1) {
				//Initialize a VRP database with the demand and customers

				CustomerVRPDB VRPdatabase(nodes, *modelIRP.getMap());

				//CustomerVRPDB VRPdatabase(demand, customers);
				VRPmodel modelVRP(VRPdatabase, *modelIRP.getMap(), modelIRP.getCapacity());
				modelVRP.solveModel();

				//Add partial solution to IRP
				modelVRP.addToIRPSolution(t, localSearchSol, modelIRP);
				customers.clear();
			}
		}
		localSearchSol->printSolution();

		VisitMatrix = sol->getVisitedMatrix();

		if (prevSol != 0) {
			changeMatrix  = modelIRP.getVisitDifference(sol, prevSol);
			modelIRP.updateTabuMatrix(changeMatrix);
		}

		//Add visit constraint
		modelIRP.addVisitConstraint(VisitMatrix);

		cout << "\n\n Objective:" << sol->getObjective() << "\n";
		prevSol = sol;
	}
	//model.addValidIneq();
	

	for (int t = 1; t <= modelIRP.getNumOfPeriods(); t++)
		for (auto route : solIRP->getRoutes(t)) {
			cout << "\n";
			cout<<"Cap left in route "<<route->getId()<<" in period "<< t<< " : "<<route->getResidualCapacity()<<"\n";
	}

	//sol->print("graphSolutions/VRP");
	//model.addRoutesToVector();
	modelIRP.printRouteMatrix();

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
		
}//End construction heurestic






int main()
{
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

	for (int p = 1; p < 3; p++) {


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


			constructionHeurestic(db, ConstructionSolutions, RelaxedSolutions);




			//Exact model
			IRP model(db, false);

			//model.addValidIneq();
			IRP::Solution *sol = model.solveModel();
			sol->printSolution();
		//	sol->print("graphSolutions/IRPexact" + to_string(i));
			//((sol->printSolution(model);

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

				

				for (auto field : ContentList){
					for (int period = 1; period <= nPeriods; period ++)
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
			for (auto field: CostDataList) {
				textFile << "&$" << colTitle[field] << "&";
			}
			textFile << "\\" << "\n" <<"lashline \\"<<"\n";

			for (int t = 1; t <= nPeriods + 1; t++){
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
					if(field == nVehicles)
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
		
	
		return 0;
}
