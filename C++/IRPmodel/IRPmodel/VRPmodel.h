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
	XPRBprob* getProblem();
	bool VRPmodel::sepStrongComponents(vector<XPRBcut> & cut);
	int nSubtourCuts = 0;

	time_t StartTime;
	double SolutionTime;
	time_t lastSolutionFound;

	void useIPSubtourElimination(int cuttype);
	vector<vector<int>> subtourIndices;
	vector<vector<int>> matrixValues;
	int CutType;

private:
	void clearVariables();
	int Period;
	XPRBprob prob;

	void buildStrongGraph(vector<NodeStrong*>& graph, bool Depot = false, double weight = -1);
	void addSubtourCut(vector<vector<Node *>>& strongComp, bool &newCut, vector<XPRBcut> &SubtourCut);

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

