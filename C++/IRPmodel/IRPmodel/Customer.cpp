#include "stdafx.h"
#include <algorithm>
using namespace ::std;
#include "Customer.h"
#include <fstream>




Customer::Customer(int id, int coordinateX, int coordinateY)
	:
	CustomerID(id),
	posX(coordinateX),
	posY(coordinateY)
{
}

Customer::Customer(int id)
	:
	CustomerID(id)
{
	posX = (rand() % 1000 + 1)/10;
	posY = (rand() % 1000 + 1)/10;
}


//Randomly generated customer, default
/*Customer::Customer(int numberOfPeriods, int holdCost[2], int lowLimit[2], int uppLim[2], int dem[2])
{
	//Check range
	
	LowerLimit = rand() %lowLimit[1]
	HoldingCost = rand();
	LowerLimit = 0;
	UpperLimit = 0;

}*/

double Customer::getXpos()
{
	return this->posX;
}

double Customer::getYpos()
{
	return this->posY;
}

int Customer::getId()
{
	return CustomerID;
}

