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
struct solution {
	double HoldingCost;
	double TransportationCost;
	double Objective;
	double Vehicles;
};

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

void constructionHeurestic(CustomerIRPDB & db, solution &VRPSol, solution &IRPRelSol) {

	//Solve IRP with relaxed arc variables.
	bool relaxArcVariables = true;
	IRP model(db, relaxArcVariables, false);
	//model.addValidIneq();
	model.solveModel();

	//Vectors to hold visited customers and their demand
	vector<Customer *> customers;
	vector <vector<double>> demand;

	//Allocate new solution
	int id = model.allocateIRPSolution();
	IRP::Solution *solIRP = model.getSolution(id);
	
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

	IRPRelSol.Objective = solIRP->getObjective();
	IRPRelSol.TransportationCost = solIRP->getTransportationCost();
	IRPRelSol.HoldingCost = solIRP->getHoldingCost();
	IRPRelSol.Vehicles = solIRP->getNumberOfRoutes();

	VRPSol.Objective = solVRP->getObjective();
	VRPSol.TransportationCost = solVRP->getTransportationCost();
	VRPSol.HoldingCost = solVRP->getHoldingCost();
	VRPSol.Vehicles = solVRP->getNumberOfRoutes();

	static int i = 1;
	//solVRP->print(model, "IRPconstruction"+to_string(i));
	i++;
		
}//End construction heurestic

string calcStatistics(vector<vector<solution>> &Pool, int size, double nRoutesEx, double nRoutesCon) {


	int instances = Pool.size();
	const int content = 4;
	const int models = 3;
	double total[models][content] = { { 0,0,0,0}, {0,0,0, 0}, {0,0,0, 0}};
	double totalAvg[models][content];
	double varTot[] = { 0,0,0,0,0,0 };
	double var[5];
	int a = 4;

	for (int i = 2; i < instances; i++){
		total[EXACT][OBJECTIVE] += Pool[i][EXACT].Objective;
		total[CONSTRUCTION][OBJECTIVE] += Pool[i][CONSTRUCTION].Objective;
		total[RELAXED][OBJECTIVE] += Pool[i][RELAXED].Objective;

		total[EXACT][HOLD] += Pool[i][EXACT].HoldingCost;
		total[CONSTRUCTION][HOLD] += Pool[i][CONSTRUCTION].HoldingCost;
		total[RELAXED][HOLD] += Pool[i][RELAXED].HoldingCost;

		total[EXACT][TRANS] += Pool[i][EXACT].TransportationCost;
		total[CONSTRUCTION][TRANS] += Pool[i][CONSTRUCTION].TransportationCost;
		total[RELAXED][TRANS] += Pool[i][RELAXED].TransportationCost;

		total[EXACT][VEHICLES] += Pool[i][EXACT].Vehicles;
		total[CONSTRUCTION][VEHICLES] += Pool[i][CONSTRUCTION].Vehicles;
		total[RELAXED][VEHICLES] += Pool[i][RELAXED].Vehicles;

	}

	//Average
	for (int i = 0; i < models; i++) {
		for (int j = 0; j < content; j++) {
			totalAvg[i][j] = total[i][j] / instances;
		}
	}

	//Sample variance
	/*for (int j = 0; j < types; j++) {
		for (int i = 0; i < size; i++) {
			varTot[j] += pow(Solution[i][j] - totalAvg[j], 2);
		}

		var[j] = sqrt(varTot[j] / (size - 1));
	}*/

	//double holdingCostPercentage = totalAvg

	//Columns = { "CH", "Z", "k", "CH", "Z", "k" };
	double h = totalAvg[EXACT][HOLD] / totalAvg[EXACT][OBJECTIVE] * 100;
	double t = totalAvg[EXACT][TRANS] / totalAvg[EXACT][OBJECTIVE] * 100;

	double deltaH = (totalAvg[CONSTRUCTION][HOLD] - totalAvg[EXACT][HOLD]) / totalAvg[EXACT][HOLD] *100;
	double deltaT = (totalAvg[CONSTRUCTION][TRANS] - totalAvg[EXACT][TRANS]) / totalAvg[EXACT][TRANS] *100;
	double deltaObj = (totalAvg[CONSTRUCTION][OBJECTIVE] - totalAvg[EXACT][OBJECTIVE]) / totalAvg[EXACT][OBJECTIVE] *100;
	double deltaV = totalAvg[CONSTRUCTION][VEHICLES] - totalAvg[EXACT][VEHICLES];
	char aa[100];
	sprintf_s(aa, "&%.2f & %.2f & %.2f & %.2f & %.2f & %.2f &", h, t, deltaH, deltaT, deltaObj, deltaV);
	return aa;
}




int main()
{
	const int size = 5;
	const int modelTypes = 3;

	//Holds IRP rel, construction and Exact for size instances. Ex 3 types * 5 instances
	vector< vector <solution>> SolutionHolder;

	//Initialize solutionholder
	SolutionHolder.resize(size);
	for (int i = 0; i < SolutionHolder.size(); i++) {
		SolutionHolder[i].resize(modelTypes);
	}


	int Customers[] = { 6, 6, 6 };
	int Periods[] = { 3, 5, 7 };

	vector <solution> modelSolutions;
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

			solution VRPSol;
			solution IRPRelSol;
			constructionHeurestic(db, VRPSol, IRPRelSol);

			SolutionHolder[i][RELAXED] = IRPRelSol;
			SolutionHolder[i][CONSTRUCTION] = VRPSol;
		

			//Exact model
			IRP model(db, false);

			//model.addValidIneq();
			IRP::Solution *sol = model.solveModel();
			//sol->printSolution(model);
			//sol->print(model, "IRPexact" + to_string(i));
			//((sol->printSolution(model);

			//Save data from solution
			solution ExactSol;
			ExactSol.Objective = sol->getObjective();
			ExactSol.TransportationCost = sol->getTransportationCost();
			ExactSol.HoldingCost = sol->getHoldingCost();
			ExactSol.Vehicles = sol->getNumberOfRoutes();

			modelSolutions.push_back(ExactSol);

			SolutionHolder[i][EXACT] = ExactSol;
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
