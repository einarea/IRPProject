#include "stdafx.h"
#include "IRP.h"
#include "ModelBase.h"
#include <algorithm>
#include "graphAlgorithm.h"
#include "ModelParameters.h"
#include <string.h>
#include <boost/tuple/tuple.hpp>
#include "gnuplot-iostream.h"
#include <stdlib.h>


using namespace ::dashoptimization;
using namespace::std;



void XPRS_CC acceptInt(XPRSprob oprob, void * vd, int soltype, int * ifreject, double *cutoff) {
	IRP * modelInstance;
	modelInstance = (IRP*)vd;
	vector<XPRBcut> subtourCut;

	modelInstance->getProblem()->beginCB(oprob);
	modelInstance->getProblem()->sync(XPRB_XPRS_SOL);
	bool addedCuts = modelInstance->sepStrongComponents(subtourCut);
	if (addedCuts) {
		*ifreject = 1;
	}
	modelInstance->getProblem()->endCB();
}

void XPRS_CC acceptIntQuantity(XPRSprob oprob, void * vd, int soltype, int * ifreject, double *cutoff) {
	IRP * modelInstance;
	modelInstance = (IRP*)vd;
	vector<XPRBcut> subtourCut;

	modelInstance->getProblem()->beginCB(oprob);
	modelInstance->getProblem()->sync(XPRB_XPRS_SOL);
	for (auto i : modelInstance->Nodes)
		for (auto t : modelInstance->Periods)
			if (modelInstance->inventory[i][t].getSol() - round(modelInstance->inventory[i][t].getSol()) > 0.01) {
				cout << modelInstance->inventory[i][t].getSol() << "\t";
				cout << round(modelInstance->inventory[i][t].getSol()) << "\n";
				*ifreject = 1;
				break;
			}
	modelInstance->getProblem()->endCB();
}

//Time callback
void XPRS_CC cbmngtimeIRP(XPRSprob oprob, void * vd,int parent, int newnode, int branch) {

	IRP * modelInstance;
	modelInstance = (IRP*)vd;

	if ((XPRB::getTime() - modelInstance->startTime) / 1000 >= ModelParameters::MAX_RUNNING_TIME_IRP)
	{
		XPRSinterrupt(oprob, XPRS_STOP_TIMELIMIT);
	}
}
//Global callBack manager
int XPRS_CC cbmng(XPRSprob oprob, void * vd)
{
	// subtour callback

	int depth;
	int node;
	int cutId;
	XPRSgetintattrib(oprob, XPRS_NODEDEPTH, &depth);
	XPRSgetintattrib(oprob, XPRS_NODES, &node);
	double objval;
	XPRSgetdblattrib(oprob, XPRS_LPOBJVAL, &objval);

	vector<XPRBcut> subtourCut;

	IRP * modelInstance;
	modelInstance = (IRP*)vd;

	//Load LP relaxation currently held in the optimizer
	modelInstance->getProblem()->beginCB(oprob);
	modelInstance->getProblem()->sync(XPRB_XPRS_SOL);
	
	//Load model instance
	XPRBprob *bprob = modelInstance->getProblem();

	bool addedCuts = modelInstance->sepStrongComponents(subtourCut);

	if (addedCuts) {
		int node;
	
		XPRSgetintattrib(oprob, XPRS_NODES, &node);
		XPRSgetdblattrib(oprob, XPRS_LPOBJVAL, &objval);

		for (XPRBcut cut : subtourCut)
		{
			
			//cut.print();
			modelInstance->nSubtourCuts += 1;
			cutId = modelInstance->nSubtourCuts;
			bprob->addCuts(&cut, 1);
			cut.print();
		}
	}
	
	//Unload LP relaxation
	modelInstance->getProblem()->endCB(); 

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
			if (y[i][t].getUB() <= 0.5)
				cout << "DD";
	}
}

IRP::IRP(CustomerIRPDB& db, bool ArcRel, bool maskOn, int ** VisitMask)
	:
	database(db),
	prob("IRP"),			//Initialize problem in BCL	
	map(db),			//Set up map of all customers
	ARC_RELAXED(ArcRel),
	SolutionCounter(0),
	MaskOn(maskOn),
	LPSubtour(false)
{
	IRP::solCounter = 1;
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

	//Calculate excess consumption and production
	calculateExcess();

	//initialize tabu matrix and countMatrix
	TabuMatrix = new XPRBctr *[getNumOfNodes() + 1];
	CountMatrix = new double *[getNumOfNodes() + 1];
	for (auto i : Nodes) {
		TabuMatrix[i] = new XPRBctr[getNumOfPeriods() + 1];
		CountMatrix[i] = new double[getNumOfPeriods() + 1];
		for (auto t : Periods) {
			TabuMatrix[i][t] = 0;
			CountMatrix[i][t] = 0;
		}
	}


	formulateProblem();

}



IRP::Solution * IRP::solveModel()
{
	if (ARC_RELAXED && LPSubtour)
	{
		bool isSubtours = false;
		bool temp = false;

		do {
			isSubtours = false;
			prob.mipOptimise();
			//Check graph for subtours in strong components
			vector <vector<Node*>> result; //matrix to store strong components
			vector <Node*> graph;		//Graph to store nodes

			for (int t : Periods) {
				graph.clear();
				result.clear();
				buildGraph(graph, t, false, EDGE_WEIGHT); //Do not include depot in graph			
				graphAlgorithm::printGraph(graph, *this, "graphSolutions/graph", ModelParameters::X);
				graphAlgorithm::sepByStrongComp(graph, result);
				graphAlgorithm::printGraph(graph, *this, "graphSolutions/graph", ModelParameters::X);
				temp = addSubtourCtr(result, t);
				if (temp)
					isSubtours = true;
			}

		} while (isSubtours);

		return allocateIRPSolution();
	}

	else

	oprob = prob.getXPRSprob();
	startTime = XPRB::getTime();
	
	//Set time callback
	XPRSsetcbnewnode(oprob, cbmngtimeIRP, &(*this));

	if (ARC_RELAXED) {

		XPRSsetcbpreintsol(oprob, acceptIntQuantity, &(*this));
	
	}
	//XPRSsetcbpreintsol(oprob, acceptInt, &(*this));
	int d = prob.mipOptimise();
//	prob.print();

	return allocateIRPSolution();
}

IRP::Solution * IRP::solveLPModel()
{
	if (LPSubtour) {
		bool isSubtours = false;
		bool temp = false;

		do {
			isSubtours = false;
			prob.lpOptimise();
			//Check graph for subtours in strong components
			vector <vector<Node*>> result; //matrix to store strong components
			vector <Node*> graph;		//Graph to store nodes

			for (int t : Periods) {
				graph.clear();
				buildGraph(graph, t, true, EDGE_WEIGHT); //Do not include depot in graph			
				graphAlgorithm::printGraph(graph, *this, "graphSolutions/graph", ModelParameters::X);
				graphAlgorithm::sepByStrongComp(graph, result);
				graphAlgorithm::printGraph(graph, *this, "graphSolutions/graph", ModelParameters::X);
				temp = addSubtourCtr(result, t);
				if (temp)
					isSubtours = true;
			}

		} while (isSubtours);

		return allocateIRPSolution();
	}

	else
		prob.lpOptimise();
}

Map * IRP::getMap()
{
	return &map;
}

void IRP::calculateExcess()
{
	int excess = 0;
	ExcessConsumption = new double **[DeliveryNodes.size()];
	for (int i : DeliveryNodes) {
		ExcessConsumption[i] = new double *[Periods.size() + 1];
		
		for (int t1 : Periods) {
			ExcessConsumption[i][t1] = new double[Periods.size() + 1];
			for (int t2 : Periods) {

				//initialize excess
				if (t1 == 1) {
					excess = LowerLimit[i] - InitInventory[i];
				}
				else
					excess = LowerLimit[i] - UpperLimit[i];

				//calculate excess between period t1 and t2
				if (t1 <= t2) {
					for (int t = t1; t <= t2; t++)
						excess += Demand[i][t];
					if (excess >= 1) ExcessConsumption[i][t1][t2] = excess;
					else  ExcessConsumption[i][t1][t2] = 0;
					cout << "Node: " << i << "\tperiod: " << t1 << t2<< "\tExcessDelivery: " << ExcessConsumption[i][t1][t2] << "\n";
				}


			}
			
		}
	}

	excess = 0;
	ExcessProd = new double **[Nodes.size() + 1];
	for (int i : PickupNodes) {
		ExcessProd[i] = new double *[Periods.size() + 1];
	
		for (int t1 : Periods) {
			ExcessProd[i][t1] = new double[Periods.size() + 1];
			for (int t2 : Periods) {

				//initialize excess
				if (t1 == 1) {
					excess = InitInventory[i] - UpperLimit[i];
				}
				else
					excess = LowerLimit[i] - UpperLimit[i];

				//calculate excess between period t1 and t2
				if (t1 <= t2) {
					for (int t = t1; t <= t2; t++)
						excess += Demand[i][t];
					if (excess >= 1) ExcessProd[i][t1][t2] = excess;
					else  ExcessProd[i][t1][t2] = 0;

					//cout << "Node: " << i << "\tperiod: " << t1 << t2<<"\tExcessPickup: " << ExcessProd[i][t1][t2] << "\n";
				}
			}
		}

	}
}

IRP::Solution * IRP::allocateIRPSolution()
{

	//Allocate x and loading solutions
	IRP *Instance =  this;
	IRP::Solution *sol = allocateSolution(*Instance);

	fillSolution(sol);
	return sol;
}

//Fills a solution based on problem solution values
void IRP::fillSolution(IRP::Solution * sol)
{
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

	//Fill load
	double value;

	for (int i : AllNodes) {
		for (int j : AllNodes) {
			if (map.inArcSet(i, j)) {
				for (int t : Periods) {
					if (x[i][j][t].getSol() >= 0.0001) {
						if (ModelParameters::Simultaneous) {
							if (map.isColocated(i, j) && simAction[i][j][t].getSol() <= 0.01) {
								value = 0;
							}
							else
								value = x[i][j][t].getSol();
						}
						else
							value = x[i][j][t].getSol();

						sol->NodeHolder[i]->Nodes[t]->addEdge(loadDelivery[i][j][t].getSol(),
							loadPickup[i][j][t].getSol(), sol->NodeHolder[j]->Nodes[t], value);
					}
				}
			} //endif
		} //end for j
	} // end x and loading

	solutions.push_back(sol);
	//Return ID to solution
}

bool IRP::sepStrongComponents(vector<XPRBcut> & cut)
{
	vector <vector<Node*>> result; //matrix to store strong components
	vector <Node*> graph;		//Graph to store nodes

	bool newCut = false;
	for (int t : Periods) {
		buildGraph(graph, t, true, 0.01); //include depot
		//graphAlgorithm::printGraph(graph, *this, "Subtour/LPrelax" + to_string(t), ModelParameters::X);
		graph.clear();
		buildGraph(graph, t, false, EDGE_WEIGHT); //Do not include depot in graph
		//graphAlgorithm::printGraph(graph, *this, "Subtour/DepotGone" + to_string(t), ModelParameters::X);
		graphAlgorithm::sepByStrongComp(graph, result);
		//graphAlgorithm::printGraph(graph, *this, "Subtour/Separation" + to_string(t), ModelParameters::X);
		addSubtourCut(result, t, newCut, cut);
		graph.clear();
		result.clear();
	}
	return newCut;
}

void IRP::buildGraphSol(vector<Node*> &graph, int t, IRP::Solution *solution)
{
	//Add nodes from particular period
	for (NodeIRPHolder * n : solution->NodeHolder) {
		NodeIRP * node = new NodeIRP(*n->Nodes[t]);
		graph.push_back(node);
	}
}

//Build graph from variables
void IRP::buildGraph(vector<Node*> &graph, int t, bool Depot, double weight)
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
				if (edgeValue > weight) {
					node->addEdge(edgeValue, endingNode);
				}
			}
		}
	}
}


	


void IRP::printGraph(vector<Node> &graph)
{
	for (Node node : graph) {
		int id = node.getId();
		vector<Node::Edge*> edges = (node.getEdges());

		for (Node::Edge * edge : (node.getEdges())) {
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

void IRP::clearVariables()
{
	for (auto i : AllNodes) {
		for (auto j : AllNodes) {
			delete[] x[i][j];
			delete[] loadDelivery[i][j];
			delete[] loadPickup[i][j];
			
		}
		delete[] x[i];
		delete[] loadDelivery[i];
		delete[] loadPickup[i];
		delete[] y[i];
		delete[] time[i];
		delete[] inventory[i];
		delete[] delivery[i];
		delete[] pickup[i];
	}

	delete [] x;
	delete[] y;
	delete[] delivery;
	delete[] pickup;
	delete time;
	delete inventory;
	delete[] loadDelivery;
	delete[] loadPickup;

	x = 0;
	y = 0;
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

			if (circleFlow >= visitSum - maxVisitSum + alpha) {
				//print subtour
				vector<vector<XPRBvar>> subtour;
				subtour.resize(2);
				
			//	graphAlgorithm::printGraph(strongComp[i], *this, "Subtour\subtour");
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
					//y[node->getId()][t].print();
					//cout << "\n";
					rSideStr = rSideStr + " + " + "y_" + to_string(node->getId());
					subtour[1].push_back(y[node->getId()][t]);
					for (Node::Edge *edge : node->getEdges()) {
						if (edge->getValue() >= 0) {
							int u = node->getId();
							int v = edge->getEndNode()->getId();
							//printf("x_%d%d + ", u, v);
							lSide += x[node->getId()][edge->getEndNode()->getId()][t];
							//x[node->getId()][edge->getEndNode()->getId()][t].print();
							//cout << "\n";

							subtour[0].push_back(x[node->getId()][edge->getEndNode()->getId()][t]);

						}
					}
				}
				subtourIndices.push_back(subtour);
				rSide -= y[maxVisitID][t];
				rSideStr = rSideStr + " - " + "y_" + to_string(maxVisitID);
				//cout << rSideStr << "\n";
				nSubtourCuts += 1;
	
				XPRBcut vv = prob.newCut( lSide <= rSide);
				//vv.print();
				
 				SubtourCut.push_back(vv);


				//SubtourCut.push_back(vv);
			
				//SubtourCut.push_back(vv);
				newCut = true;		
				maxVisitID = -1;
				
			}
		}
	}
}


bool IRP::addSubtourCtr(vector<vector<Node *>>& strongComp, int t)
{
	bool addedCut = false;
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
					if (edge->getValue() >= 0) //only inlude edges in strong component
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

			if (circleFlow >= visitSum - maxVisitSum + alpha) {
				//print subtour
				vector<vector<XPRBvar>> subtour;
				subtour.resize(2);

				//	graphAlgorithm::printGraph(strongComp[i], *this, "Subtour\subtour");
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
					//y[node->getId()][t].print();
					//cout << "\n";
					rSideStr = rSideStr + " + " + "y_" + to_string(node->getId());
					subtour[1].push_back(y[node->getId()][t]);
					for (Node::Edge *edge : node->getEdges()) {
						if (edge->getValue() >= 0) {
							int u = node->getId();
							int v = edge->getEndNode()->getId();
							//printf("x_%d%d + ", u, v);
							lSide += x[node->getId()][edge->getEndNode()->getId()][t];
							//x[node->getId()][edge->getEndNode()->getId()][t].print();
							//cout << "\n";

							subtour[0].push_back(x[node->getId()][edge->getEndNode()->getId()][t]);

						}
					}
				}
				subtourIndices.push_back(subtour);
				rSide -= y[maxVisitID][t];
				rSideStr = rSideStr + " - " + "y_" + to_string(maxVisitID);
				//cout << rSideStr << "\n";
				nSubtourCuts += 1;

				prob.newCtr(XPRBnewname("Subtour"), lSide <= rSide).print();
				//vv.print();

				addedCut = true;

			}
		}
	}

	return addedCut;
}



bool IRP::formulateProblem()
{
	bool arcIndicator;
	/****OBJECTIVE****/
		//Transportation costs
		for (int i : AllNodes)
			for (int j : AllNodes) {
				if (ModelParameters::Simultaneous)
					arcIndicator = (map.inSimultaneousArcSet(i, j) && !map.isColocated(i, j));
				else
					arcIndicator = map.inExtensiveArcSet(i, j);

				for (int t : Periods) {
					if (arcIndicator)
						objective += TransCost[i][j] * x[i][j][t];
				}
			}

	if (ModelParameters::Simultaneous) {
		for (int i : DeliveryNodes)
			for (int j : PickupNodes)
				if (map.isColocated(i, j))
					for(int t: Periods)
						objective += simAction[i][j][t] * TransCost[i][j];
	}
	//Holding costs
	for (int i : Nodes)
		for (int t : Periods)
			objective += HoldCost[i] * inventory[i][t];

	
	prob.setObj(prob.newCtr("OBJ", objective));  /* Set the objective function */
	//end objective

	/**** CONSTRAINTS ****/
	XPRBexpr p1;
	if (ModelParameters::Simultaneous) {
		for(int i: DeliveryNodes)
			for (int j : PickupNodes) {
				if(map.isColocated(i, j)) {
					for (int t : Periods) {
						p1 = actionDelivery[i][t] + actionPickup[j][t] - epsilon* simAction[i][j][t];
						prob.newCtr("SimAction", p1 <= 2 - epsilon);
						p1 = 0;
					}
				}
			}


		for(int i : PickupNodes)
			for (int t : Periods)
			{
				p1 = pickup[i][t] - min(Capacity, UpperLimit[i] - LowerLimit[i])*actionPickup[i][t];
				prob.newCtr("PickupAction", p1 <= 0);
				p1 = 0;
			}

		for (int i : DeliveryNodes)
			for (int t : Periods)
			{
				p1 = delivery[i][t] - min(Capacity, UpperLimit[i] - LowerLimit[i])*actionDelivery[i][t];
				prob.newCtr("PickupAction", p1 <= 0);
				p1 = 0;
			}
	}
	

	if (MaskOn) {

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
			p1 = 0;
		}
	}

	//Routing constraints

	XPRBexpr p2;
	for (int i : AllNodes){
		for (int t : Periods) {
			for (int j : AllNodes) {

				if (map.inArcSet(i, j)) {
					p1 += x[i][j][t];
				}

				if (map.inArcSet(j, i)){
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

	addInventoryAndLoadingCtr(prob);

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

			prob.newCtr("Vehicle capacity delivery", p1 <= 0);
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

void IRP::RouteProblem::formulateRouteProblem(int minimizeSelection)
{
	//initialize route matrix
	initializeRouteParameters();
	initializeRoutes();
	//printRouteMatrix();
	addInventoryCtr();
	addRouteConstraints();

	XPRBexpr obj = 0;

	//To get practical right answer
	for (auto node : Instance.Nodes) {
		for (auto t : Instance.Periods) {
			obj += 0.01 * loadPickup[0][node][t];
		}
	}

	//Minimize the total cost
	if (minimizeSelection == ModelParameters::HIGHEST_TOTALCOST){
	
		int t = 3;
		//Trans cost
		for (int r : Routes)
			for (int t : Instance.Periods)
				obj += RouteCost[r] * travelRoute[r][t];

		//Holding cost
		for (int i : Instance.Nodes)
			for(int t : Instance.Periods)
				obj += Instance.HoldCost[i] * inventory[i][t];

		routeProblem.setObj(routeProblem.newCtr("OBJ", obj));  /* Set the objective function */

	}

	//Minimize the service in the period
	if (minimizeSelection == ModelParameters::MIN_SERVICE) {
		XPRBexpr obj = 0;
		int t = ShiftPeriod;
		for (int i : Instance.DeliveryNodes)
			obj += delivery[i][t];
		for (int i : Instance.PickupNodes)
			obj += pickup[i][t];

		routeProblem.setObj(routeProblem.newCtr("OBJ", obj));  /* Set the objective function */
	}

	if (minimizeSelection == ModelParameters::MinimumInventory) {
		XPRBexpr obj = 0;
		for (int i : Instance.Nodes)
			for (int t : Instance.Periods)
				obj += inventory[i][t];

		routeProblem.setObj(routeProblem.newCtr("OBJ", obj));  /* Set the objective function */

	}


}

void IRP::RouteProblem::initializeRouteParameters()
{
	RouteCost.resize(routes.size());
	int nRoutes = 0;
	for (auto r : routes) {
		//r->printRoute();
		r->setId(nRoutes);
		Routes.push_back(nRoutes);
		RouteCost[nRoutes] = r->getTransportationCost();
		//Initialize the A holder
		//Find max routes
		nRoutes++;
	}
	A.resize(nRoutes);
	for(auto r: Routes)
		A[r] = routes[r]->getRouteMatrix();
}

void IRP::RouteProblem::initializeRouteVariables()
{
	loadDelivery = new XPRBvar **[Instance.AllNodes.size()];
	loadPickup = new XPRBvar **[Instance.AllNodes.size()];

	for (int i : Instance.AllNodes) {
		loadDelivery[i] = new	XPRBvar *[Instance.AllNodes.size()];
		loadPickup[i] = new	XPRBvar *[Instance.AllNodes.size()];

		for (int j : Instance.AllNodes) {
	
			loadDelivery[i][j] = new	XPRBvar[Instance.Periods.size()];
			loadPickup[i][j] = new	XPRBvar[Instance.Periods.size()];

			for(int t : Instance.Periods){
	
			if (Instance.map.inArcSet(i, j)) {
					loadDelivery[i][j][t] = routeProblem.newVar(XPRBnewname("lD_%d%d%d", i, j, t), XPRB_PL, 0, Instance.Capacity);
					loadPickup[i][j][t] = routeProblem.newVar(XPRBnewname("lP_%d%d%d", i, j, t), XPRB_PL, 0, Instance.Capacity);
				}
			}
		}
	} // end initializing of load

	//Initialize inventory
	inventory = new XPRBvar *[Instance.Nodes.size()];

	for (int i : Instance.Nodes) {
		inventory[i] = new XPRBvar[Instance.Periods.size()];
		for (int t : Instance.AllPeriods) {
			inventory[i][t] = routeProblem.newVar(XPRBnewname("i_%d%d", i, t), XPRB_PL, Instance.LowerLimit[i], Instance.UpperLimit[i]);
		}
	}

	//Initialize at delivery nodes
	delivery = new XPRBvar *[Instance.DeliveryNodes.size() + 1];
	for (int i : Instance.DeliveryNodes) {
		delivery[i] = new XPRBvar[Instance.Periods.size() + 1];
		for (int t : Instance.Periods) {
			delivery[i][t] = routeProblem.newVar(XPRBnewname("qD_%d%d", i, t), XPRB_PL, 0, Instance.Capacity);

		}
	}

	//Initialize at pickup nodes
	pickup = new XPRBvar *[Instance.PickupNodes.size() + Instance.NumOfCustomers + 1];
	for (int i : Instance.PickupNodes) {
		pickup[i] = new  XPRBvar[Instance.Periods.size() + 1];
		for (int t : Instance.Periods) {
			pickup[i][t] = routeProblem.newVar(XPRBnewname("qP_%d%d", i, t), XPRB_PL, 0, Instance.Capacity);
		}
	}

}

void IRP::RouteProblem::addInventoryCtr()
{
		XPRBexpr p1;
		XPRBexpr p2;

		//Inventory constraints
		for (int i : Instance.Nodes) {
			p1 = inventory[i][0];
			//printf("%d", InitInventory[i]);
			routeProblem.newCtr("Initial inventory", inventory[i][0] == Instance.InitInventory[i]);
			p1 = 0;
		}

		for (int i : Instance.DeliveryNodes) {
			for (int t : Instance.Periods) {
				p1 = inventory[i][t] - inventory[i][t - 1] + Instance.Demand[i][t] - delivery[i][t];
				routeProblem.newCtr("Delivery Inventory balance", p1 == 0);
				p1 = 0;

				p1 = inventory[i][t - 1] + delivery[i][t];
				routeProblem.newCtr("Delivery Inventory balance 2", p1 <= Instance.UpperLimit[i]);
				p1 = 0;
			}
		}

		for (int i : Instance.PickupNodes) {
			for (int t : Instance.Periods) {
				//printf("%d", Demand[i][t]);
				p1 = inventory[i][t] - inventory[i][t - 1] - Instance.Demand[i][t] + pickup[i][t];
				routeProblem.newCtr("Pickup Inventory balance", p1 == 0);
				p1 = 0;

				p1 = inventory[i][t - 1] - pickup[i][t];
				routeProblem.newCtr("Pickup Inventory balance 2", p1 >= Instance.LowerLimit[i]);
				p1 = 0;
			}
		}

		//Upper and lower inventory
		for (int i : Instance.Nodes) {
			for (int t : Instance.Periods) {
				p1 = inventory[i][t];
				routeProblem.newCtr("Inventory Lower Limit", p1 >= Instance.LowerLimit[i]);
				routeProblem.newCtr("Inventory Upper Limit", p1 <= Instance.UpperLimit[i]);
				p1 = 0;
			}
		}

		//Loading constraints
		for (int i : Instance.DeliveryNodes) {
			for (int t : Instance.Periods) {
				for (int j : Instance.AllNodes) {
					if (Instance.map.inArcSet(j, i)) {
						p1 += loadDelivery[j][i][t];
						p2 -= loadPickup[j][i][t];
					}
					if (Instance.map.inArcSet(i, j)) {
						p1 -= loadDelivery[i][j][t];
						p2 += loadPickup[i][j][t];
					}
				}
				p1 -= delivery[i][t];

				routeProblem.newCtr("LoadBalance Delivery", p1 == 0);
				routeProblem.newCtr("PickupBalance at deliveryNodes", p2 == 0);
				p1 = 0;
				p2 = 0;
			}

		}

		for (int i : Instance.PickupNodes) {
			for (int t : Instance.Periods) {
				for (int j : Instance.AllNodes) {
					if (Instance.map.inArcSet(j, i)) {
						p1 += loadPickup[j][i][t];
						p2 += loadDelivery[j][i][t];
					}
					if (Instance.map.inArcSet(i, j)) {
						p1 -= loadPickup[i][j][t];
						p2 -= loadDelivery[i][j][t];
					}
				}
				p1 += pickup[i][t];

				routeProblem.newCtr("LoadBalance pickup", p1 == 0);
				routeProblem.newCtr("DeliveryBalance at pickupNodes", p2 == 0);
				p1 = 0;
				p2 = 0;
			}

		}

}

int IRP::RouteProblem::getShiftPeriod()
{
	return ShiftPeriod;
}

/*void IRP::RouteProblem::shiftQuantityCtr(int quantityPic, int period)
{
	XPRBexpr p1;
	//Add shift variables
	shift= new XPRBvar
	

	//Balance shift
	for (auto i : Instance.DeliveryNodes) {
		p1 += delivery[i][period];
	}

	for (auto i : Instance.PickupNodes) {
		p2 += pickup[i][period];
	}

	for (auto i : Instance.DeliveryNodes) {
		for (auto t : Instance.Periods) {
			if (t != period)
				p2 += delivery[i][t];
		}
		
	}	
	routeProblem.newCtr("Shift quantity", quantity - p1 = shift);
}*/


void IRP::RouteProblem::initializeRoutes()
{

	//Initialize travel route variables
	travelRoute = new XPRBvar *[Routes.size()];
	for (auto r : Routes) {
		travelRoute[r] = new XPRBvar[Instance.Periods.size() + 1];
		for (auto t : Instance.Periods)
			travelRoute[r][t] = routeProblem.newVar(XPRBnewname("x%d%d", r, t), XPRB_BV, 0, 1);
	}
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







void IRP::getSubset(vector<int> subset, int subsetSize, int nodePos, vector<int> & searchNodes, int t1, int t2)
{
	subset.push_back(searchNodes[nodePos]);

	if (subset.size() < subsetSize) {
	
		vector<int> localSubset = subset;
		
		for (int i = nodePos; i < searchNodes.size() - 1; i++) {
			getSubset(subset, subsetSize, i+1,  searchNodes, t1, t2);
		
		}
	}
	else {
		XPRBexpr p1;
		XPRBexpr p2;
		double flow;
		double denominator;


		for (auto i : subset)
			if(map.isDelivery(i))
				flow = ExcessConsumption[i][t1][t2];
			else
				flow = ExcessProd[i][t1][t2];

		if (subset.size() == 1) {
			denominator = min(Capacity, UpperLimit[subset[0]] - LowerLimit[subset[0]]);
		}
		else
			denominator = Capacity;

		if (ceil(flow / denominator) - flow/denominator> ExcessParameter){

			cout << "\nPeriod" << t1 << t2 << "\n";
			for (auto i : subset) {

				cout << i << " - ";

			}
			cout << "\n";
			//Get difference set

			vector <int> Difference = ModelBase::createDifferenceSet(AllNodes, subset);
			//Add minimum flow constraint
			for (int t = t1; t <= t2; t++)
				for (auto i : subset)
					for (auto j : Difference)
						if (map.inArcSet(i, j)) {
							p1 += x[i][j][t];
						}

			p2 = ceil(flow / denominator);
			prob.newCtr("MinFlow", p1 >= p2);
		}
	}	
}

void IRP::addInventoryAndLoadingCtr(XPRBprob & problem)
{
	XPRBexpr p1;
	XPRBexpr p2;

	//Inventory constraints
	for (int i : Nodes) {
		p1 = inventory[i][0];
		//printf("%d", InitInventory[i]);
		problem.newCtr("Initial inventory", inventory[i][0] == InitInventory[i]);
		p1 = 0;
	}

	for (int i : DeliveryNodes) {
		for (int t : Periods) {
			p1 = inventory[i][t] - inventory[i][t - 1] + Demand[i][t] - delivery[i][t];
			problem.newCtr("Delivery Inventory balance", p1 == 0);
			p1 = 0;

			p1 = inventory[i][t - 1] + delivery[i][t];
			problem.newCtr("Delivery Inventory balance 2", p1 <= UpperLimit[i]);
			p1 = 0;
		}
	}

	for (int i : PickupNodes) {
		for (int t : Periods) {
			//printf("%d", Demand[i][t]);
			p1 = inventory[i][t] - inventory[i][t - 1] - Demand[i][t] + pickup[i][t];
			problem.newCtr("Pickup Inventory balance", p1 == 0);
			p1 = 0;

			p1 = inventory[i][t - 1] - pickup[i][t];
			problem.newCtr("Pickup Inventory balance 2", p1 >= LowerLimit[i]);
			p1 = 0;
		}
	}

	//Upper and lower inventory
	for (int i : Nodes) {
		for (int t : Periods) {
			p1 = inventory[i][t];
			problem.newCtr("Inventory Lower Limit", p1 >= LowerLimit[i]);
			problem.newCtr("Inventory Upper Limit", p1 <= UpperLimit[i]);
			p1 = 0;
		}
	}

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

}



void IRP::RouteProblem::addRouteConstraints()
{
	XPRBexpr p1 = 0;
	XPRBexpr p2 = 0;
	bool ind = false;

	//Arc capacity
	for(int i : Instance.AllNodes)
		for (int j : Instance.AllNodes) {
			if (Instance.map.inArcSet(i, j)) {
				for (int t : Instance.Periods) {
					for (int r : Routes) {
						p1 += A[r][i][j] * travelRoute[r][t];
					}


					p2 = loadDelivery[i][j][t] + loadPickup[i][j][t];
					XPRBctr ff = routeProblem.newCtr("Arc capacity limit", p2 <= Instance.Capacity * p1);
					//ff.print();
					p1 = 0;
					p2 = 0;
				}
			}
		}
	//end arc capacity

	//Service limit
	for (int t : Instance.Periods) {

			//For all pickup nodes
			for (int i : Instance.PickupNodes) {
				for (int j : Instance.AllNodes) {
					for (auto r : Routes) {
						p1 += A[r][i][j] * travelRoute[r][t];
					}
				}

				p1 = p1 * min(Instance.Capacity, Instance.UpperLimit[i] - Instance.LowerLimit[i]);
				p2 = pickup[i][t];
				routeProblem.newCtr("Pickup limit", p2 - p1 <= 0);

				p1 = 0;
				p2 = 0;

			}
			//For all delivery nodes
			for (int i : Instance.DeliveryNodes) {
				for (int j : Instance.AllNodes) {
					for (auto r : Routes) {
						p1 += A[r][i][j] * travelRoute[r][t];
					}
				}

				p1 = p1 * min(Instance.Capacity, Instance.UpperLimit[i] - Instance.LowerLimit[i]);
				p2 = delivery[i][t];
				routeProblem.newCtr("Delivery limit", p2 - p1 <= 0);

				p1 = 0;
				p2 = 0;
			}
		}//end service limit

		//Visit constraint
		int counter = 0;
		for (int i : Instance.Nodes) {
			for (int t : Instance.Periods) {
				counter = 0;
				ind = false;
				for (int j : Instance.AllNodes) {
					if (Instance.map.inArcSet(i, j)) {
						for (int r : Routes) {

							if (A[r][i][j] > 0.01) {
								p1 += A[r][i][j] * travelRoute[r][t];
								counter++;
								ind = true;
							}
						}
					}
				}

				if (ind) {
					routeProblem.newCtr("Visit limit", p1 <= 1);
					
				}
				p1 = 0;
			}
		} //end visit constraint


		//Vehicle constraint
		for (int t : Instance.Periods) {
			for (int r : Routes) {
				p1 += travelRoute[r][t];
			}

			routeProblem.newCtr("Vehicle limit", p1 <= ModelParameters::nVehicles);
			p1 = 0;
		}
			
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
	bool arcIndicator;
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

	//Initialize action variables if simultanoues
	if (ModelParameters::Simultaneous) {
		actionDelivery = new XPRBvar *[DeliveryNodes.size() + 1];
		actionPickup = new XPRBvar *[AllNodes.size() + 1];

		for (int i : DeliveryNodes) {
			actionDelivery[i] = new XPRBvar[Periods.size() + 1];
			for (int t : Periods) {
				actionDelivery[i][t] = prob.newVar(XPRBnewname("ActionDel_%d%d", i, t), XPRB_BV, 0, 1);
			}
		}

		for (int i : PickupNodes) {
			actionPickup[i] = new XPRBvar[Periods.size() + 1];
			for (int t : Periods) {
				actionPickup[i][t] = prob.newVar(XPRBnewname("ActionPic_%d%d", i, t), XPRB_BV, 0, 1);
			}
		}

		simAction = new XPRBvar **[AllNodes.size() + 1];
		for (int i : DeliveryNodes) {
			simAction[i] = new XPRBvar *[AllNodes.size() + 1];
			for (int j : PickupNodes) {
				simAction[i][j] = new XPRBvar[Periods.size() + 1];
				for (int t : Periods) {
					if (map.isColocated(i, j)) {
						simAction[i][j][t] = prob.newVar(XPRBnewname("Sim_%d%d%d", i, j, t), XPRB_BV, 0, 1);
					}
				}
			}
		}
		
	} //end simultaneous

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
	//Create a nodes for all customers and depot, same id as in model
	for (int i : Instance.AllNodes) {
		NodeHolder[i] = new NodeIRPHolder(i, Instance);		
	}
}



//Initilize solution from nodes
IRP::Solution::Solution(IRP &model, vector<NodeIRPHolder*> nodes)
	:
	Instance(model),
	NodeHolder(nodes)
{

}

IRP::NodeIRP * IRP::Solution::getDepot(int period)
{
	for (auto node : NodeHolder) {
		if (node->getId() == 0) {
			return node->Nodes[period];
		}
	}
}

void IRP::Solution::buildGraph(vector<Node*> &graph)
{
	for (int id : Instance.AllNodes)
		graph.push_back(new Node(id));

	for (auto r : getAllRoutes()) {
		for (Node* node : r->route) {
			for (Node::Edge *edge : node->getEdges()) {
				if (!graph[node->getId()]->hasEdge(edge))
					graph[node->getId()]->addEdge(edge->getEndNode());
			}
		}
	}
}

void IRP::Solution::routeSearch()
{
	vector<IRP::Route*> RouteHolder;
	for (int i = 0; i < 5; i++) {
		RouteHolder.clear();
		generateRoutes(RouteHolder);
		//RouteHolder = sol->getAllRoutes();
		//RouteHolder = sol->getAllRoutes();
		//Solve route problem

		IRP::RouteProblem routeProb2(Instance, RouteHolder);
		routeProb2.formulateRouteProblem(ModelParameters::HIGHEST_TOTALCOST);
		//routeProb.lockRoutes();
		auto routeSol2 = routeProb2.solveProblem();
		routeSol2->printSolution();

		//routeSol2->print("Solution/Heurestic" + to_string(i), ModelParameters::X);
		for (auto route : routeSol2->getAllRoutes()) {
			//route->printPlot("Solution/Sol2route" + to_string(route->getId()) + "Num" + to_string(i));
		}
		this->NodeHolder = routeSol2->NodeHolder;
	}
}

void IRP::Solution::routeOptimize()
{
	IRP::RouteProblem routeProb1(Instance, getAllRoutes());
	routeProb1.formulateRouteProblem(ModelParameters::HIGHEST_TOTALCOST);
	auto routeSol1 = routeProb1.solveProblem();
	this->NodeHolder = routeSol1->NodeHolder;
}



//Returns Aijt vector where 1 if arc ij is traversed in period t
int *** IRP::Solution::getRouteMatrix()
{
	int u;
	int v;
	int *** RouteMatrix = new int **[Instance.AllNodes.size()];

	for (auto i : Instance.AllNodes){
		RouteMatrix[i] = new int *[Instance.AllNodes.size()];
		for (auto j : Instance.AllNodes) {
			RouteMatrix[i][j] = new int[Instance.Periods.size()];
			for (auto t : Instance.Periods) {
				RouteMatrix[i][j][t] = 0;
			}
		}
	}

	for (auto t : Instance.Periods) {
		for (auto routeHolder : getRoutes(t)) {
			for (auto node : routeHolder->route) {
				u = node->getId();
				v = node->getEdge()->getEndNode()->getId();
				RouteMatrix[u][v][t] = 1;
			}
		}
	}

	/*for (auto t : Instance.Periods) {
		cout << "\nPeriod " << t <<"\n";
		for (auto i : Instance.AllNodes) {
			cout << "\n";
			for (auto j : Instance.AllNodes) {
				cout << RouteMatrix[i][j][t] << "\t";
			}
		}
	}*/

	return RouteMatrix;
}

//Returns a period based on a selection criteria
int IRP::Solution::selectPeriod(int selection)
{
	int period = -1;
	int resCap = -100;
	if (selection == ModelParameters::HIGHEST_RESIDUAL_CAPACITY) {
		for (int t : Instance.Periods) {
			if (resCap <= getResidualCapacity(t) + 0.01) {
				resCap = getResidualCapacity(t);
				period = t;
			}
		}
	}
	return period;
}


vector<IRP::Route*> IRP::Solution::getRoutes(int period)
{
	NodeIRP* v;
	vector<Route*> routes;
	vector <NodeIRP*> path;
	for (auto edge : getDepot(period)->getEdges()) {
		auto u = new NodeIRP(0);
		v = new NodeIRP(edge->getEndNode()->getId());
		u->Node::addEdge(v);
		//Depth first search 
		path.clear();
		path.push_back(u);
		u = v;
		do {
			path.push_back(u);
			edge = NodeHolder[u->getId()]->Nodes[period]->getEdge();
			v = new NodeIRP(edge->getEndNode()->getId());
			u->Node::addEdge(v);
			u = v;

		} while (u->getId() != 0); //while not depot

		Route * route = new Route(path, Instance);
		route->setPeriod(period);
		routes.push_back(route);
	}
	
	return routes;
}

void IRP::Solution::print(string filename, int load)
{
	Instance.solCounter++;
	vector<Node *> graph;
	for (int t : Instance.Periods) {
		Instance.buildGraphSol(graph, t, this);
		graphAlgorithm::printGraph(graph, Instance, filename+ to_string(Instance.solCounter)+"t"+ to_string(t), load);
		graph.clear();
	}
}

void IRP::Solution::mergeRoutes(int position, Route * route, vector<Route*> &Routes, vector<Route*> &newRoutes)
{
	if (position < Routes.size() - 1) {
		mergeRoutes(position + 1, route, Routes, newRoutes);
	}

	auto newroute = route->generateRoute(Routes[position]);
	
	//Remove duplicates
	bool duplicate = false;
	//Try to make it uniqe or delete it
	for (int i = 1; i <= 5; i++){
		for (auto r : Routes) {
			if (newroute->isDuplicate(r)) {
				if(i == 5)
					duplicate = true;
				else {
					int a = rand() % (Routes.size() - 1);
					newroute = newroute->generateRoute(Routes[a]);
				}
				break;
			}	
		}
	}

	for (int i = 1; i <= 5; i++) {
		for (auto r : newRoutes) {
			if (newroute->isDuplicate(r)) {
				if (i == 5)
					duplicate = true;
				else {

					int a = rand() % (Routes.size() - 1);
					newroute = newroute->generateRoute(Routes[a]);
				}
				break;
			}
		}
	}


	//newroute->printPlot("Routes/afterRemoval" + to_string(rand() % 100));
	if (!duplicate) {
		newRoutes.push_back(newroute);
	}
}

void IRP::Solution::generateRoutes(vector<IRP::Route* >&routeHolder)
{
	vector <IRP::Route*> routes = getAllRoutes();

	int count = 0;
	//push back exisiting routes
	for (auto r : routes) {
		routeHolder.push_back(r);
		r->setId(count);
		count++;
	}

	//Plot initial routes
	/*for (auto a : routes) {
		a->printPlot("Routes/initial" + to_string(a->getId()));
	}*/
	vector<IRP::Route*> generation;
	int jj = 1;
	if (routes.size() >= 2) {

		for (int i = 1; i <= 1; i++) {
			generation.clear();

			vector<IRP::Route*> newRoutes;
			//Recursively merge
			for (int position = 0; position < routes.size() - 1; position++) {
				mergeRoutes(position + 1, routes[position], routes, newRoutes);
			}

			//Plot merged routes
			for (auto a : newRoutes) {
				a->printPlot("Routes/newroute" + to_string(jj) + to_string(i));
				jj++;
			}

			//Plot graph
		

			//Delete 

			//graphAlgorithm::printGraph(graph, Instance, "graphSolutions/routegraph", ModelParameters::X);

			//Remove duplicates
			bool duplicate = false;
			//Check old routes
			
			for(auto newr : newRoutes){
				for (auto r : routeHolder) {
					if (newr->isDuplicate(r))
						duplicate = true;
				}
					
				if (!duplicate) {
					newr->setId(count);
					count++;
					routeHolder.push_back(newr);
				}

			}

			int nn = 0;
	
			routes = newRoutes;
		}

	}
	//print all routes
	int i = 1;
	for (auto route : routeHolder) {
		route->printPlot("Routes/route" + to_string(i));
		i++;
	}


}

//Returns the visited nodes in a period
vector<IRP::NodeIRP*> IRP::Solution::getVisitedNodes(int period)
{
	vector<IRP::NodeIRP*> visitedNodes;
	for (auto node : NodeHolder) {
		if (node->quantity(period) > 0.01) {
			auto nodeIRP = node->Nodes[period];
			visitedNodes.push_back(nodeIRP);
		}
	}
	return visitedNodes;
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
	for (IRP::Route* r : getRoutes(t)) {
		if (!isRouteFeasible(r))
			return false;
	}
	return true;
}


void IRP::Solution::clearEdges(int period)
{
	for (auto node : NodeHolder) {
		node->Nodes[period]->deleteEdges();
	}
}

//Returns the period shifted from
int IRP::Solution::shiftQuantity(int PeriodSelection, int ObjectiveSelection)
{
	return 0;
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

double IRP::Solution::getResidualCapacity(int period)
{
	double residual = 0;
	for (auto node : NodeHolder) {
		residual += node->quantity(period);
	}
	return residual / getNumberOfRoutes(period);
}

double IRP::Solution::getTotalDelivery(int period)
{
	double totalDelivery = 0;
	for (auto node : NodeHolder) {
		if (node->isDelivery() && node->getId() != 0)
			totalDelivery += node->quantity(period);
	}

	return totalDelivery;
}

double IRP::Solution::getTotalPickup(int period)
{
	double totalPickup = 0;
	for (auto node : NodeHolder)
		if (!node->isDelivery())
			totalPickup += node->quantity(period);

	return totalPickup;
}

double IRP::Solution::getNodeVisits(int period)
{
	return getDeliveryNodeVisits(period) + getPickupNodeVisits(period);
}

double ** IRP::Solution::getVisitedMatrix()
{
	double ** VisitedMatrix;
	cout << "\n\nVisitMatrix: 1 = visited, 0 = not visited";
	VisitedMatrix = new double *[Instance.getNumOfNodes() + 1];
	for (auto node : NodeHolder) {
		if (node->getId() != 0) {
			cout << "\n";
			VisitedMatrix[node->getId()] = new double [Instance.getNumOfPeriods() + 1];
			for (auto period : Instance.Periods) {
				cout << "\t";
				if (node->getId() != 0) {
					if (node->quantity(period) > 0.01) {
						VisitedMatrix[node->getId()][period] = 1;
						cout << VisitedMatrix[node->getId()][period];
					}
					else {
						VisitedMatrix[node->getId()][period] = 0;
						cout << VisitedMatrix[node->getId()][period];
					}
				}
			}
		}	
	}
	return VisitedMatrix;
}
		


double IRP::Solution::getDeliveryNodeVisits(int period)
{
	double nodeVisits = 0;
	for (auto node : NodeHolder)
		if(node->isDelivery() && node->quantity(period) >= 0.01)
			nodeVisits += 1;

	return nodeVisits;
}

double IRP::Solution::getPickupNodeVisits(int period)
{
	double nodeVisits = 0;
	for (auto node : NodeHolder)
		if (!node->isDelivery() && node->quantity(period) >= 0.01)
			nodeVisits += 1;

	return nodeVisits;
}

double IRP::Solution::getDelivery(int period)
{
	double service = 0;
	for (auto node : NodeHolder)
		if(node->isDelivery())
			service += node->quantity(period);
		if (service > 0.01)
			return service / getDeliveryNodeVisits(period);
	else
		return 0;
}

double IRP::Solution::getPickup(int period)
{
	double service = 0;
	for (auto node : NodeHolder)
		if (!node->isDelivery())
			service += node->quantity(period);
	if (service > 0.01)
		return service / getPickupNodeVisits(period);
	else
		return 0;
}


IRP::NodeIRPHolder * IRP::Solution::getNode(int id)
{
	for (NodeIRPHolder * n : NodeHolder) {
		if (n->getId() == id)
			return n;
	}
}

vector<IRP::NodeIRPHolder*>& IRP::Solution::getNodes()
{
	return NodeHolder;
}



void  IRP::Solution::solveInventoryProblem()
{
	IRP::RouteProblem routeProb(Instance, getAllRoutes());
	routeProb.ShiftPeriod = 4;
	routeProb.formulateRouteProblem(ModelParameters::MinimumInventory);
	routeProb.lockRoutes();
	routeProb.solveProblem(this);
}

void IRP::Solution::insertCustomer(int customerID, int period)
{
	vector<NodeIRP *> nodes = getCustomer(customerID);
	createSubroute(nodes);
			
	cout << "Old routes in " << period << "\n";
	for (auto route : getRoutes(period)) {
		route->printRoute();
	}
	insertSubrouteInRoute(nodes, period);
	
	cout << "New routes in " << period <<"\n";
	for (auto route : getRoutes(period)) {
		route->printRoute();
		
	}

	//Reoptimize inventory
	solveInventoryProblem();
}



vector<IRP::NodeIRP*> IRP::Solution::getCustomer(int id)
{
	vector<NodeIRP * > customer;
	//Deliverynode
	int delId = Instance.map.getDeliveryNode(Instance.map.getCustomer(id));
	int pickId = Instance.map.getPickupNode(Instance.map.getCustomer(id));
	NodeIRP * nodeDel = new NodeIRP(delId);
	//Pickupnode
	NodeIRP * nodePick = new NodeIRP(pickId);
	customer.push_back(nodeDel);
	customer.push_back(nodePick);
	return customer;
}


vector<IRP::Route*> IRP::Solution::getAllRoutes()
{
	vector<IRP::Route*> routes;
	for (auto t : Instance.Periods) {
		//int period = sol->selectPeriod(ModelParameters::HIGHEST_RESIDUAL_CAPACITY);
		routes.clear();
		//Solve a routing problem
		for (auto t : Instance.Periods) {
			for (auto r : getRoutes(t))
				routes.push_back(r);
		}
	}
	return routes;
}

double IRP::Solution::getObjective()
{
		return getHoldingCost() + getTransportationCost();
}


void IRP::addVisitConstraint(double ** Visit, int selection)
{
	XPRBexpr p1 = 0;
	XPRBexpr p2 = 0;

	cout << "\n";
	if (VisitCtr.isValid()) {

		prob.delCtr(VisitCtr);
	}

	if (selection == ModelParameters::ForceVisits) {
		int visits;

		for (int t : Periods) {
		visits = 0;
			for (int i : Nodes) {
				if (Visit[i][t] == 0 && !TabuMatrix[i][t].isValid())
				{
					visits += 1;
					p1 += y[i][t];
				}
			}
		}
		VisitCtr = prob.newCtr("MinVisits", p1 >= ceil(visits*0.2));
		VisitCtr.print();
		cout << "\n";
	}

	if (selection == ModelParameters::ForceChanges) {
		for (int t : Periods) {
			for (int i : Nodes) {
				if (Visit[i][t] == 0 && !TabuMatrix[i][t].isValid()) {
					p1 += y[i][t];
				}
				else if (Visit[i][t] == 1 && !TabuMatrix[i][t].isValid()) {
					p2 += (1 - y[i][t]);
				}
			}
		}

		//add constraint
		VisitCtr = prob.newCtr("MinVisits", p1 + p2 >= ceil(Nodes.size()*Periods.size()*0.1));
		p1 = 0;
		VisitCtr.print();
		cout << "\n";

	} //End force changes

} //end add visit constraint

/*IRP::Route * IRP::getRoute(int id)
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
}*/


void IRP::addValidIneq(int ValidIneq)

{
	ExcessParameter = 0.3;

	XPRBexpr p1 = 0;
	XPRBexpr p2 = 0;
	XPRBexpr p3 = 0;
	//Minimum visit

	if (ValidIneq == ModelParameters::MinimumNodeVisit) {
		double minVisit;

		for (int i : DeliveryNodes) {
			minVisit = 0;
			p1 = 0;
			for (int t2 : Periods) {
				for (int t1 : Periods) {
					if (t1 <= t2) {
						for (int t = t1; t <= t2; t++) {
							p1 += y[i][t];
						}
						minVisit = ExcessConsumption[i][t1][t2] / min(Capacity, UpperLimit[i] - LowerLimit[i]);
						double b = ExcessConsumption[i][t1][t2];
						if (ceil(minVisit) - minVisit >= ExcessParameter)
						{
							prob.newCtr("MinVisitDelivery", p1 >= ceil(minVisit)).print();
						}
						p1 = 0;
						minVisit = 0;
					}
				}
			}
		} // end delivery nodes

		for (int i : PickupNodes) {
			minVisit = 0;
			p1 = 0;
			for (int t2 : Periods) {
				for (int t1 : Periods) {
					if (t1 <= t2) {
						for (int t = t1; t <= t2; t++) {
							p1 += y[i][t];
						}
						minVisit = ExcessProd[i][t1][t2] / min(Capacity, UpperLimit[i] - LowerLimit[i]);
						if (ceil(minVisit) - minVisit >= ExcessParameter)
						{
							prob.newCtr("MinVisitPickup", p1 >= ceil(minVisit)).print();
						}
					}
				}
			}
		} //End pickup nodes
	} // End minimum visits

	if (ValidIneq == ModelParameters::MinimumInventory) {

		for (int i : DeliveryNodes) {
			for (int t2 : Periods) {
				for (int t1 : Periods) {
					if (t1 <= t2) {
						for (int t = t1; t <= t2; t++)
						{
							p2 += Demand[i][t];
							p3 += y[i][t];
						}

						p3 = 1 - p3;
						p1 = inventory[i][t1 - 1];

						prob.newCtr("MinInventory", p1 >= p2*p3 + map.getLowerLimit(i));
						p1 = 0;
						p2 = 0;
						p3 = 0;
					}
				}
			}
		} // End delivery nodes

		for (int i : PickupNodes) {
			for (int t2 : Periods) {
				for (int t1 : Periods) {
					if (t1 <= t2) {
						for (int t = t1; t <= t2; t++)
						{
							p2 += Demand[i][t];
							p3 += y[i][t];
						}

						p3 = 1 - p3;
						p1 = inventory[i][t1 - 1];

						prob.newCtr("MinInventory", p1 <= map.getUpperLimit(i) - p2*p3);
						p1 = 0;
						p2 = 0;
						p3 = 0;
					}
				}
			}
		} // End pickup nodes

	} // End minimum inventory

	if (ValidIneq == ModelParameters::MinimumFlow) {
		//Check all subsets with positive excess demand

		vector<vector<vector<int>>> IncludedNodes;
		IncludedNodes.resize(Periods.size() + 1);

		//For all delivery nodes
		for (auto t1 : Periods) {
			IncludedNodes[t1].resize(Periods.size() + 1);
			for (auto t2 : Periods) {
				if (t1 <= t2)
					for (auto i : DeliveryNodes) {
						if (ExcessConsumption[i][t1][t2] >= 0.01)
							IncludedNodes[t1][t2].push_back(i);
					}

			}
		}
		//Recursively check all possible subsets up to the subsetSize
		for (auto t1 : Periods)
			for (auto t2 : Periods) {
				if (t1 <= t2) {
					vector<int> subset;
					for (int subsetSize = 3; subsetSize <= ModelParameters::SubsetSizeMinFlow; subsetSize++)
						for (auto nodePos = 0; nodePos < IncludedNodes[t1][t2].size(); nodePos++)
							getSubset(subset, subsetSize, nodePos, IncludedNodes[t1][t2], t1, t2);
				}
			}

		//Clear nodes
		for (auto t1 : Periods)
			for (auto t2 : Periods)
				if (t1 <= t2)
					IncludedNodes[t1][t2].clear();

		//For all pickup nodes
		for (auto t1 : Periods) {
			for (auto t2 : Periods) {
				if (t1 <= t2)
					for (auto i : PickupNodes) {
						if (ExcessProd[i][t1][t2] >= 0.01)
							IncludedNodes[t1][t2].push_back(i);
					}

			}
		}

		//Recursively check all possible subsets up to the subsetSize
		for (auto t1 : Periods)
			for (auto t2 : Periods) {
				if (t1 <= t2) {
					vector<int> subset;
					for (int subsetSize = 1; subsetSize <= ModelParameters::SubsetSizeMinFlow; subsetSize++)
						for (auto nodePos = 0; nodePos < IncludedNodes[t1][t2].size(); nodePos++)
							getSubset(subset, subsetSize, nodePos, IncludedNodes[t1][t2], t1, t2);
				}
			}

	} //End minimumflow
	
}//End valid inequalitites


//Returns the difference in visits from sol2 to sol1
double ** IRP::getVisitDifference(Solution * sol1, Solution * sol2)
{
	
	double ** changedMatrix;
	double ** newVisitedMatrix = sol1->getVisitedMatrix();
	double ** prevVisitedMatrix = sol2->getVisitedMatrix();
	cout << "\n";
	cout << "\nChangeMatrix: 1 added visit, -1 removed visit\n";
	changedMatrix = new double *[getNumOfNodes() + 1];
	for (auto i : Nodes) {
		cout << "\n";
		changedMatrix[i] = new double[getNumOfPeriods() + 1];
		for (auto j : Periods) {
			double a = newVisitedMatrix[i][j];
			double b = prevVisitedMatrix[i][j];
			changedMatrix[i][j] = newVisitedMatrix[i][j] - prevVisitedMatrix[i][j];
			cout << changedMatrix[i][j] << "\t";
		}
	}

	delete newVisitedMatrix;
	delete prevVisitedMatrix;

	return changedMatrix;
}


void IRP::RouteProblem::printRouteMatrix()
{
	for (int r = 0; r < A.size(); r++) {
		cout << "\n\n\n";
		for (int i = 0; i < Instance.AllNodes.size(); i++) {
			cout << "\n";
			for (int j = 0; j < Instance.AllNodes.size(); j++) {
				cout << A[r][i][j]<<"  ";

			}
		}
	}
}





void IRP::updateTabuMatrix(double ** changeMatrix)
{
	srand(std::time(0));
	int counter = 0;
	int i = 0;
	cout << "\n\nCountMatrix: Times since last changed , >0 added visit, <0 removed visit. * locked variable\n";
	for (auto i : Nodes) {
		cout << "\n";
		for (auto t : Periods) {

			//Clear the tabu matrix
			if (TabuMatrix[i][t].isValid()) {
				prob.delCtr(TabuMatrix[i][t]);
				TabuMatrix[i][t] = 0;
			}

			//Update count matrix
			if (changeMatrix[i][t] != 0) {
				CountMatrix[i][t] = changeMatrix[i][t];
			}
			else if (CountMatrix[i][t] >= 1)
				CountMatrix[i][t] += 1;
			else if (CountMatrix[i][t] <= -1)
				CountMatrix[i][t] -= 1;


			//Remove from tabu if Count is outside tabulength
			if (CountMatrix[i][t] > ModelParameters::TabuLength || CountMatrix[i][t] < -ModelParameters::TabuLength) {
				CountMatrix[i][t] = 0;
			}

			cout << CountMatrix[i][t];

			//Do while no change tabued
	
				//If visit, lock that visit with 25% chance.
				if (CountMatrix[i][t] >= 1) {
					if (rand() % 100 + 1 <= 25) {
						TabuMatrix[i][t] = prob.newCtr(y[i][t] >= 1);
						counter += 1;
						cout << "*\t";
					}
					else
						cout << "\t";
				}
				else if (CountMatrix[i][t] == 0) cout << "\t";

				//Else if visit removed, lock that visit with 25% chance.
				else if (CountMatrix[i][t] <= -1) {
					/*if (t == 1) {
					if (rand() % 100 + 1 <= 50) {
					TabuMatrix[i][t] = prob.newCtr(y[i][t] <= 0);
					counter += 1;
					cout << "*\t";
					}
					else
					cout << "\t";

					}
					else*/
					if (rand() % 100 + 1 <= 25) {
						TabuMatrix[i][t] = prob.newCtr(y[i][t] <= 0);
						counter += 1;
						cout << "*\t";
					}
					else
						cout << "\t";

				}
			}
		
	}

	//If no locked continue.
	/*while (counter < 1 && i <= 20) {
	for (auto i : Nodes) {
	if (CountMatrix[i][1] >= 1)
	{
	if (rand() % 100 + 1 <= 50) {
	counter += 1;
	TabuMatrix[i][1] = prob.newCtr(y[i][1] >= 1);
	cout << "y" << i << 1 << "=" << 1;
	break;
	}
	}
	else if (CountMatrix[i][1] <= -1) {
	if (rand() % 100 + 1 <= 50) {
	TabuMatrix[i][1] = prob.newCtr(y[i][1] <= 0);
	counter += 1;
	cout << "y" << i << 1 << "=" << 0;
	break;
	}
	}
	}
	i++;
	} // end while*/


	for (auto i : Nodes) {
		cout << "\n";
		for (auto t : Periods) {
			if (TabuMatrix[i][t].isValid()) {
				TabuMatrix[i][t].print();
				cout << "\t";
			}
		}
	}
} // end update tabu matrix

int IRP::getNumOfNodes()
{
	return Nodes.size();
}

void IRP::addHoldingCostCtr(double holdingCost)
{
	XPRBexpr p1 = 0;

	for (int i : Nodes)
		for(int t : Periods)
			p1 += inventory[i][t];

	prob.newCtr(XPRBnewname("HoldingCost"), p1 <= holdingCost * 1.05);
	p1 = 0;

	//Delete current objective
	prob.delCtr(prob.getCtrByName("OBJ"));

	//Set transportation costs as objective
	for (int i : AllNodes)
		for (int j : AllNodes)
			if (map.inArcSet(i, j)) {
				for (int t : Periods)
					p1 += map.getTransCost(i, j) * x[i][j][t];
			}

	prob.setObj(prob.newCtr("OBJ", p1));
	prob.print();
}

void IRP::RouteProblem::addRoutesToVector()
{	
	//Update set of routes
	for (IRP::Route *r : routes) {
		Routes.push_back(r->getId());
		A[r->getId()] = r->getRouteMatrix();
	}
		
}

void IRP::RouteProblem::updateSolution(IRP::Solution * sol)
{

	auto &nodeHolder = sol->getNodes();
	//Fill inventory and quantity at the nodes
	fillNodes(nodeHolder);

	//Fill load
	fillLoad(nodeHolder);
		

}

void IRP::RouteProblem::lockRoutes()
{
	
	//Select what routes to lock
	vector<int> counter;
	counter.resize(Instance.Periods.size() + 1);

	for (auto t : Instance.Periods)
		counter[t] = 0;

	for (auto route: routes){
		lockRoute(route);
		counter[route->getPeriod()]++;
	}
	//Lock the number of routes in a period
	XPRBexpr p1 = 0;

	for (auto t : Instance.Periods) {
		for (auto r : Routes) {
			p1 += travelRoute[r][t];
		}
		routeProblem.newCtr("Number of routes", p1 <= counter[t]);
		p1 = 0;
	}
}

bool IRP::Route::isDuplicate(Route * r)
{
	if (r->route.size() != route.size())
		return false;

	int i = 0;
	while (r->route[i]->getId() == route[i]->getId()) {
		if (route[i]->getEdge()->getEndNode()->getId() == 0)
				return true;
		i++;
	};

	return false;
}






void IRP::RouteProblem::lockRoute(IRP::Route * route)
{
	int routeId = route->getId();
	int period = route->getPeriod();
	routeProblem.newCtr("RouteLock", travelRoute[routeId][period] == 1);
}

IRP::Solution * IRP::RouteProblem::solveProblem(IRP::Solution * sol)
{
	//routeProblem.print();
	routeProblem.mipOptimise();
	//If no solution, allocate a new solution
	if (sol == 0) {
		sol = Instance.allocateSolution(Instance);
	}
	
	updateSolution(sol);
	return sol;
}


int IRP::RouteProblem::getRoutePosition(int routeId)
{
	for (int i : Routes)
	{
		if (routes[i]->getId() == routeId)
			return i;
	}
}

IRP::Solution * IRP ::allocateSolution(IRP &Instance)
{
	SolutionCounter++;
	IRP::Solution * sol = new IRP::Solution(Instance);
	sol->SolID = SolutionCounter;
	return sol;
}

void IRP::RouteProblem::fillRoutes(vector<vector<IRP::Route* >>& routeHolder)
{
	for (auto t : Instance.Periods) {
		cout << "Period " << t << ":\n";
		for (auto r : Routes) {
			if (travelRoute[r][t].getSol() > 0.01) {
				auto route = routes[r];
				routeHolder[t].push_back(route);
				route->printRoute();
			}
		}
	}
}



void IRP::RouteProblem::fillNodes(vector <NodeIRPHolder*> &nodeHolder)
{
	
	for (auto node: nodeHolder) {

		for (auto t : Instance.Periods) {
			node->Nodes[t]->deleteEdges();
			int i = node->getId();
			//Fill inventory
			if (node->getId() != 0)
				node->Nodes[t]->Inventory = inventory[i][t].getSol();

			//Fill quantity
			if (node->getId() != 0) {
				if (node->isDelivery()) {
					node->Nodes[t]->Quantity = delivery[i][t].getSol();
					double a = delivery[i][t].getSol();
					int b = 0;
				}
				else
					node->Nodes[t]->Quantity = pickup[i][t].getSol();
			}

		}
	}
}

void IRP::RouteProblem::fillLoad(vector <NodeIRPHolder*> &nodeHolder) {

	int i;
	int j;
	double loadDel;
	double loadPick;
	IRP::NodeIRP::EdgeIRP * edge;

	for (auto r : Routes)
		for (int t : Instance.Periods) {
			//Fill load for entire route
			if (travelRoute[r][t].getSol() >= 0.01)
			{
				auto route = routes[r]->route;
				int n = 0;
				for (int n = 0; n < route.size(); n++) {
					i = route[n]->getId();
					if (n == route.size() - 1)
						j = 0;
					else
						j = route[n + 1]->getId();

					//Fill load
					loadDel = loadDelivery[i][j][t].getSol();
					loadPick = loadPickup[i][j][t].getSol();
					nodeHolder[i]->Nodes[t]->addEdge(loadDel, loadPick, nodeHolder[j]->Nodes[t], 1);
				}

			}

		}
}

	
	


int IRP::getCapacity()
{
	return Capacity;
}

void IRP::useIPSubtourElimination()
{
	EDGE_WEIGHT = 0.5;
	alpha = 0.3;

	oprob = prob.getXPRSprob();

	//Enable subtour elimination
	prob.setCutMode(1); // Enable the cut mode
	//XPRSsetcbcutmgr(oprob, cbmng, &(*this));
	//XPRSsetcbpreintsol(oprob, acceptInt, &(*this));
}

void IRP::useLPSubtourElimination()
{
	EDGE_WEIGHT = 0.5;
	alpha = 0.3;
	LPSubtour = true;
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
					if (edge->getValue() > 0.01) {
						printf("x%d%d: %.2f\t", i, edge->getEndNode()->getId(), edge->getValue());
						printf("loadDel%d%d: %.2f\t", i, edge->getEndNode()->getId(), edge->LoadDel);
						printf("loadPick%d%d: %.2f\t", i, edge->getEndNode()->getId(), edge->LoadPick);
					}
			}
			else {
				printf("Inv_%d: %.2f\t", i, NodeHolder[i]->Nodes[t]->Inventory);

				for (NodeIRP::EdgeIRP *edge : NodeHolder[i]->getEdges(t))
				{
					printf("y%d: %.2f\t", i, NodeHolder[i]->getOutflow(t));
					printf("t%d: %.2f\t", i, NodeHolder[i]->timeServed(t));

					if (Instance.map.isDelivery(i)) {
						printf("qD_%d: %.2f\t", i, NodeHolder[i]->quantity(t));
					}
					else {
						printf("qP_%d: %.2f\t", i, NodeHolder[i]->quantity(t));
					}

					
						j = edge->getEndNode()->getId();
						printf("x%d%d: %.2f\t", i, j, edge->getValue());
						printf("loadDel%d%d: %.2f\t", i, j, edge->LoadDel);
						printf("loadPick%d%d: %.2f\t", i, j, edge->LoadPick);
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
	double TravelCost2 = 0;
	for (NodeIRPHolder * n : NodeHolder) {
		for (Node::Edge * edge : n->getEdges(t)) {
			int i = n->getId();
			int j = edge->getEndNode()->getId();
			double value = edge->getValue();
			TravelCost = TravelCost + Instance.map.getTransCost(i, j)*value;

		
		}
	}

	for(int i : Instance.AllNodes)
		for(int j : Instance.AllNodes)
			if (Instance.map.inArcSet(i, j)) {
				TravelCost2 = TravelCost2 + Instance.x[i][j][t].getSol()* Instance.map.getTransCost(i, j);
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
	//int t = r->period;
	int t = 1;

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

IRP::Route * IRP::Route::copyRoute()
{
	vector<NodeIRP*> path;
	path.resize(route.size());
	auto u = new NodeIRP(route[0]->getId());
	for (int i = 0; i < route.size(); i++) {
			path[i] = u;
			auto id = route[i]->getEdge()->getEndNode()->getId();
			auto v = new NodeIRP(id);
			u->Node::addEdge(v);
			u = v;
		}
	return new Route(path, Instance);
}

void IRP::Route::insertSubroute(vector<NodeIRP *> subroute)
{
	printRoute();
	double minCost = 100000;
	auto nodes = cheapestInsertion(subroute, minCost);
	NodeIRP * start = subroute.front();
	NodeIRP * end = subroute.back();
	//remove edges between u and v
	auto u = nodes.front();
	auto v = nodes.back();
    u->deleteEdges();

	//Add edges
	u->Node::addEdge(start);
	end->Node::addEdge(v);

	//fix route
	route.resize(route.size() + subroute.size());

	//Update the vector containing the route
	u = route.front();
	int position = 1;
	while(u->getId() != 0) {
		u = u->getEdge()->getEndNode();
		route[position] = u;
	}
	printRoute();
}

//Returns nodes to be connected for cheapest removal
vector<IRP::NodeIRP*> IRP::Route::cheapestRemoval(int subroutesize, double &minCost)
{
	vector<NodeIRP * > Nodes;
	Nodes.resize(2);
	//Nodes to store best insertion
	//First and last node in route to insert
	NodeIRP * k;
	NodeIRP * l;
	

	double tempCost = 0;
	NodeIRP *u;
	NodeIRP * v = 0;
	double C_uk;
	double C_lv;
	double C_uv;

	for (int i = 0; i < route.size() - subroutesize; i++) {
		u = route[i];
		k = u->getEdge()->getEndNode();
		l = route[i + subroutesize];
		v = l->getEdge()->getEndNode();
		C_uk = Instance.getMap()->getTransCost(u->getId(), k->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);
		C_lv = Instance.getMap()->getTransCost(l->getId(), v->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);
		C_uv = Instance.getMap()->getTransCost(u->getId(), v->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);

		tempCost = C_uv - C_uk - C_lv;

		if (tempCost < minCost) {
			minCost = tempCost;
			Nodes[0] = u;
			Nodes[1] = v;
		}
		u = v;
	}

	return Nodes;
}

//Returns empty vector if it doesn't beat the minCost
vector<IRP::NodeIRP*> IRP::Route::cheapestInsertion(vector<NodeIRP*> subroute, double &minCost)
{
	vector<NodeIRP * > Nodes;
	Nodes.resize(2);
	//Nodes to store best insertion
	//First and last node in route to insert
	NodeIRP * k;
	NodeIRP * l;
	k = subroute.front();
	l = subroute.back();

	double tempCost = 0;
	NodeIRP * v = 0;
	double C_uk;
	double C_lv;
	double C_uv;

	for (auto u : route) {
		v = u->getEdge()->getEndNode();
		C_uk = Instance.getMap()->getTransCost(u->getId(), k->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);
		C_lv = Instance.getMap()->getTransCost(l->getId(), v->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);
		C_uv = Instance.getMap()->getTransCost(u->getId(), v->getId(), ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);

		tempCost = C_uk + C_lv - C_uv;

		if (tempCost < minCost) {

			minCost = tempCost;
			Nodes[0] = u;
			Nodes[1] = v;
		}
		u = v;
	}

	return Nodes;
}


//Uses cheapest insertion heurestic for all routes in the period
void IRP::Solution::insertSubrouteInRoute(vector<NodeIRP*> subroute, int period)
{
	IRP::Route * path = 0;
	double minCost = 100000;
	double tempCost;
	//Nodes to store best insertion
	NodeIRP *start = 0;
	NodeIRP *end= 0;

	NodeIRP * u = 0;
	NodeIRP * v = 0;
	NodeIRP * k = subroute.front();
	NodeIRP * l = subroute.back();

	IRP::Route * route = 0;
	vector<NodeIRP * > Nodes;
	//Go through all routes. Cheapest insertion

	for (auto R : getRoutes(period)) {
		if (!R->coincide(subroute)) {
			tempCost = minCost;
			Nodes = R->cheapestInsertion(subroute, tempCost);

			//If new cheapest insertion
				if (tempCost < minCost) {
					u = Nodes.front();
					v = Nodes.back();
					minCost = tempCost;
					start = NodeHolder[u->getId()]->Nodes[period];
					end = NodeHolder[v->getId()]->Nodes[period];
				}
		
		}

	}

	//Insert edges
	start->Node::deleteEdge(end);
	start->Node::addEdge(k);
	l->Node::addEdge(end);

}

void IRP::Solution::createSubroute(vector<NodeIRP*> nodes)
{
	//Create route
	for (int u = 0; u < nodes.size() - 1; u++) {
		nodes[u]->deleteEdges();
		nodes[u]->Node::addEdge(nodes[u + 1]);
	}
}



void IRP::Route::setId(int id)
{
	Id = id;
}

double IRP::Route::getTransportationCost()
{
	double transCost = 0;
	int i;
	int j;

	for (auto node : route)
	{
		i = node->getId();
		if (i == 0)
			j = route[1]->getId();
		else
			j = node->getEdge()->getEndNode()->getId();

		transCost += Instance.map.getTransCost(i, j);
	}
	return transCost;
}

int IRP::Route::getPeriod()
{
	return Period;
}

bool IRP::Route::isFeasible()
{
	double totalTime = 0;
	int u;
	int v;

	for (auto node : route) {
		u = node->getId();
		v = node->getEdge()->getEndNode()->getId();
		totalTime += Instance.map.getTravelTime(u, v, ModelParameters::TRAVELTIME_MULTIPLIER, ModelParameters::SERVICETIME);
	}
	
	return totalTime <= ModelParameters::maxTime;
}

IRP::Route * IRP::Route::generateRoute(Route * r)
{
	auto newroute = copyRoute();
	newroute->mergeRoute(r);

	//newroute->printPlot("Routes/afterMerge" + to_string(rand()%100));

	//Remove random size of the new route
	int averge = (int)round((route.size() + r->route.size()) / 2);
	int averageDifference = newroute->route.size() - (int)round((route.size() + r->route.size()) / 2);

	int max = min(averageDifference + 2, newroute->route.size() - 2);
	int min = max(averageDifference - 2, 0);

	int remove = rand() % (max - min + 1) + min;
	newroute->removeSubroute(remove);

	//Ensure route is time feasible
	while (!newroute->isFeasible())
		newroute->removeSubroute(1);

	return newroute;
}

void IRP::Route::setPeriod(int period)
{
	Period = period;
}

bool IRP::Route::coincide(vector<NodeIRP*> r)
{
	for (auto node : r) {
		for (auto thisNode : route)
			if (node->getId() == thisNode->getId())
				return true;
	}
	return false;
}

void IRP::Route::createSeperateRoute(Route * r)
{
	auto prev = r->route[0];
	for (auto node : r->route) {
		if (node->getId() != 0) {

			if (inRoute(node)) {
				//Remove edges
				prev->deleteEdges();
				prev->Node::addEdge(node->getEdge()->getEndNode());
			}
			else
				prev = node;
		}
		
	}

	r->updateRoute();
}

bool IRP::Route::inRoute(Node * n)
{
	for (auto node : route)
		if (node->getId() == n->getId())
			return true;

	return false;
}

int IRP::Route::getTotalDelivery()
{
	double totalDelivery = 0;
	for (auto node : route)
		if (Instance.getMap()->isDelivery(node->getId()))
			totalDelivery += node->Quantity;

	return totalDelivery;
}

int IRP::Route::getTotalPickup()
{
	return 0;
}

void IRP::Route::mergeRoute(IRP::Route * mergeIn)
{
	IRP::Route * copy = mergeIn->copyRoute();
	createSeperateRoute(copy);

	if (copy->route.size() >= 2) {
		//Remove depot
		vector<NodeIRP*>nodes;
		for (auto node : copy->route) {
			if (node->getId() != 0)
				nodes.push_back(node);
		}

		double minCost = 100000;
		vector<NodeIRP*> holder = cheapestInsertion(nodes, minCost);
		insert(holder.front(), holder.back(), nodes);
	}
}

void IRP::Route::insert(NodeIRP * start, NodeIRP * end, vector<NodeIRP*> subroute)
{
	auto k = subroute.front();
	auto l = subroute.back();
	//Insert edges
	start->Node::deleteEdge(end);
	start->Node::addEdge(k);
	l->Node::deleteEdges();
	l->Node::addEdge(end);

	//route[0]->deleteEdges();
	updateRoute();
	//Update route positions
}



int IRP::Route::getPosition(Node * node)
{
	auto nodeSearch = route.front();
	int position = 0;
	do {
		if (nodeSearch->getId() == node->getId())
			return position;

		nodeSearch = nodeSearch->getEdge()->getEndNode();
		position ++;
	} while (nodeSearch->getId() != 0);
}

int IRP::Route::getId()
{
	return Id;
}

//Returns a Aij matrix, 1 if arc ij is traversed, 0 otherwise
int ** IRP::Route::getRouteMatrix()
{

	int ** routeMat = new int*[Instance.AllNodes.size()];
	for (int i = 0; i < Instance.AllNodes.size(); i++) {
		routeMat[i] = new int[Instance.AllNodes.size()];
		for (int j = 0; j < Instance.AllNodes.size(); j++) {
			routeMat[i][j] = 0;
		}
	}

	for (Node *node : this->route) {
		int i = node->getId();
		int j;
		if (i == 0)
			j = this -> route[1]->getId();
		else
			j = node->getEdge()->getEndNode()->getId();

		routeMat[i][j] = 1;
	}

	//print
	/*for (int i = 0; i < Instance.AllNodes.size(); i++) {
		cout << "\n";
		for (int j = 0; j < Instance.AllNodes.size(); j++) {
			cout << routeMat[i][j] << "\t";
		}
	}
	cout << "\n\n";*/

	return routeMat;
}

void IRP::Route::printPlot(string filename)
{
	/*auto customer =  Instance.map.getCustomer(6);
	auto delnode = new Node(customer->getId());
	auto pickNode = new Node(Instance.map.getPickupNode(customer));
	delnode->addEdge(pickNode);
	
	*/vector<Node*> graph;

	//graph.push_back(delnode);
	//graph.push_back(pickNode);*/

	for (Node* node : route)
	{
		graph.push_back(node);
	}

	graphAlgorithm::printGraph(graph, Instance, filename, ModelParameters::X);
}

void IRP::Route::printRoute()
{
	cout << "\n";
	for (auto node : route)
		cout<<(node->getId())<<" - " ;

	cout << "0 \n";
}

double IRP::Route::getResidualCapacity()
{
	double load = 0;
	
	double maxLoad = 0;
	for (auto node : route) {
		auto edge = node->getEdge();
		load = node->getEdge()->LoadDel + node->getEdge()->LoadPick;
		if (load > maxLoad) {
			maxLoad = load;
		}
	}
	return  Instance.Capacity - maxLoad;
}

void IRP::Route::removeSubroute(int size)
{
	vector<NodeIRP*> Nodes = getSubroute(size);

	if (route.size() - Nodes.size() >= 2) {

		//Delete edges and add edges
			
	Nodes.front()->deleteEdges();
	Nodes.front()->Node::addEdge(Nodes.back());

	updateRoute();
		
	}
}

void IRP::Route::updateRoute()
{
	route.resize(1);
	auto u = route.front()->getEdge()->getEndNode();
	auto t = route.back();
	int position = 1;
	while (u->getId() != 0) {
		route.push_back(u);
		position++;
		u = u->getEdge()->getEndNode();
	}

}



vector<IRP::NodeIRP*> IRP::Route::getSubroute(int size)
{
	double minCost = 10000;
	vector<IRP::NodeIRP*> nodes = cheapestRemoval(size, minCost);
	return nodes;
}




int IRP::Route::counter = 1;

IRP::Route::Route(vector<NodeIRP*>& path, IRP & Instance)
	:
	Instance(Instance),
	route(path),
	Id(counter++)
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
		while (ModelParameters::WITHIN_LIMITS !=  isInventoryFeasible()) {
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
			cout<< inventory(t) - Instance.map.getUpperLimit(getId());
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

//Propagate the inventory with the quantity. Positive if more is delivered, negative if more is picked up
void IRP::NodeIRPHolder::propInvForw(int period, double quantity)
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

	if(isDelivery())
		propInvForw(period, quantity);
	else
		propInvForw(period, -quantity);

}


bool IRP::NodeIRPHolder::isDelivery()
{
	return Instance.getMap()->isDelivery(getId());
}

double IRP::NodeIRPHolder::getFeasibleServiceIncrease(int period)
{
	// Check if quantity is feasible, if not adjust it
	return Instance.Capacity - quantity(period);
}

double IRP::NodeIRPHolder::getFeasibleServiceDecrease(int period)
{
	// Check if quantity is feasible, if not adjust it
	return quantity(period);
}

double IRP::NodeIRPHolder::getFeasibleServiceMove(int from, int to)
{
	return min(getFeasibleServiceDecrease(from), getFeasibleServiceIncrease(to));
}


IRP::LocalSearch::LocalSearch(IRP & model, IRP::Solution * origSol)
	:
	Instance(model),
	OrigSol(origSol)
{
}

void IRP::LocalSearch::ShiftQuantity(IRP::Solution * sol)
{
	
	double shiftDel;
	double shiftPick;
	double pushDel;
	double pushPick;


	//Shift from period with the greatest transportation costs
	//initialize with the current solution
	IRP::Solution * newSol = sol;
	//Clear all nodes edges from the nodes.

	int period = ChoosePeriod(ModelParameters::HIGHEST_TRANSPORTATIONCOST);
	double a = OrigSol->getTotalDelivery(period);
	double c = OrigSol->getTotalPickup(period);
	double b = Instance.Capacity;
	shiftDel = max(OrigSol->getTotalDelivery(period) - Instance.Capacity*(OrigSol->getNumberOfRoutes(period) - 1), 0) + floor(0.1*OrigSol->getTotalDelivery(period));
	shiftPick = max(OrigSol->getTotalPickup(period) - Instance.Capacity*(OrigSol->getNumberOfRoutes(period) - 1), 0) + floor(0.1*OrigSol->getTotalDelivery(period));

	for (auto node : newSol->NodeHolder) {
		//Move quantity for visited nodes
		if (node->quantity(period) > 0.01)

			//Delivery move
			if (node->isDelivery()) {
				if (shiftDel > 0) {
					//Decide how much quantity to move and update quantity
					pushDel = shiftDel;
					

					//Try to shift forward
					if (period <= Instance.getNumOfPeriods()) {
						//Check if node is visited in next period
						if (node->quantity(period + 1) > 0.01) {

							pushDel = shiftDel;
							pushDel = node-> moveQuantity(period, period + 1, pushDel);
							shiftDel -= pushDel;
						}
					}

					//Try to shift forward two period
					if (period <= Instance.getNumOfPeriods()+1) {
						if (node->quantity(period + 2) > 0.01) {
							pushDel = shiftDel;
							pushDel = node->moveQuantity(period, period + 2, pushDel);
							shiftDel -= pushDel;
						}
					}

					//Try to shift backwards
					if (period - 1 > 0) {
						if (node->quantity(period - 1) > 0.01) {
							pushDel = shiftDel;
							pushDel = node->moveQuantity(period, period - 1, pushDel);
							shiftDel -= pushDel;
						}
					}
				
			
				} //end shift del
			}

		//Pickup node
			else if (shiftPick > 0) {
				//Decide how much quantity to move and update quantity
				pushPick = shiftPick;

				//Try to shift forward, only if visited in next period
				if (period <= Instance.getNumOfPeriods()) {
					if (node->quantity(period + 1) > 0.01) {
						pushPick = shiftPick;
						pushPick = node->moveQuantity(period, period + 1, pushPick);
						shiftPick -= pushPick;
					}
				}

				//Try to shift forward two period
				if (period <= Instance.getNumOfPeriods()+1) {
					if (node->quantity(period + 2) > 0.01) {
						pushPick = shiftPick;
						pushPick = node->moveQuantity(period, period + 2, pushPick);
						shiftPick -= pushPick;
					}
				}

				//Try to shift backwards
				if (period - 1 > 0 && (shiftPick > 0)) {
					if (node->quantity(period - 1) > 0.01) {
						pushPick = shiftPick;
						pushPick = node->moveQuantity(period, period - 1, pushPick);
						shiftPick -= pushPick;
					}
				}

			} //end pickup nodes
	}
	cout << Instance.Capacity;
}




int IRP::LocalSearch::ChoosePeriod(int selection)
{
	double maxValue = -100;
	int period = -1;

	if (selection == ModelParameters::HIGHEST_TRANSPORTATIONCOST) {	
		for (auto t : Instance.Periods) {
			if (maxValue <= OrigSol->getTransportationCost(t)) {
				maxValue = OrigSol->getTransportationCost(t);
				period = t;
			}
		}

		return period;
	}

	if (selection == ModelParameters::HIGHEST_RESIDUAL_CAPACITY) {
		for (auto t : Instance.Periods) {
			if (maxValue <= OrigSol->getResidualCapacity(t)) {
				maxValue = OrigSol->getResidualCapacity(t);
				period = t;
			}
		}

		return period;
	}

}

IRP::RouteProblem::RouteProblem(IRP & model, vector <IRP::Route *> r)
	:
	Instance(model),
	routes(r),
	routeProblem("RouteProblem")
{
	initializeRouteVariables();
}

/*IRP::RouteProblemWithVRP::RouteProblemWithVRP(IRP & Instance, int t, vector<IRP::Route*> routes)
	:
	RouteProblem(Instance, routes)
{
	VRPperiod = t;
}

void IRP::RouteProblemWithVRP::initializeVaribles()
{
	//for(auto i : Periods)
}*/
