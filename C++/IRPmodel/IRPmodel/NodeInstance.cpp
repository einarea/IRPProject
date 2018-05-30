#include "stdafx.h"
#include <algorithm>
#include "NodeInstance.h"
#include <fstream>
#include <time.h>
using namespace ::std;





void NodeInstance::randomQuantities(int randSeed)
{
	Demand.resize(nPeriods + 1);

	srand(time(0) + randSeed);

	HoldingCost = rand() % (ModelParameters::HoldingCostHigh - ModelParameters::HoldingCostLow) + ModelParameters::HoldingCostLow;

	int maxDemand = 0;

	//for delivery nodes
	if (isDelivery()) {
		for (int t = 1; t <= nPeriods; t++) {
			Demand[t] = rand() % (ModelParameters::DemandDelHigh - ModelParameters::DemandDelLow) + ModelParameters::DemandDelLow;

			if (Demand[t] >= maxDemand) {
				maxDemand = Demand[t];
			}

		}


		LowerLimit = ModelParameters::LBDel;

		do {
			srand(time(0) + randSeed);
			UpperLimit = rand() % (ModelParameters::UBDelHigh - ModelParameters::UBDelLow) + ModelParameters::UBDelLow;

		} while (UpperLimit - LowerLimit <= maxDemand);

		InitInventory = rand() % (UpperLimit - LowerLimit) + LowerLimit;
	}

	//For pickupnodes
	else {
		for (int t = 1; t <= nPeriods; t++) {
			Demand[t] = rand() % (ModelParameters::DemandPickHigh - ModelParameters::DemandPickLow) + ModelParameters::DemandPickLow;

			if (Demand[t] >= maxDemand) {
				maxDemand = Demand[t];
			}

		}
		LowerLimit = ModelParameters::LBPick;
		do {
			UpperLimit = rand() % (ModelParameters::UBPickHigh - ModelParameters::UBPickLow) + ModelParameters::UBPickLow;
		} while (UpperLimit - LowerLimit <= maxDemand);

		InitInventory = rand() % (UpperLimit - LowerLimit) + LowerLimit;
	}
}

bool NodeInstance::inArcSet(const NodeInstance * n) const
{
	//no arc to itself
	if (*this == *n)
		return false;

	if (ModelParameters::Simultaneous) {
		//No arcs from depot to pickup
		if (getId() == 0 && !n->isDelivery())
			return false;

		//No into pickup from pickup
		if (!isDelivery() && !n->isDelivery())
			return false;

		//no arcs out from delivery unless to colocated node
		else if (getId()!= 0 && isDelivery() && !isColocated(n))
			return false;
	}

	for (auto m : ForbiddenNodes) {
		if (m == n)
			return false;
	}

	return true;
}


bool NodeInstance::operator==(const NodeInstance & node) const
{
	if (this->NodeID == node.NodeID)
		return true;
	else
		return false;
}

NodeInstance::NodeInstance(int id, bool del, int posX, int posY, int nPer, int initial, int holdingCost, int upperLim, int lowerLim, vector<int> demand)
	:
	NodeID(id),
	Delivery(del),
	PosX(posX),
	PosY(posY),
	nPeriods(nPer),
	InitInventory(initial),
	HoldingCost(holdingCost),
	UpperLimit(upperLim),
	LowerLimit(lowerLim),
	Demand(demand)
{
}

NodeInstance::NodeInstance(int nodeId, bool Del, int posX, int posY, int nPer, int randSeed)
	:
	NodeID(nodeId),
	Delivery(Del),
	PosX(posX),
	PosY(posY),
	nPeriods(nPer)

{
	randomQuantities(randSeed);
}

//Create random node
NodeInstance::NodeInstance(int id, bool del, int nPer, int randSeed, int positionType)
	:
	NodeID(id),
	Delivery(del),
	nPeriods(nPer)
{
	srand(time(0) + randSeed);
	
	if (positionType == CLOSE_DEPOT) {
		double x;
		double y;
		x = (rand() % 50 + 0);
		y = (rand() % 50 + 0);

		//map closer to depot by convex function
		PosX =  ceil( 50/pow(50,1.5) * pow(x, 1.5));
		PosY = ceil( 50 / pow(50, 1.5) * pow(y, 1.5));

		if (rand() % 10 + 0 >= 5)
			PosX = -PosX;
		if (rand() % 10 + 0 <= 5)
			PosY = -PosY;
	}

	if (positionType == RANDOM_POS) {
		PosX = (rand() % 100 + 0) - 50;
		PosY = (rand() % 100 + 0) - 50;
	}

	randomQuantities(randSeed);
}

bool NodeInstance::isDelivery() const
{
	return Delivery;
}


bool NodeInstance::isColocated(const NodeInstance * node) const
{
	if (PosX == node->PosX && PosY == node->PosY)
		return true;
	else
		return false;
}

bool NodeInstance::hasArc(NodeInstance * node)
{
	for (NodeInstance * forbidden : ForbiddenNodes)
		if (forbidden->getId() == node->getId())
			return false;

	return true;
}

double NodeInstance::getXpos() const
{
	return this->PosX;
}

double NodeInstance::getYpos() const
{
	return this->PosY;
}

int NodeInstance::getId() const
{
	return NodeID;
}

void NodeInstance::print()
{
	printf("\nNodeInstance Id: %d, x: %.0f, y: %.0f", getId(), getXpos(), getYpos());
}

