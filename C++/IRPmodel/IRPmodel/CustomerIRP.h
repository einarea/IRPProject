#pragma once
#include "Customer.h"
class CustomerIRP :
	public Customer
{
private:
	int HoldingCost;
	int LowerLimit;
	int UpperLimit;
	int ** Demand;
	int *InitInventory;
public:
	CustomerIRP(int, int, int, int, int **, int *, int, int);
	CustomerIRP(int, int, int, int, int **, int *);

	~CustomerIRP();

	int getHoldCost();
	int getUpperLimit();
	int getLowerLimit();
	int getDemand(int period, int indicator);
	int getInitInventory(int indicator);
};

