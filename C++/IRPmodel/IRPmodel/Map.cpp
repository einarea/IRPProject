#include "stdafx.h"
#include "Map.h"
#include <iostream>
#include <math.h>
#include <algorithm>
using namespace ::std;
#include "IRP.h"
#include "ModelParameters.h"
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

int Map::getTransCost(int node1, int node2)
{
	int distance = getDistance(node1, node2);
	return distance* ModelParameters::TRANSCOST_MULTIPLIER + ModelParameters::SERVICECOST_MULTIPLIER;
}





int Map::getDeliveryNode(Customer * cust)
{
	return (cust->getId());
}

int Map::getPickupNode(Customer * c)
{
	return c->getId()+getNumCustomers();
}




int Map::getNumCustomers()
{
	return database.getnCustomers();
}

Customer * Map::getCustomer(int id)
{
	id = nodeToCustomer(id);
	return database.getCustomer(id);
}



int Map::getTravelTime(int node1, int node2, int travelTimeParam, int serviceTimeParam)
{
	int a = getDistance(node1, node2) * travelTimeParam + serviceTimeParam;
	return getDistance(node1, node2) * travelTimeParam + serviceTimeParam;
}

int Map::getDistance(int node1, int node2)
{
	int distance;
	node1 = nodeToCustomer(node1);
	node2 = nodeToCustomer(node2);

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

		distance = (int) floor(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
	}
	return distance;
}

int Map::getX(int id)
{
	id = nodeToCustomer(id);
	return database.getCustomer(id)->getXpos();
}

int Map::getY(int id)
{
	id = nodeToCustomer(id);
	return database.getCustomer(id)->getYpos();
}





bool Map::inExtensiveArcSet(int i , int j)
{
	bool a = (i == j || (i == database.getnCustomers() + j && j != 0));
	return !a;
}

bool Map::inSimultaneousArcSet(int i, int j)
{
	//Check if incident from delivery node and not to co-located pickup node
	if (i == 0)
		if (j <= getNumCustomers())
			return true;
		else
			return false;

	if (isDelivery(i) && !isColocated(i, j))
		return false;

	//Check if incident to pickup node and not from co-located delivery node
	if (!isDelivery(j) && !isColocated(i, j) && j != 0)
		return false;

	return inExtensiveArcSet(i, j);
}

bool Map::isColocated(int delivery, int pickup)
{
	if (pickup == (getNumCustomers() + delivery)  && delivery != 0) {
		return true;
	}
	else
		return false;
}


int Map::nodeToCustomer(int node)
{
	if (node > database.getnCustomers())
		return node - database.getnCustomers();
	else
		return node;
}

bool Map::inArcSet(int i, int j)
{
	if (ModelParameters::Simultaneous)
		return inSimultaneousArcSet(i, j);
	else
		return inExtensiveArcSet(i, j);
}

Map::~Map()
{
}
