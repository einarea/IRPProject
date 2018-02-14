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
	if (isDelivery(node))
		return database.getHoldCost(nodeToCustomer(node), Customer::DELIVERY);
	else
		return database.getHoldCost(nodeToCustomer(node), Customer::PICKUP);
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
