#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <iostream>

using namespace std;


class Map
{

private:
public:
	int numVertex;
	double ** coordinate;
	Map(string msg);
	double getDistance(int, int);
	string file;
	void free();
public:
	Map(FILE * dfile);
	double ** getDistanceMatrix(double ** positions);
	int getNumCustomers();
	~Map();
};

