#pragma once
#include <vector>

using namespace std;

class ModelInstance
{
public:
	ModelInstance();

	vector  <int> Periods;
	vector  <int> AllPeriods;
	vector  <int> DeliveryNodes;
	vector <int> PickupNodes;
	vector  <int> Nodes;
	vector  <int> AllNodes;
	vector  <int> Depot;
	vector <int> integers1;


	~ModelInstance();
};

