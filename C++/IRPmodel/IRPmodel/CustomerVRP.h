#pragma once
#include "Customer.h"


class CustomerVRP:
	public Customer
{
private:
	double Delivery;
	double Pickup;

public:
	CustomerVRP(int id, int x, int y, int del, int pick);
	double getDemand(int indicator);
	~CustomerVRP();
};

