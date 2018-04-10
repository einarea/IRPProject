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

using namespace std;
using namespace DataSets;

//Solution struct to hold solutions


/*The construction heurestic solves an IRP with relaxed arc variables(x_ijt).
The solution provides information about which customers are visited in each period
and how much that are picked up/delivered at each customer.
For each time period a VRP is solved exactly for the visited nodes. */

void constructionHeurestic(CustomerIRPDB & db) {

	bool relaxArcVariables = true;
	IRP modelIRP(db, relaxArcVariables, false);

	IRP::Solution * prevSol = 0;
	IRP::Solution *solIRP = 0;
	double ** VisitMatrix;
	double ** changeMatrix;
	int numOfNodes = modelIRP.getNumOfNodes();
	int nPeriods = modelIRP.getNumOfPeriods();



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
			modelIRP.getVisitedCustomers(t, customers);
			modelIRP.getDemand(t, demand, customers);
			if (customers.size() >= 1) {
				//Initialize a VRP database with the demand and customers
				vector <IRP::NodeIRP*> nodes;
				nodes = sol->getVisitedNodes(t);
				//CustomerVRPDB VRPdatabase(nodes, *model.getMap());

				CustomerVRPDB VRPdatabase(demand, customers);
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



		VisitMatrix = sol->getVisitedMatrix();

		if (prevSol != 0) {
			changeMatrix = modelIRP.getVisitDifference(sol, prevSol);
			modelIRP.updateTabuMatrix(changeMatrix);
		}

		//Add visit constraint
		modelIRP.addVisitConstraint(VisitMatrix, ModelParameters::ForceChanges);

		cout << "\n\n Objective:" << sol->getObjective() << "\n";
		prevSol = sol;
	}
		
}//End construction heurestic



int main()
{

	//Instance type
	int nCustomers = 6;
	int nPeriods = 5;
	int version = 1;

	//To create new instances
	//CustomerIRPDB *db = new CustomerIRPDB(nCustomers, nPeriods);

	//Open file
	string extension(".txt");
	string filename = "3 vehicles/C" + to_string(nCustomers) + "T" + to_string(nPeriods) + "_v" + to_string(version) + extension;
	ofstream instanceFile;
	CustomerIRPDB db(filename, true);
	
	//Write instance to a new file
	//db->writeInstanceToFile(instanceFile, filename);

	//Initialize solutions
	
	//Heurestic
	constructionHeurestic(db);


	//Exact model, false means no relaxation, true relaxes arc variables
	IRP model(db, false);

	//model.addValidIneq();
	IRP::Solution *sol = model.solveModel();
	sol->printSolution();

	return 0;
}
