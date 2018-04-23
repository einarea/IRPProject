#pragma once
#include "NodeIRP.h"
#include "NodeInstance.h"
//Class to store nodes over all periods
class NodeIRPHolder {
public:
	
	int getId();
	NodeIRPHolder(int id, NodeInstance& instance);
	vector<NodeIRP*> Nodes;
	
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
	double getFeasibleServiceIncrease(int period);
	double getFeasibleServiceDecrease(int period);
	double getFeasibleServiceMove(int from, int to);
};


