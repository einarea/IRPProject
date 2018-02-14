#pragma once
#include "CustomerDB.h"
#include <fstream>
#include <iostream>
#include "CustomerIRP.h"

class CustomerIRPDB :
	public CustomerDB
{
private:
	string getNextToken(string&, string&); //Helper function
	int nPeriods;
public:
	CustomerIRPDB(string Filename);
	~CustomerIRPDB();
	int getHoldCost(int);
	int getUpperLimit(int);
	int getnPeriods();
	CustomerIRP * getCustomer(int id);
	int getInitInventory(int id, int indicator);
	int getLowerLimit(int);
	int getDemand(int id, int period, int INDICATOR);
};

