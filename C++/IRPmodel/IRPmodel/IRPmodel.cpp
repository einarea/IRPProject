// IRPmodel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CustomerDB.h"
#include "CustomerVRPDB.h"
#include "IRP.h"
#include "VRPmodel.h"

void constructionHeurestic(CustomerIRPDB & db) {

	IRP model(db, true);
	model.solveLP();

	vector<Customer *> customers;
	vector <vector<int>> demand;
	//Solve a vrp for each period
	for (int t = 1; t <= model.getNumOfPeriods(); t++) {
		model.getVisitedCustomers(t, customers);
		if (customers.size() >= 1) {
			model.getDemand(t, demand, customers);
			CustomerVRPDB VRPdatabase(demand, customers);
			VRPmodel modelVRP();
			}
		customers.clear();
	}
}//End construction heurestic


int main()
{
	CustomerIRPDB database("CustomerData.txt");
	constructionHeurestic(database);
	
	return 0;
}


