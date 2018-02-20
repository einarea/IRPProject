// IRPmodel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CustomerDB.h"
#include "CustomerVRPDB.h"
#include "IRP.h"
#include "VRPmodel.h"

#include <vector>
#include <cmath>
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
	IRP model(db, relaxArcVariables);
	model.solveModel();

	//Vectors to hold visited customers and their demand
	vector<Customer *> customers;
	vector <vector<int>> demand;

	//Allocate new solution
	int id = model.allocateSolution();
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
				VRPmodel modelVRP(VRPdatabase, *model.getMap());
				modelVRP.solveModel();

				//Add partial solution to IRP
				modelVRP.addToIRPSolution(t, solVRP);
				solVRP->printSolution(model);
				customers.clear();
			}
	}
	values.push_back(solIRP->getObjective(&model));
	values.push_back(solIRP->getTransportationCost(&model));
	values.push_back(solIRP->getHoldingCost(&model));

	values.push_back(solVRP->getObjective(&model));
	values.push_back(solVRP->getTransportationCost(&model));
	values.push_back(solVRP->getHoldingCost(&model));
	//solIRP->printSolution(model);
	
}//End construction heurestic




int main()
{


	//Initialize database
	const int size = 5;
	double Solution[4][size];
	for (int i = 1; i < size; i++) {
		CustomerIRPDB *db = new CustomerIRPDB(4, 3);
		
		/*vector <double> val;
		constructionHeurestic(*db, val);

		for (int j = 0; j < 6; j++) {
			Solution[j][i] = val[j];
		}
		*/

		string extension(".txt");
		string filename = "customerDataTest"+ to_string(i) + extension;
		ofstream instanceFile;

		
		db->writeInstanceToFile(instanceFile, filename);

		IRP model(*db, true);
		model.addValidIneq();
		IRP::Solution *sol = model.solveModel();
	
		Solution[0][i] = sol->getObjective(&model);
		Solution[1][i] = sol->getTransportationCost(&model);
		Solution[2][i] = sol->getHoldingCost(&model);
	}

	string s[] = /*{ "IRP relaxed:\t", "Trans:\t\t", "Hold:\t\t", "\nConstr obj:\t", "Trans:\t\t", "Hold:\t\t" };*/{ "\nExact obj:\t", "Trans:\t\t", "Hold:\t\t"};
	for (int i = 0; i < 9; i++) {
		printf("\n");
		cout<<s[i]<<": ";
		for (int j = 0; j < size; j++) {
			printf("%0.f\t", Solution[i][j]);
		}
	}
	
	//database->writeInstanceToFile(instanceFile, filename);
	//CustomerIRPDB database("CustomerData2.txt", true);
	
	//IRP model(database, false);
	//IRP::Solution *sol = model.solveModel();
	//sol->printSolution(model);
	
	return 0;
}


