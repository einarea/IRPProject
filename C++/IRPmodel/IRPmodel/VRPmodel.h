#pragma once
#include "xprb_cpp.h"
#include <vector>
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
	VRPmodel(const NodeInstanceDB & db, vector<NodeIRP*> nodes, int period);
	void solveModel(Solution * currentSol = 0);
	~VRPmodel();
	void updateSolution(Solution * sol);

	time_t StartTime;
	double SolutionTime;
	time_t lastSolutionFound;

private:
	void clearVariables();
	int Period;
	XPRBprob prob;

	//Variables
	XPRBvar **x;
	XPRBvar *y;
	XPRBvar **loadDelivery;
	XPRBvar **loadPickup;
	XPRBvar *timeVar;
	XPRBvar extraVehicle;

	//Sets
	vector<NodeIRP*> AllNodes;
	vector<NodeIRP*> Nodes;
	vector<NodeIRP*> DeliveryNodes;
	vector<NodeIRP*> PickupNodes;
	
	//Parameters
	int MaxTime;
	int nVehicles;

	XPRSprob oprob;

	//Linear expreassions;
	XPRBexpr objective;
	const NodeInstanceDB &Database;

	//Helper functions
	bool initializeSets();
	bool initializeParameters();
	bool initializeVariables();
	bool formulateProblem();


	
};

