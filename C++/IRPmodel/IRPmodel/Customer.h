#pragma once

#include <vector>
#include "stdafx.h"

using namespace::std;

class Customer
{
private:
	static int nPeriods;
	
	int CustomerID;
	int HoldingCost;
	int LowerLimit;
	int UpperLimit;
	int * Demand;
	double posX;
	double posY;

public:
	Customer(int, int, int, int, int *, int, int);
	Customer(int, int, int, int, int *);

	Customer(int numberOfPeriods);
	double getXpos();
	double getYpos();
	int getHoldCost();
	


};