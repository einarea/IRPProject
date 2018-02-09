#include "stdafx.h"
#include "IRP.h"
#include "ModelBase.h"
#include <algorithm>
#include "graphAlgorithm.h"

using namespace ::dashoptimization;

//Global callBack manager
int XPRS_CC cbmng(XPRSprob oprob, void * vd)
{
	// subtour callback
	//printf("h");
	XPRBprob * prob;
	prob = (XPRBprob*) vd;
	int ind = prob->sync(XPRB_XPRS_PROB);
	double a = prob->getObjVal();
	/*
	IRP * modelInstance;
	modelInstance = (IRP*)vd;
	int Periods = modelInstance->getNumOfPeriods();

	vector <vector<Node>> result; //matrix to store strong components

	for (int t = 1; t <= Periods; t++) {
		modelInstance->sepStrongComponents(result, t);
		modelInstance->addSubtourCut(result, t);
		//}*/
		return 0;
	
}


IRP::IRP(CustomerDB& db)
	:
	database(db),
	prob("IRP"),			//Initialize problem in BCL							
	map(db)			//Set up map of all customers
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

	oprob = prob.getXPRSprob();
	int a=prob.setCutMode(1); // Enable the cut mode
	XPRSsetcbcutmgr(oprob, cbmng, &prob);

	//prob.lpOptimize();
	//int b = prob.getLPStat();
	
	int d = prob.solve("g");
	prob.print();
	for (int i : AllNodes)
		for (int t : Periods){
			y[i][t].print();
			printf("\t");
			for (int j : AllNodes) {
				if (inArcSet(i, j)) {
					XPRBvar temp = prob.getVarByName(XPRBnewname("x%d-%d%d", i, j, t));
					(x[i][j][t]).print();
					printf("\t");
					/*(loadDelivery[i][j][t]).print();
					printf("\t");
					(loadPickup[i][j][t]).print();
					printf("\t"); */
				}
			}
			
			(time[i][t]).print();
			printf("\t");

			if (i > 0 ){
				if (i <= DeliveryNodes.size()) {
					(delivery[i][t]).print();
				}
				else {
					printf("\t\t\t\t\t\t");
					(pickup[i][t]).print();
				}
				
				printf("\n");
				}
			else
				printf("\n");
	}


	int c = prob.getObjVal();
}

void IRP::sepStrongComponents(vector<vector<Node>> &result, int t)
{
	//Load LP relaxation currently held in the optimizer
	prob.sync(XPRB_XPRS_PROB);
	double a = prob.getObjVal();
	vector <Node> graph;
	buildGraph(graph, t);
	printGraph(graph);
	graphAlgorithm::sepByStrongComp(graph, result);
	graph.clear();
}


void IRP::buildGraph(vector<Node> &graph, int t)
{
	int s;
	double edgeValue;
							//Create nodes for each visited customer
		for (int i : Nodes) {
			if (y[i][t].getSol() >= 0.1); {
				double a = y[i][t].getSol();
				Node node(i);
				graph.push_back(node);
			}
		}

		//Add outgoing edges from each visited node
		for (Node &node : graph) {
			s = node.getId();
			for (Node &endingNode : graph) {
				if (inArcSet(s, endingNode.getId())) {
					edgeValue = x[s][endingNode.getId()][t].getSol();
					if (edgeValue > 0.01) {
						node.addEdge(edgeValue, endingNode);
					}
				}
			}
		}
}

void IRP::printGraph(vector<Node> &graph)
{
	for (Node node : graph) {
		int id = node.getId();
		vector<Node::Edge> edges = (*node.getEdges());

		for (Node::Edge &edge : (*node.getEdges())) {
			Node * endNode = edge.getEndNode();
			printf("Node: %d with edges to ", id);
			printf("%d with flow %f" , (*endNode).getId(), edge.getValue());
			printf("\n");
		}
		
	}
		
}

void IRP::addSubtourCut(vector<vector<Node>>& strongComp, int t)
{
	XPRBcut subtourCut[1];
	//Check if SEV is violated
	for (int i = 0; i < strongComp.size(); i++) {
		double circleFlow = 0;
		double nodeFlow = 0;
		double tempNodeFlow = 0;
		int maxVisit = -1;
		for (Node &node : strongComp[i]) {
			for (Node::Edge &edge : (*node.getEdges())) {
				circleFlow += edge.getValue();
				tempNodeFlow += edge.getValue();
			}
			//Add flow to depot for the node
			tempNodeFlow += x[node.getId()][0][t].getSol();
			if (tempNodeFlow >= nodeFlow) {
				nodeFlow = tempNodeFlow;
				maxVisit = node.getId();
			}
			tempNodeFlow = 0;
		}

		if (circleFlow >= nodeFlow + 0.1) {
			//addSubtour constraint
			//printf("Circleflow: %d:		NodeFlow: %d\n", circleFlow, nodeFlow);
			XPRBexpr rSide;
			XPRBexpr cut;
			
			for (Node &node : strongComp[i]) {
				rSide += y[node.getId()][t];
				for (Node::Edge &edge : (*node.getEdges())) {
					cut += x[node.getId()][(*edge.getEndNode()).getId()][t];
				}
			}
		
			subtourCut[0] = prob.newCut(cut <= rSide - y[maxVisit][t]);
			prob.addCuts(subtourCut, 1);
			maxVisit = -1;
			subtourCut[0] = 0;

		}
	}
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
		for (int t : Periods) {
			for (int j : Nodes){
				if (inArcSet(i, j)) {
					p1 = time[i][t] - time[j][t] + TravelTime[i][j]
						+ (maxTime + TravelTime[i][j]) * x[i][j][t];

					p2 = maxTime + TravelTime[i][j];
					prob.newCtr("Time flow", p1 <= p2);
					p1 = 0;
					p2 = 0;
				}
	
				}

			if (inArcSet(i, 0)) {
				p1 = time[i][t] + TravelTime[i][0] * x[i][0][t];
				prob.newCtr("Final time", p1 <= maxTime);
				p1 = 0;
			}
			}
	}
	
	
	//end time constraints

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
			p1 += pickup[i][t];

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
					prob.newCtr("ArcCapacity", p1 <= 0);
					p1 = 0;
				}
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

CustomerDB * IRP::getDB()
{
	return &database;
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
	NumOfPeriods = database.getnPeriods();

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
	
	for (int i : AllNodes) {
		y[i] = new XPRBvar[Periods.size()];
		
		for (int t : Periods) {
			if(i>0)
			y[i][t] = prob.newVar(XPRBnewname("y_%d%d", i, t), XPRB_PL, 0, 1);
			else
			y[i][t] = prob.newVar(XPRBnewname("y_%d%d", 0, t), XPRB_PL, 0, nVehicles);	
		}
	}


	inventory = new XPRBvar *[Nodes.size()];

	for (int i : Nodes) {
		inventory[i] = new XPRBvar[Periods.size()];
		for (int t : AllPeriods) {
			inventory[i][t] = prob.newVar(XPRBnewname("i_%d%d", i, t), XPRB_PL, 0, 100);
		}
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
			time[i][t] = prob.newVar(XPRBnewname("t_%d%d", i, t), XPRB_PL, 0, 1000);
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



int IRP::getNumOfPeriods()
{
	return Periods.size();
}


IRP::~IRP()
{
}
