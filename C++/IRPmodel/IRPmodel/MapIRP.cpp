#include "stdafx.h"
#include "MapIRP.h"


MapIRP::MapIRP(CustomerIRPDB &db)
	:
Map(db),
database(db)
{	
}

int MapIRP::getHoldCost(int node)
{
	node = nodeToCustomer(node);
	return database.getHoldCost(node);
}

int MapIRP::getUpperLimit(int node)
{
	node = nodeToCustomer(node);
	return database.getUpperLimit(node);
}

int MapIRP::getLowerLimit(int node)
{
	node = nodeToCustomer(node);
	return database.getLowerLimit(node);
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
