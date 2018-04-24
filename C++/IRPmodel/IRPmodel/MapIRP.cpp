#include "stdafx.h"


/*

MapIRP::MapIRP(CustomerIRPDB &db)
	:
Map(db),
database(db)
{	
}

double MapIRP::getHoldCost(int node)
{
	double returnValue = 0;
	if (isDelivery(node))		
		returnValue = database.getHoldCost(nodeToCustomer(node), Customer::DELIVERY);
	else
		returnValue = database.getHoldCost(nodeToCustomer(node), Customer::PICKUP);

	return returnValue;
}

int MapIRP::getUpperLimit(int node)
{
	if (isDelivery(node))
		return database.getUpperLimit(nodeToCustomer(node), Customer::DELIVERY);
	else
		return database.getUpperLimit(nodeToCustomer(node), Customer::PICKUP);
}

int MapIRP::getLowerLimit(int node)
{
	if (isDelivery(node))
		return database.getLowerLimit(nodeToCustomer(node), Customer::DELIVERY);
	else
		return database.getLowerLimit(nodeToCustomer(node), Customer::PICKUP);
}

int MapIRP::getDemand(int node, int period, int indicator)
{
	node = nodeToCustomer(node);
	return database.getDemand(node, period, indicator);
}

int MapIRP::getInitInventory(int node)
{
	int indicator;
	if (isDelivery(node))
		indicator = Customer::DELIVERY;
	else
		indicator = Customer::PICKUP;

	node = nodeToCustomer(node);

	return database.getInitInventory(node, indicator);

}

MapIRP::~MapIRP()
{
}
*/