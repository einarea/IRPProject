#include "stdafx.h"
#include "IRP.h"
#include "ModelBase.h"
#include <algorithm>
#include "graphAlgorithm.h"
#include "ModelParameters.h"
#include "NodeIRP.h"

#include <boost/tuple/tuple.hpp>

#include "gnuplot-iostream.h"


using namespace ::dashoptimization;

//Global callBack manager
int XPRS_CC cbmng(XPRSprob oprob, void * vd)
{
	// subtour callback

	int depth;
	int node;

	XPRSgetintattrib(oprob, XPRS_NODEDEPTH, &depth);
	XPRSgetintattrib(oprob, XPRS_NODES, &node);
	double objval;
	XPRSgetdblattrib(oprob, XPRS_LPOBJVAL, &objval);

/*	vector<XPRBcut> subtourCut;
	
	IRP * modelInstance;
	modelInstance = (IRP*)vd;
	if ((XPRB::getTime() - modelInstance->startTime) / 1000 >= ModelParameters::MAX_RUNNING_TIME)
	{
		XPRSinterrupt(oprob, XPRS_STOP_TIMELIMIT);
	}*/

	//Load LP relaxation currently held in the optimizer
	//modelInstance->getProblem()->beginCB(oprob);
	//modelInstance->getProblem()->sync(XPRB_XPRS_SOL);
	//double a = modelInstance->getProblem()->getObjVal();
	//Add subtourconstraints
	//bool addedCuts = modelInstance->sepStrongComponents(subtourCut);

	
	if (false){//addedCuts) {
		int node;
		/*double *lBound;
		double *uBound;
		int bb;
		int *cutPtr = &bb;
		XPRScut m[10];
		modelInstance->printBounds();*/ 
		XPRSgetintattrib(oprob, XPRS_NODES, &node);
		XPRSgetdblattrib(oprob, XPRS_LPOBJVAL, &objval);
		//cout << "Added at node ";
		//cout << node << "), obj. " << objval << endl;

		//for (XPRBcut cut : subtourCut)
		//{
			//cut.print();
			//modelInstance->nSubtourCuts += 1;
			//XPRSstorecuts(oprob, 1, 1, , 

		//	modelInstance->getProblem()->addCuts(&cut, 1);
		//}

	}
	
	//modelInstance->getProblem()->endCB(); 
	//Unload LP relaxation
	return 0;
	
}


IRP::Solution * IRP::getSolution(int id)
{
	Solution * sol = solutions[id - 1];
	return solutions[id - 1];
}

void IRP::printBounds()
{
	double *lbd;
	double *upd;
	for (int i : Nodes) {
		for (int t : Periods)
			if(y[i][t].getUB()<=0.5)
				cout<<y[i][t].getSol();
	}
}

IRP::IRP(CustomerIRPDB& db, bool ArcRel, bool maskOn, int ** VisitMask)
	:
	database(db),
	prob("IRP"),			//Initialize problem in BCL	
	map(db),			//Set up map of all customers
	ARC_RELAXED(ArcRel),
	SolutionCounter(0),
	MaskOn(maskOn)
{
	//Initialize node
	NodeIRP::instance = *this;
	//Initialize sets
	if(!initializeSets()) {
		cout<<"Data Error: Could not initialize sets.";
		return;
	}

	if (MaskOn) {
		generateMask(VisitMask);
	}

	//Initialize parameters
	if (!initializeParameters()) {
		cout << "Data Error: Could not initialize parameters.";
		return;
	}
	
	//Initialize variables
	if (!initializeVariables()) {
		cout << "Data Error: Could not initialize variables.";
		return;
	}



	formulateProblem();

}



IRP::Solution * IRP::solveModel()
{
	
	oprob = prob.getXPRSprob();
	startTime = XPRB::getTime();
	
	//Enable subtour elimination
	//int a=prob.setCutMode(1); // Enable the cut mode
	XPRSsetcbcutmgr(oprob, cbmng, &(*this));

	//double b =prob.lpOptimize();
	//int b = prob.getLPStat();
	
	int d = prob.mipOptimise();
	//prob.print();
	int SolID = allocateIRPSolution();

	return getSolution(SolID);
}

Map * IRP::getMap()
{
	return &map;
}

void IRP::calculateExcess()
{
	int excess = 0;
	ExcessConsumption = new double *[DeliveryNodes.size()];
	for (int i : DeliveryNodes) {
		ExcessConsumption[i] = new double[Periods.size()];
		excess = LowerLimit[i] - InitInventory[i];
		for (int t : Periods) {
			excess += Demand[i][t];
			if (excess >= 1) ExcessConsumption[i][t] = excess;
			else  ExcessConsumption[i][t] = 0;

			cout << "Node: " << i << "\tperiod: " << t << "\tExcessDelivery: " << ExcessConsumption[i][t]<<"\n";
		}
	}
	
	excess = 0;
	ExcessProd = new double *[Nodes.size()+1];
	for (int i : PickupNodes) {
		ExcessProd[i] = new double[Periods.size()];
		excess = InitInventory[i] - UpperLimit[i];
		for (int t : Periods) {
			excess += Demand[i][t];
			if (excess >= 1) ExcessProd[i][t] = excess;
			else  ExcessProd[i][t] = 0;

			cout << "Node: " << i << "\tperiod: " << t << "\tExcessPickup: " << ExcessProd[i][t] << "\n";
		}
	}

}

int IRP::allocateIRPSolution()
{
	SolutionCounter++;
	//Allocate x and loading solutions
	Solution * sol = new Solution(*this, false);
	sol->SolID = SolutionCounter;


	for (int i : AllNodes) {
		for (int j : AllNodes) {
			if (map.inArcSet(i, j)) {
				for (int t : Periods) {
					if (x[i][j][t].getSol() > 0) {
						if (map.isDelivery(i))
							sol->Nodes[i]->addEdge(loadDelivery[i][j][t].getSol(), sol->Nodes[j], t, x[i][j][t].getSol());
						else
							sol->Nodes[i]->addEdge(loadPickup[i][j][t].getSol(), sol->Nodes[j], t, x[i][j][t].getSol());
					}
				}
			} //endif
		} //end for j
	} // end x and loading

	  //Fill inventory
	for (int i : Nodes) {
		for (int t : Periods) {
			sol->Nodes[i]->Inventory[t] = inventory[i][t].getSol();
		}
	}

	//Fill delivery and pickup
	for (int i : Nodes) {
		for (int t : Periods) {
			if (map.isDelivery(i)) {
				if (delivery[i][t].getSol() > 0) {
					sol->Nodes[i]->Quantity[t] = delivery[i][t].getSol();
					sol->Nodes[i]->TimeServed[t] = time[i][t].getSol();
				}
				else {
					sol->Nodes[i]->Quantity[t] = 0;
					sol->Nodes[i]->TimeServed[t] = time[i][t].getSol();
				}
			}
			else {
				if (pickup[i][t].getSol() > 0)
					sol->Nodes[i]->Quantity[t] = pickup[i][t].getSol();
				else
					sol->Nodes[i]->Quantity[t] = 0;
			} //end else
		} //end t
	} //end i

	solutions.push_back(sol);
	//Return ID to solution
	return getCounter();
}

int IRP::allocateSolution()
{
	SolutionCounter++;
	Solution * sol = new Solution(*this, true);
	sol->SolID = SolutionCounter;

	//Fill Inventory
	for (int i : Nodes) {
		for (int t : Periods) {
			sol->Nodes[i]->Inventory[t] = inventory[i][t].getSol();
		}
	}

	//Fill Quantity and allocate time
	for (int i : Nodes){ 
		for (int t : Periods) {
			if (map.isDelivery(i)) {
				if (delivery[i][t].getSol() > 0) {
					sol->Nodes[i]->Quantity[t] = delivery[i][t].getSol();
					sol->Nodes[i]->TimeServed[t] = time[i][t].getSol();
				}
				else
					sol->Nodes[i]->Quantity[t] = 0;
			}
			else {
				if (pickup[i][t].getSol() > 0) {
					sol->Nodes[i]->Quantity[t] = pickup[i][t].getSol();
					sol->Nodes[i]->TimeServed[t] = time[i][t].getSol();
				}
				else
					sol->Nodes[i]->Quantity[t] = 0;
			}
		}
	}

	solutions.push_back(sol);
	//Return ID to solution
	return getCounter();
}

bool IRP::sepStrongComponents(vector<XPRBcut> & cut)
{
	vector <vector<Node*>> result; //matrix to store strong components
	vector <Node*> graph;		//Graph to store nodes

	bool newCut = false;
	for (int t : Periods) {
		buildGraph(graph, t, true); //include depot
		//graphAlgorithm::printGraph(graph, *this);
		graph.clear();
		buildGraph(graph, t, false); //Do not include depot in graph
		//graphAlgorithm::printGraph(graph, *this);
		graphAlgorithm::sepByStrongComp(graph, result);
		//graphAlgorithm::printGraph(graph, *this);
		addSubtourCut(result, t, newCut, cut);
		graph.clear();
		result.clear();
	}
	return newCut;
}


void IRP::buildGraph(vector<Node*> &graph, int t, bool Depot)
{
	int s;
	double edgeValue;

	if (Depot){
		Node * node = new Node(0);
		graph.push_back(node);
	}
	//Create nodes for each visited customer
	for (int i : Nodes) {
		if (y[i][t].getSol() >= 0.01){
			Node * node = new Node(i);
			graph.push_back(node);
		}
	}

	//Add outgoing edges from each visited node
	for (Node *node : graph) {
		s = node->getId();
		for (Node *endingNode : graph) {
			if (map.inArcSet(s, endingNode->getId())) {
				edgeValue = x[s][endingNode->getId()][t].getSol();
				if (edgeValue > 0.01) {
					node->addEdge(edgeValue, *endingNode);
				}
			}
		}
	}
}

void IRP::buildGraph(vector<Node*> &graph, int t, IRP::Solution *solution)
{
	int s;
	double edgeValue;

	//Create nodes for each visited customer
	for (int i : AllNodes) {
		if (solution->ySol[i][t] >= 0.01) {
			Node * node = new Node(i);
			graph.push_back(node);
		}
	}

	//Add outgoing edges from each visited node
	for (Node *node : graph) {
		s = node->getId();
		for (Node *endingNode : graph) {
			if (map.inArcSet(s, endingNode->getId())) {
				edgeValue = solution->xSol[s][endingNode->getId()][t];
				if (edgeValue > 0.01) {
					node->addEdge(edgeValue, *endingNode);
				}
			}
		}
	}
}

void IRP::printGraph(vector<Node> &graph)
{
	for (Node node : graph) {
		int id = node.getId();
		vector<Node::Edge*> edges = (*node.getEdges());

		for (Node::Edge* edge : (*node.getEdges())) {
			Node * endNode = edge->getEndNode();
			/*printf("Node: %d with edges to ", id);
			printf("%d with flow %f" , (*endNode).getId(), edge.getValue());
			printf("\n");*/
		}
		
	}
		
}

int IRP::getCounter()
{
	return SolutionCounter;
}

void IRP::addSubtourCut(vector<vector<Node *>>& strongComp, int t, bool &newCut, vector<XPRBcut> &SubtourCut)
{
	//Check if SEC is violated
	for (int i = 0; i < strongComp.size(); i++) {
		if (strongComp[i].size() >= 2) {// only cut for subsets of size 2 or greater
			double circleFlow = 0;
			double visitSum = 0;
			double maxVisitSum = 0;
			double tempNodeVisit = 0;
			//double tempNodeFlow = 0;
			int maxVisitID = -1;
			for (Node *node : strongComp[i]) {

				//Value for y variable in the strong component
				tempNodeVisit = y[node->getId()][t].getSol();
			//	printf("y%d%d: %.2f\t", node.getId(), t, y[node.getId()][t].getSol());
				visitSum += tempNodeVisit;

				for (Node::Edge *edge : *(node->getEdges())) {
					if(edge->getValue() >= 0) //only inlude edges in strong component
						circleFlow += edge->getValue();
						int u = node->getId();
						int v = edge->getEndNode()->getId();
					//	printf("x_%d%d: %.2f\t + ", u, v, x[u][v][t].getSol());
				}

				if (tempNodeVisit >= maxVisitSum) {
					maxVisitID = node->getId();
					maxVisitSum = tempNodeVisit;
				}

			}

			if (circleFlow >= visitSum - maxVisitSum + 0.7) {
				//print subtour
				//graphAlgorithm::printGraph(strongComp[i], *this);
				// save current basis
				//SavedBasis.push_back(prob.saveBasis());
				//addSubtour constraint
				//printf("Circleflow: %d:		NodeFlow: %d\n", circleFlow, nodeFlow);
				XPRBexpr rSide;
				XPRBexpr lSide;
				
				//cout << "LP relaxation before cut: " << prob.getObjVal() << "\n";
				string rSideStr = "<=";
				//printf("\nAdded subtour cut: ");

				for (Node *node : strongComp[i]) {
					rSide += y[node->getId()][t];
					rSideStr = rSideStr + " + " + "y_" + to_string(node->getId());
					for (Node::Edge *edge : *(node->getEdges())) {
						if (edge->getValue() >= 0) {
							int u = node->getId();
							int v = edge->getEndNode()->getId();
							//printf("x_%d%d + ", u, v);
							lSide += x[node->getId()][edge->getEndNode()->getId()][t];
						}
					}
				}

			
				rSideStr = rSideStr + " - " + "y_" + to_string(maxVisitID);
				//cout << rSideStr << "\n";
				nSubtourCuts += 1;
				XPRBcut vv = prob.newCut( lSide <= rSide - y[maxVisitID][t]);
 				SubtourCut.push_back(vv);
				newCut = true;		
				maxVisitID = -1;
			}
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
				if (map.inArcSet(i, j))
					objective += TransCost[i][j] * x[i][j][t];
			}
	//Holding costs
	for (int i : Nodes)
		for (int t : Periods)
			objective += HoldCost[i] * inventory[i][t];

	prob.setObj(prob.newCtr("OBJ", objective));  /* Set the objective function */
	//end objective

	/**** CONSTRAINTS ****/

	if (MaskOn) {
		XPRBexpr p1;
		int visits;
		for (int t : Periods) {
			p1 = 0;
			visits = 0;
			for (int i : Nodes) {
				if (VisitNode[i][t] == 1) {
					visits += 1;
					p1 += y[i][t];
				}
			}
			prob.newCtr("MinVisits", p1 >= floor(visits*0.6));
		}
	}

	//Routing constraints
	XPRBexpr p1;
	XPRBexpr p2;
	for (int i : AllNodes){
		for (int t : Periods) {
			for (int j : AllNodes) {
				if (map.inArcSet(i, j)) {
					p1 += x[i][j][t];
				}
				if (map.inArcSet(j, i)) {
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
				if (map.inArcSet(i, j)) {
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
		prob.newCtr("Max vehicles", p1 <= ModelParameters::nVehicles);
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
		//printf("%d", InitInventory[i]);
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
			//printf("%d", Demand[i][t]);
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
				if (map.inArcSet(i, j)) {
					p1 = time[i][t] - time[j][t] + TravelTime[i][j]
						+ (ModelParameters::maxTime + TravelTime[i][j]) * x[i][j][t];

					p2 = ModelParameters::maxTime + TravelTime[i][j];
					prob.newCtr("Time flow", p1 <= p2);
					p1 = 0;
					p2 = 0;

					//p1 = time[i][t] + TravelTime[i][j] * x[i][j][t] <= ModelParameters::maxTime;
					//prob.newCtr("Final time", p1 <= ModelParameters::maxTime);
					//p1 = 0;
				}
	
				}

			if (map.inArcSet(i, 0)) {
				p1 = time[i][t] + TravelTime[i][0] * x[i][0][t];
				prob.newCtr("Final time", p1 <= ModelParameters::maxTime);
				p1 = 0;
			}
			}
	}
	
	
	//end time constraints

	//Loading constraints
	for (int i : DeliveryNodes) {
		for (int t : Periods) {
			for (int j : AllNodes) {
				if (map.inArcSet(j, i)) {
					p1 += loadDelivery[j][i][t];
					p2 -= loadPickup[j][i][t];
				}
				if (map.inArcSet(i, j)) {
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
				if (map.inArcSet(j, i)) {
					p1 += loadPickup[j][i][t];
					p2 += loadDelivery[j][i][t];
				}
				if (map.inArcSet(i, j)) {
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
			if (map.inArcSet(i, j)) {
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



XPRBprob * IRP::getProblem() {
	return &prob;
}

CustomerDB * IRP::getDB()
{
	return &database;
}


bool IRP::initializeParameters() {
	TransCost = new int *[AllNodes.size()];
	TravelTime = new int *[AllNodes.size()];

	for (int i : AllNodes) {
		//printf("\n");
		TransCost[i] = new  int[AllNodes.size()];
		TravelTime[i] = new  int[AllNodes.size()];
		for (int j : AllNodes) {
			if (map.inArcSet(i, j)) {
				TravelTime[i][j] = map.getTravelTime(i, j, ModelParameters:: TRAVELTIME_MULTIPLIER, ModelParameters::SERVICETIME);
				TransCost[i][j] = map.getTransCost(i, j, ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);
				//printf("%-5d", TransCost[i][j]);
			}
			else {
				TransCost[i][j] = -1;
				TravelTime[i][j] = -1;
				//printf("%-5d", TransCost[i][j]);
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
		//printf("%-5d", InitInventory[i]);
	} //end initialization initial inventory


	UpperLimit = new int[Nodes.size()];
	LowerLimit = new int[Nodes.size()];
	for (int i : Nodes) {	
		UpperLimit[i] = map.getUpperLimit(i);
		LowerLimit[i] = map.getLowerLimit(i);
	} //end limit initialization

	//printf("\n");
	//printf("Demand Delivery");

	Demand = new int * [Nodes.size()+1];
	int customer;

	for (int i : DeliveryNodes) {
		//printf("\n");
		Demand[i] = new int [Periods.size()+1];
		for (int t : Periods) {
			if (t > 0) {
				Demand[i][t] = map.getDemand(i, t, Customer::DELIVERY);
				//printf("%-10d", Demand[i][t]);
			}
		}
	} //end demand delivery nodes

	//printf("\n");
	//printf("Pickup Delivery");

	for (int i : PickupNodes) {
		//printf("\n");
		Demand[i] = new int[Periods.size()];
		for (int t : Periods) {
			if (t > 0) {
				Demand[i][t] = map.getDemand(i, t, Customer::PICKUP);
				//printf("%-10d", Demand[i][t]);
			}
		}
	} //end demand pickup Nodes

	//Initialize capacity
	int TotalDemand = 0;
	for (int t : Periods) {
		for (int i : DeliveryNodes) {
			TotalDemand += Demand[i][t];
		}
	}
	IRP::Capacity = 2 * floor(TotalDemand / (ModelParameters::nVehicles*getNumOfPeriods()));

	//Initialize max time
	MaxTime = 480;
	
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

void IRP::generateMask()
{
	VisitNode = new int *[Nodes.size()];
	for (int i : Nodes) {
		VisitNode[i] = new int[Periods.size()];
		//cout<<"\n";
		for (int t : Periods) {
			VisitNode[i][t] = (rand() % 2 + 0);
			//cout << VisitNode[i][t]<<"\t";

		}
	}
}

void IRP::generateMask(int ** mask)
{
	VisitNode = new int *[Nodes.size()];
	for (int i : Nodes) {
		VisitNode[i] = new int[Periods.size()];
		//cout<<"\n";
		for (int t : Periods) {
			VisitNode[i][t] = mask[i][t];
			//cout << VisitNode[i][t]<<"\t";

		}
	}
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

			if (map.inArcSet(i, j)) {
			for (int t : Periods) {
				if (ARC_RELAXED)
					x[i][j][t] = prob.newVar(XPRBnewname("x%d-%d%d", i, j, t), XPRB_PL, 0, 1);
				else
					x[i][j][t] = prob.newVar(XPRBnewname("x%d-%d%d", i, j, t), XPRB_BV, 0, 1);

				loadDelivery[i][j][t] = prob.newVar(XPRBnewname("lD_%d%d%d", i, j, t), XPRB_PL, 0, Capacity);
				loadPickup[i][j][t] = prob.newVar(XPRBnewname("lP_%d%d%d", i, j, t), XPRB_PL, 0, Capacity);
			}
			}
		}
	} // end initializing of x and load

	//initialize y-variables and inventory
	y = new XPRBvar *[AllNodes.size()];
	if (ARC_RELAXED) {
		for (int i : AllNodes) {
			y[i] = new XPRBvar[Periods.size()];
			for (int t : Periods) {
				if (i > 0) 
					y[i][t] = prob.newVar(XPRBnewname("y_%d%d", i, t), XPRB_BV, 0, 1);
				
				else
					y[i][t] = prob.newVar(XPRBnewname("y_%d%d", 0, t), XPRB_UI, 0, ModelParameters::nVehicles);
			}
		}
	}
	else
	{
		for (int i : AllNodes) {
			y[i] = new XPRBvar[Periods.size()];
			for (int t : Periods) {
				if (i > 0)
					y[i][t] = prob.newVar(XPRBnewname("y_%d%d", i, t), XPRB_PL, 0, 1);
				else
					y[i][t] = prob.newVar(XPRBnewname("y_%d%d", 0, t), XPRB_PL, 0, ModelParameters::nVehicles);
			}
		}
	}


	inventory = new XPRBvar *[Nodes.size()];

	for (int i : Nodes) {
		inventory[i] = new XPRBvar[Periods.size()];
		for (int t : AllPeriods) {
			inventory[i][t] = prob.newVar(XPRBnewname("i_%d%d", i, t), XPRB_PL, LowerLimit[i], UpperLimit[i]);
		}
	}


	//Initialize at delivery nodes
	delivery = new XPRBvar *[DeliveryNodes.size()+1];
	for (int i : DeliveryNodes) {
		delivery[i] = new XPRBvar [Periods.size()+1];
		for (int t : Periods) {
			delivery[i][t] = prob.newVar(XPRBnewname("qD_%d%d", i, t), XPRB_PL, 0, Capacity);
			
		}
	}

	//Initialize at pickup nodes
	pickup = new XPRBvar *[PickupNodes.size()+NumOfCustomers+1];
	for (int i : PickupNodes){
		pickup[i] = new  XPRBvar[Periods.size()+1];
		for (int t : Periods) {
			pickup[i][t] = prob.newVar(XPRBnewname("qP_%d%d", i, t), XPRB_PL, 0, Capacity);
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





int IRP::getNumOfCustomers()
{
	return NumOfCustomers;
}


//Construct vector of visited nodes
void IRP::getVisitedCustomers(int period, vector<Customer *> &custVisit)
{
	if (ARC_RELAXED == true) {
		for (int i : DeliveryNodes) {
			if (y[i][period].getSol() > 0.01 || y[i+getNumOfCustomers()][period].getSol() > 0.01)
				custVisit.push_back(map.getCustomer(i));
		}
	}
	else
		cout << "Error, problem not solved when creating visited customers";
}

void IRP::getDemand(int t, vector<vector<double>>& demand, vector<Customer *> &customers)
{
	demand.resize(2);
	demand[Customer::DELIVERY].resize(getNumOfCustomers()+1);
	demand[Customer::PICKUP].resize(getNumOfCustomers()+1);
	int node;
	for (Customer* c : customers) {
			demand[Customer::DELIVERY][c->getId()] = round(delivery[map.getDeliveryNode(c)][t].getSol());
			demand[Customer::PICKUP][c->getId()] = round(pickup[map.getPickupNode(c)][t].getSol());
	}
}



int IRP::getNumOfPeriods()
{
	return Periods.size();
}


IRP::~IRP()
{
}



IRP::Solution::Solution(IRP & model, bool integer = false)
	:
	IntegerSolution(integer),
	instance(model)
{
	//Initializa node holder
	Nodes.resize(instance.getNumOfCustomers * 2 + 1);
	//Create a route holder for each period
	Routes.resize(instance.getNumOfPeriods());
	//Create a nodes for all customers and depot
	for (int i : instance.AllNodes) {
		NodeIRP * node = new NodeIRP(instance.map.nodeToCustomer(i), instance.getNumOfPeriods());
		Nodes[i] = node;
	}
}

IRP::Solution::Solution(IRP &model, vector<vector<IRP::Route*>>& routes, bool integer = false)
	:
	IntegerSolution(integer),
	instance(model),
	Routes(routes)
{
	Routes.resize(5);
	//Initialize solution
	
}




void IRP::Solution::print(string filename)
{
	vector<Node *> graph;
	for (int t : instance.Periods) {
		buildGraph(graph, t);
		graphAlgorithm::printGraph(graph, instance, filename+to_string(t));
		graph.clear();
	}
}

bool IRP::Solution::isFeasible()
{
	//Check if too many vehicles
	for (int t : instance.Periods) {
		if (ySol[0][t] > ModelParameters::nVehicles) {
			return false;
		}
	}

	//Check if each route is feasible
	for(int t : instance.Periods)
	for (IRP::Route* r : SolutionRoutes[t]) {
		if (!isRouteFeasible(r))
			return false;
	}
	return true;
}

bool IRP::Solution::isRouteFeasible(IRP::Route * r)
{
	int u;
	int v;
	int t;

	for (Node * n : r->route) {
		u = n->getId();
		v = n->getEdge(0)->getEndNode()->getId();
		t = r->period;
		if (loadDelSol[u][v][t] > instance.Capacity || (loadPickSol[u][v][t] > instance.Capacity))
			return false;
		}
	return true;
}

double IRP::Solution::getNumberOfRoutes()
{
	if (IntegerSolution)
		return SolutionRoutes.size();
	else {
		double totRoutes = 0;
		for (int t : instance.Periods) {
			totRoutes += ySol[0][t];
		}
		return totRoutes;
	}
}

int IRP::Solution::newRoute(vector<Node*>& route, int t)
{
	int id = SolutionRoutes[t].size();
	SolutionRoutes[t].push_back(new Route(route, id, t));
	return SolutionRoutes[t].size() - 1;
}

double IRP::Solution::getObjective()
{
	if (IntegerSolution)
		return getHoldingCost() + getTransInteger();
	else
		return getHoldingCost() + getTransportationCost();
	
}


IRP::Route * IRP::getRoute(int id)
{
	return routes[id];
}


vector<IRP::Route const *> IRP::getRoutes()
{
	vector<IRP::Route const *> routePtr;
	for (int i = 0; i < routes.size(); i++) {
		routePtr.push_back(routes[i]);
	}
	return routePtr;
}


int IRP::newRoute(vector <Node*> & route)
{
	int id = routes.size();
	routes.push_back(new Route(route, id));
	return routes.size()-1;
}

void IRP::addValidIneq()

{
	XPRBexpr p1;

	//Minimum visit

	calculateExcess();

	double minVisit;

	for (int i : DeliveryNodes) {
		minVisit = 0;
		p1 = 0;
			for (int t : Periods) {
				p1 += y[i][t];
				minVisit = ExcessConsumption[i][t] / min(Capacity, UpperLimit[i] - LowerLimit[i]);
				if (ceil(minVisit) - minVisit >= 0.3)
				{
					prob.newCtr("MinVisitDelivery", p1 >= ceil(minVisit));
				}
			}
	}
	for (int i : PickupNodes) {
		minVisit = 0;
		p1 = 0;
		for (int t : Periods) {
			p1 += y[i][t];
			minVisit = ExcessProd[i][t] / min(Capacity, UpperLimit[i] - LowerLimit[i]);
			if (ceil(minVisit) - minVisit >= 0.3)
			{
				prob.newCtr("MinVisitPickup", p1 >= ceil(minVisit));
			}
		}
	}

}

void IRP::printRouteMatrix()
{
	for (int r = 0; r < A.size(); r++) {
		cout << "\n\n\n";
		for (int i = 0; i < AllNodes.size(); i++) {
			cout << "\n";
			for (int j = 0; j < AllNodes.size(); j++) {
				cout << A[r][i][j]<<"  ";

			}
		}
	}
}

void IRP::addRoutesToVector()
{		
	for (IRP :: Route *r : routes)

		A.push_back(r->getRouteMatrix(this));
}
	


void IRP::printMatrix()
{
	for (int i : AllNodes) {
		cout << "\n";
		for (int r = 0; r < (routes).size(); r++) {
			cout << A[i][r];
		}
	}
}

int IRP::getCapacity()
{
	return Capacity;
}


void IRP::Solution::printSolution()
{
	printf("\n\n\nObjective value: %.2f\n", getObjective());
	printf("Transporationcost: %.2f\n", getTransportationCost());
	printf("Holding cost: %.2f\n", getHoldingCost());
	
	int j;

	for (int t : instance.Periods) {
		printf("\n\nPeriod %d:", t);
		for (int i : instance.AllNodes) {
			printf("\n");
			if (i == 0) {
				printf("y0: %d", Nodes[i]->getnEdges());

				for (NodeIRP::EdgeIRP *edge : Nodes[i]->getEdges())
					if (edge->getValue() > 0) {
						printf("x%d%d: %d\t", edge->getValue());
						if (edge->getEndNode()->isDelivery())
							printf("loadDel%d%d: %.2f\t", i, edge->getEndNode()->getId(), edge->Load);
						else
							printf("loadPick%d%d: %.2f\t", i, edge->getEndNode()->getId(), edge->Load);
					}
			}
			else {
				printf("Inv_%d: %.2f\t", i, Nodes[i]->Inventory[t]);


				if (Nodes[i]->Quantity[t] > 0.5) {

					j = Nodes[i]->getEdge(t)->getEndNode()->getId();

					printf("y%d: %.2f\t", i, 1);
					printf("t%d: %.2f\t", i, Nodes[i]->TimeServed[t]);
					printf("x%d%d: %.2f\t", i, j, Nodes[i]->getEdge(t)->getValue());
					if (instance.map.isDelivery(i)) {
						printf("qD_%d: %.2f\t", i, Nodes[i]->Quantity[t]);
						printf("loadDel%d%d: %.2f\t", i, j, Nodes[i]->getEdge(t)->Load);
					}
					else {
						printf("qP_%d: %.2f\t", i, Nodes[i]->Quantity[t]);
						printf("loadPick%d%d: %.2f\t", i, j, Nodes[i]->getEdge(t)->Load);
					}
				}
					
			} // end if
		} // end t
	}
}

double IRP::Solution::getTransportationCost()
{
	double TravelCost = 0;
	for (int i : instance.AllNodes) {
		for (int j : instance.AllNodes) {
			if (instance.map.inArcSet(i, j))
				for (int t : instance.Periods)
					if (xSol[i][j][t] > 0.01) {
						TravelCost = TravelCost + instance.TransCost[i][j] * xSol[i][j][t];
					}
		}
	}
	return TravelCost;
}

double IRP::Solution::getHoldingCost()
{
	double HoldingCost = 0;
	//Transportation cost


	//Holding Cost
	for (int i : instance.Nodes) {
		for (int t : instance.Periods) {
			HoldingCost += instance.HoldCost[i] * invSol[i][t];
		}
	}

	return HoldingCost;
}

void IRP::Solution::removeVisit(IRP::Route * route, int selection)
{
	
	vector<Node*> pair = selectPair(route, ModelParameters::HIGHEST_HOLDINGCOST);
	
	for (Node * n : pair) {

		//Removes node from the route
		route->removeNode(n, route);
		//Remove the service, either 0 pickup or 0 delivery, the inventory is updates in this function as well
		changeQuantity(n, route->period, 0);
	}
	
}

void IRP::Solution::updateInventory(Node * u,  int t)
{
	int i = u->getId();

	//If delivery, update inventory in all time periods
	if (instance.map.isDelivery(i)){
		invSol[i][t] = invSol[u->getId()][t] - delSol[i][t];
		propDelInvForw(i, t + 1);
	}
	else {
		invSol[i][t] = invSol[u->getId()][t] + pickSol[i][t];
		propPickInvForw(i, t + 1);
	}
}


vector<Node*> IRP::Solution::selectPair(IRP::Route * r, int Selection)
{
	vector <Node*> pair;
	vector <Node*> tempPair;

	switch (Selection) {
	case ModelParameters::HIGHEST_HOLDINGCOST:

		//Locate the node with the highest removal cost in terms of holding cost
		double tempCost = 0;
		//Initialize tempPair
		tempPair.resize(2);
		pair.resize(2);
		Node * tempNode = new Node(-1);

		int minCost = 1000000;
		int i = 1;
		Node * u;
		Node * v;
		while (i < r->route.size())
			tempPair[0] = tempNode;
		tempPair[1] = tempNode;
		u = r->route[i];
		v = r->route[i + 1];

		//Check if node is delivery
		if (instance.map.isDelivery(u->getId())) {
			tempCost = delSol[u->getId()][r->period] * instance.HoldCost[u->getId()];
			tempPair[0] = u;
			//Check if next node is on the route is the same customer
			if (instance.map.getCustomer(u->getId()) == instance.map.getCustomer(v->getId())) {
				tempCost += pickSol[v->getId()][r->period] * instance.HoldCost[v->getId()];
				i += 2;
				tempPair[1] = v;
			}
			else
				i += 1;
		}
		//Else the node is a pickup node
		else {
			tempCost += pickSol[v->getId()][r->period] * instance.HoldCost[v->getId()];
			i += 1;
			tempPair[0] = v;
		}

		//Check if visit is cheapest
		if (tempCost < minCost) {

			//Update mincost
			minCost = tempCost;

			//push the new nodes to the return value
			for (Node * n : tempPair)
			{
				pair.clear();
				pair.push_back(n);
			}
		}

		//Check if pair only has one node
		if (pair[1]->getId() == -1)
			pair.pop_back();

		return pair;
		break;

	case ModelParameters::HIGHEST_TOTALCOST:
	{
		double tempCost = 0;
		for (Node * u : r->route)
			tempCost = delSol[u->getId()][r->period] * instance.HoldCost[u->getId()];

		break;
	}

	} // end switch
}


void IRP::Solution::propDelInvBack(int i, int period){
	if (period > 0) {
		invSol[i][period] = invSol[i][period+1] - delSol[i][period];
		propDelInvBack(i, period - 1);
	}
}

void IRP::Solution::propDelInvForw(int i, int period)
{
	if (period <= instance.Periods.size()) {
		invSol[i][period] = invSol[i][period - 1] + delSol[i][period];
		propDelInvForw(i, period + 1);
	}
}


void IRP::Solution::propPickInvBack(int i, int period)
{
	if (period > 0) {
		invSol[i][period] = invSol[i][period + 1] + pickSol[i][period];
		propPickInvBack(i, period - 1);
	}
}

void IRP::Solution::propPickInvForw(int i, int period)
{
	if (period <= instance.Periods.size()) {
		invSol[i][period] = invSol[i][period - 1] - pickSol[i][period];
		propPickInvForw(i, period + 1);
	}

}


double IRP::Solution::getTransInteger()
{
	int i;
	int j;
	double TravelCost = 0;
	for(int t : instance.Periods)
		for(IRP::Route * r: SolutionRoutes[t])
			for (Node * u : r->route) {
				i = u->getId();
				j = u->getEdge(0)->getEndNode()->getId();
				TravelCost += instance.TransCost[i][j];
			}

	return TravelCost;
}

void IRP::Solution::buildGraph(vector<Node*>& graph, int t)
{
	int s;
	double edgeValue;

	Node * node = new Node(0);
	graph.push_back(node);

	//Create nodes for each visited customer
	for (int i : instance.Nodes) {
		Node * node = new Node(i);
		graph.push_back(node);
	}

	//Add outgoing edges from each visited node
	for (Node *node : graph) {
		s = node->getId();
		for (Node *endingNode : graph) {
			if (instance.map.inArcSet(s, endingNode->getId())) {
				edgeValue = xSol[s][endingNode->getId()][t];
				if (edgeValue > 0.01) {
					node->addEdge(edgeValue, *endingNode);
				}
			}
		}
	}
}

int IRP::Route::removeNode(Node * node, IRP::Route *r)
{
	int position = getPosition(node);
	//previous node
	Node * u = r->route[position - 1];

	//prior node
	Node * v = node->getEdge(0)->getEndNode();

	u->removeEdge(*node);
	u->addEdge(1, *v);
	route.erase(r->route.begin()+position);

	return position;
}

void IRP::Route::insertSubRoute(vector<Node *> subroute, Node *u, Node *v)
{
	//resize route
	route.resize(route.size() + subroute.size());

	

	//remove edges between start and end
	Node * n = u;
	Node *nextNode;
	do {
		if(i > 0)
			changeQuantity(n, t, 0);
		nextNode = n->getEdge(0)->getEndNode();
		n->removeEdge(*nextNode);
		n = n->getEdge(0)->getEndNode();
	} while (n != v);

	//add edge to subroute
	u->addEdge(*subroute[0]);

	//add edge from subroute to route
	subroute.back()->addEdge(*v);

	int position = getPosition(u);

	//Update the vector containing the route
	for (int i = 0; i < subroute.size(); i++) {
		route.insert(route.begin() + position + i, subroute[i]);
	}
}

void IRP::Solution::changeQuantity(Node * n, int period,  int quantity)
{
	//Remove the service
	int i = n->getId();
	if (instance.map.isDelivery(i)) {
		delSol[i][period] = quantity;
	}
	else
		pickSol[i][period] = quantity;

	updateInventory(n, period);
}

IRP::Route * IRP::Solution::insertSubrouteInRoute(IRP::Route * subroute, int period)
{
	double minCost = 100000;
	double tempCost = 0;
	IRP::Route * path;
	double C_uk;
	double C_lv;
	double C_uv;
	Node * start;
	Node * end;
	Node * k;
	Node * l;
	k = subroute->route[0];
	l = subroute->route.back();

	for (IRP::Route *r : SolutionRoutes[period]) {
		for (Node *n : r->route) {
			C_uk = instance.getMap()->getTransCost(n->getId(), k->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);
			C_lv = instance.getMap()->getTransCost(end->getId(), l->getEdge(0)->getEndNode()->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);
			C_uv = instance.getMap()->getTransCost(n->getId(), n->getEdge(0)->getEndNode()->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);

			tempCost = C_uk + C_lv - C_uv;
		}

		if (tempCost < minCost) {
			minCost = tempCost;
			start = k;
			end = l;
			path = r;
		}

	}
	//Insert route
	path->insertSubroute(subroute->route, end);
	
}

int IRP::Route::getPosition(Node * node)
{
	int position = 0;
	for (Node * n : route) {
		if (n->getId() == node->getId())
			return position;
		position += 1;
	}
}

int IRP::Route::getId()
{
	return id;
}

int ** IRP::Route::getRouteMatrix(IRP * const instance)
{
	int ** route = new int*[instance->AllNodes.size()];
	for (int i = 0; i < instance->AllNodes.size(); i++) {
		route[i] = new int[instance->AllNodes.size()];
		for (int j = 0; j < instance->AllNodes.size(); j++) {
			route[i][j] = 0;
		}
	}

	for (Node *node : this->route) {
		int i = node->getId();
		int j = node->getEdge(0)->getEndNode()->getId();
		route[i][j] = 1;
	}
	return route;
}

IRP::Route::Route(vector<Node*>& path, int ref, int t)
	:
	route(path),
	id(ref),
	period(t)
{
}
