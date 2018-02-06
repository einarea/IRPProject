#pragma once


#include "xprb_cpp.h"
#include "Map.h"
#include <vector>
#include "CustomerDB.h"

using namespace ::dashoptimization;
using namespace::std;

class IRP
{
private:
	//Problem
	CustomerDB database;
	Map map;
	XPRBprob prob;

	//Parameters
	static const int TRANSCOST_MULTIPLIER = 1;
	static const int SERVICECOST_MULTIPLIER = 1;

	int NumOfCustomers;
	int NumOfPeriods;
	int nVehicles;
	int ** Dist;
	int ** TransCost;
	int * HoldCost;
	int * InitInventory;
	int * UpperLimit;
	int * LowerLimit;
	int ** Demand;
	//Map map;

	//Variables
	XPRBvar *** x;					//Arc variables
	XPRBvar ** y;					//Customer visits variables
	XPRBvar ** inventory;		
	XPRBvar ** delivery;
	XPRBvar ** pickup;
	XPRBvar *** loadDelivery;
	XPRBvar *** loadPickup;

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
	

public:

	IRP(string);
	XPRBprob & getProblem();
	int getNumOfPeriods(IRP * model);
	int getNumOfCustomers();

	~IRP();
};

