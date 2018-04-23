#include "stdafx.h"
#include <algorithm>
#include "NodeInstance.h"
#include <fstream>
#include <time.h>
using namespace ::std;



NodeInstance::NodeInstance(int id, int posX, int posY, int nPeriods, int initial, int holdingCost, vector<int> demand)
	:
	NodeID(id),
	PosX(posX),
	PosY(posY),
	nPeriods(nPeriods),
	InitInventory(initial),
	HoldingCost(holdingCost),
	Demand(Demand)
{
}

NodeInstance::NodeInstance(int nodeId, int posX, int posY)
	:
	NodeID(nodeId),
	PosX(posX),
	PosY(posY)
{
}

//Create random node
NodeInstance::NodeInstance(int id, int randSeed)
	:
	NodeID(id)
{
	srand(time(0) + randSeed);
	PosX = (rand() % 100 + 0) - 50;
	PosY = (rand() % 100 + 0) - 50;

	Demand.resize(nPeriods + 1);

	srand(time(0) + randSeed);

	HoldingCost= rand() % (ModelParameters::HoldingCostHigh - ModelParameters::HoldingCostLow) + ModelParameters::HoldingCostLow;
	
	int maxDemand = 0;

	//for delivery nodes
	if (isDelivery()){
		for (int t = 1; t <= nPeriods; t++) {
			Demand[t] = rand() % (ModelParameters::DemandDelHigh - ModelParameters::DemandDelLow) + ModelParameters::DemandDelLow;

			if (Demand[t] >= maxDemand) {
				maxDemand = Demand[t];
			}
		
		}

	 
		LowerLimit = ModelParameters::LBDel;

		do {
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

int NodeInstance::getId()
{
	return NodeID;
}

void NodeInstance::print()
{
	printf("\nNodeInstance Id: %d, x: %.0f, y: %.0f", getId(), getXpos(), getYpos());
}

