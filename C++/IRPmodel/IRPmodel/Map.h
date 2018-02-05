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
	int getDistance(int, int);
	string file;
	void free();
	void nodeToCustomer(int&);
	int TRANSCOST_MULTIPLIER;
	int SERVICECOST_MULTIPLIER;
public:
	Map(CustomerDB&, int, int);
	CustomerDB & database;
	int getTransCost(int, int);
	int getHoldCost(int);
	int getNumCustomers();
	~Map();
};

