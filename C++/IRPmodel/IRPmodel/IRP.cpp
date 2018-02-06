#include "stdafx.h"
#include "IRP.h"
#include "ModelBase.h"
using namespace ::dashoptimization;

IRP::IRP(string filename)
	:
	prob("IRP"),						//Initialize problem in BCL
	database(filename),					//Initialize database of customers
	map(database)			//Set up map of all customers
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
		cout << "Data Error: Could not initialize parameters.";
		return;
	}

	formulateProblem();
	
}

bool IRP::formulateProblem()
{

	/****OBJECTIVE****/

	//Transportation costs
	for (int i : AllNodes)
		for (int j : AllNodes)
			for (int t : Periods) {
				if (inArcSet(i, j))
					objective += TransCost[i][j] * x[i][j][t];
			}
	//Holding costs
	for (int i : Nodes)
		for (int t : Periods)
			objective += HoldCost[i] * y[i][t];

	prob.setObj(prob.newCtr("OBJ", objective));  /* Set the objective function */
	//end objective

	/**** CONSTRAINTS ****/

	//Routing constraints
	XPRBexpr p1;
	XPRBexpr p2;
	for (int i : AllNodes){
		for (int t : Periods) {
			for (int j : AllNodes) {
				if (inArcSet(i, j)) {
					p1 += x[i][j][t];
				}
				if (inArcSet(j, i)) {
					p2 += x[j][i][t];
				}
			}
			prob.newCtr("RoutingFlow", p1 - p2 = 0);
			p1 = XPRBexpr();
			p2 = XPRBexpr();
		}
	}

	//Linking x and y
	for (int i : AllNodes){
		for (int t : Periods) {
			for (int j : AllNodes) {
				if (inArcSet(i, j)) {
					p1 += x[i][j][t];
				}
				p2 = y[i][t];
				prob.newCtr("LinkingArcandVisit", p1 - p2 = 0);
				p1 = XPRBexpr();
				p2 = XPRBexpr();
			}
		}
	}	

	//Max visit
	for (int i : Nodes) {
		for (int t : Periods) {
			p1 = y[i][t];
			prob.newCtr("Max visit", p1 = 0);
			p1 = XPRBexpr();
		}
	}

	//Depot
	for (int t : Periods) {
		p1 = y[0][t];
		prob.newCtr("Max vehicles", p1 <= nVehicles);
		p1 = XPRBexpr();
	}

	//Inventory constraints
	for (int i : Nodes) {
		p1 = inventory[i][0];
		prob.newCtr("Initial inventory", p1 <= InitInventory[i]);
	}


	return false;
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
				TransCost[i][j] = map.getTransCost(i, j, TRANSCOST_MULTIPLIER, SERVICECOST_MULTIPLIER);
				printf("%-10d", TransCost[i][j]);
			}
			else {
				TransCost[i][j] = 0;
				printf("%-10d", TransCost[i][j]);
			}
			
		}
	} // end initialization TransCost
	
	HoldCost = new int [Nodes.size()];
	for (int i : Nodes) {
		HoldCost[i] = map.getHoldCost(i);
	} //end initialization HoldCost

	InitInventory = new int[Nodes.size()];

	for (int i : Nodes) {
		InitInventory[i] = map.getInitInventory(i);
	}


	UpperLimit = new int[Nodes.size()];
	LowerLimit = new int[Nodes.size()];
	for (int i : Nodes) {	
		UpperLimit[i] = map.getUpperLimit(i);
		LowerLimit[i] = map.getLowerLimit(i);
	} //end limit initialization

	printf("\n");
	printf("Demand Delivery");

	Demand = new int * [Nodes.size()];

	for (int i : DeliveryNodes) {
		printf("\n");
		Demand[i] = new int [Periods.size()];
		for (int t : Periods) {
			if (t > 0) {
				Demand[i][t] = map.getDemand(i, t, Customer::DELIVERY);
				printf("%-10d", Demand[i][t]);
			}
		}
	} //end demand delivery nodes

	printf("\n");
	printf("Pickup Delivery");

	for (int i : PickupNodes) {
		printf("\n");
		Demand[i] = new int[Periods.size()];
		for (int t : Periods) {
			if (t > 0) {
				Demand[i][t] = map.getDemand(i, t, Customer::PICKUP);
				printf("%-10d", Demand[i][t]);
			}
		}
	} //end demand delivery nodes

	return true;
}





bool IRP::initializeSets()
{
	NumOfCustomers = database.getnCustomers();					//Number of customers
	NumOfPeriods = 2;

	ModelBase::createRangeSet(1, NumOfPeriods, Periods);
	ModelBase::createRangeSet(0, NumOfPeriods, AllPeriods);
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
	
	for (int i : Nodes) {
		y[i] = new XPRBvar[Periods.size()];
		
		for (int t : Periods) {
			y[i][t] = prob.newVar(XPRBnewname("y_%d%d", i, t), XPRB_PL, 0, 1);
		
		}
	}

	inventory = new XPRBvar *[DeliveryNodes.size()];

	for (int i : Nodes) {
		inventory[i] = new XPRBvar[Periods.size()];
		for (int t : AllPeriods) {
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
}// end initialize variables


bool IRP::inArcSet(int i, int j)
{
	
	bool a = (i == j || (i == getNumOfCustomers() + j && j != 0));
	return !a;
}




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
