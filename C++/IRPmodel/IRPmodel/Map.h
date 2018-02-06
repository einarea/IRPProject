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
	void free();
	void nodeToCustomer(int&);
	bool isDelivery(int node);
public:
	Map(CustomerDB&);
	CustomerDB & database;
	int getTransCost(int node1, int node2, int transCostMultiplier, int serviceCostMultipler);
	int getDistance(int node1, int node2);
	int getHoldCost(int);
	int getNumCustomers();
	int getUpperLimit(int node);
	int getLowerLimit(int node);
	int getDemand(int node, int period, int indicator);
	int getInitInventory(int node);
	~Map();
};

