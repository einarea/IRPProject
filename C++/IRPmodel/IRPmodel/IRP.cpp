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

void XPRS_CC preNode(XPRSprob oprob, void * vd, int * ifreject)
{
}
void  XPRS_CC integerSolutionIRP(XPRSprob oprob, void * vd, int soltype, int * ifreject, double *cutoff) {
	IRP * modelInstance;
	modelInstance = (IRP*)vd;
	modelInstance->lastSolutionFound = time(NULL);
}

void XPRS_CC acceptInt(XPRSprob oprob, void * vd, int soltype, int * ifreject, double *cutoff) {
	IRP * modelInstance;
	modelInstance = (IRP*)vd;
	vector<XPRBcut> subtourCut;


	modelInstance->getProblem()->beginCB(oprob);
	modelInstance->getProblem()->sync(XPRB_XPRS_SOL);
	auto bprob = modelInstance->getProblem();
	bool addedCuts = modelInstance->sepStrongComponents(subtourCut);
	if (addedCuts) {
		*ifreject = 1;

	}
	modelInstance->getProblem()->endCB();

	if (!addedCuts)
		modelInstance->lastSolutionFound = time(NULL);
}

void XPRS_CC acceptIntQuantity(XPRSprob oprob, void * vd, int soltype, int * ifreject, double *cutoff) {

	
	IRP * modelInstance;
	modelInstance = (IRP*)vd;
	vector<XPRBcut> subtourCut;
	int i;
	bool subtours = false;


	modelInstance->getProblem()->beginCB(oprob);
	modelInstance->getProblem()->sync(XPRB_XPRS_SOL);
	if (modelInstance->SubtourElimination) {
		subtours = modelInstance->sepStrongComponents(subtourCut);
	}

	for (auto node : modelInstance->getDB()->Nodes) {
		i = node->getId();
		for (auto t : modelInstance->getDB()->Periods) {
			if (abs(modelInstance->inventory[i][t].getSol() - round(modelInstance->inventory[i][t].getSol())) > 0.001 || subtours)
				{
				*ifreject = 1;
				break;
				}
			else
				modelInstance->lastSolutionFound = time(NULL);
		}
	}

	if (subtours) {
		//Load model instance
		XPRBprob *bprob = modelInstance->getProblem();

		//Columns number
		int * mindo = nullptr;
		//Matrix values
		double * dvalo = nullptr;
		vector<int> subtour;
		vector<int> rowValues;

		for (int i = 0; i < subtourCut.size(); i++) {
			rowValues.clear();
			subtour.clear();
			//	cout << "\n";
			subtour = modelInstance->subtourIndices[i];
			mindo = (int*)malloc(subtour.size()*sizeof(int));

			//	cout << "Sub: ";
			for (int s = 0; s < subtour.size(); s++) {
				mindo[s] = subtour[s];
				//		cout <<subtour[s] << "\t";
			}
			//	cout << "\n";
			//	cout << "MatValues: ";
			rowValues = modelInstance->matrixValues[i];
			dvalo = (double*)malloc(rowValues.size()*sizeof(double));
			for (int j = 0; j < rowValues.size(); j++) {
				dvalo[j] = rowValues[j];
				//cout << rowValues[j]<< "\t";
			}

			//cout << "\n";

			char qrtype = 'L';
			double drhso = 0;
			int nzp, status, mtype = 1, mstart[2], *mindp, ncols;
			int nzo = subtour.size();
			double drhsp, *dvalp;
			XPRScut mindex;

			XPRSgetintattrib(oprob, XPRS_COLS, &ncols);

			mindp = (int*)malloc(ncols*sizeof(int));
			dvalp = (double*)malloc(ncols*sizeof(double));
			XPRSpresolverow(oprob, qrtype, nzo, mindo, dvalo, drhso, ncols,
				&nzp, mindp, dvalp, &drhsp, &status);

			/*cout << "Presolve var: \n";
			for (int a = 0; a < nzp; a++) {
			cout << mindp[a] << "\t";
			}
			cout << "\tCut End\n"; */

			if (status >= 0) {
				int nCuts = 1;
				mstart[0] = 0; mstart[1] = nzp;
				XPRSstorecuts(oprob, nCuts, 1, &mtype, &qrtype, &drhsp, mstart, &mindex, mindp, dvalp);
				modelInstance->nSubtourCuts++;

			}
			XPRSloadcuts(oprob, 1, -1, -1, NULL);
			//cout << "Stored\n";
		}
	
	}
	modelInstance->getProblem()->endCB();
}

//Time callback
void XPRS_CC cbmngtimeIRP(XPRSprob oprob, void * vd,int parent, int newnode, int branch) {

	IRP * modelInstance;
	modelInstance = (IRP*)vd;
	//cout << (int)floor((XPRB::getTime() - modelInstance->startTime) / 1000) << "\n";

	//cout << difftime(time(NULL), modelInstance->startTime) << "\n";
	if (difftime(time(NULL), modelInstance->startTime) >= ModelParameters::MAX_RUNNING_TIME_IRP)
	{
		XPRSinterrupt(oprob, XPRS_STOP_TIMELIMIT);
	}
}



//Global callBack manager

int XPRS_CC cbmng(XPRSprob oprob, void * vd)
{
	
	// subtour callback
	//cout << "cb\n";
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
	//For printing

	//Load model instance
	XPRBprob *bprob = modelInstance->getProblem();

	//Columns number
	int * mindo = nullptr;
	//Matrix values
	double * dvalo = nullptr;
	vector<int> subtour;
	vector<int> rowValues;

	bool addedCuts = modelInstance->sepStrongComponents(subtourCut);

	if (addedCuts) {
		if (modelInstance->CutType == ModelParameters::GLOBAL_CUTS) {
			for (int i = 0; i < subtourCut.size(); i++) {
				rowValues.clear();
				subtour.clear();
			//	cout << "\n";
				subtour = modelInstance->subtourIndices[i];
				mindo = (int*)malloc(subtour.size()*sizeof(int));

			//	cout << "Sub: ";
				for (int s = 0; s < subtour.size(); s++) {
					mindo[s] = subtour[s];
			//		cout <<subtour[s] << "\t";
				}
			//	cout << "\n";
			//	cout << "MatValues: ";
				rowValues = modelInstance->matrixValues[i];
				dvalo = (double*)malloc(rowValues.size()*sizeof(double));
				for (int j = 0; j < rowValues.size(); j++) {
					dvalo[j] = rowValues[j];
					//cout << rowValues[j]<< "\t";
				}

				//cout << "\n";
				
				char qrtype = 'L';
				double drhso = 0;
				int nzp, status, mtype = 1, mstart[2], *mindp, ncols;
				int nzo = subtour.size();
				double drhsp, *dvalp;
				XPRScut mindex;

				XPRSgetintattrib(oprob, XPRS_COLS, &ncols);

				mindp = (int*)malloc(ncols*sizeof(int));
				dvalp = (double*)malloc(ncols*sizeof(double));
				XPRSpresolverow(oprob, qrtype, nzo, mindo, dvalo, drhso, ncols,
					&nzp, mindp, dvalp, &drhsp, &status);

				/*cout << "Presolve var: \n";
				for (int a = 0; a < nzp; a++) {
					cout << mindp[a] << "\t";
				}
				cout << "\tCut End\n"; */

				if (status >= 0) {
					int nCuts = 1;
					mstart[0] = 0; mstart[1] = nzp;
					XPRSstorecuts(oprob, nCuts, 1, &mtype, &qrtype, &drhsp, mstart, &mindex, mindp, dvalp);
					modelInstance->nSubtourCuts++;
				
				}
				XPRSloadcuts(oprob, 1, -1, -1, NULL);
				//cout << "Stored\n";
			}
		}
		else if (modelInstance->CutType == ModelParameters::LOCAL_CUTS) {
			//modelInstance->getProblem()->setCutMode(1); // Enable the cut mode
			for (int i = 0; i < subtourCut.size(); i++) {
				subtour = modelInstance->subtourIndices[i];
				mindo = (int*)malloc(subtour.size()*sizeof(int));

				for (int s = 0; s < subtour.size(); s++) {
					mindo[s] = subtour[s];
				}

				rowValues = modelInstance->matrixValues[i];
				dvalo = (double*)malloc(rowValues.size()*sizeof(double));
				for (int j = 0; j < rowValues.size(); j++)
					dvalo[j] = rowValues[j];


				char qrtype = 'L';
				double drhso = 0;
				int nzp, status, mtype = 1, mstart[2], *mindp, ncols;
				int nzo = subtour.size();
				double drhsp, *dvalp;
				XPRScut mindex;

				XPRSgetintattrib(oprob, XPRS_COLS, &ncols);

				mindp = (int*)malloc(ncols*sizeof(int));
				dvalp = (double*)malloc(ncols*sizeof(double));
				XPRSpresolverow(oprob, qrtype, nzo, mindo, dvalo, drhso, ncols,
					&nzp, mindp, dvalp, &drhsp, &status);

				for (int a = 0; a < nzp; a++) {
					cout << mindp[a] << "\t";
				}
				cout << "\tCut End\n";

				if (status >= 0) {
					int nCuts = 1;
					mstart[0] = 0; mstart[1] = nzp;
					XPRSaddcuts(oprob, nCuts, &mtype, &qrtype, &drhsp, mstart, mindp, dvalp);
				}
			}

			/*
			for (XPRBcut cut : subtourCut)
			{

				modelInstance->nSubtourCuts += 1;
				cutId = modelInstance->nSubtourCuts;
				cut.print();
				bprob->addCuts(&cut, 1);
			}*/
			//modelInstance->getProblem()->setCutMode(0);
		}
	} //end add cut


	/*if(addedCuts){
	XPRSgetintattrib(oprob, XPRS_NODES, &node);
	XPRSgetdblattrib(oprob, XPRS_LPOBJVAL, &objval); */
	
	
	//Unload LP relaxation
	modelInstance->getProblem()->endCB(); 

	return 0;
	
}


Solution * IRP::getSolution(int id)
{
	Solution * sol = solutions[id - 1];
	return solutions[id - 1];
}

NodeInstanceDB const * IRP::getDB()
{
	return &Database;
}

/*void IRP::printBounds()
{
	double *lbd;
	double *upd;
	for (int i : Nodes) {
		for (int t : Database.Periods)
			if (y[i][t].getUB() <= 0.5)
				cout << "DD";
	}
}*/

IRP::IRP(const NodeInstanceDB& db, bool ArcRel, bool maskOn, int ** VisitMask)
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

	int i, j;
	//initialize tabu matrix and countMatrix
	TabuMatrix.resize(Database.AllNodes.size());
	CountMatrix = new double *[getNumOfNodes() + 1];
	for (auto node : Database.Nodes) {
		i = node->getId();
		TabuMatrix[i].resize(Database.AllPeriods.size());
		CountMatrix[i] = new double[getNumOfPeriods() + 1];
		for (auto t : Database.Periods) {
			//TabuMatrix[i][t] = 0;
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
			prob.print();
			prob.mipOptimise();
			//Check graph for subtours in strong components
			vector <vector<Node*>> result; //matrix to store strong components
			vector <NodeStrong*> graph;		//Graph to store nodes

			for (int t : Database.Periods) {
				graph.clear();
				result.clear();
				subtourIndices.clear();
				matrixValues.clear();
				buildStrongGraph(graph, t, false); //Do not include depot in graph		
				//graphAlgorithm::printGraph(graph, "graphSolutions/graph", ModelParameters::X);
				graphAlgorithm::sepByStrongComp(graph, result);
				//graphAlgorithm::printGraph(graph, "graphSolutions/graph", ModelParameters::X);
				temp = addSubtourCtr(result, t);
				if (temp)
					isSubtours = true;
			}

		} while (isSubtours);

	
		return allocateIRPSolution();
	}

	else

	oprob = prob.getXPRSprob();
	startTime = time(NULL);
	
	//Set time callback
	XPRSsetcbnewnode(oprob, cbmngtimeIRP, &(*this));

	//
	//XPRSsetcboptnode(oprob, cbOptNode, )

	if (ARC_RELAXED) {

		XPRSsetcbpreintsol(oprob, acceptIntQuantity, &(*this));
	
	}
	else
		XPRSsetcbpreintsol(oprob, integerSolutionIRP, &(*this));
	//XPRSsetcbpreintsol(oprob, acceptInt, &(*this));
	//prob.print();
	//XPRSsetcbcutmgr(oprob, cbmng, &(*this));
	//prob.print();
	prob.mipOptimise();
	vector<XPRBcut> ass;


	solutionTime = difftime(time(NULL), startTime);
	//vector<XPRBcut> ass;
	//bool a = sepStrongComponents(ass);
	//prob.print();
	//For printing

	XPRSgetdblattrib(oprob, XPRS_BESTBOUND, &bestBound);
	XPRSgetdblattrib(oprob, XPRS_MIPBESTOBJVAL, &bestSol);
	XPRSgetintattrib(oprob, XPRS_COLS, &nVariables);
	XPRSgetintattrib(oprob, XPRS_ROWS, &nConstraints);
	solutionTime = difftime(time(NULL), startTime);
	XPRSgetintattrib(oprob, XPRS_NODES, &nNodes);
	//prob.print();
	Solution * sol = allocateIRPSolution();
	sol->SolutionTime = solutionTime;
	return sol;
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
			vector <NodeStrong*> graph;		//Graph to store nodes

			for (int t : Database.Periods) {
				graph.clear();
				buildStrongGraph(graph, t, true); //Do not include depot in graph			
				//graphAlgorithm::printGraph(graph, Database, "graphSolutions/graph", ModelParameters::X);
				graphAlgorithm::sepByStrongComp(graph, result);
				//graphAlgorithm::printGraph(graph, Database, "graphSolutions/graph", ModelParameters::X);
				temp = addSubtourCtr(result, t);
				if (temp)
					isSubtours = true;
			}

		} while (isSubtours);

		return allocateIRPSolution();
	}

	else
	prob.lpOptimise();
	return allocateIRPSolution();
}

XPRBprob * IRP::getProblem()
{
	return &prob;
}



void IRP::calculateExcess()
{
	int i, j;
	int excess = 0;

	ExcessConsumption = new double **[Database.AllNodes.size()];
	for (auto nodeDel : Database.DeliveryNodes) {
		i = nodeDel->getId();
		ExcessConsumption[i] = new double *[Database.AllPeriods.size()];
		
		for (int t1 : Database.Periods) {
			ExcessConsumption[i][t1] = new double[Database.AllPeriods.size()];
			for (int t2 : Database.Periods) {

				//initialize excess
				if (t1 == 1) {
					excess = nodeDel->LowerLimit - nodeDel->InitInventory;
				}
				else
					excess = nodeDel->LowerLimit - nodeDel->UpperLimit;

				//calculate excess between period t1 and t2
				if (t1 <= t2) {
					for (int t = t1; t <= t2; t++)
						excess += nodeDel->Demand[t];
					if (excess >= 1) ExcessConsumption[i][t1][t2] = excess;
					else  ExcessConsumption[i][t1][t2] = 0;

					//cout << "Node: " << i << "\tperiod: " << t1 << t2<< "\tExcessDelivery: " << ExcessConsumption[i][t1][t2] << "\n";
				}


			}
			
		}
	}

	excess = 0;
	ExcessProd = new double **[Database.AllNodes.size()];
	for (auto node : Database.PickupNodes) {
		i = node->getId();
		ExcessProd[i] = new double *[Database.AllPeriods.size()];
	
		for (int t1 : Database.Periods) {
			ExcessProd[i][t1] = new double[Database.AllPeriods.size()];
			for (int t2 : Database.Periods) {

				//initialize excess
				if (t1 == 1) {
					excess = node->InitInventory - node->UpperLimit;
				}
				else
					excess = node->LowerLimit - node->UpperLimit;

				//calculate excess between period t1 and t2
				if (t1 <= t2) {
					for (int t = t1; t <= t2; t++)
						excess += node->Demand[t];
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

	//Allocate x and loading solutions
	Solution *sol = Solution::allocateSolution(Database);
	
	fillSolution(sol);
	return sol;
}

//Fills a solution based on problem solution values
void IRP::fillSolution(Solution * sol)
{
	//Fill Inventory
	for (auto node : Database.Nodes) {
		int i = node->getId();
		for (int t : Database.Periods) {
			sol->Nodes[i]->NodePeriods[t]->Inventory = inventory[i][t].getSol();
		}
	}

	//Fill Quantity and allocate time
	for (auto node: Database.Nodes){ 
		for (int t : Database.Periods) {
			int i = node->getId();
			if (node->isDelivery()) {
				if (delivery[i][t].getSol() > 0) {
					sol->Nodes[i]->NodePeriods[t]->Quantity = delivery[i][t].getSol();
					sol->Nodes[i]->NodePeriods[t]->TimeServed = timeVar[i][t].getSol();
				}
				else
					sol->Nodes[i]->NodePeriods[t]->Quantity = 0;
			}
			else {
				if (pickup[i][t].getSol() > 0) {
					sol->Nodes[i]->NodePeriods[t]->Quantity = pickup[i][t].getSol();
					sol->Nodes[i]->NodePeriods[t]->TimeServed = timeVar[i][t].getSol();
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

	for (auto node1 : Database.AllNodes) {
		for (auto node2 : Database.AllNodes) {
			if (node1->inArcSet(node2)) {
				i = node1->getId();
				j = node2->getId();
				for (int t : Database.Periods) {
					if (x[i][j][t].getSol() >= 0.0001) {
						if (ModelParameters::Simultaneous) {
							if (node1->isColocated(node2) && simAction[i][j][t].getSol() <= 0.01) {
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
	vector <NodeStrong*> graph;		//Graph to store nodes
	vector<Node*> tempGraph;
	vector<Node*> tempGraph2;

	bool newCut = false;
	subtourIndices.clear();
	matrixValues.clear();
	for (int t : Database.Periods) {;
		buildStrongGraph(graph, t, false); //Do not include depot in graph
		graphAlgorithm::sepByStrongComp(graph, result);
		addSubtourCut(result, t, newCut, cut);

		for (Node * node : graph) {
			delete node;
		}
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
	int i, j;

	if (Depot){
		Node * node = new Node(0);
		graph.push_back(node);
	}
	//Create nodes for each visited customer
	for (NodeInstance* node : Database.Nodes) {
		i = node->getId();
		if (y[i][t].getSol() >= 0.001){
			Node * node = new Node(i);
			graph.push_back(node);
		}
	}
	//Not robust, be aware of changing node set logic
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

void IRP::buildStrongGraph(vector<NodeStrong*>& graph, int t, bool Depot)
{
	int s;
	double edgeValue;
	int i, j;

	if (Depot) {
		NodeStrong * node = new NodeStrong(0);
		graph.push_back(node);
	}
	//Create nodes for each visited customer
	for (NodeInstance* node : Database.Nodes) {
		i = node->getId();
		if (y[i][t].getSol() >= 0.001) {
			NodeStrong * node = new NodeStrong(i);
			graph.push_back(node);
		}
	}
	//Not robust, be aware of changing node set logic
	//Add outgoing edges from each visited node
	for (NodeStrong *node : graph) {
		s = node->getId();
		for (NodeStrong *endingNode : graph) {
			if (Database.inArcSet(s, endingNode->getId())) {
				edgeValue = x[s][endingNode->getId()][t].getSol();
				if (edgeValue >= (double) ModelParameters::EDGE_WEIGHT/100) {
					node->addEdge(edgeValue, endingNode);
				}
			}
		}
	}
}

void IRP::copyGraph(vector<Node*>& newGraph, vector<Node*>& cpGraph)
{
	int j;
	for (Node * n : cpGraph) {
		newGraph.push_back(new NodeIRP(*Database.getNode(n->getId())));
	}

	for (int i = 0; i < cpGraph.size(); i++) {
		if (cpGraph[i]->getEdges().size() >= 1) {
			j = cpGraph[i]->getEdge()->getEndNode()->getId();
			for (auto node : newGraph) {
				if (node->getId() == j) {
					newGraph[i]->addEdge(cpGraph[i]->getEdge()->getValue(), node);
				}
			}
		}
	}
}

bool IRP::getSubtourGraph(vector<Node*>& strongComp, int t)
{
	//Check if SEC is violated
	if (strongComp.size() >= 2) {// only cut for subsets of size 2 or greater
		double circleFlow = 0;
		double visitSum = 0;
		double maxVisitSum = 0;
		double tempNodeVisit = 0;
		//double tempNodeFlow = 0;
		int maxVisitID = -1;
		for (Node *node : strongComp) {

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

		if (circleFlow >= visitSum - maxVisitSum +(double) ModelParameters::ALPHA/100) {
			//print subtour
			return true;
		}
	}

	return false;
}

void IRP::copyGraph(vector<Node*>& newGraph, vector<NodeStrong*>& cpGraph)
{
	int j;
	for (NodeStrong * n : cpGraph) {
		newGraph.push_back(new NodeIRP(*Database.getNode(n->getId())));
	}

	for (int i = 0; i < cpGraph.size(); i++) {
		if (cpGraph[i]->getEdges().size() >= 1) {
			j = cpGraph[i]->getEdge()->getEndNode()->getId();
			for (auto node : newGraph) {
				if (node->getId() == j) {
					newGraph[i]->addEdge(cpGraph[i]->getEdge()->getValue(), node);
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
	int i, j;
	for (auto node1 : Database.AllNodes) {
		i = node1->getId();
		for (auto node2 : Database.AllNodes) {
			j = node2->getId();
			delete[] x[i][j];
			delete[] loadDelivery[i][j];
			delete[] loadPickup[i][j];
			
		}
		delete[] x[i];
		delete[] loadDelivery[i];
		delete[] loadPickup[i];
		delete[] y[i];
		delete[] timeVar[i];
		delete[] inventory[i];
		delete[] delivery[i];
		delete[] pickup[i];
	}

	delete [] x;
	delete[] y;
	delete[] delivery;
	delete[] pickup;
	delete timeVar;
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

			if (circleFlow >= visitSum - maxVisitSum + (double) ModelParameters::ALPHA/100) {
				//print subtour
				vector<int> varColumns;
				vector<int> rowValues;
				
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
				
					//y[node->getId()][t].print();
					//cout << "\n";
					rSideStr = rSideStr + " + " + "y_" + to_string(node->getId());

					if (node->getId() != maxVisitID)
					{
						rSide += y[node->getId()][t];
						varColumns.push_back(y[node->getId()][t].getColNum());
						rowValues.push_back(-1);
					}
					else
						rSide += y[node->getId()][t];

					for (Node *node2 : strongComp[i]) {
						if (Database.inExtensiveArcSet(node->getId(), node2->getId())) {
							//printf("x_%d%d + ", u, v);
							lSide += x[node->getId()][node2->getId()][t];
			
							rowValues.push_back(1);
							//x[node->getId()][edge->getEndNode()->getId()][t].print();
							//cout << "\n";

							varColumns.push_back(x[node->getId()][node2->getId()][t].getColNum());
						}
					}
				}
			
				subtourIndices.push_back(varColumns);
				matrixValues.push_back(rowValues);

				rSide -= y[maxVisitID][t];
				rSideStr = rSideStr + " - " + "y_" + to_string(maxVisitID);
				//cout << rSideStr << "\n";
		
	
				XPRBcut vv = prob.newCut( lSide <= rSide);
				//vv.print();
				
 				SubtourCut.push_back(vv);


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

			if (circleFlow >= visitSum - maxVisitSum + ModelParameters::ALPHA/100) {
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
				//subtourIndices.push_back(subtour);
				rSide -= y[maxVisitID][t];
				rSideStr = rSideStr + " - " + "y_" + to_string(maxVisitID);
				//cout << rSideStr << "\n";
		

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
	int i, j;
	bool arcIndicator;
	/****OBJECTIVE****/
		//Transportation costs
	if (!ModelParameters::Simultaneous) {
		for (auto node1 : Database.AllNodes) {
			i = node1->getId();
			for (auto node2 : Database.AllNodes) {
				if (node1->inArcSet(node2)) {
					j = node2->getId();
					for (int t : Database.Periods) {

						objective += node1->getTransCost(node2) * x[i][j][t];
					}
				}
			}
		}
	}

	else {
		for (auto node1 : Database.AllNodes) {
			i = node1->getId();
			for (auto node2 : Database.AllNodes) {
				if (node1->inArcSet(node2)) {
					j = node2->getId();
					for (int t : Database.Periods) {
						if (node1->isColocated(node2)) {
							objective += simAction[i][j][t] * node1->getTransCost(node2);
						}
						else {
							objective += node1->getTransCost(node2) * x[i][j][t];
						}
					}
				}
			}
		}
	}
	


	//Add penalty cost for extra vehicle
	for (int t : Database.Periods)
		objective += ModelParameters::VehiclePenalty * extraVehicle[t];

	//Holding costs
	for (auto node : Database.Nodes) {
		i = node->getId();
		for (int t : Database.Periods)
			objective += node->HoldingCost * inventory[i][t];
	}
	
	prob.setObj(prob.newCtr("OBJ", objective));  /* Set the objective function */
	//end objective

	/**** CONSTRAINTS ****/
	XPRBexpr p1;
	if (ModelParameters::Simultaneous) {
		for (auto node1 : Database.DeliveryNodes) {
			i = node1->getId();
			for (auto node2 : Database.PickupNodes) {
				if (node1->inArcSet(node2) && node1->isColocated(node2)) {
					j = node2->getId();
					for (int t : Database.Periods) {
						p1 = actionDelivery[i][t] + actionPickup[j][t] - epsilon* simAction[i][j][t];
						prob.newCtr("SimAction", p1 <= 2 - epsilon);
						p1 = 0;
					}
				}
			}
		}



		for (auto node : Database.PickupNodes) {
			i = node->getId();
			for (int t : Database.Periods)
			{
				p1 = pickup[i][t] - min(Database.Capacity, node->UpperLimit - node->LowerLimit)*actionPickup[i][t];
				prob.newCtr("PickupAction", p1 <= 0);
				p1 = 0;
			}

			for (auto node : Database.DeliveryNodes) {
				i = node->getId();
				for (int t : Database.Periods)
				{
					p1 = delivery[i][t] - min(Database.Capacity, node->UpperLimit - node->LowerLimit)*actionDelivery[i][t];
					prob.newCtr("Deliveryction", p1 <= 0);
					p1 = 0;
				}
			}
		}
	}
	

	if (MaskOn) {

		int visits;
		for (int t : Database.Periods) {
			p1 = 0;
			visits = 0;
			for (auto node : Database.Nodes) {
				i = node->getId();
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
	for (auto node1 : Database.AllNodes){
		i = node1->getId();
		for (int t : Database.Periods) {
			for (auto node2 : Database.AllNodes) {
				j = node2->getId();
				if (node1->inArcSet(node2)) {
					p1 += x[i][j][t];
				}

				if (node2->inArcSet(node1)){
					p2 += x[j][i][t];
				}
			}
			prob.newCtr("RoutingFlow", p1 - p2 == 0);
			p1 = 0;
			p2 = 0;
		}
	}

	//Linking x and y
	for (auto node1 : Database.AllNodes){
		i = node1->getId();
		for (int t : Database.Periods) {
			for (auto node2 : Database.AllNodes) {
				j = node2->getId();
				if (node1->inArcSet(node2)) {
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
	for (int t : Database.Periods) {
		p1 = y[0][t];
		prob.newCtr("Max vehicles", p1 <= ModelParameters::nVehicles + extraVehicle[t]);
		p1 = 0;
	}

	//Max visit
	for (auto node : Database.Nodes) {
		i = node->getId();
		for (int t : Database.Periods) {
			p1 = y[i][t];
			prob.newCtr("Max visit", p1 <= 1);
			p1 = 0;
		}
	}


	//timeVar constraints
	
	for (int t : Database.Periods) {
		p1 = timeVar[0][t];
		prob.newCtr("Initial time", p1 == 0);
		p1 = 0;
	}

	for (auto node1 : Database.AllNodes) {
		i = node1->getId();
		for (int t : Database.Periods) {
			for (auto node2 : Database.Nodes){
				j = node2->getId();
				if (node1->inArcSet(node2)) {
					p1 = timeVar[i][t] - timeVar[j][t] + node1->getTravelTime(node2)
						+ (ModelParameters::maxTime + node1->getTravelTime(node2)) * x[i][j][t];

					p2 = ModelParameters::maxTime + node1->getTravelTime(node2);
					prob.newCtr("Time flow", p1 <= p2);
					p1 = 0;
					p2 = 0;

					//p1 = time[i][t] + TravelTime[i][j] * x[i][j][t] <= ModelParameters::maxTime;
					//prob.newCtr("Final time", p1 <= ModelParameters::maxTime);
					//p1 = 0;
				}
	
				}

			if (node1->inArcSet(Database.getDepot())) {
				p1 = timeVar[i][t] + node1->getTravelTime(Database.getDepot()) * x[i][0][t];
				prob.newCtr("Final time", p1 <= ModelParameters::maxTime);
				p1 = 0;
			}
			}
	}
	
	//end time constraints

	addInventoryAndLoadingCtr(prob);

	//Arc Database.Capacity
	for (auto node1 : Database.AllNodes) {
		i = node1->getId();
		for (auto node2 : Database.AllNodes) {
			j = node2->getId();
			if (node1->inArcSet(node2)) {
				for (int t : Database.Periods) {
					p1 = loadDelivery[i][j][t] + loadPickup[i][j][t] - Database.Capacity * x[i][j][t];
					prob.newCtr("Arc_Capacity", p1 <= 0);
					p1 = 0;
				}
			}
		}
	}

	//Vehicle Database.Capacity
	for (auto nodeDel : Database.DeliveryNodes) {
		i = nodeDel->getId();
		for (int t : Database.Periods) {
			p1 = delivery[i][t] - min(Database.Capacity, nodeDel->UpperLimit - nodeDel->LowerLimit)*y[i][t];

			prob.newCtr("Vehicle Database.Capacity delivery", p1 <= 0);
			p1 = 0;
		}
	}

	for (auto nodePick : Database.PickupNodes) {
		i = nodePick->getId();
		for (int t : Database.Periods) {
			p1 = pickup[i][t] - min(Database.Capacity, nodePick->UpperLimit - nodePick->LowerLimit)*y[i][t];

			prob.newCtr("Vehicle Database.Capacity pickup", p1 <= 0);
			p1 = 0;
		}
	}
	return true;
}



bool IRP::initializeParameters() {

	//Initialize max time
	MaxTime = 480;
	//initialize cap
	return true;
}

void IRP::printStrongCompAlgorithm()
{
	vector <vector<Node*>> result; //matrix to store strong components
	vector <NodeStrong*> graph;		//Graph to store nodes
	vector<Node*> tempGraph;
	vector<Node*> tempGraph2;

	vector<XPRBcut> cut;
	bool newCut = false;
	subtourIndices.clear();
	matrixValues.clear();
	for (int t : Database.Periods) {
		//buildGraph(graph, t, true, 0.01); //include depot
		//	graphAlgorithm::printGraph(graph, "Subtour/LPrelax" + to_string(t), ModelParameters::X);
		//graph.clear();
		buildStrongGraph(graph, t, false); //Do not include depot in graph

		copyGraph(tempGraph, graph);
		graphAlgorithm::printGraph(tempGraph, "Subtour/DepotGone" + to_string(t), ModelParameters::X);
		tempGraph.clear();
		graphAlgorithm::sepByStrongComp(graph, result);

		for (auto graph : result) {
			for (auto n : graph)
				tempGraph2.push_back(n);
		}


	
		copyGraph(tempGraph, tempGraph2);

		graphAlgorithm::printGraph(tempGraph, "Subtour/Separation" + to_string(t), ModelParameters::X);
		tempGraph.clear();
		tempGraph2.clear();
		for (auto graph : result) {
			if (getSubtourGraph(graph, t)) {
				for (auto n : graph) {
					tempGraph2.push_back(n);
				}
				copyGraph(tempGraph, tempGraph2);
				graphAlgorithm::printGraph(tempGraph, "Subtour/SUBTOUR" + to_string(t), ModelParameters::X);
				tempGraph.clear();
				tempGraph2.clear();
			}
		}

		

		for (Node * node : graph) {
			delete node;
		}
		graph.clear();
		result.clear();
		tempGraph.clear();
		tempGraph2.clear();
	}
}

void IRP::printSolutionToFile(double lpOptima, double trans, double hold,  int version, int alpha)
{
	ofstream instanceFile;
	instanceFile.open("Results/C" + to_string(Database.getNumNodes() / 2) + "V" + to_string(ModelParameters::nVehicles) + "version " +to_string(version) + "_alpha_ "+to_string(alpha)+".txt");
	instanceFile << "Sol. time: \t " << solutionTime << "\n";
	instanceFile << "IP objective: \t" << bestSol << "\n";
	instanceFile << "Trans. cost: \t" << getTransportationCost() << "\n";
	instanceFile << "Holding cost: \t" << getHoldingCost() << "\n";
	instanceFile << "LP optima: \t" << lpOptima << "\n";
	instanceFile << "LP trans: \t" << trans << "\n";
	instanceFile << "LP hold: \t" << hold << "\n";
	instanceFile << "Dual gap: \t" << to_string((bestSol - bestBound) / bestSol) << "\n";
	instanceFile << "numNodes: \t" << nNodes << "\n";
	instanceFile << "Variables: \t" << nVariables <<"\n";
	instanceFile << "Constraints: \t" << nConstraints << "\n";
	instanceFile << "nRoutes: \t" << getnRoutes()<<"\n";
	instanceFile << "nCuts:\t" << nSubtourCuts << "\n";

	instanceFile.close();
}







void IRP::getSubset(vector<NodeInstance*> subset, int subsetSize, int nodePos, vector<NodeInstance*> & searchNodes, int t1, int t2)
{
	subset.push_back(searchNodes[nodePos]);


	if (subset.size() < subsetSize) {
	
		vector<NodeInstance*> localSubset = subset;
		
		for (int i = nodePos; i < searchNodes.size() - 1; i++) {
			getSubset(subset, subsetSize, i+1,  searchNodes, t1, t2);
		}
	}
	else {
		if (subset.size() >= 2) {
			int size = subset.size();
			for (int i = 0; i < size; i++) {
				subset.push_back(subset[i]->getColocatedNode());
			}
		}
		XPRBexpr p1;
		XPRBexpr p2;
		double flow = 0;
		double denominator;
		int i;

		for (NodeInstance * node : subset) {
			i = node->getId();
			if (node->isDelivery() && searchNodes[0]->isDelivery()) {
				flow += ExcessConsumption[i][t1][t2];
				cout << ExcessConsumption[i][t1][t2];
			}
			else if (!node->isDelivery() && !searchNodes[0]->isDelivery()) {
				flow = ExcessProd[i][t1][t2];
			}

			if (subset.size() == 1) {
				denominator = min(Database.Capacity, (double) node->UpperLimit - node->LowerLimit);
			}
			else
				denominator = Database.Capacity;
		}

		if (ceil(flow / denominator) - flow/denominator >= ModelParameters::ExcessParameter/100){
			//Add colocated nodes to subset
			int size = subset.size();
			if (subset.size() >= 2) {
				//subset.resize(subset.size() * 2);
			}
			//cout << "\n";
			//Get difference set
			vector <int> allNodes;
			allNodes.resize(Database.AllNodes.size());
			for (auto node : Database.AllNodes)
				allNodes[node->getId()] = node->getId();

			vector <NodeInstance*> Difference = Database.getDifference(Database.AllNodes, subset);
			//Add minimum flow constraint
			int i, j;
			for (int t = t1; t <= t2; t++) {
				for (auto node1 : subset) {
					i = node1->getId();
					for (auto node2 : Difference) {
						j = node2->getId();
						if (node1->inArcSet(node2)) {
							p1 += x[i][j][t];
						}
					}

				}
			}
			p2 = ceil(flow / denominator);
			prob.newCtr("MinFlow", p1 >= p2).print();
		}
	}	
}



void IRP::addInventoryAndLoadingCtr(XPRBprob & problem)
{
	XPRBexpr p1;
	XPRBexpr p2;
	int i, j;

	//Inventory constraints
	for (auto node : Database.Nodes) {
		i = node->getId();
		p1 = inventory[i][0];
		//printf("%d", InitInventory[i]);
		problem.newCtr("Initial inventory", inventory[i][0] == node->InitInventory);
		p1 = 0;
	}

	for (auto nodeDel : Database.DeliveryNodes) {
		i = nodeDel->getId();
		for (int t : Database.Periods) {
			p1 = inventory[i][t] - inventory[i][t - 1] + nodeDel->Demand[t] - delivery[i][t];
			problem.newCtr("Delivery Inventory balance", p1 == 0);
			p1 = 0;

			p1 = inventory[i][t - 1] + delivery[i][t];
			problem.newCtr("Delivery Inventory balance 2", p1 <= nodeDel->UpperLimit);
			p1 = 0;
		}
	}

	for (auto nodePick : Database.PickupNodes) {
		i = nodePick->getId();
		for (int t : Database.Periods) {
			//printf("%d", Demand[i][t]);
			p1 = inventory[i][t] - inventory[i][t - 1] - nodePick->Demand[t] + pickup[i][t];
			problem.newCtr("Pickup Inventory balance", p1 == 0);
			p1 = 0;

			p1 = inventory[i][t - 1] - pickup[i][t];
			problem.newCtr("Pickup Inventory balance 2", p1 >= nodePick->LowerLimit);
			p1 = 0;
		}
	}

	//Upper and lower inventory
	for (auto node : Database.Nodes) {
		i = node->getId();
		for (int t : Database.Periods) {
			p1 = inventory[i][t];
			problem.newCtr("Inventory Lower Limit", p1 >= node->LowerLimit);
			problem.newCtr("Inventory Upper Limit", p1 <= node->UpperLimit);
			p1 = 0;
		}
	}

	//Loading constraints
	for (auto nodeDel : Database.DeliveryNodes) {
		i = nodeDel->getId();
		for (int t : Database.Periods) {
			for (auto node2: Database.AllNodes) {
				j = node2->getId();
				if (node2->inArcSet(nodeDel)) {
					p1 += loadDelivery[j][i][t];
					p2 -= loadPickup[j][i][t];
				}
				if (nodeDel->inArcSet(node2)) {
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

	for (auto nodePick : Database.PickupNodes) {
		i = nodePick->getId();
		for (int t : Database.Periods) {
			for (auto node : Database.AllNodes) {
				j = node->getId();
				if (node->inArcSet(nodePick)) {
					p1 += loadPickup[j][i][t];
					p2 += loadDelivery[j][i][t];
				}
				if (nodePick->inArcSet(node)) {
					p1 -= loadPickup[i][j][t];
					p2 -= loadDelivery[i][j][t];
				}
			}
			p1 += pickup[i][t];

			prob.newCtr("LoadBalance pickup", p1 == 0);
			prob.newCtr("DeliveryBalance at Database.PickupNodes", p2 == 0);
			p1 = 0;
			p2 = 0;
		}

	}

}

double IRP::getTransportationCost()
{
	double transCost = 0;
	int i, j;
	for (NodeInstance * node1 : Database.AllNodes) {
		i = node1->getId();
		for (NodeInstance * node2 : Database.AllNodes) {
			if (node1->inArcSet(node2)) {
				j = node2->getId();
				for (int t : Database.Periods)
					transCost += x[i][j][t].getSol() *node1->getTransCost(node2);
			}

		}
	}

	return transCost;
}

double IRP::getHoldingCost()
{
	double holdingCost = 0;

	for (NodeInstance * node : Database.Nodes)
		for (int t : Database.Periods)
			holdingCost += inventory[node->getId()][t].getSol() * node->HoldingCost;

	return holdingCost;
}

double IRP::getnRoutes()
{
	double nRoutes = 0;
	for (int t : Database.Periods)
		nRoutes += y[0][t].getSol();

	return nRoutes;
}






bool IRP::initializeSets()
{
	NumOfCustomers = (int) Database.Nodes.size();				//Number of customers
	NumOfPeriods = (int) Database.getnPeriods();

	

	//for (int i = 0; i < Database.AllNodes.size(); i++) printf("%d", Database.AllNodes[i]);
	return true;
}

void IRP::generateMask()
{
	int i;
	VisitNode = new int *[Database.Nodes.size()];
	for (auto node : Database.Nodes) {
		i = node->getId();
		VisitNode[i] = new int[Database.Periods.size()];
		//cout<<"\n";
		for (int t : Database.Periods) {
			VisitNode[i][t] = (rand() % 2 + 0);
			//cout << VisitNode[i][t]<<"\t";

		}
	}
}

void IRP::generateMask(int ** mask)
{
	int i;
	VisitNode = new int *[Database.Nodes.size()];
	for (auto node : Database.Nodes) {
		i = node->getId();
		VisitNode[i] = new int[Database.Periods.size()];
		//cout<<"\n";
		for (int t : Database.Periods) {
			VisitNode[i][t] = mask[i][t];
			//cout << VisitNode[i][t]<<"\t";

		}
	}
}

bool IRP::initializeVariables()
{
	int i, j;
	bool arcIndicator;
	x = new XPRBvar **[Database.AllNodes.size()];
	loadDelivery = new XPRBvar **[Database.AllNodes.size()];
	loadPickup = new XPRBvar **[Database.AllNodes.size()];

	for (auto node1 : Database.AllNodes) {
		i = node1->getId();
		x[i] = new XPRBvar *[Database.AllNodes.size()];
		loadDelivery[i] = new	XPRBvar *[Database.AllNodes.size()];
		loadPickup[i] = new	XPRBvar *[Database.AllNodes.size()];

		for (auto node2 : Database.AllNodes) {
			if(node1->inArcSet(node2)){
				j = node2->getId();
				x[i][j] = new XPRBvar[Database.Periods.size()];
				loadDelivery[i][j] = new	XPRBvar[Database.Periods.size()];
				loadPickup[i][j] = new	XPRBvar[Database.Periods.size()];

				for (int t : Database.Periods) {
					if (ARC_RELAXED)
						x[i][j][t] = prob.newVar(XPRBnewname("x%d-%d%d", i, j, t), XPRB_PL, 0, 1);
					else
						x[i][j][t] = prob.newVar(XPRBnewname("x%d-%d%d", i, j, t), XPRB_BV, 0, 1);

					loadDelivery[i][j][t] = prob.newVar(XPRBnewname("lD_%d%d%d", i, j, t), XPRB_PL, 0, Database.Capacity);
					loadPickup[i][j][t] = prob.newVar(XPRBnewname("lP_%d%d%d", i, j, t), XPRB_PL, 0, Database.Capacity);
				}
			}
		}
	} // end initializing of x and load

	//Initialize action variables if simultanoues
	if (ModelParameters::Simultaneous) {
		actionDelivery = new XPRBvar *[Database.AllNodes.size() + 1];
		actionPickup = new XPRBvar *[Database.AllNodes.size() + 1];

		for (auto nodeDel : Database.DeliveryNodes) {
			i = nodeDel->getId();
			actionDelivery[i] = new XPRBvar[Database.Periods.size() + 1];
			for (int t : Database.Periods) {
				actionDelivery[i][t] = prob.newVar(XPRBnewname("ActionDel_%d%d", i, t), XPRB_BV, 0, 1);
			}
		}

		for (auto nodePick : Database.PickupNodes) {
			i = nodePick->getId();
			actionPickup[i] = new XPRBvar[Database.AllPeriods.size()];
			for (int t : Database.Periods) {
				actionPickup[i][t] = prob.newVar(XPRBnewname("ActionPic_%d%d", i, t), XPRB_BV, 0, 1);
			}
		}

		simAction = new XPRBvar **[Database.AllNodes.size() + 1];
		for (auto nodeDel : Database.DeliveryNodes) {
			i = nodeDel->getId();
			simAction[i] = new XPRBvar *[Database.AllNodes.size() + 1];
			for (auto nodePick : Database.PickupNodes) {
				if (nodeDel->isColocated(nodePick)) {
					j = nodePick->getId();
					simAction[i][j] = new XPRBvar[Database.Periods.size() + 1];
					for (int t : Database.Periods) {
						simAction[i][j][t] = prob.newVar(XPRBnewname("Sim_%d%d%d", i, j, t), XPRB_BV, 0, 1);

					}
				}
			}
		}
		
	} //end simultaneous

	//initialize y-variables and inventory
	y = new XPRBvar *[Database.AllNodes.size()];
	if (ARC_RELAXED) {
		for (auto node : Database.AllNodes) {
			i = node->getId();
			y[i] = new XPRBvar[Database.Periods.size()];
			for (int t : Database.Periods) {
				if (i > 0) 
					y[i][t] = prob.newVar(XPRBnewname("y_%d%d", i, t), XPRB_BV, 0, 1);
				
				else
					y[i][t] = prob.newVar(XPRBnewname("y_%d%d", 0, t), XPRB_UI, 0, ModelParameters::nVehicles);
			}
		}
	}
	else
	{
		for (auto node : Database.AllNodes) {
			i = node->getId();
			y[i] = new XPRBvar[Database.Periods.size()];
			for (int t : Database.Periods) {
				if (i > 0)
					y[i][t] = prob.newVar(XPRBnewname("y_%d%d", i, t), XPRB_PL, 0, 1);
				else
					y[i][t] = prob.newVar(XPRBnewname("y_%d%d", 0, t), XPRB_PL, 0, ModelParameters::nVehicles);
			}
		}
	}

	//Initialize extra vehicle
	extraVehicle = new XPRBvar [Database.AllPeriods.size()];
	for (int t : Database.Periods) {
		extraVehicle[t] = prob.newVar(XPRBnewname("extravehicle_%d", t), XPRB_PL, 0, 2*ModelParameters::nVehicles);
	}

	inventory = new XPRBvar *[Database.AllNodes.size()];

	for (auto node : Database.Nodes) {
		i = node->getId();
		inventory[i] = new XPRBvar[Database.AllPeriods.size()];
		for (int t : Database.AllPeriods) {
			inventory[i][t] = prob.newVar(XPRBnewname("i_%d%d", i, t), XPRB_PL, node->LowerLimit, node->UpperLimit);
		}
	}


	//Initialize at delivery nodes
	delivery = new XPRBvar *[Database.AllNodes.size()];
	for (auto nodeDel : Database.DeliveryNodes) {
		i = nodeDel->getId();
		delivery[i] = new XPRBvar [Database.Periods.size()+1];
		for (int t : Database.Periods) {
			delivery[i][t] = prob.newVar(XPRBnewname("qD_%d%d", i, t), XPRB_PL, 0, Database.Capacity);
			
		}
	}

	//Initialize at pickup nodes
	pickup = new XPRBvar *[Database.AllNodes.size()];
	for (auto nodePick : Database.PickupNodes){
		i = nodePick->getId();
		pickup[i] = new  XPRBvar[Database.Periods.size()+1];
		for (int t : Database.Periods) {
			pickup[i][t] = prob.newVar(XPRBnewname("qP_%d%d", i, t), XPRB_PL, 0, Database.Capacity);
		}
	}

	//Initialize time variables
	timeVar = new XPRBvar *[Database.AllNodes.size()];
	for (auto node : Database.AllNodes) {
		i = node->getId();
		timeVar[i] = new XPRBvar[Database.Periods.size()];
		for (int t : Database.Periods) {
			timeVar[i][t] = prob.newVar(XPRBnewname("t_%d%d", i, t), XPRB_PL, 0, 1000);
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
		for (int i : Database.DeliveryNodes) {
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
	return Database.Periods.size();
}


IRP::~IRP()
{
}





void IRP::addVisitConstraint(double ** Visit, int selection)
{
	XPRBexpr p1 = 0;
	XPRBexpr p2 = 0;
	int i, j;

	cout << "\n";
	if (VisitCtr.isValid()) {
		prob.delCtr(VisitCtr);
	}

	if (selection == ModelParameters::ForceVisits) {
		int visits;

		for (int t : Database.Periods) {
		visits = 0;
			for (auto node : Database.Nodes) {
				i = node->getId();
				if (Visit[i][t] == 0 && !TabuMatrix[i][t].isValid())
				{
					visits += 1;
					p1 += y[i][t];
				}
			}
		}
		VisitCtr = prob.newCtr("MinVisits", p1 >= ceil(visits*0.4));
		VisitCtr.print();
		cout << "\n";
	}

	if (selection == ModelParameters::ForceChanges) {
		for (int t : Database.Periods) {
			for (auto node : Database.Nodes) {
				i = node->getId();
				if (Visit[i][t] == 0 && !TabuMatrix[i][t].isValid()) {
					p1 += y[i][t];
				}
				else if (Visit[i][t] == 1 && !TabuMatrix[i][t].isValid()) {
					p2 += (1 - y[i][t]);
				}
			}
		}

		//add constraint
		VisitCtr = prob.newCtr("MinVisits", p1 + p2 >= ceil(Database.Nodes.size()*Database.Periods.size()*0.25));
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
	int i, j;
	XPRBexpr p1 = 0;
	XPRBexpr p2 = 0;
	XPRBexpr p3 = 0;
	//Minimum visit

	if (ValidIneq == ModelParameters::MinimumNodeVisit) {
		double minVisit;

		for (auto nodeDel : Database.DeliveryNodes) {
			i = nodeDel->getId();
			minVisit = 0;
			p1 = 0;
			for (int t2 : Database.Periods) {
				for (int t1 : Database.Periods) {
					if (t1 <= t2) {
						for (int t = t1; t <= t2; t++) {
							p1 += y[i][t];
						}
						minVisit = ExcessConsumption[i][t1][t2] / min(Database.Capacity, (double) nodeDel->UpperLimit - nodeDel->LowerLimit);
						
						if (ceil(minVisit) - minVisit >= ModelParameters::ExcessParameter/100)
						{
							prob.newCtr("MinVisitDelivery", p1 >= ceil(minVisit));
						}
						p1 = 0;
						minVisit = 0;
					}
				}
			}
		} // end delivery nodes

		for (auto nodePick : Database.PickupNodes) {
			i = nodePick->getId();
			minVisit = 0;
			p1 = 0;
			for (int t2 : Database.Periods) {
				for (int t1 : Database.Periods) {
					if (t1 <= t2) {
						for (int t = t1; t <= t2; t++) {
							p1 += y[i][t];
						}
						minVisit = ExcessProd[i][t1][t2] / min(Database.Capacity, nodePick->UpperLimit - nodePick->LowerLimit);
						if (ceil(minVisit) - minVisit >= ModelParameters::ExcessParameter/100)
						{
							prob.newCtr("MinVisitPickup", p1 >= ceil(minVisit));
						}
						p1 = 0;
						minVisit = 0;
					}
				}
			}
		} //End pickup nodes
	} // End minimum visits

	cout << "\n";

	if (ValidIneq == ModelParameters::MinimumInventory) {
		int i;
		for (auto node: Database.DeliveryNodes) {
			i = node->getId();
			for (int t2 : Database.Periods) {
				for (int t1 : Database.Periods) {
					if (t1 <= t2) {
						for (int t = t1; t <= t2; t++)
						{
							p2 += node->Demand[t];
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

		for (auto node : Database.PickupNodes) {
			i = node->getId();
			for (int t2 : Database.Periods) {
				for (int t1 : Database.Periods) {
					if (t1 <= t2) {
						for (int t = t1; t <= t2; t++)
						{
							p2 += node->Demand[t];
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

		vector<vector<vector<NodeInstance*>>> IncludedNodes;
		IncludedNodes.resize(Database.Periods.size() + 1);

		//For all delivery nodes
		for (auto t1 : Database.Periods) {
			IncludedNodes[t1].resize(Database.Periods.size() + 1);
			for (auto t2 : Database.Periods) {
				if (t1 <= t2)
					for (auto nodeDel : Database.DeliveryNodes) {
						i = nodeDel->getId();
						if (ExcessConsumption[i][t1][t2] >= 0.01) {
							IncludedNodes[t1][t2].push_back(nodeDel);
				
						}
					}

			}
		}
		//Recursively check all possible subsets up to the subsetSize
		for (auto t1 : Database.Periods)
			for (auto t2 : Database.Periods) {
				if (t1 <= t2) {
					vector<NodeInstance *> subset;
					for (int subsetSize = 1; subsetSize <= ModelParameters::SubsetSizeMinFlow; subsetSize++)
						for (auto nodePos = 0; nodePos < IncludedNodes[t1][t2].size(); nodePos++)
							getSubset(subset, subsetSize, nodePos, IncludedNodes[t1][t2], t1, t2);
				}
			}

		//Clear nodes
		for (auto t1 : Database.Periods)
			for (auto t2 : Database.Periods)
				if (t1 <= t2)
					IncludedNodes[t1][t2].clear();

		//For all pickup nodes
		for (auto t1 : Database.Periods) {
			for (auto t2 : Database.Periods) {
				if (t1 <= t2)
					for (auto nodePick : Database.PickupNodes) {
						i = nodePick->getId();
						if (ExcessProd[i][t1][t2] >= 0.01)
							IncludedNodes[t1][t2].push_back(nodePick);
					}

			}
		}

		//Recursively check all possible subsets up to the subsetSize
		for (auto t1 : Database.Periods)
			for (auto t2 : Database.Periods) {
				if (t1 <= t2) {
					vector<NodeInstance *> subset;
					for (int subsetSize = 1; subsetSize <= ModelParameters::SubsetSizeMinFlow; subsetSize++)
						for (auto nodePos = 0; nodePos < IncludedNodes[t1][t2].size(); nodePos++)
							getSubset(subset, subsetSize, nodePos, IncludedNodes[t1][t2], t1, t2);
				}
			}

	} //End minimumflow
	
}//End valid inequalitites



void IRP::updateTabuMatrix(double ** changeMatrix)
{
	srand(std::time(0));
	int counter = 0;
	int i = 0;
	cout << "\n\nCountMatrix: Times since last changed , >0 added visit, <0 removed visit. * locked variable\n";
	for (auto node : Database.Nodes) {
		i = node->getId();
		cout << "\n";
		for (auto t : Database.Periods) {

			//Clear the tabu matrix
			if (TabuMatrix[i][t].isValid()) {
				prob.delCtr(TabuMatrix[i][t]);
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
	
				//If visit, lock that visit with 20% chance.
				if (CountMatrix[i][t] >= 1) {
					if (rand() % 100 + 1 <= ModelParameters::TABU_LOCK){
					//if(fmod(i, 7) - floor(i/7) <= 0.2){
						TabuMatrix[i][t] = prob.newCtr(XPRBnewname("ReqVisit"), y[i][t] >= 1);
						counter += 1;
						cout << "*\t";
					}
					else
						cout << "\t";
				}
				else if (CountMatrix[i][t] == 0) cout << "\t";

				//Else if visit removed, lock that visit with 20% chance.
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
					if (rand() % 100 + 1 <= ModelParameters::TABU_LOCK) {
					//if (fmod(i, 7) - floor(i / 7) <= 0.2){
						TabuMatrix[i][t] = prob.newCtr(XPRBnewname("ReqNotVisit"), y[i][t] <= 0);
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


	for (auto node : Database.Nodes) {
		i = node->getId();
		cout << "\n";
		for (auto t : Database.Periods) {
			if (TabuMatrix[i][t].isValid()) {
				TabuMatrix[i][t].print();
				cout << "\t";
			}
		}
	}
} // end update tabu matrix

int IRP::getNumOfNodes()
{
	return (int) Database.Nodes.size();
}

void IRP::addHoldingCostCtr(double holdingCost)
{
	XPRBexpr p1 = 0;
	int i, j;
	for (auto node : Database.Nodes)
		i = node->getId();
		for(int t : Database.Periods)
			p1 += inventory[i][t];

	prob.newCtr(XPRBnewname("HoldingCost"), p1 <= holdingCost * (1+ModelParameters::HOLDING_COST_INCREMENT));
	p1 = 0;

	//Delete current objective
	prob.delCtr(prob.getCtrByName("OBJ"));

	//Set transportation costs as objective
	for (auto node1 : Database.AllNodes) {
		i = node1->getId();
		for (auto node2 : Database.AllNodes)
			if (node1->inArcSet(node2)) {
				j = node2->getId();
				for (int t : Database.Periods)
					p1 += node1->getTransCost(node2) * x[i][j][t];
			}
	}
	prob.setObj(prob.newCtr("OBJ", p1));
	prob.print();
}

void IRP::addRouteCtr(vector<Route*> routes)
{
	for (XPRBctr ctr : RouteCtr)
		if(ctr.isValid())
			prob.delCtr(ctr);

	if (RouteChangeCtr.isValid())
		prob.delCtr(RouteChangeCtr);

	XPRBexpr p1 = 0;
	XPRBexpr p2 = 0;
	changeRoute.clear();
	changeRoute.resize(routes.size());
	int rr = 0;
	for (Route * r : routes){
		changeRoute[rr] = prob.newVar(XPRBnewname("changeRoute_%d", rr), XPRB_BV, 0, 1);
		for (NodeIRP* node : r->route) {
			if (!node->isDepot()) {
				p1 += y[node->getId()][r->getPeriod()];
			}
		}

		//Require change to route
		RouteCtr.push_back(prob.newCtr(XPRBnewname("Route change"), p1 - (r->getSize()-2) <= 1-changeRoute[rr]));
		p1 = 0;
		p2 += changeRoute[rr];
		rr++;
	}

	RouteChangeCtr = prob.newCtr(XPRBnewname("Logical"), p2 >= floor(routes.size()*ModelParameters::ROUTE_LOCK/100));

	//What percentage of routes that need change

}		

int IRP::getCapacity()
{
	return Capacity;
}

void IRP::useIPSubtourElimination(int cuttype)
{
	CutType = cuttype;
	oprob = prob.getXPRSprob();
	SubtourElimination = true;
	//Enable subtour elimination
	//prob.setCutMode(1);
	XPRSsetcbcutmgr(oprob, cbmng, &(*this));
	//XPRSsetcbprenode(oprob, preNode, &(*this));
	XPRSsetcbpreintsol(oprob, acceptInt, &(*this));
}

void IRP::useLPSubtourElimination()
{
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
					if (period <= 33 ) {
						//Check if node is visited in next period
						if (node->quantity(period + 1) > 0.01) {

							pushDel = shiftDel;
							pushDel = node-> moveQuantity(period, period + 1, pushDel);
							shiftDel -= pushDel;
						}
					}

					//Try to shift forward two period
					if (period <= 77) {
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
				if (period <= 88) {
					if (node->quantity(period + 1) > 0.01) {
						pushPick = shiftPick;
						pushPick = node->moveQuantity(period, period + 1, pushPick);
						shiftPick -= pushPick;
					}
				}

				//Try to shift forward two period
				if (period <= 77) {
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
		for (auto t : Instance.Database.Periods) {
			if (maxValue <= OrigSol->getTransportationCost(t)) {
				maxValue = OrigSol->getTransportationCost(t);
				period = t;
			}
		}

		return period;
	}

	if (selection == ModelParameters::HIGHEST_RESIDUAL_CAPACITY) {
		for (auto t : Instance.Database.Periods) {
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
	//for(auto i : Database.Periods)
}*/
