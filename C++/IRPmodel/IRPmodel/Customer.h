#pragma once

#include <vector>
#include "ModelParameters.h"
#include "stdafx.h"

using namespace::std;

class Customer
{
private:
	static int nPeriods;
	
	int CustomerID;
	double posX;
	double posY;

public:

	Customer(int CustomerID, int posX, int posY);
	Customer(int id, int randSeed);
	double getXpos();
	double getYpos();
	int getId();	
	void print();

	static const int  DELIVERY = 0;
	static const int PICKUP = 1;

};