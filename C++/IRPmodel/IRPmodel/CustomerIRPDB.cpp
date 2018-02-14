#include "stdafx.h"
#include "CustomerIRPDB.h"


CustomerIRPDB::CustomerIRPDB(string FileName, bool COORDINATE_INPUT)
{
	ifstream CustomerRecords(FileName, ios::in);

	if (!CustomerRecords)
	{
		cerr << "Customer records file could not be opened" << endl;
		exit(1);
	}


	string line;

	//Get number of periods
	getline(CustomerRecords, line);
	string delimiter = "=";
	getNextToken(line, delimiter);
	nPeriods = stoi(getNextToken(line, delimiter));

	//check file for errors
	getline(CustomerRecords, line);

	int CustomerID = 1;
	int * HoldingCost;
	int ** Demand;
	int * InitInventory;
	int * LowerLimit;
	int * UpperLimit;


	while (!getline(CustomerRecords, line).eof()) {

		size_t pos = 0;
		string token;
		delimiter = "\t";

		//Inventory parameters
		HoldingCost = new int[2];
		LowerLimit = new int[2];
		UpperLimit = new int[2];

		for (int i = 0; i < 2; i++) {
			HoldingCost[i] = stoi(getNextToken(line, delimiter));
		}

		for (int i = 0; i < 2; i++) {
			LowerLimit[i] = stoi(getNextToken(line, delimiter));
		}
		for (int i = 0; i < 2; i++) {
			UpperLimit[i] = stoi(getNextToken(line, delimiter));
		}

		//Dynamically allocated array
		Demand = new int *[2]; //pickup and delivery demand

		for (int i = 0; i < 2; i++) {
			Demand[i] = new int[nPeriods];
			for (int t = 0; t < nPeriods; t++) {
				Demand[i][t] = stoi(getNextToken(line, delimiter));
			}
		}

		//Inital inventory
		InitInventory = new int[2]; //pickup and delivery demand
		for (int i = 0; i < 2; i++) {
			InitInventory[i] = stoi(getNextToken(line, delimiter));
		}

		//If datafile provide coordinates
		if (COORDINATE_INPUT) {
			int x = stoi(getNextToken(line, delimiter));
			int y = stoi(getNextToken(line, delimiter));
			CustomerIRP * cust = new CustomerIRP(CustomerID, HoldingCost, LowerLimit, UpperLimit, Demand, InitInventory, x, y);
			Customers.push_back(cust);
			CustomerID++;
		}

		else {
			CustomerIRP * cust = new CustomerIRP(CustomerID, HoldingCost, LowerLimit, UpperLimit, Demand, InitInventory);
			Customers.push_back(cust);
			CustomerID++;
		}
	}
}

int CustomerIRPDB::getnPeriods()
{
	return nPeriods;
}

int CustomerIRPDB::getHoldCost(int id, int indicator)
{
	return getCustomer(id)->getHoldCost(indicator);
}

int CustomerIRPDB::getUpperLimit(int id, int indicator)
{
	return getCustomer(id)->getUpperLimit(indicator);
}


int CustomerIRPDB::getInitInventory(int id, int indicator)
{
	return getCustomer(id)->getInitInventory(indicator);
}

int CustomerIRPDB::getLowerLimit(int id, int indicator)
{
	
	return getCustomer(id)->getLowerLimit(indicator);
}

CustomerIRP * CustomerIRPDB::getCustomer(int id) {
	CustomerIRP * derivedPtr = static_cast <CustomerIRP *> (CustomerDB::getCustomer(id));
	if (derivedPtr != 0)
		return derivedPtr;
}

int CustomerIRPDB::getDemand(int id, int period, int indicator)
{
	return getCustomer(id)->getDemand(period - 1, indicator);
}

string CustomerIRPDB::getNextToken(string &str, string& delimiter)
{
	size_t pos = 0;
	pos = str.find(delimiter);
	string token = str.substr(0, pos);
	str.erase(0, pos + delimiter.length());
	return token;
}



CustomerIRPDB::~CustomerIRPDB()
{
}
