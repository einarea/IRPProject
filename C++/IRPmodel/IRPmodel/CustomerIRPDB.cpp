#include "stdafx.h"
#include "CustomerIRPDB.h"


CustomerIRPDB::CustomerIRPDB(string FileName)
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

	int CustomerID = 0;
	int HoldingCost;
	int ** Demand;
	int * InitInventory;
	int LowerLimit;
	int UpperLimit;


	while (!getline(CustomerRecords, line).eof()) {

		size_t pos = 0;
		string token;
		delimiter = "\t";
		HoldingCost = stoi(getNextToken(line, delimiter));
		LowerLimit = stoi(getNextToken(line, delimiter));
		UpperLimit = stoi(getNextToken(line, delimiter));

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

		CustomerIRP * cust = new CustomerIRP(CustomerID, HoldingCost, LowerLimit, UpperLimit, Demand, InitInventory);
		printf("%d ", cust->getInitInventory(Customer::PICKUP));
		Customers.push_back(cust);
		CustomerID++;
	}
}

int CustomerIRPDB::getnPeriods()
{
	return nPeriods;
}

int CustomerIRPDB::getHoldCost(int id)
{
	return getCustomer(id)->getHoldCost();
}

int CustomerIRPDB::getUpperLimit(int id)
{
	return getCustomer(id)->getUpperLimit();
}


int CustomerIRPDB::getInitInventory(int id, int indicator)
{
	return getCustomer(id)->getInitInventory(indicator);
}

int CustomerIRPDB::getLowerLimit(int id)
{
	
	return getCustomer(id)->getLowerLimit();
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
