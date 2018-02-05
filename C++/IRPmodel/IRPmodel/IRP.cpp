#include "stdafx.h"
#include "IRP.h"
#include "ModelBase.h"
using namespace ::dashoptimization;

IRP::IRP(string filename)
	:
	prob("IRP"),						//Initialize problem in BCL
	database(filename),					//Initialize database of customers
	map(database, TRANSCOST_MULTIPLIER, SERVICECOST_MULTIPLIER)			//Set up map of all customers
{
	
	//Initialize sets
	if(!initializeSets()) {
		cout<<"Data Error: Could not initialize sets.";
		return;
	}
	
	//Initialize variables
	if (!initializeVariables()) {
		cout << "Data Error: Could not initialize variables.";
		return;
	}

	//Initialize parameters
	if (!initializeParameters()) {
		cout << "Data Error: Could not initialize variables.";
		return;
	}
	


}

XPRBprob & IRP::getProblem() {
	return prob;
}


bool IRP::initializeParameters() {
	TransCost = new int *[AllNodes.size()];
	for (int i : AllNodes) {
		printf("\n");
		TransCost[i] = new  int[AllNodes.size()];
		for (int j : AllNodes) {
			
			if (inArcSet(i, j)) {
				TransCost[i][j] = map.getTransCost(i, j);
				printf("%-10d", TransCost[i][j]);
			}
			else {
				TransCost[i][j] = -1;
				printf("%-10d", TransCost[i][j]);
			}
			
		}
	} // end initialization TransCost
	
	HoldCost = new int *[Nodes.size()];
	for (int i : Nodes) {
		HoldCost[i] = new int[Periods.size()];
			for (int t : Periods) {
				HoldCost[i][t] = map.getHoldCost(i);
			}
	}


	return true;
}




bool IRP::initializeSets()
{
	NumOfCustomers = database.getnCustomers();					//Number of customers
	NumOfPeriods = 2;

	ModelBase::createRangeSet(1, NumOfPeriods, Periods);
	ModelBase::createRangeSet(1, NumOfCustomers, DeliveryNodes);
	ModelBase::createRangeSet(NumOfCustomers+1, 2*NumOfCustomers, PickupNodes);
	ModelBase::createUnion(DeliveryNodes, PickupNodes, Nodes);
	ModelBase::createRangeSet(0, 0, Depot);
	ModelBase::createUnion(Depot, Nodes, AllNodes);

	//for (int i = 0; i < AllNodes.size(); i++) printf("%d", AllNodes[i]);
	return true;
}

bool IRP::initializeVariables()
{
	x = new XPRBvar **[AllNodes.size()];
	loadDelivery = new XPRBvar **[AllNodes.size()];
	loadPickup = new XPRBvar **[AllNodes.size()];

	for (int i : AllNodes) {
		x[i] = new XPRBvar *[AllNodes.size()];
		loadDelivery[i] = new	XPRBvar *[AllNodes.size()];
		loadPickup[i] = new	XPRBvar *[AllNodes.size()];

		for (int j : AllNodes) {
			x[i][j] = new XPRBvar[Periods.size()];
			loadDelivery[i][j] = new	XPRBvar[Periods.size()];
			loadPickup[i][j] = new	XPRBvar[Periods.size()];

			for (int t : Periods) {
				if (inArcSet(i, j)) {
					x[i][j][t] = prob.newVar("x", XPRB_BV, 0, 1);
					loadDelivery[i][j][t] = prob.newVar(XPRBnewname("loadDelivery_%d%d%d", i, j, t), XPRB_PL, 0, 100);
					loadPickup[i][j][t] = prob.newVar(XPRBnewname("loadPickup_%d%d%d", i, j, t), XPRB_PL, 0, 100);
				}

			}
		}
	} // end initializing of x and load

	//initialize y-variables and inventory
	y = new XPRBvar *[AllNodes.size()];
	inventory = new XPRBvar *[DeliveryNodes.size()];
	for (int i : Nodes) {
		y[i] = new XPRBvar[Periods.size()];
		inventory[i] = new XPRBvar[Periods.size()];
		for (int t : Periods) {
			y[i][t] = prob.newVar(XPRBnewname("y_%d%d", i, t), XPRB_PL, 0, 1);
			inventory[i][t] = prob.newVar(XPRBnewname("inventory_%d%d", i, t), XPRB_PL, 0, 100);
		}
	}

	//Initalize depot
	int nVehicles = 3;
	y[0] = new XPRBvar[Periods.size()];
	for (int t : Periods) {
		y[0][t] = prob.newVar(XPRBnewname("y_%d", t), XPRB_PL, 0, nVehicles);
	}

	//Initialize at delivery nodes
	delivery = new XPRBvar *[DeliveryNodes.size()];
	for (int i : DeliveryNodes) {
		delivery[i] = new XPRBvar [Periods.size()];
		for (int t : Periods) {
			delivery[i][t] = prob.newVar(XPRBnewname("delivery_%d%d", i, t), XPRB_PL, 0, 100);
			
		}
	}

	//Initialize at pickup nodes
	pickup = new XPRBvar *[PickupNodes.size()];
	for (int i : PickupNodes){
		pickup[i] = new  XPRBvar[Periods.size()];
		for (int t : Periods) {
			pickup[i][t] = prob.newVar(XPRBnewname("pickup_%d%d", i, t), XPRB_PL, 0, 1);
		}
	}
	return true;
}

bool IRP::inArcSet(int i, int j)
{
	
	bool a = (i == j || (i == getNumOfCustomers() + j && j != 0));
	return !a;
}

// end initialize variables



int IRP::getNumOfCustomers()
{
	return NumOfCustomers;
}

int IRP::getNumOfPeriods(IRP * model)
{
	return (*model).NumOfPeriods;
}


IRP::~IRP()
{
}
