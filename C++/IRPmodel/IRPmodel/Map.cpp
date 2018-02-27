#include "stdafx.h"
#include "Map.h"
#include <iostream>
#include <math.h>
#include <algorithm>
using namespace ::std;
#include "IRP.h";
#include "math.h"



bool Map::isDelivery(int node)
{
	if (node <= getNumCustomers())
		return true;
	else
		return false;
}

Map::Map(CustomerDB& db)
	:
	database(db)
{	
}

int Map::getTransCost(int node1, int node2, int TRANSCOST_MULTIPLIER, int SERVICECOST_MULTIPLER)
{
	//Make sure node corresponds to node in network


	int distance = getDistance(node1, node2);
	return distance* TRANSCOST_MULTIPLIER + SERVICECOST_MULTIPLER;
}

int Map::getHoldCost(int node)
{
	nodeToCustomer(node);
	return database.getHoldCost(node);
}

int Map::getNumCustomers()
{
	return database.getnCustomers();
}

int Map::getUpperLimit(int node)
{
	nodeToCustomer(node);
	return database.getUpperLimit(node);
}

int Map::getLowerLimit(int node)
{
	nodeToCustomer(node);
	return database.getLowerLimit(node);
}

int Map::getDemand(int node, int period, int indicator)
{
	nodeToCustomer(node);
	return database.getDemand(node, period, indicator);
}

int Map::getInitInventory(int node)
{
	int indicator;
	if (isDelivery(node))
		indicator = Customer::DELIVERY;
	else
		indicator = Customer::PICKUP;
	
	nodeToCustomer(node);

	return database.getInitInventory(node, indicator);
	
}

int Map::getTravelTime(int node1, int node2, int travelTimeParam)
{
	return getDistance(node1, node2) * travelTimeParam;
}

int Map::getDistance(int node1, int node2)
{
	int distance;
	nodeToCustomer(node1);
	nodeToCustomer(node2);

	if (node1 == 0 || node2 == 0) {
		int x = database.getX(max(node1, node2));
		int y = database.getY(max(node1, node2));
		distance = floor(sqrt(pow(x, 2) + pow(y, 2)));
	}
	else
	{
		int x1 = database.getX(node1);
		int y1 = database.getY(node1);
		int x2 = database.getX(node2);
		int y2 = database.getY(node2);

		distance = floor(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
	}
	return distance;
}

void Map::nodeToCustomer(int & node)
{
	if (node > database.getnCustomers()) { node = node - database.getnCustomers(); };
}

Map::~Map()
{
}
