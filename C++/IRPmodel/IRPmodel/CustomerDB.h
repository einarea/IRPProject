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
	int * Demand;
	
	vector <Customer> Customers;
	string getNextToken(string&, string&);

public:
	CustomerDB(string);
};