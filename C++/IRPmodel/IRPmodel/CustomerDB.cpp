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
	//check file header
	getline(CustomerRecords, line);

	int nPeriods = 2;
	int CustomerID = nCustomers + 1;
	int HoldingCost;
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
			int a = stoi(getNextToken(line, delimiter));
			Demand[i] = a;
			cout << Demand[i];
		};

		Customer cust(HoldingCost, LowerLimit, UpperLimit, Demand, 50, 50);
		Customers.push_back(cust);
	}
}

string CustomerDB::getNextToken(string &str, string& delimiter)
{
	size_t pos = 0;
	pos = str.find(delimiter);
	string token = str.substr(0, pos);
	str.erase(0, pos + delimiter.length());
	return token;
};