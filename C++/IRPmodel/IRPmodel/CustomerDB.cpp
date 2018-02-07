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
		Demand = new int * [2]; //pickup and delivery demand

		for (int i = 0; i < 2; i++){
			Demand[i] = new int[nPeriods];
			for (int t = 0; t < nPeriods; t++) {
				Demand[i][t] = stoi(getNextToken(line, delimiter));
				
			}
		}

		//Inital inventory
		InitInventory = new int [2]; //pickup and delivery demand
		for (int i = 0; i < 2; i++) {
			InitInventory[i] = stoi(getNextToken(line, delimiter));
		}

		Customer cust(CustomerID, HoldingCost, LowerLimit, UpperLimit, Demand, InitInventory);
		printf("%d ", cust.getInitInventory(Customer::PICKUP));
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

int CustomerDB::getnPeriods()
{
	return nPeriods;
}

double CustomerDB::getX(int id)
{
	return getCustomer(id)->getXpos();
}

double CustomerDB::getY(int id)
{
	return getCustomer(id)->getYpos();
}

int CustomerDB::getUpperLimit(int id)
{
	return getCustomer(id)->getUpperLimit();
}

int CustomerDB::getInitInventory(int id, int indicator)
{
	return getCustomer(id)->getInitInventory(indicator);
}

int CustomerDB::getLowerLimit(int id)
{
	return getCustomer(id)->getLowerLimit();
}

int CustomerDB::getDemand(int id, int period, int indicator)
{
	return getCustomer(id)->getDemand(period-1, indicator);
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
