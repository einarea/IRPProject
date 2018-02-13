#include "stdafx.h"
#include <string>
#include "Customer.h"
#include "CustomerDB.h"
using namespace std;



int CustomerDB::getnCustomers()
{
	return (int) Customers.size();
}


int CustomerDB::getX(int id)
{
	return getCustomer(id)->getXpos();
}

int CustomerDB::getY(int id)
{
	return getCustomer(id)->getYpos();
}



CustomerDB::CustomerDB()
{
}

vector<Customer*>* CustomerDB::getCustomers()
{
	return &Customers;
}

Customer * CustomerDB::getCustomer(int id)
{
	for (Customer *c: Customers) {
		if (c->getId()==id)
			return c;
	}
}
