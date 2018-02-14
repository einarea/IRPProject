#include "stdafx.h"
#include "CustomerVRP.h"
#include "IRP.h"




CustomerVRP::CustomerVRP(int id, int x, int y, int del, int pick)
	:
	Customer(id, x, y),
	Delivery(del),
	Pickup(pick)
{
}

int CustomerVRP::getDemand(int indicator)
{
	if (indicator == Customer::PICKUP)
		return  Pickup;
	else
		return Delivery;
}


CustomerVRP::~CustomerVRP()
{
}
