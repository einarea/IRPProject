#pragma once


#include "xprb_cpp.h"
#include "MapIRP.h"
#include <vector>
#include "CustomerDB.h"
#include "Node.h"
#include "xprs.h"
#include <string.h>

using namespace ::dashoptimization;
using namespace::std;

class IRP
{
private:
	//Problem
	CustomerIRPDB database;
	MapIRP map;
	XPRBprob prob;
	XPRSprob oprob;		//Express problem to use with cuts

	//Parameters
	bool ARC_RELAXED;

	int NumOfCustomers;
	int NumOfPeriods;
	int ** Dist;
	int ** TransCost;
	int * HoldCost;
	int * InitInventory;
	int * UpperLimit;
	int * LowerLimit;
	int ** Demand;
	int ** TravelTime;
	//Map map;

	//Variables
	XPRBvar *** x;					//Arc variables
	XPRBvar ** y;					//Customer visits variables
	XPRBvar ** inventory;		
	XPRBvar ** delivery;
	XPRBvar ** pickup;
	XPRBvar *** loadDelivery;
	XPRBvar *** loadPickup;
	XPRBvar ** time;


	XPRBexpr objective;
	int ** t;





	//Utility functions
	bool initializeSets();
	bool initializeVariables();
	bool initializeParameters();
	bool formulateProblem();
	void buildGraph(vector <Node> &, int, bool includeDepot);
	void printGraph(vector <Node> &);

	int SolutionCounter; // Tracks number of solutions, solution's ID equals the counter value at the time added
	int getCounter();

	//Forward declaration
	class Solution;

	vector <Solution *> solutions;


public:
	//Sets
	vector  <int> Periods;
	vector  <int> AllPeriods;
	vector  <int> DeliveryNodes;
	vector <int> PickupNodes;
	vector  <int> Nodes;
	vector  <int> AllNodes;
	vector  <int> Depot;
	vector <int> integers1;
	
	//For valid ineq
	double ** ExcessConsumption;
	double ** ExcessProd;

	//Class to store solutions to the instance
	class Solution {
	public:
		Solution();
		Solution(double ** y, double ***x, double **del, double **pic, double ***loadDel, double ***loadPic, double **inv, double ** time);
		int SolID;
		double **ySol;
		double ***xSol;
		double **delSol;
		double **pickSol;
		double	***loadDelSol;
		double ***loadPickSol;
		double **invSol;
		double **timeSol;
		void print(IRP &);

		double getObjective(IRP * instance);
		void printSolution(IRP &instance);
		double getTransportationCost(IRP * instance);
		double getHoldingCost(IRP * instance);
	
	};

	IRP(CustomerIRPDB&, bool);
	//void addSolution(int ** y, int ***x, int **d, int **pic, int ***loadDel, int ***loadPic, int **inv, int ** t);


	void addValidIneq();
	Map * getMap();
	void calculateExcess();
	int allocateSolution();
	bool sepStrongComponents(vector<XPRBcut> &);
	void addSubtourCut(vector<vector <Node>> &, int t, bool &, vector<XPRBcut> &);
	IRP::Solution * solveModel();
	XPRBprob * getProblem();
	CustomerDB * getDB();
	vector <XPRBbasis> SavedBasis;
	int nSubtourCuts;
	int getNumOfPeriods();
	int getNumOfCustomers();
	void getVisitedCustomers(int period, vector <Customer *> &);
	void getDemand(int period, vector<vector<int>> &, vector <Customer *> &);
	Solution * getSolution(int id);
	void printBounds();

	~IRP();
};

