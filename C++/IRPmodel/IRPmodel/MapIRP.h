#pragma once
#include "Map.h"
#include "CustomerIRPDB.h"
class MapIRP :
	public Map
{
private:
	CustomerIRPDB & database;
public:
	MapIRP(CustomerIRPDB &);
	~MapIRP();
	int getHoldCost(int);
	int getUpperLimit(int node);
	int getLowerLimit(int node);
	int getDemand(int node, int period, int indicator);
	int getInitInventory(int node);

};

