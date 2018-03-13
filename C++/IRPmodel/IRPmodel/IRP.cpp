#include "stdafx.h"
#include "IRP.h"
#include "ModelBase.h"
#include <algorithm>
#include "graphAlgorithm.h"
#include "ModelParameters.h"

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

	vector<XPRBcut> subtourCut;
	
	IRP * modelInstance;
	modelInstance = (IRP*)vd;

	/*if ((XPRB::getTime() - modelInstance->startTime) / 1000 >= ModelParameters::MAX_RUNNING_TIME)
	{
		XPRSinterrupt(oprob, XPRS_STOP_TIMELIMIT);
	}

	//Load LP relaxation currently held in the optimizer
	//modelInstance->getProblem()->beginCB(oprob);
	//modelInstance->getProblem()->sync(XPRB_XPRS_SOL);
	double a = modelInstance->getProblem()->getObjVal();*/
	//Add subtourconstraints
	bool addedCuts = modelInstance->sepStrongComponents(subtourCut);

	
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

		for (XPRBcut cut : subtourCut)
		{
			cut.print();
			modelInstance->nSubtourCuts += 1;
			//XPRSstorecuts(oprob, 1, 1, , 

			modelInstance->getProblem()->addCuts(&cut, 1);
		}

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
						sol->NodeHolder[i]->Nodes[t]->addEdge(loadDelivery[i][j][t].getSol(), 
							loadPickup[i][j][t].getSol(), sol->NodeHolder[j]->Nodes[t], x[i][j][t].getSol());		
					}
				}
			} //endif
		} //end for j
	} // end x and loading

	  //Fill inventory
	for (int i : Nodes) {
		for (int t : Periods) {
			sol->NodeHolder[i]->Nodes[t]->Inventory = inventory[i][t].getSol();
		}
	}

	//Fill delivery and pickup
	for (int i : Nodes) {
		for (int t : Periods) {
			if (map.isDelivery(i)) {
				if (delivery[i][t].getSol() > 0) {
					sol->NodeHolder[i]->Nodes[t]->Quantity = delivery[i][t].getSol();
					sol->NodeHolder[i]->Nodes[t]->TimeServed = time[i][t].getSol();
				}
				else {
					sol->NodeHolder[i]->Nodes[t]->Quantity = 0;
					sol->NodeHolder[i]->Nodes[t]->TimeServed = time[i][t].getSol();
				}
			}
			else {
				if (pickup[i][t].getSol() > 0)
					sol->NodeHolder[i]->Nodes[t]->Quantity = pickup[i][t].getSol();
				else
					sol->NodeHolder[i]->Nodes[t]->Quantity = 0;
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
			sol->NodeHolder[i]->Nodes[t]->Inventory = inventory[i][t].getSol();
		}
	}

	//Fill Quantity and allocate time
	for (int i : Nodes){ 
		for (int t : Periods) {
			if (map.isDelivery(i)) {
				if (delivery[i][t].getSol() > 0) {
					sol->NodeHolder[i]->Nodes[t]->Quantity = delivery[i][t].getSol();
					sol->NodeHolder[i]->Nodes[t]->TimeServed = time[i][t].getSol();
				}
				else
					sol->NodeHolder[i]->Nodes[t]->Quantity = 0;
			}
			else {
				if (pickup[i][t].getSol() > 0) {
					sol->NodeHolder[i]->Nodes[t]->Quantity = pickup[i][t].getSol();
					sol->NodeHolder[i]->Nodes[t]->TimeServed = time[i][t].getSol();
				}
				else
					sol->NodeHolder[i]->Nodes[t]->Quantity = 0;
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


//Build graph from variables
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
	//Add nodes from particular period
	for (IRP::NodeIRPHolder * n :solution->NodeHolder) {
			Node * node = new Node(*n->Nodes[t]);
			graph.push_back(node);
		}
}
	


void IRP::printGraph(vector<Node> &graph)
{
	for (Node node : graph) {
		int id = node.getId();
		vector<Node::Edge*> edges = (node.getEdges());

		for (Node::Edge* edge : (node.getEdges())) {
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

				for (Node::Edge *edge : node->getEdges()) {
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
					for (Node::Edge *edge : node->getEdges()) {
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
	Instance(model)
{
	//Initializa node hold
	//Create a route holder for each period
	NodeHolder.resize(Instance.AllNodes.size());
	Routes.resize(Instance.getNumOfPeriods()+1);
	//Create a nodes for all customers and depot, same id as in model
	for (int i : Instance.AllNodes) {
		NodeHolder[i] = new NodeIRPHolder(i, Instance);		
	}
}

IRP::Solution::Solution(IRP &model, vector<vector<IRP::Route*>>& routes, bool integer = false)
	:
	IntegerSolution(integer),
	Instance(model),
	Routes(routes)
{
	Routes.resize(5);
	//Initialize solution
	
}


void IRP::Solution::print(string filename)
{
	vector<Node *> graph;
	for (int t : Instance.Periods) {
		Instance.buildGraph(graph, t, this);
		graphAlgorithm::printGraph(graph, Instance, filename+to_string(t));
		graph.clear();
	}
}

bool IRP::Solution::isFeasible()
{
	//Check if too many vehicles
	for (int t : Instance.Periods) {
		if (getNumberOfRoutes(t) > ModelParameters::nVehicles) {
			return false;
		}
	}

	//Check if each route is feasible
	for(int t : Instance.Periods)
	for (IRP::Route* r : Routes[t]) {
		if (!isRouteFeasible(r))
			return false;
	}
	return true;
}

bool IRP::Solution::isRouteFeasible(IRP::Route * r)
{

	for (NodeIRP * n : r->route) {

		if ( n->getEdge()->LoadDel + n->getEdge()->LoadPick> Instance.Capacity)
			return false;
		}
	return true;
}

double IRP::Solution::getNumberOfRoutes(int period)
{
	NodeIRPHolder * depot = getNode(0);
	return depot->getOutflow(period);
}

IRP::NodeIRPHolder * IRP::Solution::getNode(int id)
{
	for (NodeIRPHolder * n : NodeHolder) {
		if (n->getId() == id)
			return n;
	}
}

int IRP::Solution::newRoute(vector<Node*>& route, int t)
{
	vector<IRP::NodeIRP *> ptrHolder;
	for (Node* n : route) {
		IRP::NodeIRP * nodeptr = static_cast<IRP::NodeIRP*> (n);
		ptrHolder.push_back(nodeptr);
	}
	int id = Routes[t].size();
	Routes[t].push_back(new Route(ptrHolder, id, Instance, t));
	return Routes[t].size() - 1;
}

int IRP::Solution::getnPeriods()
{
	return Routes.size();
}

double IRP::Solution::getObjective()
{
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

	for (int t : Instance.Periods) {
		printf("\n\nPeriod %d:", t);
		for (int i : Instance.AllNodes) {
			printf("\n");
			if (i == 0) {
				printf("y0: %.2f\t", NodeHolder[i]->getOutflow(t));

				for (NodeIRP::EdgeIRP *edge : NodeHolder[i]->getEdges(t))
					if (edge->getValue() > 0) {
						printf("x%d%d: %.2f\t", i, edge->getEndNode()->getId(), edge->getValue());
						printf("loadDel%d%d: %.2f\t", i, edge->getEndNode()->getId(), edge->LoadDel);
						printf("loadPick%d%d: %.2f\t", i, edge->getEndNode()->getId(), edge->LoadPick);
					}
			}
			else {
				printf("Inv_%d: %.2f\t", i, NodeHolder[i]->Nodes[t]->Inventory);


				if (NodeHolder[i]->Nodes[t]->Quantity > 0.5) {

					j = NodeHolder[i]->getEdge(t)->getEndNode()->getId();

					printf("y%d: %.2f\t", i, NodeHolder[i]->getOutflow(t));
					printf("t%d: %.2f\t", i, NodeHolder[i]->timeServed(t));
					printf("x%d%d: %.2f\t", i, j, NodeHolder[i]->getEdge(t)->getValue());
					if (Instance.map.isDelivery(i)) {
						printf("qD_%d: %.2f\t", i, NodeHolder[i]->quantity(t));
					}
					else {
						printf("qP_%d: %.2f\t", i, NodeHolder[i]->quantity(t));
					}
					printf("loadDel%d%d: %.2f\t", i, j, NodeHolder[i]->getEdge(t)->LoadDel);
					printf("loadPick%d%d: %.2f\t", i, j, NodeHolder[i]->getEdge(t)->LoadPick);
				}
					
			} // end if
		} // end t
	}
}

double IRP::Solution::getTransportationCost()
{
	double TravelCost = 0;
	for (int t : Instance.Periods)
		TravelCost += getTransportationCost(t);
	return TravelCost;
}

double IRP::Solution::getHoldingCost()
{
	double HoldingCost = 0;

	//Holding Cost
	for (int t : Instance.Periods) {
		HoldingCost += getHoldingCost(t);
	}
	
	return HoldingCost;
}

double IRP::Solution::getHoldingCost(int period)
{
	double HoldingCost = 0;
	for (NodeIRPHolder * n : NodeHolder) {
		if (n->getId() > 0) {
			HoldingCost += n->getHoldCost(period);
		}
	}
	return HoldingCost;
}


double IRP::Solution::getTransportationCost(int t)
{
	double TravelCost = 0;
	for (NodeIRPHolder * n : NodeHolder) {
		for (Node::Edge * edge : n->getEdges(t)) {
			int i = n->getId();
			int j = edge->getEndNode()->getId();
			double value = edge->getValue();
			TravelCost = TravelCost + Instance.map.getTransCost(i, j)*value;
		}
	}

	return TravelCost;
}

/*void IRP::Solution::removeVisit(IRP::Route * route, int selection)
{
	
	vector<NodeIRP*> pair = selectPair(route, ModelParameters::HIGHEST_HOLDINGCOST);
	
	for (NodeIRP * n : pair) {

		//Removes node from the route
		route->removeNode(n, route);
		//Remove the service, the inventory is updated in this function as well
		n->changeQuantity(route->period, 0);
	}
	
}*/



vector<IRP::NodeIRP*> IRP::Solution::selectPair(IRP::Route * r, int Selection)
{
	vector <NodeIRP*> pair;
	vector <NodeIRP*> tempPair;
	int t = r->period;

	switch (Selection) {
	case ModelParameters::HIGHEST_HOLDINGCOST:
	{
		//Locate the node with the highest removal cost in terms of holding cost
		double tempCost = 0;
		//Initialize tempPair
		tempPair.resize(2);
		pair.resize(2);
		NodeIRP * tempNode = new NodeIRP(-1);

		int minCost = 1000000;
		
		int i = 1;
		NodeIRP * u;
		NodeIRP * v;
		while (i < r->route.size())
			tempPair[0] = tempNode;
		tempPair[1] = tempNode;
		u = r->route[i];
		v = r->route[i + 1];

		//Check if node is delivery
		if (true) {
			tempCost = u->Quantity * Instance.HoldCost[u->getId()];
			tempPair[0] = u;
			//Check if next node is on the route is the same customer
			if (Instance.map.getCustomer(u->getId()) == Instance.map.getCustomer(v->getId())) {
				tempCost += v->Quantity * Instance.HoldCost[v->getId()];
				i += 2;
				tempPair[1] = v;
			}
			else
				i += 1;
		}
		//Else the node is a pickup node
		else {
			tempCost += v->Quantity * Instance.HoldCost[v->getId()];
			i += 1;
			tempPair[0] = v;
		}

		//Check if visit is cheapest
		if (tempCost < minCost) {

			//Update mincost
			minCost = tempCost;

			//push the new nodes to the return value
			for (NodeIRP * n : tempPair)
			{
				pair.clear();
				pair.push_back(n);
			}
		}

		//Check if pair only has one node
		if (pair[1]->getId() == -1)
			pair.pop_back();

		return pair;
	}
	break;

	case ModelParameters::HIGHEST_TOTALCOST:
	{
		double tempCost = 0;
		for (NodeIRP * u : r->route)
			tempCost = u->Quantity * Instance.HoldCost[u->getId()];

	}
	break;

	} // end switch
}



/*void IRP::Solution::buildGraph(vector<Node*>& graph, int t)
{
	int s;
	double edgeValue;

	Node * node = new Node(0);
	graph.push_back(node);

	//Create nodes for each visited customer
	for (int i : Instance.Nodes) {
		Node * node = new Node(i);
		graph.push_back(node);
	}

	//Add outgoing edges from each visited node
	for (Node *node : graph) {
		s = node->getId();
		for (Node *endingNode : graph) {
			if (Instance.map.inArcSet(s, endingNode->getId())) {
				edgeValue = xSol[s][endingNode->getId()][t];
				if (edgeValue > 0.01) {
					node->addEdge(edgeValue, *endingNode);
				}
			}
		}
	}
}*/

/*int IRP::Route::removeNode(NodeIRP * node, IRP::Route *r)
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
}*/

/*void IRP::Route::insertSubRoute(vector<NodeIRP *> subroute, NodeIRP *u, NodeIRP *v)
{
	//resize route
	route.resize(route.size() + subroute.size());

	

	//remove edges between start and end
	NodeIRP * n = u;
	NodeIRP *nextNode;
	do {
		if(i > 0)
		n->changeQuantity(period, 0);
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
}*/


/*IRP::Route * IRP::Solution::insertSubrouteInRoute(IRP::Route * subroute, int period)
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
			C_uk = Instance.getMap()->getTransCost(n->getId(), k->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);
			C_lv = Instance.getMap()->getTransCost(end->getId(), l->getEdge(0)->getEndNode()->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);
			C_uv = Instance.getMap()->getTransCost(n->getId(), n->getEdge(0)->getEndNode()->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);

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
	
}*/

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

int ** IRP::Route::getRouteMatrix(IRP * const Instance)
{
	int ** route = new int*[Instance->AllNodes.size()];
	for (int i = 0; i < Instance->AllNodes.size(); i++) {
		route[i] = new int[Instance->AllNodes.size()];
		for (int j = 0; j < Instance->AllNodes.size(); j++) {
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

IRP::Route::Route(vector<NodeIRP*>& path, int ref, IRP & Instance, int t = 0)
	:
	Instance(Instance),
	route(path),
	id(ref),
	period(t)
{
}


IRP::NodeIRP::NodeIRP(int id)
	:
	Node(id)
{
	//
	Quantity = -1;
	TimeServed = -1;
	Inventory = -1;
}


IRP::NodeIRP::~NodeIRP()
{
}

IRP::NodeIRP::EdgeIRP::EdgeIRP(Node * child, double loadDel, double loadPick, double value = 1)
	:
	Edge(*child, value),
	LoadDel(loadDel),
	LoadPick(loadPick)
{
}
void IRP::NodeIRP::addEdge(double loadDel, double loadPick, NodeIRP * child, double value = 1)
{
	Node * nodePtr = child;
	EdgeIRP * edge = new EdgeIRP(nodePtr, loadDel, loadPick, value);
	Edge * edgePtr = edge;
	Node::addEdge(edgePtr);
}

IRP::NodeIRP::EdgeIRP * IRP::NodeIRP::getEdge()
{
	vector<EdgeIRP *> edge = getEdges();
	EdgeIRP * derivedPtr = edge[0];
	if (derivedPtr != 0) {
		return derivedPtr;
	}
}

IRP::NodeIRP * IRP::NodeIRP::getNode(Node * n)
{
	NodeIRP * derivedPtr = static_cast <NodeIRP *> (n);
	if (derivedPtr != 0) {
		return derivedPtr;
	}
}

vector<IRP::NodeIRP::EdgeIRP*> IRP::NodeIRP::getEdges()
{
	vector<Edge*> edges = Node::getEdges();
	vector<EdgeIRP*> edgesIRP;
	for (int i = 0; i < edges.size(); i++) {
		EdgeIRP * derivedPtr = static_cast <EdgeIRP *> (edges[i]);
		edgesIRP.push_back(derivedPtr);
	}
	return edgesIRP;
}

double IRP::NodeIRP::getOutflow()
{
	double outflow = 0;
	vector<EdgeIRP*> edges = getEdges();
	for (EdgeIRP * edge : edges) {
		outflow += edge->getValue();
	}
	return outflow;
}




IRP::NodeIRP * IRP::NodeIRP::EdgeIRP::getEndNode()
{
	Node * node = Node::Edge::getEndNode();
	NodeIRP * derivedPtr = static_cast <NodeIRP *> (node);
	if (derivedPtr != 0) {
		return derivedPtr;
	}
}

int IRP::NodeIRPHolder::getId()
{
	return Id;
}


IRP::NodeIRPHolder::NodeIRPHolder(int id, IRP &instance)
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

void IRP::NodeIRPHolder::addEdge(double loadDel, double loadPick, NodeIRPHolder * NodeHolder, int period, double value)
{
	NodeIRP * u = Nodes[period];
	NodeIRP * v = NodeHolder->Nodes[period];
	u->addEdge(loadDel, loadPick, v, value);
}

void IRP::NodeIRPHolder::propInvForw(int period)
{
		if (period <= Instance.getNumOfPeriods()) {
			Nodes[period]->Inventory = Nodes[period-1]->Inventory + Nodes[period]->Quantity;
			propInvForw(period + 1);
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

void IRP::NodeIRPHolder::changeQuantity(int period, int quantity)
{
	// Remove the service
		if (isDelivery()) {
			Nodes[period]->Quantity = quantity;
		}
		else
			Nodes[period]->Quantity = -quantity;

	updateInventory(period);
}

void IRP::NodeIRPHolder::updateInventory(int t)
{
	Nodes[t]->Inventory = Nodes[t]->Inventory - Nodes[t]->Quantity;
	propInvForw(t + 1);
}

bool IRP::NodeIRPHolder::isDelivery()
{
	return Instance.getMap()->isDelivery(getId());
}
