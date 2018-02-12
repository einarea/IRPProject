#pragma once
#include "Customer.h"

class CustomerVRP:
	public Customer
{
private:
	int Delivery;
	int Pickup;

public:
	CustomerVRP(int id, int x, int y, int del, int pick);
	int getDelivery();
	int getPickup();
	~CustomerVRP();
};

