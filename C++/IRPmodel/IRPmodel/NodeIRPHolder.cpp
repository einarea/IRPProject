#include "stdafx.h"
#include "NodeIRPHolder.h"



using namespace std;

//Destructor
NodeIRPHolder::~NodeIRPHolder()
{
	//Delete nodeIRP
	for (NodeIRP * node : NodePeriods) {
		delete node;
	}
}


int NodeIRPHolder::getId() const
{
	return Instance.getId();
}

//Constructor
NodeIRPHolder::NodeIRPHolder(NodeInstance& instance)
	:
	Instance(instance),
	Id(instance.getId())
{
	NodePeriods.resize(Instance.nPeriods + 1);
	//Initialize the holder with one node for each period
	for (auto t = 1; t <= Instance.nPeriods; t++) {
		NodePeriods[t] = new NodeIRP(instance);
	}
}

NodeIRPHolder::NodeIRPHolder(NodeIRPHolder & cpNode)
	:
	Instance(cpNode.Instance),
	Id(cpNode.Id)
{
	NodePeriods.resize(cpNode.NodePeriods.size());
	for (int t = 1; t < cpNode.NodePeriods.size(); t++) {
		//Does not copy the NodeIRP's edges
		NodePeriods[t] = new NodeIRP(*cpNode.NodePeriods[t]);
	}
}

NodeIRP * NodeIRPHolder::getNode(int period)
{
	return (NodeIRP*) NodePeriods[period];
}

int NodeIRPHolder::isInventoryFeasible()
{
	for (int t: Periods) {
		if (isInventoryFeasible(t) == ModelParameters::LOWER_LIMIT_BREAK || isInventoryFeasible(t) == ModelParameters::UPPER_LIMIT_BREAK)
			return isInventoryFeasible(t);
	}
	return  ModelParameters::WITHIN_LIMITS;
}

int NodeIRPHolder::isInventoryFeasible(int period)
{
	if (NodePeriods[period]->Inventory < Instance.LowerLimit)
		return ModelParameters::LOWER_LIMIT_BREAK;
	if (NodePeriods[period]->Inventory > Instance.UpperLimit)
		return ModelParameters::UPPER_LIMIT_BREAK;
	else
		return ModelParameters::WITHIN_LIMITS;
}

double NodeIRPHolder::moveQuantity(int from, int to, double quantity)
{
	double holder;
	//Move from period
	//move from period, check capacity feasible
	quantity = min(getFeasibleServiceMove(from, to, 1000), quantity);
	changeQuantity(from, -quantity);
	//insert in period
	changeQuantity(to, quantity);

	//Check if inventory and capacity is feasible
	while (ModelParameters::WITHIN_LIMITS != isInventoryFeasible()) {
		if (isInventoryFeasible() == ModelParameters::LOWER_LIMIT_BREAK) {
			holder = getMinQuantity();
			changeQuantity(from, holder);
			changeQuantity(to, -holder);
		}
		else {
			holder = getExcessQuantity();
			changeQuantity(from, holder);
			changeQuantity(to, -holder);
		}
	}
	return quantity;
}



//inserts minimum quantity to make node feasible with respect to lower bound
double NodeIRPHolder::getMinQuantity()
{
	double minQuantity = 0;
	for (auto t : Periods) {
		if (isInventoryFeasible(t) == ModelParameters::LOWER_LIMIT_BREAK)
			minQuantity = max(minQuantity, Instance.LowerLimit - inventory(t));
	}

	return minQuantity;
}

double NodeIRPHolder::getExcessQuantity()
{
	double temp = 0;
	for (auto t : Periods) {
		temp = max(temp, inventory(t) - Instance.LowerLimit);
	}
	return temp;
}

void NodeIRPHolder::removeMinQuantity()
{
	for (auto t : Periods) {
		if (isInventoryFeasible(t) == ModelParameters::UPPER_LIMIT_BREAK)
			//cout << inventory(t) - Instance.UpperLimit;
		changeQuantity(t, inventory(t) - Instance.UpperLimit);
		return;
	}
}



void NodeIRPHolder::addEdge(double loadDel, double loadPick, NodeIRPHolder * NodeHolder, int period, double value)
{
	NodeIRP * u = NodePeriods[period];
	NodeIRP * v = NodeHolder->NodePeriods[period];
	u->addEdge(loadDel, loadPick, v, value);
}

int NodeIRPHolder::getXPos()
{
	return Instance.PosX;
}

int NodeIRPHolder::getYPos()
{
	return Instance.PosY;
}

int NodeIRPHolder::getHoldingCost()
{
	return Instance.HoldingCost;
}

int NodeIRPHolder::getInitialInventory()
{
	return Instance.InitInventory;
}

int NodeIRPHolder::getDemand(int period)
{
	//Check
	if (period > 0 && period <= Instance.nPeriods)
		return Instance.Demand[period];

	else
		exit(123);
}

//Propagate the inventory with the quantity. Positive if more is delivered, negative if more is picked up
void NodeIRPHolder::propInvForw(int period, double quantity)
{
	if (period <= Instance.nPeriods)
	{
		NodePeriods[period]->Inventory += quantity;
		propInvForw(period + 1, quantity);
	}

}

double NodeIRPHolder::getOutflow(int period)
{
	return NodePeriods[period]->getOutflow();
}

double NodeIRPHolder::quantity(int period)
{
	return NodePeriods[period]->Quantity;
}

double NodeIRPHolder::inventory(int period)
{
	return NodePeriods[period]->Inventory;
}

double NodeIRPHolder::timeServed(int period)
{
	return NodePeriods[period]->TimeServed;
}

vector<NodeIRP::EdgeIRP*> NodeIRPHolder::getEdges(int period)
{
	return NodePeriods[period]->getEdges();
}

NodeIRP::EdgeIRP * NodeIRPHolder::getEdge(int period)
{
	return NodePeriods[period]->getEdge();
}

double NodeIRPHolder::getHoldCost(int period)
{
	return Instance.HoldingCost * NodePeriods[period]->Inventory;
}

//Returns quantity inserted
void NodeIRPHolder::changeQuantity(int period, double quantity)
{
	NodePeriods[period]->Quantity += quantity;

	if (isDelivery())
		propInvForw(period, quantity);
	else
		propInvForw(period, -quantity);

}


bool NodeIRPHolder::isDelivery() const
{
	return Instance.isDelivery();
}

const NodeInstance & NodeIRPHolder::getData()
{
	return Instance;
}

double NodeIRPHolder::getFeasibleServiceIncrease(int period, int capacity)
{
	// Check if quantity is feasible, if not adjust it
	return capacity - quantity(period);
}

double NodeIRPHolder::getFeasibleServiceDecrease(int period)
{
	// Check if quantity is feasible, if not adjust it
	return quantity(period);
}

double NodeIRPHolder::getFeasibleServiceMove(int from, int to, int capacity)
{
	return min(getFeasibleServiceDecrease(from), getFeasibleServiceIncrease(to, capacity));
}

