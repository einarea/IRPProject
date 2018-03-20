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

double ** getChange(double ** prevVisitedMatrix, double **newVisitedMatrix, int dimensions[]){
	double ** changedMatrix;

	int r = dimensions[0];
	int c = dimensions[1];

	changedMatrix = new double *[r];
	for (int i = 1; i < r; i++) {
		cout << "\n";
		changedMatrix[i] = new double[c];
		for (int j = 1; j < c; j++) {
			double a = newVisitedMatrix[i][j];
			double b = prevVisitedMatrix[i][j];
			changedMatrix[i][j] = newVisitedMatrix[i][j] - prevVisitedMatrix[i][j];
			cout << changedMatrix[i][j] << "\t";
		}
	}

	return changedMatrix;
}

/*The construction heurestic solves an IRP with relaxed arc variables(x_ijt).
The solution provides information about which customers are visited in each period
and how much that are picked up/delivered at each customer.
For each time period a VRP is solved exactly for the visited nodes. */

void constructionHeurestic(CustomerIRPDB & db) {

	bool relaxArcVariables = true;
	IRP model(db, relaxArcVariables, false);

	int numOfNodes = model.getNumOfNodes();
	int nPeriods = model.getNumOfPeriods();
	int dimensions[] = { numOfNodes+1, nPeriods+1 };
	//Initialize visited matrix

	double ** prevVisitedMatrix = 0;
	double ** newVisitedMatrix = 0;
	double ** changeMatrix = 0;
	newVisitedMatrix = new double *[numOfNodes + 1];
	for (int node = 1; node <= numOfNodes; node++) {
		newVisitedMatrix[node] = new double[nPeriods + 1];
		for (int period = 1; period <= nPeriods; period++) {
			newVisitedMatrix[node][period] = -1;
		}
	}
	bool addConstraints = false;

	for (int iteration = 0; iteration <= 10; iteration++) {

		//Constrain the relaxed IRP problem
		XPRBctr addedConstraint;
		if (addConstraints) {
			prevVisitedMatrix = newVisitedMatrix;
			
			addedConstraint = model.addVisitConstraint(prevVisitedMatrix);
		}

		IRP::Solution *solIRP = model.solveModel();

		//Vectors to hold visited customers and their demand
		vector<Customer *> customers;
		vector <vector<double>> demand;


		//solIRP->print("IRPSolution/irp", ModelParameters::X);
		//vector<XPRBcut> subtourCut;
		int idVRP = model.allocateSolution();
		IRP::Solution *solVRP = model.getSolution(idVRP);
		//bool addedCuts = model.sepStrongComponents(subtourCut);

		//Solve a VRP for each period
		for (int t = 1; t <= model.getNumOfPeriods(); t++) {
			model.getVisitedCustomers(t, customers);
			model.getDemand(t, demand, customers);
			if (customers.size() >= 1) {
				//Initialize a VRP database with the demand and customers
				vector <IRP::NodeIRP*> nodes;
				nodes = solVRP->getVisitedNodes(t);
				//CustomerVRPDB VRPdatabase(nodes, *model.getMap());

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

		//Create set of nodes visited
		double ** Visited;
		Visited = new double *[nNodes];
		for (auto node : solVRP->NodeHolder) {
			cout << "\n";
			for (int period = 1; period <= model.getNumOfPeriods(); period++) {
				cout << "\t";
				if (node->getId() != 0) {
					if (node->quantity(period) > 0.01) {
						newVisitedMatrix[node->getId()][period] = 1;
						cout << newVisitedMatrix[node->getId()][period];
					}
					else {
						newVisitedMatrix[node->getId()][period] = 0;
						cout << newVisitedMatrix[node->getId()][period];
					}
				}
			}
		}

		if (addConstraints) {
			changeMatrix = getChange(prevVisitedMatrix, newVisitedMatrix, dimensions);
		}

		addConstraints = true;
		if (addedConstraint.isValid()) {
			model.getProblem()->delCtr(addedConstraint);
		}
	}
		
}//End construction heurestic



int main()
{

	//Instance type
	int nCustomers = 6;
	int nPeriods = 5;
	int version = 2;

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
