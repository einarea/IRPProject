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
//#include <boost/tuple/tuple.hpp>

//#include "gnuplot-iostream.h"

using namespace std;

//Solution struct to hold solutions


enum Types {
	CONSTRUCTION = 0,
	EXACT,
	RELAXED
};

enum CONTENT {
	OBJECTIVE = 0,
	HOLD,
	TRANS,
	VEHICLES
};

/*The construction heurestic solves an IRP with relaxed arc variables(x_ijt).
The solution provides information about which customers are visited in each period
and how much that are picked up/delivered at each customer.
For each time period a VRP is solved exactly for the visited nodes. */

void constructionHeurestic(CustomerIRPDB & db, vector<solution *> &VRPSol, vector<solution *> &RelSol) {

	//Solve IRP with relaxed arc variables.
	bool relaxArcVariables = true;
	IRP model(db, relaxArcVariables, false);
	//model.addValidIneq();
	IRP::Solution *solIRP = model.solveModel();

	//Vectors to hold visited customers and their demand
	vector<Customer *> customers;
	vector <vector<double>> demand;

	solIRP->printSolution();
	int idVRP = model.allocateSolution();
	IRP::Solution *solVRP = model.getSolution(idVRP);

	
	//Solve a VRP for each period
	for (int t = 1; t <= model.getNumOfPeriods(); t++) {
			model.getVisitedCustomers(t, customers);
			model.getDemand(t, demand, customers);
			if (customers.size() >= 1) {
				//Initialize a VRP database with the demand and customers
				CustomerVRPDB VRPdatabase(demand, customers);
				VRPmodel modelVRP(VRPdatabase, *model.getMap(), model.getCapacity());
				modelVRP.solveModel();

				//Add partial solution to IRP
				modelVRP.addToIRPSolution(t, solVRP, model);
				customers.clear();
			}
			
	}
	//bool feasible = solVRP->isFeasible();
	solVRP->printSolution();
	model.addRoutesToVector();
	model.printRouteMatrix();

	for (int t = 1; t <= model.getNumOfPeriods(); t++) {
		VRPSol[t]->TransportationCost = solVRP->getTransportationCost(t);
		VRPSol[t]->HoldingCost = solVRP->getHoldingCost(t);
		VRPSol[t]->Vehicles = solVRP->getNumberOfRoutes(t);

		RelSol[t]->TransportationCost = solIRP->getTransportationCost(t);
		RelSol[t]->HoldingCost = solIRP->getHoldingCost(t);
		RelSol[t]->Vehicles = solVRP->getNumberOfRoutes(t);
	}

	static int i = 1;
	//solVRP->print(model, "IRPconstruction"+to_string(i));
	i++;
		
}//End construction heurestic






int main()
{
	const int size = 5;
	const int modelTypes = 3;

	//Holds IRP rel, construction and Exact for size instances. Ex 3 types * 5 instances
	vector< vector <solution*>> SolutionHolder;

	int Customers[] = { 6, 6, 6 };
	int Periods[] = { 3, 5, 7 };

	vector<solution*> ConstructionSolutions;
	vector<solution*> RelaxedSolutions;
	vector<solution*> ExactSol;
	//Number of instance types
	ofstream outputFile;
	outputFile.open("ConstructionVsExact");

	for (int p = 1; p < 3; p++) {


		int nCustomers = Customers[p];
		int nPeriods = Periods[p];
		double nRoutesEx = 0;
		double nRoutesCon = 0;

		//Instance of same type
		for (int i = 0; i < size; i++) {


			//Sleep(1000);
			//CustomerIRPDB *db = new CustomerIRPDB(nCustomers, nPeriods);

			string extension(".txt");
			string filename = "1 vehicle/C" + to_string(nCustomers) + "T" + to_string(nPeriods) + "_v" + to_string(i + 1) + extension;
			ofstream instanceFile;
			CustomerIRPDB db(filename, true);

			vector <double> val;
			//db->writeInstanceToFile(instanceFile, filename);

			//Initialize solutions
			ConstructionSolutions.resize(Periods[p]);
			RelaxedSolutions.resize(Periods[p]);
			ExactSol.resize(Periods[p]);

			for (int t = 1; t <= Periods[p]; t++) {
				ConstructionSolutions[t] = new solution();
				RelaxedSolutions[t] = new solution();
				ExactSol[t] = new solution();
			}

			constructionHeurestic(db, ConstructionSolutions, RelaxedSolutions);




			//Exact model
			IRP model(db, false);

			//model.addValidIneq();
			IRP::Solution *sol = model.solveModel();
			//sol->printSolution(model);
			//sol->print(model, "IRPexact" + to_string(i));
			//((sol->printSolution(model);

			//Save data from solution
			
			for (int t = 1; t <= model.getNumOfPeriods; t++){
				ExactSol[t]->TransportationCost = sol->getTransportationCost(t);
				ExactSol[t]->HoldingCost = sol->getHoldingCost(t);
			}
//			ExactSol.Vehicles = sol->getNumberOfRoutes();

			SolutionHolder.resize(modelTypes);
			SolutionHolder[RELAXED] =(RelaxedSolutions);
			SolutionHolder[CONSTRUCTION] = (ConstructionSolutions);
			SolutionHolder[EXACT] = (ExactSol);
		} //End instance type

		string row = "$C_" + to_string(nCustomers) + "V_" + to_string(ModelParameters::nVehicles) + "T_" + to_string(nPeriods) + "$ ";
		row = row + calcStatistics(SolutionHolder, size, nRoutesEx / size, nRoutesCon / size);

		cout << row;



		string s[] = { "IRP relaxed:\t","\nConstr obj:\t", "\nExact obj:\t" };

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
		outputFile.close();
		
	
		return 0;
}
