#include "stdafx.h"
#include "VRPmodel.h"
#include "ModelParameters.h"


VRPmodel::VRPmodel(CustomerVRPDB &db, Map &map)
	:
	database(db),
	map(map),
	prob("VRP")
{
	initializeSets();
	initializeParameters();
	initializeVariables();

	formulateProblem();

}

void VRPmodel::solveModel()
{

	//prob.lpOptimize();
	//int b = prob.getLPStat();

	int d = prob.mipOptimise();
	//prob.print();
	for (int i : AllNodes)
	{
		y[i].print();
		printf("\t");
		for (int j : AllNodes) {
			if (map.inArcSet(i, j))
				if (x[i][j].getSol() > 0.1) {
					x[i][j].print();
					printf("\t");
				}
		}
		
			/*(loadDelivery[i][j][t]).print();
			printf("\t");
			(loadPickup[i][j][t]).print();
			printf("\t");
			}
			}*/

			time[i].print();
			printf("\n");


		
	}
	int c = prob.getObjVal();
}


bool VRPmodel::initializeSets()
{
	vector <Customer *> * customers = database.getCustomers();

	int node = 1;
	for (Customer * c : (*customers)) {
		CustomerVRP * cust = database.getCustomer(c->getId());
		if (cust->getDemand(Customer::DELIVERY) > 0)
			DeliveryNodes.push_back(map.getDeliveryNode(cust));
			
		if (cust->getDemand(Customer::PICKUP) > 0)
			PickupNodes.push_back(map.getPickupNode(cust));
	}

	ModelBase::createUnion(DeliveryNodes, PickupNodes, Nodes);
	ModelBase::createRangeSet(0, 0, Depot);
	ModelBase::createUnion(Depot, Nodes, AllNodes);
	return true;
}

bool VRPmodel::initializeParameters()
{

	Demand = new int [Nodes.back()];
	int cust;

	for (int i : DeliveryNodes) {
		cust = map.nodeToCustomer(i);
		Demand[i] = database.getDemand(cust, Customer::DELIVERY);
		//printf("%-10d", Demand[i]);
			
		}

	for (int i : PickupNodes) {
		cust = map.nodeToCustomer(i);
		Demand[i] = database.getDemand(cust, Customer::PICKUP);
	} // end demand

	TransCost = new int *[AllNodes.back()];
	for (int i : AllNodes) {
		//printf("\n");
		TransCost[i] = new int[AllNodes.back()];
		for (int j : AllNodes)
			if (map.inArcSet(i, j))
			{
				TransCost[i][j] = map.getTransCost(i, j, ModelParameters::TRANSCOST_MULTIPLIER, ModelParameters::SERVICECOST_MULTIPLIER);
				//printf("%d\t", TransCost[i][j]);
			}
			else {
				TransCost[i][j] = 0;
				//printf("%d\t", TransCost[i][j]);
			}
	} // end transcost

	//TravelTime
	TravelTime = new int *[AllNodes.back()];
	for (int i : AllNodes) {
		TravelTime[i] = new int[AllNodes.back()];
		for (int j : AllNodes)
			if (map.inArcSet(i, j)) {
				TravelTime[i][j] = map.getTravelTime(i, j, ModelParameters::TRAVELTIME_MULTIPLIER, ModelParameters::SERVICETIME);
			}
	}

	Capacity = ModelParameters::Capacity;
	MaxTime = ModelParameters::maxTime;
	nVehicles = ModelParameters::nVehicles;

	return true;
}


bool VRPmodel::initializeVariables()
{
	//Loading variables


	//Initialize routing variables and load
	loadDelivery = new XPRBvar *[AllNodes.back()];
	loadPickup = new XPRBvar *[AllNodes.back()];

	x = new XPRBvar * [AllNodes.back()];

	for (int i : AllNodes) {
		x[i] = new XPRBvar[AllNodes.back()];

		loadDelivery[i] = new XPRBvar[AllNodes.back()];
		loadPickup[i] = new XPRBvar[AllNodes.back()];

		for (int j : AllNodes) {
			if (map.inArcSet(i, j)) {
				x[i][j] = prob.newVar(XPRBnewname("x%d-%d", i, j), XPRB_BV, 0, 1);

				loadDelivery[i][j] = prob.newVar(XPRBnewname("loadDel_%d%d", i, j), XPRB_PL, 0, Capacity);
				loadPickup[i][j] = prob.newVar(XPRBnewname("loadPic_%d%d", i, j), XPRB_PL, 0, Capacity);
				//loadDelivery[i][j].print();
			}
		}
	}

	y = new XPRBvar[AllNodes.back()];
	for (int i : Nodes) {
		y[i] = prob.newVar(XPRBnewname("y%d", i), XPRB_PL, 0, 1);
	}

	//depot
	y[0] = prob.newVar(XPRBnewname("y%d", 0), XPRB_PL, 0, nVehicles);



	//Time variables
	time = new XPRBvar[AllNodes.back()];
	for (int i : AllNodes) {
		time[i] = prob.newVar(XPRBnewname("time_%d", i), XPRB_PL, 0, MaxTime);
	}

	return true;
}

bool VRPmodel::formulateProblem()
{
	
	//Objective
	objective = 0;
	for (int i : AllNodes)
		for (int j : AllNodes) {
			if (map.inArcSet(i, j)) {
				objective += TransCost[i][j] * x[i][j];
				int a = TransCost[i][j];
				printf("%d\n", a);
			}
			}
	prob.setObj(prob.newCtr("OBJ", objective));  /* Set the objective function */


	/**** CONSTRAINTS ****/
	//Routing constraints
	XPRBexpr p1;
	XPRBexpr p2;
	for (int i : AllNodes) {
		for (int j : AllNodes) {
			if (map.inArcSet(i, j)) {
				p1 += x[i][j];
			}
			if (map.inArcSet(j, i)) {
				p2 += x[j][i];
			}
		}
		prob.newCtr("RoutingFlow", p1 - p2 == 0);
		p1 = 0;
		p2 = 0;
	}
	

	//Linking x and y
	for (int i : AllNodes) {
		for (int j : AllNodes) {
			if (map.inArcSet(i, j)) {
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
	prob.newCtr("Max vehicles", p1 <= ModelParameters::nVehicles);
	p1 = 0;
	

	//Max visit
	for (int i : Nodes) {
		p1 = y[i];
		prob.newCtr("Max visit", p1 = 1);
		p1 = 0;
	}

	//Quantity
	//Loading constraints
	for (int i : DeliveryNodes) {
		for (int j : AllNodes) {
			if (map.inArcSet(j, i)) {
				p1 += loadDelivery[j][i];
				p2 -= loadPickup[j][i];
			}
			if (map.inArcSet(i, j)) {
				p1 -= loadDelivery[i][j];
				p2 += loadPickup[i][j];
			}
		}
		p1 -= Demand[i];

		prob.newCtr("LoadBalance Delivery", p1 == 0);
		prob.newCtr("PickupBalance at deliveryNodes", p2 == 0);
		p1 = 0;
		p2 = 0;
	}



	for (int i : PickupNodes) {
		for (int j : AllNodes) {
			if (map.inArcSet(j, i)) {
				p1 += loadPickup[j][i];
				p2 += loadDelivery[j][i];
			}
			if (map.inArcSet(i, j)) {
				p1 -= loadPickup[i][j];
				p2 -= loadDelivery[i][j];
			}
		}
		p1 += Demand[i];
		printf("%d\n", Demand[i]);
		prob.newCtr("LoadBalance pickup", p1 == 0);
		prob.newCtr("DeliveryBalance at pickupNodes", p2 == 0);
		p1 = 0;
		p2 = 0;
	}

	

	//Arc capacity
	for (int i : AllNodes) {
		for (int j : AllNodes) {
			if (map.inArcSet(i, j)) {			
				p1 = loadDelivery[i][j] + loadPickup[i][j] - ModelParameters::Capacity * x[i][j];
				prob.newCtr("ArcCapacity", p1 <= 0);
				p1 = 0;
				
			}
		}
	}

	//Time constraints
	p1 = time[0];
	prob.newCtr("Initial time", p1 == 0);
	p1 = 0;


	for (int i : AllNodes) {
		for (int j : Nodes) {
			if (map.inArcSet(i, j)) {
				p1 = time[i] - time[j] + TravelTime[i][j]
					+ (ModelParameters::maxTime + TravelTime[i][j]) * x[i][j];

				p2 = ModelParameters::maxTime + TravelTime[i][j];
				prob.newCtr("Time flow", p1 <= p2);
				p1 = 0;
				p2 = 0;
			}

		}

		if (map.inArcSet(i, 0)) {
			p1 = time[i] + TravelTime[i][0] * x[i][0];
			prob.newCtr("Final time", p1 <= ModelParameters::maxTime);
			p1 = 0;
		}
		
	}
	


	return false;
}

VRPmodel::~VRPmodel()
{
}

void VRPmodel::addToIRPSolution(int t, IRP::Solution * sol, IRP &instance)
{
	//Add arc and load variables
	for (int i : AllNodes) {
		sol->ySol[i][t] = y[i].getSol();
		for (int j : AllNodes) {
			if (map.inArcSet(i, j)) {
				//get solution from VRP
				sol->xSol[i][j][t] = x[i][j].getSol();
				x[i][j].print();
				cout << "\n";
				sol->loadDelSol[i][j][t] = (int)loadDelivery[i][j].getSol();
				int a = (int)loadPickup[i][j].getSol();
				sol->loadPickSol[i][j][t] = (int)loadPickup[i][j].getSol();
			}
		
		}
	}

	//Add time variables
	for (int i : Nodes) {
		sol->timeSol[i][t] = time[i].getSol();
		}
}

void VRPmodel::addRoutesToIRP(IRP & instance, int t,  IRP::Solution * sol)
{
	vector<Node*> graph;
	vector<vector<Node*>> routes;
	instance.buildGraph(graph, t, sol);

	
	graphAlgorithm::getRoutes(graph, routes);

	//Check if route exist.
	for (vector <Node*> r : routes) {
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
}
		
