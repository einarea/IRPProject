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

	double getDistance(const NodeInstance * node) const;
	int HoldingCost;
	void randomQuantities(int randSeed);
	int LowerLimit;
	int UpperLimit;
	vector<int> Demand;
	int InitInventory;
	int nPeriods;
	bool inArcSet(const NodeInstance * n) const;
	//Set of forbidden traversals
	vector<NodeInstance*> ForbiddenNodes;

	double getTransCost(const NodeInstance * node) const;
	double getTravelTime(const NodeInstance* node) const;


public:

	NodeInstance(int NodeId, bool Del, int posX, int posY, int nPer, int inital, int holdingCost, int upperLim, int lowerLim, vector<int> Demand);
	NodeInstance(int NodeId, bool Del, int posX, int posY, int nPer, int randSeed);
	NodeInstance(int id, bool Del, int nPer, int randSeed);
	bool isDelivery() const;
	bool isColocated(NodeInstance *) const;
	bool hasArc(NodeInstance * node);
	double getXpos() const;
	double getYpos() const ;
	int getId() const;	
	void print();


	//Operator overloading
	bool operator==(const NodeInstance &) const;
	bool operator!=(const NodeInstance &node) const
	{
		return !(*this == node); // invokes Array::operator==
	}


};


inline double NodeInstance::getTransCost(const NodeInstance * node) const
{
	return getDistance(node) * ModelParameters::TRANSCOST_MULTIPLIER + ModelParameters::SERVICECOST_MULTIPLIER;
}


inline double NodeInstance::getTravelTime(const NodeInstance * node) const
{
	return getDistance(node) * ModelParameters::TRAVELTIME_MULTIPLIER + ModelParameters::SERVICETIME;
}


inline double NodeInstance::getDistance(const NodeInstance * node) const
{
	return (int)floor(sqrt(pow(PosX - node->PosX, 2) + pow(PosY - node->PosY, 2)));
}