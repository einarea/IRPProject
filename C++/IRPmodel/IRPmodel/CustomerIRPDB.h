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
	CustomerIRPDB(string Filename, bool COORDINATE_INPUT);
	~CustomerIRPDB();
	int getHoldCost(int id, int indicator);
	int getUpperLimit(int id, int indicator);
	int getnPeriods();
	CustomerIRP * getCustomer(int id);
	int getInitInventory(int id, int indicator);
	int getLowerLimit(int id, int indicator);
	int getDemand(int id, int period, int INDICATOR);
};

