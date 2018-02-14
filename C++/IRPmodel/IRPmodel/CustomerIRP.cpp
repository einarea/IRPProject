#include "stdafx.h"
#include "CustomerIRP.h"




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
	Customer(id),
	HoldingCost(holdCost),
	LowerLimit(lowLim),
	InitInventory(init),
	UpperLimit(uppLim),
	Demand(dem)
{
}

int CustomerIRP::getHoldCost(int indicator)
{
	return this->HoldingCost[indicator];
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
		return  Demand[Customer::PICKUP][period];
	else
		return Demand[Customer::DELIVERY][period];

}

int CustomerIRP::getInitInventory(int indicator)
{
	return this->InitInventory[indicator];
}



CustomerIRP::~CustomerIRP()
{
}
