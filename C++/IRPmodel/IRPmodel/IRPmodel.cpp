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

	//Solve IRP with relaxed arc variables.
	bool relaxArcVariables = true;
	IRP model(db, relaxArcVariables, false);
	//model.addValidIneq();
	IRP::Solution *solIRP = model.solveModel();

	//Vectors to hold visited customers and their demand
	vector<Customer *> customers;
	vector <vector<double>> demand;

	solIRP->printSolution();
	solIRP->print("IRPSolution/irp");
	vector<XPRBcut> subtourCut;
	int idVRP = model.allocateSolution();
	IRP::Solution *solVRP = model.getSolution(idVRP);
	//bool addedCuts = model.sepStrongComponents(subtourCut);

	
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
	solVRP->print("graphSolutions/VRP");
	model.addRoutesToVector();
	model.printRouteMatrix();

	for (int t = 1; t <= model.getNumOfPeriods(); t++) {
		VRPSol[TransCost][t] = solVRP->getTransportationCost(t);
		VRPSol[HoldingCost][t]= solVRP->getHoldingCost(t);
		VRPSol[Routes][t]= solVRP->getNumberOfRoutes(t);
		VRPSol[Objective][t] = solVRP->getTransportationCost(t) + solVRP->getHoldingCost(t);;

		RelSol[TransCost][t] = solIRP->getTransportationCost(t);
		RelSol[HoldingCost][t] = solIRP->getHoldingCost(t);
		RelSol[Routes][t] = solIRP->getNumberOfRoutes(t);
		RelSol[Objective][t] = solIRP->getTransportationCost(t) + solIRP->getHoldingCost(t);;
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


	int Customers[] = { 15, 6, 6 };
	int Periods[] = { 3, 5, 7 };

	vector<double>* ConstructionSolutions;
	vector<double>* RelaxedSolutions;
	vector<double>* ExactSol;
	vector <double> *** SolutionHolder;



	//Number of instance types
	ofstream outputFile;
	outputFile.open("ConstructionVsExact");

	for (int p = 0; p < 2; p++) {


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
			sol->print("graphSolutions/IRPexact" + to_string(i));
			//((sol->printSolution(model);

			//Save data from solution

			for (int t = 1; t <= model.getNumOfPeriods(); t++) {
				ExactSol[TransCost][t] = sol->getTransportationCost(t);
		
				ExactSol[HoldingCost][t] = sol->getHoldingCost(t);
				double a = ExactSol[HoldingCost][t];
				ExactSol[Routes][t] = sol->getNumberOfRoutes(t);
				ExactSol[Objective][t] = sol->getHoldingCost(t) + sol->getTransportationCost(t);
			}

				

				for (auto field : ContentList){
					for (int period = 1; period <= nPeriods; period ++)
					{
						SolutionHolder[Construction][field][period][i] = ConstructionSolutions[field][period];
						SolutionHolder[Exact][field][period][i] = ExactSol[field][period];
					}
				}
				vector <double> a = SolutionHolder[Construction][TransCost][1];
		}//End instance type

			vector <double> a = SolutionHolder[Construction][TransCost][1];
			Calculations CalcObject(SolutionHolder, size, nPeriods);
			double *** CostData = CalcObject.getCostDistribution();
			double *** RouteData = CalcObject.getRouteInformation();
			ofstream textFile;
			textFile.open("DataFromInstances");
			string row = "$C_" + to_string(nCustomers) + "V_" + to_string(ModelParameters::nVehicles) + "T_" + to_string(nPeriods) + "$ ";

			vector<string> colTitle(CostDataList.size());

			colTitle[TransExact] = "$C^T$ &";
			colTitle[HoldExact] = "$C^H$ &";
			colTitle[deltaTrans] = "$\Delta C^T$ &";
			colTitle[deltaHold] = "$\Delta C^T$ &";
			colTitle[deltaHold] = "$\Delta Z &";

			vector<string> rowTitle(nPeriods + 1);

			stringstream stream;
		

			textFile << "\tab\tab\tab\tab";
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
					textFile << stream.str() << "\%\ &";
					stream.str(string());
				}

				stream << fixed << setprecision(2) << RouteData[nVehicles][t][Average];
				textFile << stream.str() << "\&";
				stream.str(string());

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
