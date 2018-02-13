#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "CustomerDB.h"

using namespace std;


class Map
{

private:
	int numVertex;
	string file;

public:
	Map(CustomerDB&);
	int nodeToCustomer(int);
	bool inArcSet(int, int);
	CustomerDB & database;
	int getTransCost(int node1, int node2, int transCostMultiplier, int serviceCostMultipler);
	int getDistance(int node1, int node2);
	int getDeliveryNode(Customer *);
	int getPickupNode(Customer *);
	int getNumCustomers();
	bool isDelivery(int node);
	
	Customer * getCustomer(int);
	
	int getTravelTime(int node1, int node2, int timeParam, int serviceTimeParameter);
	~Map();
};

