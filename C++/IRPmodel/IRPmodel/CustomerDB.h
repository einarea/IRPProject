#pragma once

#include "stdafx.h"
#include <vector>
#include "Customer.h"
#include <iostream>
#include <string>

using namespace::std;

class CustomerDB
{
private:
	int nPeriods;
	int nCustomers = 0;

	
	vector <Customer> Customers; //Vector of all customers 

	string getNextToken(string&, string&); //Helper function
	Customer * getCustomer(int); 

public:
	CustomerDB(string);
	int getnCustomers();
	int getHoldCost(int);
	double getX(int);
	double getY(int);
	int getUpperLimit(int);
	int getInitInventory(int id, int indicator);
	int getLowerLimit(int);
	int getDemand(int id, int period, int INDICATOR);
};