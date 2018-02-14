#pragma once
#include "Customer.h"
class CustomerIRP :
	public Customer
{
private:
	int * HoldingCost;
	int * LowerLimit;
	int * UpperLimit;
	int ** Demand;
	int *InitInventory;
public:
	CustomerIRP(int id, int* h, int*l, int *u, int ** dem, int * init, int x, int y);
	CustomerIRP(int id, int* h, int*l, int *u, int ** dem, int * init);

	~CustomerIRP();

	int getHoldCost(int);
	int getUpperLimit(int);
	int getLowerLimit(int);
	int getDemand(int period, int indicator);
	int getInitInventory(int indicator);
};

