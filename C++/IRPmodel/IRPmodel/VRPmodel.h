#pragma once
#include "xprb_cpp.h"
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
	VRPmodel(NodeInstanceDB & db, vector<NodeIRP*> nodes, int Capacity);
	VRPmodel(CustomerVRPDB & db, Map &map, int Capacity);
	void solveModel();
	~VRPmodel();
	void addToIRPSolution(int t, IRP::Solution * sol, IRP &);
	void addRoutesToIRP(IRP& instance, int t, IRP::Solution * sol);

private:
	//Sets
	vector<NodeIRP*> AllNodes;
	vector<NodeIRP*> DeliveryNodes;
	vector<NodeIRP*> PickupNodes;
	vector<NodeIRP*> Nodes;

	XPRBprob prob;




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
	NodeInstanceDB Database;



	//Helper functions
	bool initializeSets();
	bool initializeParameters();
	bool initializeVariables();
	bool formulateProblem();


	
};

