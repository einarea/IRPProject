#include "stdafx.h"
#include "VRPmodel.h"
#include "ModelParameters.h"


//Time callback
void XPRS_CC cbmngtimeVRP(XPRSprob oprob, void * vd, int parent, int newnode, int branch) {

	VRPmodel * modelInstance;
	modelInstance = (VRPmodel*)vd;

	if (difftime(time(NULL), modelInstance->StartTime) >= ModelParameters::MAX_RUNNING_TIME_VRP || difftime(time(NULL), modelInstance->lastSolutionFound) >= ModelParameters::TERMINATE_IF_NO_NEW_SOLUTION)
	{
		XPRSinterrupt(oprob, XPRS_STOP_TIMELIMIT);
	}
}

void XPRS_CC integerSolution(XPRSprob oprob, void * vd, int soltype, int * ifreject, double *cutoff) {
	VRPmodel * modelInstance;
	modelInstance = (VRPmodel*)vd;
	modelInstance->lastSolutionFound = time(NULL);
}

VRPmodel::VRPmodel(const NodeInstanceDB & db, vector<NodeIRP*> nodes, int period)
	:
	AllNodes(nodes),
	prob("VRP"),
	Database(db),
	Period(period)

{
	initializeSets();
	initializeParameters();
	initializeVariables();
	formulateProblem();
}

int XPRS_CC cbmngVRP(XPRSprob oprob, void * vd)
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



	VRPmodel * modelInstance;
	modelInstance = (VRPmodel*)vd;

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
				//	subtourCut[i].print();
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

				}
				XPRSloadcuts(oprob, 1, -1, -1, NULL);
				//cout << "Stored\n";
			}
		}
	} //end add cut
	modelInstance->getProblem()->endCB();
	return 0;
}


void VRPmodel::useIPSubtourElimination(int cuttype)
{
	CutType = cuttype;
	//Enable subtour elimination
	//prob.setCutMode(1);
	XPRSsetcbcutmgr(oprob, cbmngVRP, &(*this));
	//XPRSsetcbprenode(oprob, preNode, &(*this));
	//XPRSsetcbpreintsol(oprob, acceptInt, &(*this));
}

bool VRPmodel::sepStrongComponents(vector<XPRBcut> & cut)
{
	vector <vector<Node*>> result; //matrix to store strong components
	vector <NodeStrong*> graph;		//Graph to store nodes
	vector<Node*> tempGraph;
	vector<Node*> tempGraph2;

	bool newCut = false;
	subtourIndices.clear();
	matrixValues.clear();
	for (int t : Database.Periods) {
		;
		buildStrongGraph(graph, false); //Do not include depot in graph
		graphAlgorithm::sepByStrongComp(graph, result);
		addSubtourCut(result, newCut, cut);

		for (Node * node : graph) {
			delete node;
		}
		graph.clear();
		result.clear();
	}
	return newCut;
}

void VRPmodel::buildStrongGraph(vector<NodeStrong*>& graph , bool Depot, double weight)
{
	int s;
	double edgeValue;
	int i, j;
	double w;

	if (weight != -1) {
		w = weight;
	}
	else
		w = (double)ModelParameters::EDGE_WEIGHT / 100;

	if (Depot) {
		NodeStrong * node = new NodeStrong(0);
		graph.push_back(node);
	}
	//Create nodes for each visited customer
	for (NodeIRP* node : Nodes) {
		i = node->getId();
		if (y[i].getSol() >= 0.001) {
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
				edgeValue = x[s][endingNode->getId()].getSol();
				if (edgeValue >= w) {
					node->addEdge(edgeValue, endingNode);
				}
			}
		}
	}
}

void VRPmodel::addSubtourCut(vector<vector<Node *>>& strongComp, bool &newCut, vector<XPRBcut> &SubtourCut)
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
				tempNodeVisit = y[node->getId()].getSol();
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

			if (circleFlow >= visitSum - maxVisitSum + (double)ModelParameters::ALPHA / 100) {
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
					rSide += y[node->getId()];
					//y[node->getId()][t].print();
					//cout << "\n";
					rSideStr = rSideStr + " + " + "y_" + to_string(node->getId());

					if (node->getId() != maxVisitID)
					{
						varColumns.push_back(y[node->getId()].getColNum());
						rowValues.push_back(-1);
						rSide -= y[node->getId()];
					}
					else
						rSide += y[node->getId()];

					for (Node *node2 : strongComp[i]) {
						if (Database.inArcSet(node->getId(), node2->getId())) {
							//printf("x_%d%d + ", u, v);
							lSide += x[node->getId()][node2->getId()];

							rowValues.push_back(1);
							//x[node->getId()][edge->getEndNode()->getId()][t].print();
							//cout << "\n";

							varColumns.push_back(x[node->getId()][node2->getId()].getColNum());
						}
					}
				}

				subtourIndices.push_back(varColumns);
				matrixValues.push_back(rowValues);

				rSide -= y[maxVisitID];
				rSideStr = rSideStr + " - " + "y_" + to_string(maxVisitID);
				//cout << rSideStr << "\n";
				nSubtourCuts += 1;

				XPRBcut vv = prob.newCut(lSide <= rSide);
				//vv.print();

				SubtourCut.push_back(vv);


				//SubtourCut.push_back(vv);


				newCut = true;
				maxVisitID = -1;

			}
		}
	}
}




void VRPmodel::solveModel(Solution * prevSol)
{

	oprob = prob.getXPRSprob();
	StartTime = time(NULL);

	//Set time callback
	XPRSsetcbnewnode(oprob, cbmngtimeVRP, &(*this));
	//Set integer solution callback
	XPRSsetcbpreintsol(oprob, integerSolution, &(*this));

	//Set callback for subtour elimination
	useIPSubtourElimination(ModelParameters::GLOBAL_CUTS);

	//prob.print();
	prob.mipOptimise();

	//prob.print();
	SolutionTime = difftime(time(NULL), StartTime);
	prevSol->SolutionTime = SolutionTime;
	updateSolution(prevSol);
}


bool VRPmodel::initializeSets()
{
	for (NodeIRP * node : AllNodes) {
		if (node->getId() != 0) {
			Nodes.push_back(node);
			if (node->isDelivery())
				DeliveryNodes.push_back(node);
			else
				PickupNodes.push_back(node);
		}
	
	}
	return true;
}

bool VRPmodel::initializeParameters()
{
	nVehicles = ModelParameters::nVehicles;
	return true;
}

bool VRPmodel::initializeVariables()
{
	//Loading variables


	//Initialize routing variables, load and penalty variables
	loadDelivery = new XPRBvar *[Database.AllNodes.size()];
	loadPickup = new XPRBvar *[Database.AllNodes.size()];

	x = new XPRBvar * [Database.AllNodes.size()];
	int i; 
	int j;
	for (NodeIRP* node1 : AllNodes) {
		i = node1->getId();
		x[i] = new XPRBvar[Database.AllNodes.size()];

		loadDelivery[i] = new XPRBvar[Database.AllNodes.size()];
		loadPickup[i] = new XPRBvar[Database.AllNodes.size()];

		for (NodeIRP* node2 : AllNodes) {
			j = node2->getId();
			if (node1->inArcSet(node2)) {
				x[i][j] = prob.newVar(XPRBnewname("x%d-%d", i, j), XPRB_BV, 0, 1);

				//pCapacity[i][j] = prob.newVar(XPRBnewname("pCap%d%d", i, j), XPRB_PL, 0, Capacity);
				loadDelivery[i][j] = prob.newVar(XPRBnewname("loadDel_%d%d", i, j), XPRB_PL, 0, Database.Capacity);
				loadPickup[i][j] = prob.newVar(XPRBnewname("loadPic_%d%d", i, j), XPRB_PL, 0, Database.Capacity);
				//loadDelivery[i][j].print();
			}
		}
	}

	y = new XPRBvar[Database.AllNodes.size()];
	for (NodeIRP * node1 : Nodes) {
		i = node1->getId();
		y[i] = prob.newVar(XPRBnewname("y%d", i), XPRB_PL, 0, 1);
	}

	//depot
	y[0] = prob.newVar(XPRBnewname("y%d", 0), XPRB_PL, 0, 4*nVehicles);

	//Ectra vehicles
	extraVehicle = prob.newVar(XPRBnewname("extraVeh", 0), XPRB_PL, 0, 2*nVehicles);


	//Time variables
	timeVar = new XPRBvar[Database.AllNodes.size()];
	for (NodeIRP * node1 : AllNodes) {
		i = node1->getId();
		timeVar[i] = prob.newVar(XPRBnewname("time_%d", i), XPRB_PL, 0, ModelParameters::maxTime);
	}

	return true;
}

bool VRPmodel::formulateProblem()
{
	int i;
	int j;
	//Objective
	objective = 0;
	for (auto node1 : AllNodes)
		for (auto node2 : AllNodes) {
			if (node1->inArcSet(node2)) {
				i = node1->getId();
				j = node2->getId();
				objective += node1->getTransCost(node2) * x[i][j];
			}
		}
	//Avoid mathematical equivalent solution
	for (NodeIRP * node : Nodes){
		objective += loadDelivery[node->getId()][0];
		objective += loadPickup[0][node->getId()];
	}

		

	//Vehicle penalty
	objective += ModelParameters::VehiclePenalty * extraVehicle;

	/*Penalty capacity
	for(int i: Database.AllNodes)
		for (int j : Database.AllNodes) {
			if (map.inArcSet(i, j)) {
				
			}

		}*/

		
	prob.setObj(prob.newCtr("OBJ", objective));  /* Set the objective function */


	/**** CONSTRAINTS ****/
	//Routing constraints
	XPRBexpr p1;
	XPRBexpr p2;
	for (NodeIRP* node1 : AllNodes) {
		i = node1->getId();
		for (NodeIRP* node2 : AllNodes) {
			j = node2->getId();
			if (node1->inArcSet(node2)) {
				p1 += x[i][j];
			}
			if (node2->inArcSet(node1)) {
				p2 += x[j][i];
			}
		}
		prob.newCtr("RoutingFlow", p1 - p2 == 0);
		p1 = 0;
		p2 = 0;
	}
	

	//Linking x and y
	for (NodeIRP* node1 : AllNodes) {
		i = node1->getId();
		for (NodeIRP* node2 : AllNodes) {
			if (node1->inArcSet(node2)) {
				j = node2->getId();
				p1 += x[i][j];
			}
		}
		p2 = y[i];
		prob.newCtr("LinkingArcandVisit", p1 - p2 == 0);
		p1 = 0;
		p2 = 0;
	}
	


	//Depot
	p1 = y[0];
	prob.newCtr("Max vehicles", p1 <= ModelParameters::nVehicles + extraVehicle);
	p1 = 0;
	

	//Max visit
	for (auto node : Nodes) {
		p1 = y[node->getId()];
		prob.newCtr("Max visit", p1 == 1);
		p1 = 0;
	}

	//Quantity
	//Loading constraints
	for (NodeIRP* node1 : DeliveryNodes) {
		i = node1->getId();
		for (NodeIRP* node2 : AllNodes) {
			j = node2->getId();
			if (node2->inArcSet(node1)) {
				p1 += loadDelivery[j][i];
				p2 -= loadPickup[j][i];
			}
			if (node1->inArcSet(node2)) {
				p1 -= loadDelivery[i][j];
				p2 += loadPickup[i][j];
			}
		}
		p1 -= node1->Quantity;

		prob.newCtr("LoadBalance Delivery", p1 == 0);
		prob.newCtr("PickupBalance at deliveryNodes", p2 == 0);
		p1 = 0;
		p2 = 0;
	}



	for (NodeIRP* node1 : PickupNodes) {
		i = node1->getId();
		for (NodeIRP* node2 : AllNodes) {
			j = node2->getId();
			if (node2->inArcSet(node1)) {
				p1 += loadPickup[j][i];
				p2 += loadDelivery[j][i];
			}
			if (node1->inArcSet(node2)) {
				p1 -= loadPickup[i][j];
				p2 -= loadDelivery[i][j];
			}
		}
		p1 += node1->Quantity;
		prob.newCtr("LoadBalance pickup", p1 == 0);
		prob.newCtr("DeliveryBalance at pickupNodes", p2 == 0);
		p1 = 0;
		p2 = 0;
	}

	

	//Arc capacity
	for (NodeIRP* node1 : AllNodes) {
		i = node1->getId();
		for (NodeIRP * node2 : AllNodes) {
			if (node1->inArcSet(node2)) {	
				j = node2->getId();
				p1 = loadDelivery[i][j] + loadPickup[i][j] - Database.Capacity * x[i][j];
				prob.newCtr("ArcCapacity", p1 <= 0);
				p1 = 0;
				
			}
		}
	}

	//Time constraints
	p1 = timeVar[0];
	prob.newCtr("Initial time", p1 == 0);
	p1 = 0;


	for (NodeIRP* node1 : AllNodes) {
		i = node1->getId();
		for (NodeIRP* node2 : Nodes) {
			if (node1->inArcSet(node2)) {
				j = node2->getId();
				p1 = timeVar[i] - timeVar[j] + node1->getTravelTime(node2)
					+ (ModelParameters::maxTime + node1->getTravelTime(node2)) * x[i][j];

				p2 = ModelParameters::maxTime + node1->getTravelTime(node2);
				prob.newCtr("Time flow", p1 <= p2);
				p1 = 0;
				p2 = 0;

				p1 = timeVar[i] + node1->getTravelTime(node2) * x[i][j];
			
				//prob.newCtr("Final time", p1 <= ModelParameters::maxTime );
				p1 = 0;
			}

		}

		if (node1->inArcSet(Database.getDepot())) {
			p1 = timeVar[i] + node1->getTravelTime(Database.getDepot()) * x[i][0];
			prob.newCtr("Final time", p1 <= ModelParameters::maxTime);
			p1 = 0;
		}	
	}
	

	//Valid inequalitites
	/*
	for (int i : Database.AllNodes) {
		for (int j : Database.AllNodes) {
			if (map.inArcSet(i, j))
				p1 += TravelTime[i][j] * x[i][j];
		}
	}

	p2 = ModelParameters::maxTime*(y[0] + extraVehicle);
	prob.newCtr("Totaltime", p1 <= p2);

	p1 = 0;
	p2 = 0;
	*/
	return false;
}

VRPmodel::~VRPmodel()
{
	//Delete the variables
	clearVariables();
}

void VRPmodel::updateSolution(Solution * sol)
{
	//If an exisiting solution is defined
	if (sol != 0) {
		int i;
		int j;
		//Add load variables
		for (NodeIRPHolder* node1 : sol->Nodes)
			//Clear old arcs
			node1->NodePeriods[Period]->deleteEdges();

		for (NodeIRP* node1 : AllNodes) {
			i = node1->getId();
	
			for (NodeIRP* node2 : AllNodes) {
				if (node1->inArcSet(node2)) {
					j = node2->getId();
				
					if (x[i][j].getSol() > 0.001)
						//get solution from VRP
						node1->addEdge(loadDelivery[i][j].getSol(), loadPickup[i][j].getSol(), node2, x[i][j].getSol());
				}


			}
		}

		//Add time variables
		for (auto node : Nodes) {
			i = node->getId();
			node->TimeServed = timeVar[i].getSol();
		}
	}

}

XPRBprob* VRPmodel::getProblem()
{
	return &prob;
}


void VRPmodel::clearVariables()
{
	int i;
	for (auto node1 : AllNodes) {
		i = node1->getId();
		delete[] x[i];
		delete[] loadDelivery[i];
		delete[] loadPickup[i];
	}

	delete[] x;
	delete [] timeVar;
	delete[] y;
	delete[] loadDelivery;
	delete[] loadPickup;
}

/*void VRPmodel::addRoutesToIRP(IRP & instance, int t,  IRP::Solution * sol)
{

	//Add unique routes to IRP
	//Check if route exist.
	/*for (vector <Node*> r : routes) {
		vector <IRP::Route const *> routePtr = instance.getRoutes();
		int i = 0;
		while (routePtr.size() > 0 && i < r.size()) {
			i = 0;
			Node* u = r[i];
			for (int j = 0; j < routePtr.size(); j++) {
				if (u->getId() != routePtr[j]->route[i]->getId()) {
					routePtr.erase(routePtr.begin() + j);
				}
			}
			i++;
		}
		//No equal routes
		if (routePtr.size() == 0){
			instance.newRoute(r);
		}

	}

	for (int i = 0; i < routes.size(); i++) {
		graphAlgorithm::printGraph(instance.getRoute(i)->route, instance, "route"+to_string(i));

	}
}*/
		
