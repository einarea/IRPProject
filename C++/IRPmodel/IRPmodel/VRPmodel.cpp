#include "stdafx.h"
#include "VRPmodel.h"
#include "ModelParameters.h"


//Time callback
void XPRS_CC cbmngtimeVRP(XPRSprob oprob, void * vd, int parent, int newnode, int branch) {

	VRPmodel * modelInstance;
	modelInstance = (VRPmodel*)vd;

	if (difftime(time(NULL), modelInstance->StartTime) >= ModelParameters::MAX_RUNNING_TIME_VRP)
	{
		XPRSinterrupt(oprob, XPRS_STOP_TIMELIMIT);
	}
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


void VRPmodel::solveModel(Solution * prevSol)
{

	oprob = prob.getXPRSprob();
	StartTime = time(NULL);

	//Set time callback
	XPRSsetcbnewnode(oprob, cbmngtimeVRP, &(*this));

	//prob.print();
	int d = prob.mipOptimise();

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
	pCapacity = new XPRBvar *[Database.AllNodes.size()];

	x = new XPRBvar * [Database.AllNodes.size()];
	int i; 
	int j;
	for (NodeIRP* node1 : AllNodes) {
		i = node1->getId();
		x[i] = new XPRBvar[Database.AllNodes.size()];

		pCapacity[i] = new XPRBvar[Database.AllNodes.size()];
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
		timeVar[i] = prob.newVar(XPRBnewname("time_%d", i), XPRB_PL, 0, MaxTime);
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
				p1 = timeVar[i] - timeVar[j] + node1->getTravelTime(node2);
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


void VRPmodel::clearVariables()
{
	int i, j;
	for (auto node1 : AllNodes) {
		i = node1->getId();
		delete[] x[i];
		delete[] loadDelivery[i];
		delete[] loadPickup[i];
	}
	
	delete[] x;
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
		
