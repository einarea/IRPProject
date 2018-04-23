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


Solution * IRP::getSolution(int id)
{
	Solution * sol = solutions[id - 1];
	return solutions[id - 1];
}

/*void IRP::printBounds()
{
	double *lbd;
	double *upd;
	for (int i : Nodes) {
		for (int t : Periods)
			if (y[i][t].getUB() <= 0.5)
				cout << "DD";
	}
}*/

IRP::IRP(NodeInstanceDB& db, bool ArcRel, bool maskOn, int ** VisitMask)
	:
	Database(db),
	prob("IRP"),			//Initialize problem in BCL		
	ARC_RELAXED(ArcRel),
	SolutionCounter(0),
	MaskOn(maskOn),
	LPSubtour(false)
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



Solution * IRP::solveModel()
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
				graphAlgorithm::printGraph(graph, Database, "graphSolutions/graph", ModelParameters::X);
				graphAlgorithm::sepByStrongComp(graph, result);
				graphAlgorithm::printGraph(graph, Database, "graphSolutions/graph", ModelParameters::X);
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

Solution * IRP::solveLPModel()
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
				graphAlgorithm::printGraph(graph, Database, "graphSolutions/graph", ModelParameters::X);
				graphAlgorithm::sepByStrongComp(graph, result);
				graphAlgorithm::printGraph(graph, Database, "graphSolutions/graph", ModelParameters::X);
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

Solution * IRP::allocateIRPSolution()
{
	NodeIRPHolder * d;
	Node * u = d->getNode(3);
	//Allocate x and loading solutions
	Solution *sol = allocateSolution(Database);
	u = sol->getDepot(3);

	fillSolution(sol);
	return sol;
}

//Fills a solution based on problem solution values
void IRP::fillSolution(Solution * sol)
{
	//Fill Inventory
	for (auto node : Database.NodeData) {
		int i = node->getId();
		for (int t : Periods) {
			sol->Nodes[i]->NodePeriods[t]->Inventory = inventory[i][t].getSol();
		}
	}

	//Fill Quantity and allocate time
	for (auto node: Database.NodeData){ 
		for (int t : Periods) {
			int i = node->getId();
			if (node->isDelivery()) {
				if (delivery[i][t].getSol() > 0) {
					sol->Nodes[i]->NodePeriods[t]->Quantity = delivery[i][t].getSol();
					sol->Nodes[i]->NodePeriods[t]->TimeServed = time[i][t].getSol();
				}
				else
					sol->Nodes[i]->NodePeriods[t]->Quantity = 0;
			}
			else {
				if (pickup[i][t].getSol() > 0) {
					sol->Nodes[i]->NodePeriods[t]->Quantity = pickup[i][t].getSol();
					sol->Nodes[i]->NodePeriods[t]->TimeServed = time[i][t].getSol();
				}
				else
					sol->Nodes[i]->NodePeriods[t]->Quantity = 0;
			}
		}
	}

	//Fill load
	double value;
	int i;
	int j;

	for (auto node1 : Database.NodeData) {
		for (auto node2 : Database.NodeData) {
			if (Database.inArcSet(node1, node1)) {
				i = node1->getId();
				j = node2->getId();
				for (int t : Periods) {
					if (x[i][j][t].getSol() >= 0.0001) {
						if (ModelParameters::Simultaneous) {
							if (Database.isColocated(i, j) && simAction[i][j][t].getSol() <= 0.01) {
								value = 0;
							}
							else
								value = x[i][j][t].getSol();
						}
						else
							value = x[i][j][t].getSol();

						sol->Nodes[i]->NodePeriods[t]->addEdge(loadDelivery[i][j][t].getSol(),
							loadPickup[i][j][t].getSol(), sol->Nodes[j]->NodePeriods[t], value);
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
			if (Database.inArcSet(s, endingNode->getId())) {
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
					arcIndicator = (Database.inSimultaneousArcSet(i, j) && !Database.isColocated(i, j));
				else
					arcIndicator = Database.inExtensiveArcSet(i, j);

				for (int t : Periods) {
					if (arcIndicator)
						objective += TransCost[i][j] * x[i][j][t];
				}
			}

	if (ModelParameters::Simultaneous) {
		for (int i : DeliveryNodes)
			for (int j : PickupNodes)
				if (Database.isColocated(i, j))
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
				if(Database.isColocated(i, j)) {
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

				if (Database.inArcSet(i, j)) {
					p1 += x[i][j][t];
				}

				if (Database.inArcSet(j, i)){
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
				if (Database.inArcSet(i, j)) {
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
				if (Database.inArcSet(i, j)) {
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

			if (Database.inArcSet(i, 0)) {
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
			if (Database.inArcSet(i, j)) {
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



XPRBprob * IRP::getProblem() {
	return &prob;
}

NodeInstanceDB * IRP::getDB()
{
	return &Database;
}


bool IRP::initializeParameters() {
	TransCost = new int *[AllNodes.size()];
	TravelTime = new int *[AllNodes.size()];

	for (int i : AllNodes) {
		//printf("\n");
		TransCost[i] = new  int[AllNodes.size()];
		TravelTime[i] = new  int[AllNodes.size()];
		for (int j : AllNodes) {
			if (Database.inArcSet(i, j)) {
				TravelTime[i][j] = Database.getTravelTime(i, j);
				TransCost[i][j] = Database.getTransCost(i, j);
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
	int i;
	for (auto node : Database.NodeData) {
		i = node->getId();
		HoldCost[i] = node->HoldingCost;
		
	} //end initialization HoldCost

	InitInventory = new int[Nodes.size()];

	for (auto node : Database.NodeData) {
		InitInventory[node->getId()] = node->InitInventory;
		//printf("%-5d", InitInventory[i]);
	} //end initialization initial inventory


	UpperLimit = new int[Nodes.size()];
	LowerLimit = new int[Nodes.size()];
	for (auto node : Database.NodeData) {	
		UpperLimit[i] = node->UpperLimit;
		LowerLimit[i] = node->LowerLimit;
	} //end limit initialization

	//printf("\n");
	//printf("Demand Delivery");

	Demand = new int * [Nodes.size()+1];
	int customer;

	for (auto node : Database.DeliveryNodes) {
		//printf("\n");
		Demand[node->getId()] = new int [Periods.size()+1];
		for (int t : Periods) {
			if (t > 0) {
				Demand[i][t] = node->Demand[t];
				//printf("%-10d", Demand[i][t]);
			}
		}
	} //end demand delivery nodes

	//printf("\n");
	//printf("Pickup Delivery");

	for (auto node : Database.PickupNodes) {
		//printf("\n");
		Demand[node->getId()] = new int[Periods.size()];
		for (int t : Periods) {
			if (t > 0) {
				Demand[i][t] = node->Demand[t];
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
	Capacity = 2 * floor(TotalDemand / (ModelParameters::nVehicles*getNumOfPeriods()));

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
			if(Database.isDelivery(i))
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
						if (Database.inArcSet(i, j)) {
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
				if (Database.inArcSet(j, i)) {
					p1 += loadDelivery[j][i][t];
					p2 -= loadPickup[j][i][t];
				}
				if (Database.inArcSet(i, j)) {
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
				if (Database.inArcSet(j, i)) {
					p1 += loadPickup[j][i][t];
					p2 += loadDelivery[j][i][t];
				}
				if (Database.inArcSet(i, j)) {
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





bool IRP::initializeSets()
{
	NumOfCustomers = Database.Nodes.size();				//Number of customers
	NumOfPeriods = Database.getnPeriods();

	

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

			if (Database.inArcSet(i, j)) {
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
					if (Database.isColocated(i, j)) {
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
/*void IRP::getVisitedCustomers(int period, vector<Customer *> &custVisit)
{
	if (ARC_RELAXED == true) {
		for (int i : DeliveryNodes) {
			if (y[i][period].getSol() > 0.01 || y[i+getNumOfCustomers()][period].getSol() > 0.01)
				custVisit.push_back(Database.getCustomer(i));
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
			demand[Customer::DELIVERY][c->getId()] = round(delivery[Database.getDeliveryNode(c)][t].getSol());
			demand[Customer::PICKUP][c->getId()] = round(pickup[map.getPickupNode(c)][t].getSol());
	}
}*/



int IRP::getNumOfPeriods()
{
	return Periods.size();
}


IRP::~IRP()
{
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
		int i;
		for (auto node: Database.DeliveryNodes) {
			i = node->getId();
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

						prob.newCtr("MinInventory", p1 >= p2*p3 + node->LowerLimit);
						p1 = 0;
						p2 = 0;
						p3 = 0;
					}
				}
			}
		} // End delivery nodes
		int j;
		for (auto node : Database.PickupNodes) {
			j = node->getId();
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

						prob.newCtr("MinInventory", p1 <= node->UpperLimit - p2*p3);
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
	for (auto node1 : Database.NodeData)
		for (auto node2 : Database.NodeData)
			if (Database.inArcSet(node1, node2)) {
				for (int t : Periods)
					p1 += Database.getTransCost(*node1, *node2) * x[node1->getId()][node2->getId()][t];
			}

	prob.setObj(prob.newCtr("OBJ", p1));
	prob.print();
}










Solution * IRP ::allocateSolution(NodeInstanceDB &Instance)
{
	SolutionCounter++;
	Solution * sol = new Solution(Instance, true);
	sol->SolID = SolutionCounter;
	return sol;
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



/*void Solution::buildGraph(vector<Node*>& graph, int t)
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




IRP::LocalSearch::LocalSearch(IRP & model, Solution * origSol)
	:
	Instance(model),
	OrigSol(origSol)
{
}

void IRP::LocalSearch::ShiftQuantity(Solution * sol)
{
	
	double shiftDel;
	double shiftPick;
	double pushDel;
	double pushPick;


	//Shift from period with the greatest transportation costs
	//initialize with the current solution
	Solution * newSol = sol;
	//Clear all nodes edges from the nodes.

	int period = ChoosePeriod(ModelParameters::HIGHEST_TRANSPORTATIONCOST);
	double a = OrigSol->getTotalDelivery(period);
	double c = OrigSol->getTotalPickup(period);
	double b = Instance.Capacity;
	shiftDel = max(OrigSol->getTotalDelivery(period) - Instance.Capacity*(OrigSol->getNumberOfRoutes(period) - 1), 0) + floor(0.1*OrigSol->getTotalDelivery(period));
	shiftPick = max(OrigSol->getTotalPickup(period) - Instance.Capacity*(OrigSol->getNumberOfRoutes(period) - 1), 0) + floor(0.1*OrigSol->getTotalDelivery(period));

	for (auto node : newSol->Nodes) {
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
