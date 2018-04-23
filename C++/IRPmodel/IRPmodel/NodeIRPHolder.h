#pragma once
#include "NodeIRP.h"
#include "Node.h"
#include <algorithm>
#include <stdlib.h>
#include "stdafx.h"

using namespace std;

//Class to store nodes over all periods
class NodeIRPHolder {
public:
	friend class Solution;

	int getId();
	NodeIRPHolder(NodeInstance& instance);
	vector< NodeIRP* > NodePeriods;
	
	NodeIRP * getNode(int period);
	int isInventoryFeasible();
	int isInventoryFeasible(int period);
	double moveQuantity(int from, int to, double quantity);
	double getMinQuantity();
	double getExcessQuantity();
	void removeMinQuantity();

	//Changes
	void addEdge(double loadDel, double loadPic, NodeIRPHolder * child, int period, double value);

	//Position information
	int getXPos();
	int getYPos();
	int getHoldingCost();
	int getInitialInventory();
	int getDemand(int Period);

	//Information
	void propInvForw(int period, double quantity);
	double getOutflow(int period);
	double quantity(int period);
	double inventory(int period);
	double timeServed(int period);

	vector <NodeIRP::EdgeIRP*> getEdges(int period);
	NodeIRP::EdgeIRP * getEdge(int period);
	double getHoldCost(int period);
	void changeQuantity(int period, double quantity);
	bool isDelivery();

private:

	int Id;
	//To store node information
	NodeInstance & Instance;
	double getFeasibleServiceIncrease(int period, int capacity);
	double getFeasibleServiceDecrease(int period);
	double getFeasibleServiceMove(int from, int to, int capacity);

	// Sets
	vector  <int> Periods;
	vector  <int> AllPeriods;
	vector  <int> DeliveryNodes;
	vector <int> PickupNodes;
	vector  <int> Nodes;
	vector  <int> AllNodes;
	vector  <int> Depot;
};


