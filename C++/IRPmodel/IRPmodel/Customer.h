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
	int ** Demand;
	double posX;
	double posY;
	int *InitInventory;


public:
	Customer(int, int, int, int, int **, int *, int, int);
	Customer(int, int, int, int, int **, int *);

	Customer(int numberOfPeriods);
	double getXpos();
	double getYpos();
	int getHoldCost();
	int getUpperLimit();
	int getLowerLimit();
	int getDemand(int period, int indicator);
	int getInitInventory(int indicator);

	const int static DELIVERY = 0;
	const int static PICKUP = 1;

};