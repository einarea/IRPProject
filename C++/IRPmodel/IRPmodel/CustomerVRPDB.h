#pragma once
#include "CustomerVRP.h"
#include "CustomerIRP.h"
#include "IRP.h"
#include "NodeIRP.h"

class CustomerVRPDB	: public CustomerDB
{
public:
	CustomerVRPDB(vector <vector<double>> &Demand, vector <Customer *> & );
	CustomerVRPDB(vector <IRP::NodeIRP*> nodes, Map &map);
	int getnCustomers();
	int getDemand(int id, int indicator);
	CustomerVRP * getCustomer(int);
	~CustomerVRPDB();
};

