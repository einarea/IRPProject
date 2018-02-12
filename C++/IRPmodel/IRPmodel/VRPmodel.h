#pragma once
#include "xprb_cpp.h"
#include "Map.h"
#include <vector>
#include "CustomerVRPDB.h"
#include "Node.h"
#include "xprs.h"
#include <string.h>
#include "ModelBase.h"

using namespace dashoptimization;
class VRPmodel
{
public:
	VRPmodel(CustomerVRPDB db);

private:

	CustomerVRPDB database;
	Map map;
	XPRBprob prob;

	//Sets
	vector <int> DeliveryNodes;
	vector <int> PickupNodes;
	vector <int> Nodes;
	vector <int> AllNodes;

	//Variables
	XPRBvar **x;
	XPRBvar *y;
	XPRBvar **loadDelivery;
	XPRBvar **loadPikcup;

	//Parameters
	int Capacity;
	int * Demand;
	int ** Dist;
	int ** TransCost;


	//Helper functions
	bool initializeSets();
	bool initializeParameters();
	bool initializeVariables();
	bool formulateProblem();

	~VRPmodel();
};

