#pragma once
#include "CustomerVRP.h"
#include "CustomerIRP.h"
#include "IRP.h"

class CustomerVRPDB	: public CustomerDB
{
public:
	CustomerVRPDB(vector <vector<double>> &Demand, vector <Customer *> & );
	int getnCustomers();
	int getDemand(int id, int indicator);
	CustomerVRP * getCustomer(int);
	~CustomerVRPDB();
};

