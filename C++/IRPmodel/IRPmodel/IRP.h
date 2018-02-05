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
	static const int TRANSCOST_MULTIPLIER = 10;
	static const int SERVICECOST_MULTIPLIER = 5;

	int NumOfCustomers;
	int NumOfPeriods;
	int ** Dist;
	int ** TransCost;
	int ** HoldCost;
	//Map map;

	//Variables
	XPRBvar *** x;					//Arc variables
	XPRBvar ** y;					//Customer visits variables
	XPRBvar ** inventory;		
	XPRBvar ** delivery;
	XPRBvar ** pickup;
	XPRBvar *** loadDelivery;
	XPRBvar *** loadPickup;
	int ** t;

	//Sets
	vector  <int> Periods;
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
	

public:

	IRP(string);
	XPRBprob & getProblem();
	int getNumOfPeriods(IRP * model);
	int getNumOfCustomers();

	~IRP();
};

