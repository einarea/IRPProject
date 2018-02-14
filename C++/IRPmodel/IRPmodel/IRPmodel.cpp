// IRPmodel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CustomerDB.h"
#include "CustomerVRPDB.h"
#include "IRP.h"
#include "VRPmodel.h"

/*The construction heurestic solves an IRP with relaxed arc variables(x_ijt).
The solution provides information about which customers are visited in each period
and how much that are picked up/delivered at each customer.
For each time period a VRP is solved exactly for the visited nodes. */
void constructionHeurestic(CustomerIRPDB & db) {

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

	solVRP->printSolution(model);
	solIRP->printSolution(model);
	
}//End construction heurestic




int main()
{
	//Initialize database
	CustomerIRPDB database("CustomerData.txt");
	constructionHeurestic(database);
//	IRP model(database, false);
//	IRP::Solution *sol = model.solveModel();
//	sol->printSolution(model);
	
	return 0;
}


