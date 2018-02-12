#include "stdafx.h"
#include <string>
#include "Customer.h"
#include "CustomerDB.h"
using namespace std;



int CustomerDB::getnCustomers()
{
	return (int) Customers.size();
}


double CustomerDB::getX(int id)
{
	return getCustomer(id)->getXpos();
}

double CustomerDB::getY(int id)
{
	return getCustomer(id)->getYpos();
}



CustomerDB::CustomerDB()
{
}

Customer * CustomerDB::getCustomer(int id)
{
	return Customers[id-1];
}
