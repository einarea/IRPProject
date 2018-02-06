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
	int TRANSCOST_MULTIPLIER= 10;
	int SERVICECOST_MULTIPLIER = 5;
	bool isDelivery(int node);
public:
	Map(CustomerDB&, int, int);
	CustomerDB & database;
	int getTransCost(int, int);
	int getDistance(int, int);
	int getHoldCost(int);
	int getNumCustomers();
	int getUpperLimit(int node);
	int getLowerLimit(int node);
	int getDemand(int node, int period, int indicator);
	int getInitInventory(int node);
	~Map();
};

