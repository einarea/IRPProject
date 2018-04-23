#include "stdafx.h"
#include "NodeIRPHolder.h"


//Constructor
NodeIRPHolder::NodeIRPHolder()
{
}

//Destructor
NodeIRPHolder::~NodeIRPHolder()
{
}


//Constructor
NodeIRPHolder::NodeIRPHolder(int id, NodeInstance& instance)
	:
	Instance(instance),
	Id(id)
{
	Nodes.resize(Instance.getNumOfPeriods() + 1);
	//Initialize the holder with one node for each period
	for (auto t : Instance.Periods) {
		Nodes[t] = new NodeIRP(getId());
	}
}

int IRP::NodeIRPHolder::isInventoryFeasible()
{
	for (auto t : Instance.Periods) {
		if (isInventoryFeasible(t) == ModelParameters::LOWER_LIMIT_BREAK || isInventoryFeasible(t) == ModelParameters::UPPER_LIMIT_BREAK)
			return isInventoryFeasible(t);
	}
	return  ModelParameters::WITHIN_LIMITS;
}

int IRP::NodeIRPHolder::isInventoryFeasible(int period)
{
	if (Nodes[period]->Inventory < Instance.map.getLowerLimit(getId()))
		return ModelParameters::LOWER_LIMIT_BREAK;
	if (Nodes[period]->Inventory > Instance.map.getUpperLimit(getId()))
		return ModelParameters::UPPER_LIMIT_BREAK;
	else
		return ModelParameters::WITHIN_LIMITS;
}

double IRP::NodeIRPHolder::moveQuantity(int from, int to, double quantity)
{
	double holder;
	//Move from period
	//move from period, check capacity feasible
	quantity = min(getFeasibleServiceMove(from, to), quantity);
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
double IRP::NodeIRPHolder::getMinQuantity()
{
	double minQuantity = 0;
	for (auto t : Instance.Periods) {
		if (isInventoryFeasible(t) == ModelParameters::LOWER_LIMIT_BREAK)
			minQuantity = max(minQuantity, Instance.map.getLowerLimit(getId()) - inventory(t));
	}

	return minQuantity;
}

double IRP::NodeIRPHolder::getExcessQuantity()
{
	double temp = 0;
	for (auto t : Instance.Periods) {
		temp = max(temp, inventory(t) - Instance.map.getUpperLimit(getId()));
	}
	return temp;
}

void IRP::NodeIRPHolder::removeMinQuantity()
{
	for (auto t : Instance.Periods) {
		if (isInventoryFeasible(t) == ModelParameters::UPPER_LIMIT_BREAK)
			cout << inventory(t) - Instance.map.getUpperLimit(getId());
		changeQuantity(t, inventory(t) - Instance.map.getUpperLimit(getId()));
		return;
	}
}



void IRP::NodeIRPHolder::addEdge(double loadDel, double loadPick, NodeIRPHolder * NodeHolder, int period, double value)
{
	NodeIRP * u = Nodes[period];
	NodeIRP * v = NodeHolder->Nodes[period];
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
	if (period <= Instance.getNumOfPeriods())
	{
		Nodes[period]->Inventory += quantity;
		propInvForw(period + 1, quantity);
	}

}

double IRP::NodeIRPHolder::getOutflow(int period)
{
	return Nodes[period]->getOutflow();
}

double IRP::NodeIRPHolder::quantity(int period)
{
	return Nodes[period]->Quantity;
}

double IRP::NodeIRPHolder::inventory(int period)
{
	return Nodes[period]->Inventory;
}

double IRP::NodeIRPHolder::timeServed(int period)
{
	return Nodes[period]->TimeServed;
}

vector<IRP::NodeIRP::EdgeIRP*> IRP::NodeIRPHolder::getEdges(int period)
{
	return Nodes[period]->getEdges();
}

IRP::NodeIRP::EdgeIRP * IRP::NodeIRPHolder::getEdge(int period)
{
	return Nodes[period]->getEdge();
}

double IRP::NodeIRPHolder::getHoldCost(int period)
{
	return Instance.map.getHoldCost(getId()) * Nodes[period]->Inventory;
}

//Returns quantity inserted
void IRP::NodeIRPHolder::changeQuantity(int period, double quantity)
{
	Nodes[period]->Quantity += quantity;

	if (isDelivery())
		propInvForw(period, quantity);
	else
		propInvForw(period, -quantity);

}


bool NodeIRPHolder::isDelivery()
{
	return Instance.getMap()->isDelivery(getId());
}

double NodeIRPHolder::getFeasibleServiceIncrease(int period)
{
	// Check if quantity is feasible, if not adjust it
	return Instance.Capacity - quantity(period);
}

double NodeIRPHolder::getFeasibleServiceDecrease(int period)
{
	// Check if quantity is feasible, if not adjust it
	return quantity(period);
}

double NodeIRPHolder::getFeasibleServiceMove(int from, int to)
{
	return min(getFeasibleServiceDecrease(from), getFeasibleServiceIncrease(to));
}

