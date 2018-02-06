#include "stdafx.h"
#include "Customer.h"
#include <fstream>

Customer::Customer(int id, int holdCost, int lowLim, int uppLim, int ** dem, int * init, int coordinateX, int coordinateY)
	:
	CustomerID(id),
	HoldingCost(holdCost),
	LowerLimit(lowLim),
	UpperLimit(uppLim),
	Demand(dem),
	InitInventory(init),
	posX(coordinateX),
	posY(coordinateY)
{
}

Customer::Customer(int id, int holdCost, int lowLim, int uppLim, int ** dem, int * init)
	:
	CustomerID(id),
	HoldingCost(holdCost),
	LowerLimit(lowLim),
	InitInventory(init),
	UpperLimit(uppLim),
	Demand(dem)
{
	posX = (rand() % 1000 + 1)/10;
	posY = (rand() % 1000 + 1)/10;
}


//Randomly generated customer, default
Customer::Customer(int numberOfPeriods)
{
	HoldingCost = rand();
	LowerLimit = 0;
	UpperLimit = 0;

}

double Customer::getXpos()
{
	return this->posX;
}

double Customer::getYpos()
{
	return this->posY;
}

int Customer::getHoldCost()
{
	return this->HoldingCost;
}

int Customer::getUpperLimit()
{
	return UpperLimit;
}

int Customer::getLowerLimit()
{
	return LowerLimit;
}

int Customer::getDemand(int period, int indicator)
{
	if (indicator == Customer::PICKUP)
		return  Demand[Customer::PICKUP][period];
	else
		return Demand[Customer::DELIVERY][period];

}

int Customer::getInitInventory(int indicator)
{
	return this->InitInventory[indicator];
}

