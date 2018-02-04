#include "stdafx.h"
#include "Customer.h"
#include <fstream>

Customer::Customer(int holdCost, int lowLim, int uppLim, int * dem, int coordinateX, int coordinateY)
	:
	HoldingCost(holdCost),
	LowerLimit(lowLim),
	UpperLimit(uppLim),
	Demand(dem),
	posX(coordinateX),
	posY(coordinateY)
{
}

//Randomly generated customer, default
Customer::Customer(int numberOfPeriods)
{
	HoldingCost = rand();
	LowerLimit = 0;
	UpperLimit = 0;

}

