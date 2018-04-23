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
	friend class IRP;
private:
	static int nPeriods;
	
	int NodeID;
	double PosX;
	double PosY;

	int HoldingCost;
	int LowerLimit;
	int UpperLimit;
	vector<int> Demand;
	int InitInventory;
	int nPeriods;
	//Set of forbidden traversals
	vector<NodeInstance*> ForbiddenNodes;

public:

	NodeInstance(int NodeId, int posX, int posY, int nPeriods, int inital, int holdingCost, vector<int> Demand);
	NodeInstance(int NodeId, int posX, int posY);
	NodeInstance(int id, int randSeed);
	bool isDelivery();
	bool hasArc(NodeInstance * node);
	double getXpos() const;
	double getYpos() const ;
	int getId();	
	void print();

	static const int  DELIVERY = 0;
	static const int PICKUP = 1;

};