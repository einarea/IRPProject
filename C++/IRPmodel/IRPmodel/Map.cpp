#include "stdafx.h"
#include "Map.h"
#include <iostream>
#include <math.h>
#include <algorithm>
using namespace ::std;
#include "IRP.h";
#include "math.h"



Map::Map(CustomerDB& db, int transCost, int serviceCost)
	:
	database(db),
	TRANSCOST_MULTIPLIER(transCost),
	SERVICECOST_MULTIPLIER(serviceCost)
{	
}

int Map::getTransCost(int node1, int node2)
{
	//Make sure node corresponds to node in network
	nodeToCustomer(node1);
	nodeToCustomer(node2);

	int distance = getDistance(node1, node2);
	return distance* TRANSCOST_MULTIPLIER + SERVICECOST_MULTIPLIER;
}

int Map::getHoldCost(int node)
{
	nodeToCustomer(node);
	return database.getHoldCost(node);
}

int Map::getDistance(int id1, int id2)
{
	int distance;

	if (id1 == 0 || id2 == 0) {
		int x = database.getX(max(id1, id2));
		int y = database.getY(max(id1, id2));
		distance = floor(sqrt(pow(x, 2) + pow(y, 2)));
	}
	else
	{
		int x1 = database.getX(id1);
		int y1 = database.getY(id1);
		int x2 = database.getX(id2);
		int y2 = database.getY(id2);

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
