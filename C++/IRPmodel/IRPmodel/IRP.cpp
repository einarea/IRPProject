#include "stdafx.h"
#include "IRP.h"
#include "ModelBase.h"
#include <algorithm>
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

void IRP::solveLP()
{
	prob.lpOptimize();
	int b = prob.getLPStat();
	prob.print();
	int c = 2;
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
			prob.newCtr("RoutingFlow", p1 - p2 == 0);
			p1 = 0;
			p2 = 0;
		}
	}

	//Linking x and y
	for (int i : AllNodes){
		for (int t : Periods) {
			for (int j : AllNodes) {
				if (inArcSet(i, j)) {
					p1 += x[i][j][t];
				}
			}
			p2 = y[i][t];
			prob.newCtr("LinkingArcandVisit", p1 - p2 == 0);
			p1 = 0;
			p2 = 0;	
		}
	}	

	//Depot
	for (int t : Periods) {
		p1 = y[0][t];
		prob.newCtr("Max vehicles", p1 <= nVehicles);
		p1 = 0;
	}

	//Max visit
	for (int i : Nodes) {
		for (int t : Periods) {
			p1 = y[i][t];
			prob.newCtr("Max visit", p1 <= 1);
			p1 = 0;
		}
	}

	

	//Inventory constraints
	for (int i : Nodes) {
		p1 = inventory[i][0];
		printf("%d", InitInventory[i]);
		prob.newCtr("Initial inventory", inventory[i][0] == InitInventory[i]);
		p1 = 0;
	}

	for (int i : DeliveryNodes) {
		for (int t: Periods) {
			p1 = inventory[i][t] - inventory[i][t - 1] + Demand[i][t] - delivery[i][t];
			prob.newCtr("Delivery Inventory balance", p1 == 0);
			p1 = 0;

			p1 = inventory[i][t - 1] + delivery[i][t];
			prob.newCtr("Delivery Inventory balance 2", p1 <= UpperLimit[i]);
			p1 = 0;
		}
	}

	for (int i : PickupNodes) {
		for (int t : Periods) {
			printf("%d", Demand[i][t]);
			p1 = inventory[i][t] - inventory[i][t - 1] - Demand[i][t] + pickup[i][t];
			prob.newCtr("Pickup Inventory balance", p1 == 0);
			p1 = 0;

			p1 = inventory[i][t - 1] - pickup[i][t];
			prob.newCtr("Pickup Inventory balance 2", p1 >= LowerLimit[i]);
			p1 = 0;
		}
	}

	//Upper and lower inventory
	for (int i : Nodes) {
		for (int t : Periods) {
			p1 = inventory[i][t];
			prob.newCtr("Inventory Lower Limit", p1 >= LowerLimit[i]);
			prob.newCtr("Inventory Upper Limit", p1 <= UpperLimit[i]);
			p1 = 0;
		}
	}


	//Time constraints
	for (int t : Periods) {
		p1 = time[0][t];
		prob.newCtr("Initial time", p1 == 0);
		p1 = 0;
	}

	for (int i : AllNodes) {
		for (int j : AllNodes) {
			if (inArcSet(i, j)) {
				for (int t : Periods) {
					p1 = time[i][t] - time[j][t] + TravelTime[i][j]
						+ (maxTime + TravelTime[i][j]) * x[i][j][t];
					prob.newCtr("Time flow", p1 <= (maxTime + TravelTime[i][j]));
					p1 = 0;

					p1 = time[i][t] + TravelTime[i][j] * x[i][j][t];
					prob.newCtr("Time flow 2", p1 <= maxTime);
					p1 = 0;
				}

			}
		}
	} //end time constraints

	//Loading constraints
	for (int i : DeliveryNodes) {
		for (int t : Periods) {
			for (int j : AllNodes) {
				if (inArcSet(j, i)) {
					p1 += loadDelivery[j][i][t];
					p2 -= loadPickup[j][i][t];
				}
				if (inArcSet(i, j)) {
					p1 -= loadDelivery[i][j][t];
					p2 += loadPickup[i][j][t];
				}
			}
			p1 -= delivery[i][t];
			
			prob.newCtr("LoadBalance Delivery", p1 == 0);
			prob.newCtr("PickupBalance at deliveryNodes", p2 == 0);
			p1 = 0;
			p2 = 0;
		}

	}

	for (int i : PickupNodes) {
		for (int t : Periods) {
			for (int j : AllNodes) {
				if (inArcSet(j, i)) {
					p1 += loadPickup[j][i][t];
					p2 += loadDelivery[j][i][t];
				}
				if (inArcSet(i, j)) {
					p1 -= loadPickup[i][j][t];
					p2 -= loadDelivery[i][j][t];
				}
			}
			p1 -= pickup[i][t];

			prob.newCtr("LoadBalance pickup", p1 == 0);
			prob.newCtr("DeliveryBalance at pickupNodes", p2 == 0);
			p1 = 0;
			p2 = 0;
		}

	}

	//Arc capacity
	for (int i : AllNodes) {
		for (int j : AllNodes) {
			if (inArcSet(i, j)) {
				for (int t : Periods) {
					p1 = loadDelivery[i][j][t] + loadPickup[i][j][t] - Capacity * x[i][j][t];
				}
				prob.newCtr("ArcCapacity", p1 <= 0);
				p1 = 0;
			}
		}
	}

	//Vehicle capacity
	for (int i : DeliveryNodes) {
		for (int t : Periods) {
			p1 = delivery[i][t] - min(Capacity, UpperLimit[i] - LowerLimit[i])*y[i][t];

			prob.newCtr("Vehicle capacity delivert", p1 <= 0);
			p1 = 0;
		}
	}

	for (int i : PickupNodes) {
		for (int t : Periods) {
			p1 = pickup[i][t] - min(Capacity, UpperLimit[i] - LowerLimit[i])*y[i][t];

			prob.newCtr("Vehicle capacity pickup", p1 <= 0);
			p1 = 0;
		}
	}
	return true;
}



XPRBprob & IRP::getProblem() {
	return prob;
}


bool IRP::initializeParameters() {
	TransCost = new int *[AllNodes.size()];
	TravelTime = new int *[AllNodes.size()];

	for (int i : AllNodes) {
		printf("\n");
		TransCost[i] = new  int[AllNodes.size()];
		TravelTime[i] = new  int[AllNodes.size()];
		for (int j : AllNodes) {
			if (inArcSet(i, j)) {
				TravelTime[i][j] = map.getTravelTime(i, j, TRAVELTIME_MULTIPLIER);
				TransCost[i][j] = map.getTransCost(i, j, TRANSCOST_MULTIPLIER, SERVICECOST_MULTIPLIER);
				printf("%-10d", TravelTime[i][j]);
			}
			else {
				TransCost[i][j] = 0;
				TravelTime[i][j] = 0;
				printf("%-10d", TravelTime[i][j]);
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
	} //end initialization initial inventory


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
	} //end demand pickup Nodes


	

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

			if (inArcSet(i, j)) {
			for (int t : Periods) {
				x[i][j][t] = prob.newVar(XPRBnewname("x%d-%d%d", i, j, t), XPRB_BV, 0, 1);
				loadDelivery[i][j][t] = prob.newVar(XPRBnewname("lD_%d%d%d", i, j, t), XPRB_PL, 0, 100);
				loadPickup[i][j][t] = prob.newVar(XPRBnewname("lP_%d%d%d", i, j, t), XPRB_PL, 0, 100);
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

	inventory = new XPRBvar *[Nodes.size()];

	for (int i : Nodes) {
		inventory[i] = new XPRBvar[Periods.size()];
		for (int t : AllPeriods) {
			inventory[i][t] = prob.newVar(XPRBnewname("i_%d%d", i, t), XPRB_PL, 0, 100);
		}
	}

	//Initalize depot
	
	y[0] = new XPRBvar[Periods.size()];
	for (int t : Periods) {
		y[0][t] = prob.newVar(XPRBnewname("y_%d", t), XPRB_PL, 0, nVehicles);
	}

	//Initialize at delivery nodes
	delivery = new XPRBvar *[DeliveryNodes.size()+1];
	for (int i : DeliveryNodes) {
		delivery[i] = new XPRBvar [Periods.size()+1];
		for (int t : Periods) {
			delivery[i][t] = prob.newVar(XPRBnewname("qD_%d%d", i, t), XPRB_PL, 0, 100);
			
		}
	}

	//Initialize at pickup nodes
	pickup = new XPRBvar *[PickupNodes.size()+NumOfCustomers+1];
	for (int i : PickupNodes){
		pickup[i] = new  XPRBvar[Periods.size()+1];
		for (int t : Periods) {
			pickup[i][t] = prob.newVar(XPRBnewname("qP_%d%d", i, t), XPRB_PL, 0, 100);
		}
	}

	//Initialize time variables
	time = new XPRBvar *[AllNodes.size()];
	for (int i : AllNodes) {
		time[i] = new XPRBvar[Periods.size()];
		for (int t : Periods) {
			time[i][t] = prob.newVar(XPRBnewname("t_%d%d", i, t), XPRB_PL, 0, 100);
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
