#pragma once


#include "xprb_cpp.h"
#include "Map.h"
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
	CustomerDB database;
	Map map;
	XPRBprob prob;
	XPRSprob oprob;		//Express problem to use with cuts

	//Parameters
	static const int TRANSCOST_MULTIPLIER = 4;
	static const int SERVICECOST_MULTIPLIER = 2;
	static const int TRAVELTIME_MULTIPLIER = 1;
	const int maxTime = 2000;
	int Capacity = 1000;
	int nVehicles = 4;
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

	//Sets
	vector  <int> Periods;
	vector  <int> AllPeriods;
	vector  <int> DeliveryNodes;
	vector <int> PickupNodes;
	vector  <int> Nodes;
	vector  <int> AllNodes;
	vector  <int> Depot;
	vector <int> integers1;

	//Utility functions
	bool initializeSets();
	bool initializeVariables();
	bool inArcSet(int, int);
	bool initializeParameters();
	bool formulateProblem();
	void buildGraph(vector <Node> &, int);
	void printGraph(vector <Node> &);
	
	

public:
	static const int DELIVERY = 0;
	static const int PICKUP = 1;

	IRP(CustomerDB&, bool);
	void sepStrongComponents();
	void addSubtourCut(vector<vector <Node>> &, int t);
	void solveLP();
	XPRBprob * getProblem();
	CustomerDB * getDB();
	int getNumOfPeriods();
	int getNumOfCustomers();
	void getVisitedCustomers(int period, vector <CustomerIRP *> &);
	void getDemand(int period, vector<vector<int>> &, vector <CustomerIRP *> &);

	~IRP();
};

