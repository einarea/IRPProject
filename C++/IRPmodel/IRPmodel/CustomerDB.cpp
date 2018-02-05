#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Customer.h"
#include "CustomerDB.h"
using namespace std;

CustomerDB::CustomerDB(string FileName)
{
	ifstream CustomerRecords(FileName, ios::in);

	if (!CustomerRecords)
	{
		cerr << "Customer records file could not be opened" << endl;
		exit(1);
	}


	string line;
	//check file for errors
	getline(CustomerRecords, line);

	int nPeriods = 2;
	int CustomerID = 0;
	int HoldingCost;
	int * Demand;
	int LowerLimit;
	int UpperLimit;


	while (!getline(CustomerRecords, line).eof()) {
	
		size_t pos = 0;
		string token;
		string delimiter = "\t";
		HoldingCost = stoi(getNextToken(line, delimiter));
		LowerLimit = stoi(getNextToken(line, delimiter));
		UpperLimit = stoi(getNextToken(line, delimiter));

		//Dynamically allocated array
		Demand = new int[nPeriods];

		for (int i = 0; i < nPeriods; i++) {
			Demand[i] = stoi(getNextToken(line, delimiter));
		};

		Customer cust(CustomerID, HoldingCost, LowerLimit, UpperLimit, Demand);
		Customers.push_back(cust);
		CustomerID++;
	}
}

int CustomerDB::getnCustomers()
{
	return (int) Customers.size();
}

int CustomerDB::getHoldCost(int id)
{
	return getCustomer(id)->getHoldCost();
}

double CustomerDB::getX(int id)
{
	return getCustomer(id)->getXpos();
}

double CustomerDB::getY(int id)
{
	return getCustomer(id)->getYpos();
}

string CustomerDB::getNextToken(string &str, string& delimiter)
{
	size_t pos = 0;
	pos = str.find(delimiter);
	string token = str.substr(0, pos);
	str.erase(0, pos + delimiter.length());
	return token;
}
Customer * CustomerDB::getCustomer(int id)
{
	return &Customers[id-1];
}
