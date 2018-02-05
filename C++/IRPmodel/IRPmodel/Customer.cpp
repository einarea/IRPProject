#include "stdafx.h"
#include "Customer.h"
#include <fstream>

Customer::Customer(int id, int holdCost, int lowLim, int uppLim, int * dem, int coordinateX, int coordinateY)
	:
	CustomerID(id),
	HoldingCost(holdCost),
	LowerLimit(lowLim),
	UpperLimit(uppLim),
	Demand(dem),
	posX(coordinateX),
	posY(coordinateY)
{
}

Customer::Customer(int id, int holdCost, int lowLim, int uppLim, int * dem)
	:
	CustomerID(id),
	HoldingCost(holdCost),
	LowerLimit(lowLim),
	UpperLimit(uppLim),
	Demand(dem)
{
	posX = rand() % 100 + 1;
	posY = rand() % 100 + 1;
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

