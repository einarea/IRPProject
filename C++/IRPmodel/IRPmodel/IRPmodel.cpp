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
//#include <boost/tuple/tuple.hpp>

//#include "gnuplot-iostream.h"

using namespace std;

/*The construction heurestic solves an IRP with relaxed arc variables(x_ijt).
The solution provides information about which customers are visited in each period
and how much that are picked up/delivered at each customer.
For each time period a VRP is solved exactly for the visited nodes. */
void constructionHeurestic(CustomerIRPDB & db, vector <double> &values) {

	//Solve IRP with relaxed arc variables.
	bool relaxArcVariables = true;
	IRP model(db, relaxArcVariables, false);
	model.addValidIneq();
	model.solveModel();

	//Vectors to hold visited customers and their demand
	vector<Customer *> customers;
	vector <vector<double>> demand;

	//Allocate new solution
	int id = model.allocateIRPSolution();
	IRP::Solution *solIRP = model.getSolution(id);
	

	int idVRP = model.allocateSolution();
	IRP::Solution *solVRP = model.getSolution(idVRP);
	solVRP->printSolution(model);
	
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
//				modelVRP.addRoutesToIRP(model, t, solVRP);
				solVRP->printSolution(model);
				customers.clear();
			}

			
	}

	model.addRoutesToVector();
	model.printRouteMatrix();

	values.push_back(solIRP->getObjective(&model));
	values.push_back(solIRP->getTransportationCost(&model));
	values.push_back(solIRP->getHoldingCost(&model));

	values.push_back(solVRP->getObjective(&model));
	values.push_back(solVRP->getTransportationCost(&model));
	values.push_back(solVRP->getHoldingCost(&model));
	values.push_back(solVRP->getNumberOfRoutes(model));
	solVRP->printSolution(model);
	static int i = 1;
	solVRP->print(model, "IRPconstruction"+to_string(i));
	i++;
		
}//End construction heurestic

string calcStatistics(double ** Solution, int size, double nRoutesEx, double nRoutesCon) {

	const int types = 6;
	double total[6] = { 0,0,0,0,0,0 };
	double totalAvg[types];
	double varTot[] = { 0,0,0,0,0,0 };
	double var[types];

	for (int j = 3; j < 9; j++)
		for (int i = 0; i < size; i++) {
			//Exact objective
			total[j-3] += Solution[i][j];
		}

	//Average
	for (int j = 0; j < types; j++) {
		totalAvg[j] = total[j] / size;
	}


	//Sample variance
	for (int j = 0; j < types; j++) {
		for (int i = 0; i < size; i++) {
			varTot[j] += pow(Solution[i][j] - totalAvg[j], 2);
		}

		var[j] = sqrt(varTot[j] / (size - 1));
	}

	//Columns = { "CH", "Z", "k", "CH", "Z", "k" };
	string row = to_string(round(totalAvg[3])) + "&" + to_string(round(totalAvg[5])) + "&" + to_string(nRoutesEx)
		+ to_string(totalAvg[0]) + "&" + to_string(totalAvg[5]) + "&" + to_string(nRoutesCon);

	return row;
}




int main()
{
	//Initialize database
	const int size = 5;
	double ** Solution;
	Solution = new double * [size];

	

	int Customers[] = { 6, 6, 6 };
	int Periods[] = { 3, 5, 7 };

	for (int p = 0; p < 3; p++) {
		int nCustomers = Customers[p];
		int nPeriods = Periods[p];
		double nRoutesEx = 0;
		double nRoutesCon = 0;
		for (int i = 0; i < size; i++) {

			Solution[i] = new double[9];

			//Sleep(1000);
			//CustomerIRPDB *db = new CustomerIRPDB(nCustomers, nPeriods);

			string extension(".txt");
			string filename = "3 vehicles/C" + to_string(nCustomers) + "T" + to_string(nPeriods) + "_v" + to_string(i + 1) + extension;
			ofstream instanceFile;
			CustomerIRPDB db(filename, true);

			vector <double> val;
			//db->writeInstanceToFile(instanceFile, filename);

				constructionHeurestic(db, val);


				for (int j = 0; j < 6; j++) {
					Solution[i][j] = val[j];
				}

				nRoutesCon = nRoutesCon + val[6];



				IRP model(db, false);




				//model.addValidIneq();
				IRP::Solution *sol = model.solveModel();
				//sol->printSolution(model);
				//sol->print(model, "IRPexact" + to_string(i));
				//((sol->printSolution(model);

				nRoutesEx = nRoutesEx + sol->getNumberOfRoutes(model);

				Solution[i][6] = sol->getObjective(&model);
				Solution[i][7] = sol->getTransportationCost(&model);
				Solution[i][8] = sol->getHoldingCost(&model);


			}

			string row = "$C_" + to_string(nCustomers) + "V_" + to_string(ModelParameters::nVehicles) + "T_" + to_string(nPeriods) + "$ ";
			row = row + calcStatistics(Solution, size, nRoutesEx / size, nRoutesCon / size);
			//cout << row;



			string s[] = { "IRP relaxed:\t", "Trans:\t\t", "Hold:\t\t", "\nConstr obj:\t", "Trans:\t\t", "Hold:\t\t", "\nExact obj:\t", "Trans:\t\t", "Hold:\t\t" };
			for (int j = 0; j < 9; j++) {
				printf("\n");
				cout << s[j] << ": ";
				for (int i = 0; i < size; i++) {
					printf("%0.f\t", Solution[i][j]);
				}

			}
		}
		
	
			return 0;


	}
