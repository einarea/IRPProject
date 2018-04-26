#include "stdafx.h"
#include "RouteProblem.h"




RouteProblem::~RouteProblem()
{
	//Delete the variables
	clearVariables();
}



RouteProblem::RouteProblem(const NodeInstanceDB & model, vector <Route *> r)
	:
	Instance(model),
	routes(r),
	routeProblem("RouteProblem")
{
	initializeRouteVariables();
}

void RouteProblem::formulateRouteProblem(int minimizeSelection)
{
	//initialize route matrix
	initializeRouteParameters();
	initializeRoutes();
	//printRouteMatrix();
	addInventoryCtr();
	addRouteConstraints();

	XPRBexpr obj = 0;

	int i;
	//To get practical right answer
	for (auto node : Instance.Nodes) {
		i = node->getId();
		for (auto t : Instance.Periods) {
			obj += 0.01 * loadPickup[0][i][t];
		}
	}

	//Minimize the total cost
	if (minimizeSelection == ModelParameters::HIGHEST_TOTALCOST) {

		int t = 3;
		//Trans cost
		for (int r : Routes)
			for (int t : Instance.Periods)
				obj += RouteCost[r] * travelRoute[r][t];

		//Holding cost
		for (auto node : Instance.Nodes) {
			i = node->getId();
			for (int t : Instance.Periods)
				obj += node->HoldingCost * inventory[i][t];
		}
		routeProblem.setObj(routeProblem.newCtr("OBJ", obj));  /* Set the objective function */

	}

	//Minimize the service in the period
	if (minimizeSelection == ModelParameters::MIN_SERVICE) {
		XPRBexpr obj = 0;
		int t = ShiftPeriod;
		for (auto nodeDel : Instance.DeliveryNodes) {
			i = nodeDel->getId();
			obj += delivery[i][t];
		}
		for (auto nodePick : Instance.PickupNodes) {
			i = nodePick->getId();
			obj += pickup[i][t];
		}

		routeProblem.setObj(routeProblem.newCtr("OBJ", obj));  /* Set the objective function */
	}

	if (minimizeSelection == ModelParameters::MinimumInventory) {
		XPRBexpr obj = 0;
		for (auto node : Instance.Nodes) {
			i = node->getId();
			for (int t : Instance.Periods)
				obj += inventory[i][t];
		}

		routeProblem.setObj(routeProblem.newCtr("OBJ", obj));  /* Set the objective function */

	}


}

void RouteProblem::initializeRouteParameters()
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
	for (auto r : Routes)
		A[r] = routes[r]->getRouteMatrix();
}

void RouteProblem::initializeRouteVariables()
{
	int i, j;
	loadDelivery = new XPRBvar **[Instance.AllNodes.size()];
	loadPickup = new XPRBvar **[Instance.AllNodes.size()];

	for (auto node1 : Instance.AllNodes) {
		i = node1->getId();
		loadDelivery[i] = new	XPRBvar *[Instance.AllNodes.size()];
		loadPickup[i] = new	XPRBvar *[Instance.AllNodes.size()];

		for (auto node2 : Instance.AllNodes) {
			if (node1->inArcSet(node2)) {
				j = node2->getId();
				loadDelivery[i][j] = new XPRBvar[Instance.Periods.size()];
				loadPickup[i][j] = new XPRBvar[Instance.Periods.size()];

				for (int t : Instance.Periods) {
					loadDelivery[i][j][t] = routeProblem.newVar(XPRBnewname("lD_%d%d%d", i, j, t), XPRB_PL, 0, Instance.Capacity);
					loadPickup[i][j][t] = routeProblem.newVar(XPRBnewname("lP_%d%d%d", i, j, t), XPRB_PL, 0, Instance.Capacity);
					
				}
			}
		}
	} // end initializing of load

	  //Initialize inventory
	inventory = new XPRBvar *[Instance.AllNodes.size()];

	for (auto node : Instance.Nodes) {
		i = node->getId();
		inventory[i] = new XPRBvar[Instance.Periods.size()];
		for (int t : Instance.AllPeriods) {
			inventory[i][t] = routeProblem.newVar(XPRBnewname("i_%d%d", i, t), XPRB_PL, node->LowerLimit, node->UpperLimit);
		}
	}

	//Initialize at delivery nodes
	delivery = new XPRBvar *[Instance.AllNodes.size()];
	for (auto node : Instance.DeliveryNodes) {
		i = node->getId();
		delivery[i] = new XPRBvar[Instance.Periods.size() + 1];
		for (int t : Instance.Periods) {
			delivery[i][t] = routeProblem.newVar(XPRBnewname("qD_%d%d", i, t), XPRB_PL, 0, Instance.Capacity);

		}
	}

	//Initialize at pickup nodes
	pickup = new XPRBvar *[Instance.AllNodes.size()];
	for (auto node : Instance.PickupNodes) {
		i = node->getId();
		pickup[i] = new  XPRBvar[Instance.Periods.size() + 1];
		for (int t : Instance.Periods) {
			pickup[i][t] = routeProblem.newVar(XPRBnewname("qP_%d%d", i, t), XPRB_PL, 0, Instance.Capacity);
		}
	}

}

void RouteProblem::addInventoryCtr()
{
	XPRBexpr p1;
	XPRBexpr p2;
	int i, j;
	//Inventory constraints
	for (auto node : Instance.Nodes) {
		i = node->getId();
		p1 = inventory[i][0];
		//printf("%d", InitInventory[i]);
		routeProblem.newCtr("Initial inventory", inventory[i][0] == node->InitInventory);
		p1 = 0;
	}

	for (auto node : Instance.DeliveryNodes) {
		i = node->getId();
		for (int t : Instance.Periods) {
			p1 = inventory[i][t] - inventory[i][t - 1] + node->Demand[t] - delivery[i][t];
			routeProblem.newCtr("Delivery Inventory balance", p1 == 0);
			p1 = 0;

			p1 = inventory[i][t - 1] + delivery[i][t];
			routeProblem.newCtr("Delivery Inventory balance 2", p1 <= node->UpperLimit);
			p1 = 0;
		}
	}

	for (auto node : Instance.PickupNodes) {
		i = node->getId();
		for (int t : Instance.Periods) {
			//printf("%d", Demand[i][t]);
			p1 = inventory[i][t] - inventory[i][t - 1] - node->Demand[t] + pickup[i][t];
			routeProblem.newCtr("Pickup Inventory balance", p1 == 0);
			p1 = 0;

			p1 = inventory[i][t - 1] - pickup[i][t];
			routeProblem.newCtr("Pickup Inventory balance 2", p1 >= node->LowerLimit);
			p1 = 0;
		}
	}

	//Upper and lower inventory
	for (auto node: Instance.Nodes) {
		i = node->getId();
		for (int t : Instance.Periods) {
			p1 = inventory[i][t];
			routeProblem.newCtr("Inventory Lower Limit", p1 >= node->LowerLimit);
			routeProblem.newCtr("Inventory Upper Limit", p1 <= node->UpperLimit);
			p1 = 0;
		}
	}

	//Loading constraints
	for (auto nodeDel : Instance.DeliveryNodes) {
		i = nodeDel->getId();
		for (int t : Instance.Periods) {
			for (auto node : Instance.AllNodes) {
				j = node->getId();
				if (node->inArcSet(nodeDel)) {
					p1 += loadDelivery[j][i][t];
					p2 -= loadPickup[j][i][t];
				}
				if (nodeDel->inArcSet(node)) {
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

	for (auto nodePick : Instance.PickupNodes) {
		i = nodePick->getId();
		for (int t : Instance.Periods) {
			for (auto node : Instance.AllNodes) {
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

			routeProblem.newCtr("LoadBalance pickup", p1 == 0);
			routeProblem.newCtr("DeliveryBalance at pickupNodes", p2 == 0);
			p1 = 0;
			p2 = 0;
		}

	}

}

int RouteProblem::getShiftPeriod()
{
	return ShiftPeriod;
}

/*void RouteProblem::shiftQuantityCtr(int quantityPic, int period)
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


void RouteProblem::initializeRoutes()
{

	//Initialize travel route variables
	travelRoute = new XPRBvar *[Routes.size()];
	for (auto r : Routes) {
		travelRoute[r] = new XPRBvar[Instance.Periods.size() + 1];
		for (auto t : Instance.Periods)
			travelRoute[r][t] = routeProblem.newVar(XPRBnewname("x%d%d", r, t), XPRB_BV, 0, 1);
	}
}



void RouteProblem::addRouteConstraints()
{
	XPRBexpr p1 = 0;
	XPRBexpr p2 = 0;
	bool ind = false;
	int i, j;

	//Arc capacity
	for (auto node1 : Instance.AllNodes) {
		i = node1->getId();
		for (auto node2 : Instance.AllNodes) {
			if (node1->inArcSet(node2)) {
				j = node2->getId();
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
	}
	//end arc capacity

	//Service limit
	for (int t : Instance.Periods) {

		//For all pickup nodes
		for (auto nodePick : Instance.PickupNodes) {
			i = nodePick->getId();
			for (auto node : Instance.AllNodes) {
				if (nodePick->inArcSet(node)) {
					j = node -> getId();
					for (auto r : Routes) {
						p1 += A[r][i][j] * travelRoute[r][t];
					}
				}
			}

			p1 = p1 * min(Instance.Capacity, nodePick->UpperLimit - nodePick->LowerLimit);
			p2 = pickup[i][t];
			routeProblem.newCtr("Pickup limit", p2 - p1 <= 0);

			p1 = 0;
			p2 = 0;

		}
		//For all delivery nodes
		for (auto nodeDel : Instance.DeliveryNodes) {
			i = nodeDel->getId();
			for (auto node : Instance.AllNodes) {
				if (nodeDel->inArcSet(node)) {
					j = node->getId();
					for (auto r : Routes) {
						p1 += A[r][i][j] * travelRoute[r][t];
					}
				}
			}

			p1 = p1 * min(Instance.Capacity, nodeDel->UpperLimit - nodeDel->LowerLimit);
			p2 = delivery[i][t];
			routeProblem.newCtr("Delivery limit", p2 - p1 <= 0);

			p1 = 0;
			p2 = 0;
		}
	}//end service limit

	 //Visit constraint
	int counter = 0;
	for (auto node1 : Instance.Nodes) {
		i = node1->getId();
		for (int t : Instance.Periods) {
			counter = 0;
			ind = false;
			for (auto node2 : Instance.AllNodes) {
				if (node1->inArcSet(node2)) {
					j = node2->getId();
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


void RouteProblem::printRouteMatrix()
{
	for (int r = 0; r < A.size(); r++) {
		cout << "\n\n\n";
		for (int i = 0; i < Instance.AllNodes.size(); i++) {
			cout << "\n";
			for (int j = 0; j < Instance.AllNodes.size(); j++) {
				cout << A[r][i][j] << "  ";

			}
		}
	}
}

void RouteProblem::addRoutesToVector()
{
	//Update set of routes
	for (Route *r : routes) {
		Routes.push_back(r->getId());
		A[r->getId()] = r->getRouteMatrix();
	}

}

void RouteProblem::updateSolution(Solution * sol)
{

	vector<NodeIRPHolder*> &nodeHolder = sol->getNodes();
	//Fill inventory and quantity at the nodes
	fillNodes(nodeHolder);

	//Fill load
	fillLoad(nodeHolder);
}

void RouteProblem::lockRoutes(vector<int> Periods)
{

	//Select what routes to lock
	vector<int> counter;
	counter.resize(Instance.Periods.size() + 1);

	for (auto t : Periods)
		counter[t] = 0;

	for (auto route : routes) {
		lockRoute(route);
		counter[route->getPeriod()]++;
	}
	//Lock the number of routes in a period
	XPRBexpr p1 = 0;

	for (auto t : Periods) {
		for (auto r : Routes) {
			p1 += travelRoute[r][t];
		}
		routeProblem.newCtr("Number of routes", p1 <= counter[t]);
		p1 = 0;
	}
}


void RouteProblem::clearVariables()
{
	int i, j;
	for (auto node1 : Instance.AllNodes) {
		i = node1->getId();
		for (auto node2 : Instance.AllNodes) {
			j = node2->getId();
			delete[] loadDelivery[i][j];
			delete[] loadPickup[i][j];

		}
		delete[] loadDelivery[i];
		delete[] loadPickup[i];
		delete[] inventory[i];
		delete[] delivery[i];
		delete[] pickup[i];
	}

	delete[] delivery;
	delete[] pickup;
	delete[] inventory;
	delete[] loadDelivery;
	delete[] loadPickup;

	for (int r : Routes)
		delete[] travelRoute[r];

	delete[] travelRoute;
}

void RouteProblem::lockRoute(Route * route)
{
	int routeId = route->getId();
	int period = route->getPeriod();
	routeProblem.newCtr("RouteLock", travelRoute[routeId][period] == 1);
}

Solution * RouteProblem::solveProblem(Solution * sol)
{
	routeProblem.mipOptimise();
	//If no solution, allocate a new solution
	if (sol == 0) {
		sol = Solution::allocateSolution(Instance);
	}

	updateSolution(sol);
	return sol;
}


int RouteProblem::getRoutePosition(int routeId)
{
	for (int i : Routes)
	{
		if (routes[i]->getId() == routeId)
			return i;
	}
}


void RouteProblem::fillRoutes(vector<vector<Route* >>& routeHolder)
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



void RouteProblem::fillNodes(vector <NodeIRPHolder*> &nodeHolder)
{

	for (auto node : nodeHolder) {

		for (auto t : Instance.Periods) {
			node->NodePeriods[t]->deleteEdges();
			int i = node->getId();
			//Fill inventory
			if (node->getId() != 0)
				node->NodePeriods[t]->Inventory = inventory[i][t].getSol();

			//Fill quantity
			if (node->getId() != 0) {
				if (node->isDelivery()) {
					node->NodePeriods[t]->Quantity = delivery[i][t].getSol();
					double a = delivery[i][t].getSol();
					int b = 0;
				}
				else
					node->NodePeriods[t]->Quantity = pickup[i][t].getSol();
			}

		}
	}
}

void RouteProblem::fillLoad(vector <NodeIRPHolder*> &nodeHolder) {

	int i;
	int j;
	double loadDel;
	double loadPick;
	NodeIRP::EdgeIRP * edge;

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
					nodeHolder[i]->NodePeriods[t]->addEdge(loadDel, loadPick, nodeHolder[j]->NodePeriods[t], 1);
				}

			}

		}
}
