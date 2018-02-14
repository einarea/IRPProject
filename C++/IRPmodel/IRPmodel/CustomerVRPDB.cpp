#include "stdafx.h"
#include "CustomerVRPDB.h"


CustomerVRP * CustomerVRPDB::getCustomer(int id)
{
	for (Customer *c : Customers) {
		if (c->getId() == id) {
			CustomerVRP * derivedPtr = static_cast <CustomerVRP *> (CustomerDB::getCustomer(id));
			if (derivedPtr != 0)
				return derivedPtr;
		}
	}
}

//Recieves demand and position for the customers
CustomerVRPDB::CustomerVRPDB(vector<vector<int>> & Demand,  vector<Customer *> &customers)
{
	for (Customer * c : customers) {
		CustomerVRP * cust = new CustomerVRP(c->getId(), c->getXpos(), c->getYpos(), Demand[Customer::DELIVERY][c->getId()], Demand[Customer::PICKUP][c->getId()]);
		Customers.push_back(cust);
	}
}

int CustomerVRPDB::getnCustomers()
{
	return Customers.size();
}



int CustomerVRPDB::getDemand(int id, int indicator)
{
	return getCustomer(id)->getDemand(indicator);
}


CustomerVRPDB::~CustomerVRPDB()
{
}
