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
	double Vehicles;
	int Fields = 3;
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

string calcStatistics(vector<vector<solution*>> Construction, vector<vector<solution*>> Exact) {

	int nModels = 2;
	int nInstances = Exact.size();
	int nPeriods = Exact[0].size();
	int nFields = Exact[0][0]->Fields;

	double *** total;
	double *** totalAvg;
	total = new double** [nModels];
	totalAvg = new double*[nModels];

	for (int i = 0; i < nModels; i++) {
		total[i] = new double *[nFields];
		totalAvg[i] = new double*[nModels];

		for (int j = 0; j < nFields; j++) {
			total[i][j] = new double[nPeriods + 1];
			totalAvg[i][j] = new double[nPeriods + 1];

			for (int t = 1; t <= nPeriods; t++) {
				total[i][j][t] = 0;
				totalAvg[i][j][t] = 0;
			}
		}
	}

	
	double var[5];
	int a = 4;

	for (int i = 2; i < nInstances; i++){
		for (int t = 1; t <= nPeriods; t++) {

			total[EXACT][HOLD][t] += Exact[t][i]->HoldingCost;
			total[CONSTRUCTION][HOLD][t] += Construction[t][i]->HoldingCost;

			total[EXACT][TRANS][t] += Exact[t][i]->TransportationCost;
			total[CONSTRUCTION][TRANS][t] += Construction[t][i]->TransportationCost;

			total[EXACT][VEHICLES][t] += Exact[t][i]->Vehicles;
			total[CONSTRUCTION][VEHICLES][t] += Construction[t][i]->Vehicles;
		}

	}

	//Average
	for (int i = 0; i < nModels; i++) {
		for (int j = 0; j < nFields; j++) {
			for (int t = 1; t <= nPeriods; t++) {
				totalAvg[i][j][t] = total[i][j][t] / nInstances;
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
