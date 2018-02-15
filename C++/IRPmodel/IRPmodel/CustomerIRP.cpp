#include "stdafx.h"
#include "CustomerIRP.h"
#include <time.h>




CustomerIRP::CustomerIRP(int id, int * holdCost, int * lowLim, int * uppLim, int ** dem, int * init, int coordinateX, int coordinateY)
	:
	Customer(id, coordinateX, coordinateY),
	HoldingCost(holdCost),
	LowerLimit(lowLim),
	UpperLimit(uppLim),
	Demand(dem),
	InitInventory(init)
{
}

//randomly generated coordinates
CustomerIRP::CustomerIRP(int id, int * holdCost, int * lowLim, int * uppLim, int ** dem, int * init)
	:
	Customer(id, 1),
	HoldingCost(holdCost),
	LowerLimit(lowLim),
	InitInventory(init),
	UpperLimit(uppLim),
	Demand(dem)
{
}

//randomly generate customer
CustomerIRP::CustomerIRP(int id, int periods, int randSeed)
	:
	Customer(id, randSeed),
	nPeriods(periods),
	HoldingCost(new int[2]),
	LowerLimit(new int[2]),
	UpperLimit(new int[2]),
	InitInventory(new int[2]),
	Demand(new int * [2])
{	
	srand(time(0)+randSeed);
	if (ModelParameters::DemandDel >= ModelParameters::UBDel ||
		ModelParameters::DemandPick >= ModelParameters::UBPick) {
		printf("Error: Demand parameter cannot exceed upper limit parameter in Model parameters");
		return;
	}
		HoldingCost[DELIVERY] = (rand() % 10 + 1)*ModelParameters::HoldingCost;
		HoldingCost[PICKUP] = (rand() % 10 + 1)*ModelParameters::HoldingCost;
		

		Demand[DELIVERY] = new int[periods];
		Demand[PICKUP] = new int[periods];
		int maxDemandDel = 0;
		int maxDemandPic = 0;

		for (int t = 0; t < periods; t++) {
			Demand[DELIVERY][t] = (rand() % 10 + 1)*ModelParameters::DemandDel;
			Demand[PICKUP][t] = (rand() % 10 + 1)*ModelParameters::DemandPick;
			if (Demand[DELIVERY][t] >= maxDemandDel) {
				maxDemandDel = Demand[DELIVERY][t];
			}
			if (Demand[PICKUP][t] >= maxDemandPic) {
				maxDemandPic = Demand[PICKUP][t];
			}
		}

		LowerLimit[DELIVERY] = (rand() % 10 + 1)*ModelParameters::LBDel;
		
		do {
			UpperLimit[DELIVERY] = LowerLimit[DELIVERY] + (rand() % 10 + 1)*ModelParameters::UBDel;
			
		}
		while (UpperLimit[DELIVERY] - LowerLimit[DELIVERY] <= maxDemandDel);


		int a = ModelParameters::LBPick;
		LowerLimit[PICKUP] = (rand() % 10 + 1)*ModelParameters::LBPick;	
		do {
			UpperLimit[PICKUP] = LowerLimit[PICKUP] + (rand() % 10 + 1)*ModelParameters::UBPick;
		} while (UpperLimit[PICKUP] - LowerLimit[PICKUP] <= maxDemandPic);

		do {
			InitInventory[DELIVERY] = (rand() % 10 + 1)*ModelParameters::InitInventoryDel;
		} while (InitInventory[DELIVERY] <= LowerLimit[DELIVERY] || InitInventory[DELIVERY] >= UpperLimit[DELIVERY]);

		do {
			InitInventory[PICKUP] = (rand() % 10 + 1)*ModelParameters::InitInventoryPick;
		} while (InitInventory[PICKUP] <= LowerLimit[PICKUP] || InitInventory[PICKUP] >= UpperLimit[PICKUP]);
}


int CustomerIRP::getHoldCost(int indicator)
{
	return this->HoldingCost[indicator];
}

void CustomerIRP::print()
{
	Customer::print();
	printf("\nDelivery:\n\tHolding cost: %d\n\tLower Limit: %d\n\tUpper Limit: %d\n\tInit inventory: %d\n",
		HoldingCost[DELIVERY], LowerLimit[DELIVERY], UpperLimit[DELIVERY], InitInventory[DELIVERY]);

	printf("\nPickup:\n\tHolding cost: %d\n\tLower Limit: %d\n\tUpper Limit: %d\n\tInit inventory: %d\n",
		HoldingCost[PICKUP], LowerLimit[PICKUP], UpperLimit[PICKUP], InitInventory[PICKUP]);

	for (int t = 1; t <= nPeriods; t++) {
		printf("Period %d: Delivery: %d\tPickup: %d\n", t, Demand[DELIVERY][t-1], Demand[PICKUP][t-1]);
	}
}

int CustomerIRP::getUpperLimit(int indicator)
{
	return UpperLimit[indicator];
}

int CustomerIRP::getLowerLimit(int indicator)
{
	return LowerLimit[indicator];
}



int CustomerIRP::getDemand(int period, int indicator)
{
	if (indicator == Customer::PICKUP)
		return  Demand[Customer::PICKUP][period-1];
	else
		return Demand[Customer::DELIVERY][period-1];

}

int CustomerIRP::getInitInventory(int indicator)
{
	return this->InitInventory[indicator];
}



CustomerIRP::~CustomerIRP()
{
}
