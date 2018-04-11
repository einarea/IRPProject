#pragma once
#include "xprb_cpp.h"
#include "Map.h"
#include <vector>
#include "CustomerVRPDB.h"
#include "NodeIRP.h"
#include "xprs.h"
#include <string.h>
#include "IRP.h"
#include "ModelBase.h"
#include "graphAlgorithm.h"

using namespace dashoptimization;
class VRPmodel
{
public:
	VRPmodel(CustomerVRPDB & db, Map &map, int Capacity);
	void solveModel();
	~VRPmodel();
	void addToIRPSolution(int t, IRP::Solution * sol, IRP &);
	void addRoutesToIRP(IRP& instance, int t, IRP::Solution * sol);

private:

	CustomerVRPDB database;
	Map map;
	XPRBprob prob;

	//Sets
	vector <int> DeliveryNodes;
	vector <int> PickupNodes;
	vector <int> Nodes;
	vector <int> AllNodes;
	vector <int> Depot;

	//Variables
	XPRBvar **x;
	XPRBvar *y;
	XPRBvar **loadDelivery;
	XPRBvar **loadPickup;
	XPRBvar *time;
	XPRBvar extraVehicle;
	XPRBvar **pCapacity; //Penalty capacity

	//Parameters
	int Capacity;
	int MaxTime;
	int nVehicles;
	int * Demand;
	int ** TransCost;
	int ** TravelTime;

	int startTime;
	XPRSprob oprob;

	//Linear expreassions;
	XPRBexpr objective;



	//Helper functions
	bool initializeSets();
	bool initializeParameters();
	bool initializeVariables();
	bool formulateProblem();


	
};

