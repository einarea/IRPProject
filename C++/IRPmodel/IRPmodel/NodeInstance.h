#pragma once

#include <vector>
#include "ModelParameters.h"
#include "stdafx.h"

using namespace::std;

//Class to store instance information about a node
class NodeInstance
{
	friend class NodeIRPHolder;
	friend class NodeIRP;
	friend class NodeInstanceDB;
	friend class RouteProblem;
	friend class IRP;
private:
	
	int NodeID;
	double PosX;
	double PosY;
	bool Delivery;


	int HoldingCost;
	void randomQuantities(int randSeed);
	int LowerLimit;
	int UpperLimit;
	vector<int> Demand;
	int InitInventory;
	int nPeriods;
	bool inArcSet(NodeInstance * n);
	//Set of forbidden traversals
	vector<NodeInstance*> ForbiddenNodes;


public:

	NodeInstance(int NodeId, bool Del, int posX, int posY, int nPer, int inital, int holdingCost, vector<int> Demand);
	NodeInstance(int NodeId, bool Del, int posX, int posY, int nPer, int randSeed);
	NodeInstance(int id, bool Del, int nPer, int randSeed);
	bool isDelivery();
	bool isColocated(NodeInstance *);
	bool hasArc(NodeInstance * node);
	double getXpos() const;
	double getYpos() const ;
	int getId();	
	void print();

};