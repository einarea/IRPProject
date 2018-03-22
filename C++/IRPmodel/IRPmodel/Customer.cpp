#include "stdafx.h"
#include <algorithm>
#include "Customer.h"
#include <fstream>
#include <time.h>
using namespace ::std;




Customer::Customer(int id, int coordinateX, int coordinateY)
	:
	CustomerID(id),
	posX(coordinateX),
	posY(coordinateY)
{
}

Customer::Customer(int id, int randSeed)
	:
	CustomerID(id)
{
	//std::time_t time(0);
	srand(time(0) + randSeed);
	posX = (rand() % 100 + 0) - 50;
	posY = (rand() % 100 + 0) - 50;
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

void Customer::print()
{
	printf("\nCustomer Id: %d, x: %.0f, y: %.0f", getId(), getXpos(), getYpos());
}

