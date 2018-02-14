#pragma once

#include "stdafx.h"
#include <vector>
#include "CustomerIRP.h"
#include <iostream>
#include <string>

using namespace::std;

class CustomerDB
{
private:
	int nCustomers = 0;	
	
protected:
	vector <Customer *> Customers; //Vector of all customers 

public:
	CustomerDB();
	vector <Customer *> * getCustomers();
	Customer * getCustomer(int id);
	int getnCustomers();
	int getX(int);
	int getY(int);

};