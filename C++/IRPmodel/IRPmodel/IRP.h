#pragma once


#include "xprb_cpp.h"
#include "Map.h"
#include <vector>
using namespace ::dashoptimization;
using namespace::std;

class IRP
{
private:
	//Problem
	XPRBprob prob;

	//Parameters
	int NumOfCustomers;
	int NumOfPeriods;
	int ** DistanceMatrix;
	int ** CostMatrix;
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
	bool initializeParameters();

public:

	IRP(FILE * InstanceFile);
	XPRBprob & getProblem();
	int getNumOfPeriods(IRP * model);
	int getNumOfCustomers();

	~IRP();
};

