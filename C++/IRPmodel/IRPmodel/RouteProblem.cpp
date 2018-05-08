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
		RouteCost[nRoutes] = r->getTransCost();
		//Initialize the A holder
		//Find max routes
		nRoutes++;
	}

	A.resize(nRoutes);
	for (auto r : Routes)
		A[r] = routes[r]->getRouteMatrix(Instance.AllNodes.size());
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
				loadDelivery[i][j] = new XPRBvar[Instance.AllPeriods.size()];
				loadPickup[i][j] = new XPRBvar[Instance.AllPeriods.size()];

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
		inventory[i] = new XPRBvar[Instance.AllPeriods.size()];
		for (int t : Instance.AllPeriods) {
			inventory[i][t] = routeProblem.newVar(XPRBnewname("i_%d%d", i, t), XPRB_PL, node->LowerLimit, node->UpperLimit);
		}
	}

	//Initialize at delivery nodes
	delivery = new XPRBvar *[Instance.AllNodes.size()];
	for (auto nodeDel : Instance.DeliveryNodes) {
		i = nodeDel->getId();
		delivery[i] = new XPRBvar[Instance.Periods.size() + 1];
		for (int t : Instance.Periods) {
			delivery[i][t] = routeProblem.newVar(XPRBnewname("qD_%d%d", i, t), XPRB_PL, 0, min(Instance.Capacity, nodeDel->UpperLimit - nodeDel->LowerLimit));

		}
	}

	//Initialize at pickup nodes
	pickup = new XPRBvar *[Instance.AllNodes.size()];
	for (auto nodePick : Instance.PickupNodes) {
		i = nodePick->getId();
		pickup[i] = new  XPRBvar[Instance.Periods.size() + 1];
		for (int t : Instance.Periods) {
			pickup[i][t] = routeProblem.newVar(XPRBnewname("qP_%d%d", i, t), XPRB_PL, 0, min(Instance.Capacity, nodePick->UpperLimit - nodePick->LowerLimit));
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
	loadingCtr.resize(Instance.AllPeriods.size());
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

			loadingCtr[t].push_back(routeProblem.newCtr("LoadBalance Delivery", p1 == 0));
			loadingCtr[t].push_back(routeProblem.newCtr("PickupBalance at deliveryNodes", p2 == 0));
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

			loadingCtr[t].push_back(routeProblem.newCtr("LoadBalance pickup", p1 == 0));
			loadingCtr[t].push_back(routeProblem.newCtr("DeliveryBalance at pickupNodes", p2 == 0));
			p1 = 0;
			p2 = 0;
		}

	}

}

void RouteProblem::addChangeCtr()
{
	XPRBexpr p2;
	int i, j;
	int nRoutes= 0;
	for (int r : Routes) {
		if (routes[r]->State == ModelParameters::ORIG_ROUTE) {
			nRoutes++;
			for (int t : Instance.Periods) {
				{
					p2 += travelRoute[r][t];
				}
			}
		}
	}

	routeProblem.newCtr("Change", p2 <= nRoutes - 1);
}

void RouteProblem::printRouteType()
{
	for(int r : Routes)
		for (int t : Instance.Periods) {
			if (travelRoute[r][t].getSol() >= 0.01) {
				switch (routes[r]->State) {
				case ModelParameters::ORIG_ROUTE: {

				}
				}
			}
		}
}

int RouteProblem::getShiftPeriod()
{
	return ShiftPeriod;
}

void RouteProblem::formulateMinVisitProblem()
{
	//Initialize variables
	delta = new XPRBvar[Instance.Nodes.size()];
	for(auto node : Instance.Nodes)
		delta[node->getId()] = routeProblem.newVar(XPRBnewname("delta_%d", node->getId()), XPRB_BV, 0, 1);

	//Add constraints
	XPRBexpr p1 = 0;
	int i;
	for (auto nodeDel : Instance.DeliveryNodes) {
		i = nodeDel->getId();
		p1 = delivery[i][ShiftPeriod] - min(Instance.Capacity, nodeDel->UpperLimit - nodeDel->LowerLimit)*delta[i];

		routeProblem.newCtr("Logical visit", p1 <= 0);
	}

	p1 = 0;

	for (auto nodePick : Instance.PickupNodes) {
		i = nodePick->getId();
		p1 = pickup[i][ShiftPeriod] - min(Instance.Capacity, nodePick->UpperLimit - nodePick->LowerLimit)*delta[i];

		routeProblem.newCtr("Logical visit", p1 <= 0);
	}

	p1 = 0;

	//Set objective
	for (auto node : Instance.Nodes) {
		i = node->getId();
		p1 += delta[i];
	}
	//Add small value to minimize quantity
	for (auto node : Instance.DeliveryNodes) {
		i = node->getId();
		p1 += 0.0001 * delivery[i][ShiftPeriod];
	}

	for (auto node : Instance.PickupNodes) {
		i = node->getId();
		p1 += 0.0001 * pickup[i][ShiftPeriod];
	}

	routeProblem.setObj(p1);
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
			travelRoute[r][t] = routeProblem.newVar(XPRBnewname("travel%d%d", r, t), XPRB_BV, 0, 1);
	}
}

void RouteProblem::updateEdges(Solution * sol)
{
	double loadDel;
	double loadPick;
	//clear edges
	for (NodeIRPHolder * node : sol->Nodes) {
		node->NodePeriods[ShiftPeriod]->deleteEdges();
	}

	//add edges
	int i, j;
	for (auto node1 : AllNodes) {
		i = node1->getId();
		for (auto node2 : AllNodes) {
			if (node1->inArcSet(node2)) {
				j = node2->getId();

				if (x[i][j].getSol() >= 0.001) {
					//Fill load
					loadDel = loadDelivery[i][j][ShiftPeriod].getSol();
					loadPick = loadPickup[i][j][ShiftPeriod].getSol();
					sol->Nodes[i]->NodePeriods[ShiftPeriod]->addEdge(loadDel, loadPick, sol->Nodes[j]->NodePeriods[ShiftPeriod], 1);

				}
			}

		}
	}
}



void RouteProblem::addRouteConstraints()
{
	XPRBexpr p1 = 0;
	XPRBexpr p2 = 0;
	bool ind = false;
	int i, j;

	ArcCtr.resize(Instance.AllPeriods.size());

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
					ArcCtr[t].push_back(routeProblem.newCtr("Arc capacity limit", p2 <= Instance.Capacity * p1));
					//ff.print();
					p1 = 0;
					p2 = 0;
				}
			}
		}
	}
	//end arc capacity


	//Visit constraint
	VisitCtr.resize(Instance.AllPeriods.size());

	for (auto node1 : Instance.Nodes) {
		i = node1->getId();
		for (int t : Instance.Periods) {

			ind = false;
			for (auto node2 : Instance.AllNodes) {
				if (node1->inArcSet(node2)) {
					j = node2->getId();
					for (int r : Routes) {

						if (A[r][i][j] > 0.01) {
							p1 += A[r][i][j] * travelRoute[r][t];
		
							ind = true;
						}
					}
				}
			}

			if (ind) {
				VisitCtr[t].push_back(routeProblem.newCtr("Visit limit", p1 <= 1));

			}
			p1 = 0;
		}
	} //end visit constraint

	//Service limit
	/*for (int t : Instance.Periods) {

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
	}//end service limit*/




	  //Vehicle constraint
	for (int t : Instance.Periods) {
		for (int r : Routes) {
			p1 += travelRoute[r][t];
		}

		routeProblem.newCtr("Vehicle limit", p1 <= ModelParameters::nVehicles + ModelParameters::VehiclePenalty*extraVehicle);
		p1 = 0;
	}

}

void RouteProblem::addRestrictedShiftCtr(double nRoutes, double oldDel, double oldPick)
{
	XPRBexpr p1 = 0;
	int i;

	for (auto nodeDel : Instance.DeliveryNodes) {
		i = nodeDel->getId();
		p1 += delivery[i][ShiftPeriod];
	}

	routeProblem.newCtr("Delivery minimum", p1 >= floor(oldDel/Instance.Capacity)*Instance.Capacity - ModelParameters::SLACK/100*Instance.Capacity);
	p1 = 0;
	

	for (auto nodePick : Instance.PickupNodes) {
		i = nodePick->getId();
		p1 += pickup[i][ShiftPeriod];
	}
	
	routeProblem.newCtr("Pickup minimum", p1 >= floor(oldPick / Instance.Capacity)*Instance.Capacity - ModelParameters::SLACK/100*Instance.Capacity);
	p1 = 0;
}

void RouteProblem::formulateMIP()
{
	MIP = true;
	//Delete route constraints for shift period


	for (XPRBctr ctr : ArcCtr[ShiftPeriod]) {
		routeProblem.delCtr(ctr);
	}

	for (XPRBctr ctr : VisitCtr[ShiftPeriod]) {
		routeProblem.delCtr(ctr);
	}

	//Initialize sets
	AllNodes = getNodesInShiftPeriod();

	vector<int> AllIntNodes;
	for (auto node : AllNodes)
		AllIntNodes.push_back(node->getId());

	for (auto node : AllNodes) {
		if (!node->isDepot()) {
			Nodes.push_back(node);
			if (node->isDelivery())
				DeliveryNodes.push_back(node);
			else
				PickupNodes.push_back(node);
		}
	}

	//Set variables to not visited nodes to 0
	int i, j;
	bool include;
	for (auto node1 : Instance.AllNodes) {
		i = node1->getId();
		include = true;
		for (auto node2 : AllNodes) {
			if (i == node2->getId())
				include = false;
		}

		if (include) {
			if (!node1->getId() == 0) {
				if (node1->isDelivery())
					delivery[i][ShiftPeriod].setUB(0);
				else
					pickup[i][ShiftPeriod].setUB(0);
			}

			for (auto node2 : Instance.AllNodes) {
				if (node1->inArcSet(node2)) {
					j = node2->getId();
					loadDelivery[i][j][ShiftPeriod].setUB(0);
					loadPickup[i][j][ShiftPeriod].setUB(0);
				}
				if (node2->inArcSet(node2)) {
					loadDelivery[j][i][ShiftPeriod].setUB(0);
					loadPickup[j][i][ShiftPeriod].setUB(0);
				}
			}
		}
	}



	//Initialize variables

	x = new XPRBvar *[Instance.AllNodes.size()];
	y = new XPRBvar[Instance.AllNodes.size()];
	timeVar = new XPRBvar[Instance.AllNodes.size()];
	for (auto node1 : AllNodes) {
		i = node1->getId();
		x[i] = new XPRBvar[Instance.AllNodes.size()];
		i = node1->getId();
		for (auto node2 : AllNodes) {
			j = node2->getId();
			if (node1->inArcSet(node2)) {
				x[i][j] = routeProblem.newVar(XPRBnewname("x%d%d", i, j), XPRB_BV, 0, 1);
			}
		}
		if(node1->isDepot())
			y[i] = routeProblem.newVar(XPRBnewname("y%d", i), XPRB_PL, 0, ModelParameters::nVehicles);
		else
			y[i] = routeProblem.newVar(XPRBnewname("y%d", i), XPRB_PL, 0, 1);

		timeVar[i] = routeProblem.newVar(XPRBnewname("t%d", i), XPRB_PL, 0, ModelParameters::maxTime);
	}

	


	//Add constraints

	//Objective
	for (auto node1 : AllNodes)
		for (auto node2 : AllNodes) {
			if (node1->inArcSet(node2)) {
				i = node1->getId();
				j = node2->getId();
				objective += node1->getTransCost(node2) * x[i][j];
			}
		}

	for (auto r : Routes)
		for (auto t : Instance.Periods)
			if (t != ShiftPeriod)
				objective += RouteCost[r] * travelRoute[r][t];

	for (auto node : Instance.Nodes) {
		i = node->getId();
		for (auto t : Instance.Periods)
			objective += node->HoldingCost*inventory[i][t];
	}

	//Avoid mathematical equivalent solution
/*	for (NodeIRP * node : Nodes) {
		objective += loadDelivery[node->getId()][0][ShiftPeriod];
		objective += loadPickup[0][node->getId()][ShiftPeriod];
	}*/

	routeProblem.setObj(routeProblem.newCtr("OBJ", objective));  /* Set the objective function */

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
		routeProblem.newCtr("RoutingFlow", p1 - p2 == 0);
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
		routeProblem.newCtr("LinkingArcandVisit", p1 - p2 == 0);
		p1 = 0;
		p2 = 0;
	}



	//Depot
	p1 = y[0];
	routeProblem.newCtr("Max vehicles", p1 <= ModelParameters::nVehicles);


	//Max visit
	for (auto node : Nodes) {
		p1 = y[node->getId()];
		routeProblem.newCtr("Max visit", p1 == 1);
		p1 = 0;
	}

	//Quantity
	//Loading constraints
	/*for (NodeIRP* node1 : DeliveryNodes) {
		i = node1->getId();
		for (NodeIRP* node2 : AllNodes) {
			j = node2->getId();
			if (node2->inArcSet(node1)) {
				p1 += loadDelivery[j][i][ShiftPeriod];
				p2 -= loadPickup[j][i][ShiftPeriod];
			}
			if (node1->inArcSet(node2)) {
				p1 -= loadDelivery[i][j][ShiftPeriod];
				p2 += loadPickup[i][j][ShiftPeriod];
			}
		}
		p1 -= delivery[i][ShiftPeriod];

		routeProblem.newCtr("LoadBalance Delivery", p1 == 0);
		routeProblem.newCtr("PickupBalance at deliveryNodes", p2 == 0);
		p1 = 0;
		p2 = 0; 
	}*/



	/*for (NodeIRP* node1 : PickupNodes) {
		i = node1->getId();
		for (NodeIRP* node2 : AllNodes) {
			j = node2->getId();
			if (node2->inArcSet(node1)) {
				p1 += loadPickup[j][i][ShiftPeriod];
				p2 += loadDelivery[j][i][ShiftPeriod];
			}
			if (node1->inArcSet(node2)) {
				p1 -= loadPickup[i][j][ShiftPeriod];
				p2 -= loadDelivery[i][j][ShiftPeriod];
			}
		}
		p1 += pickup[i][ShiftPeriod];
		routeProblem.newCtr("LoadBalance pickup", p1 == 0);
		routeProblem.newCtr("DeliveryBalance at pickupNodes", p2 == 0);
		p1 = 0;
		p2 = 0;
	}*/



	//Arc capacity
	for (NodeIRP* node1 : AllNodes) {
		i = node1->getId();
		for (NodeIRP * node2 : AllNodes) {
			if (node1->inArcSet(node2)) {
				j = node2->getId();
				p1 = loadDelivery[i][j][ShiftPeriod] + loadPickup[i][j][ShiftPeriod] - Instance.Capacity * x[i][j];
				routeProblem.newCtr("ArcCapacity", p1 <= 0);
				p1 = 0;

			}
		}
	}

	//Time constraints
	p1 = timeVar[0];
	routeProblem.newCtr("Initial time", p1 == 0);
	p1 = 0;


	for (NodeIRP* node1 : AllNodes) {
		i = node1->getId();
		for (NodeIRP* node2 : Nodes) {
			if (node1->inArcSet(node2)) {
				j = node2->getId();
				p1 = timeVar[i] - timeVar[j] + node1->getTravelTime(node2)
				+(ModelParameters::maxTime + node1->getTravelTime(node2)) * x[i][j];

				p2 = ModelParameters::maxTime + node1->getTravelTime(node2);
				routeProblem.newCtr("Time flow", p1 <= p2);
				p1 = 0;
				p2 = 0;

				p1 = timeVar[i] + node1->getTravelTime(node2) * x[i][j];

				//prob.newCtr("Final time", p1 <= ModelParameters::maxTime );
				p1 = 0;
			}

		}

		if (node1->inArcSet(Instance.getDepot())) {
			p1 = timeVar[i] + node1->getTravelTime(Instance.getDepot()) * x[i][0];
			routeProblem.newCtr("Final time", p1 <= ModelParameters::maxTime);
			p1 = 0;
		}
	}

}

vector<NodeIRP*> RouteProblem::getNodesInShiftPeriod()
{
	vector<NodeIRP*> nodes;
	nodes.push_back(new NodeIRP(*Instance.getDepot()));
	for (Route * r : routes) {
		if (r->getPeriod() == ShiftPeriod)
			for (NodeIRP* node : r->route) {
				if (!node->isDepot())
					nodes.push_back(new NodeIRP(node->getData()));
			}
	}
	return nodes;
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
		A[r->getId()] = r->getRouteMatrix(Instance.AllNodes.size());
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

	for (auto t : Periods) {
		counter[t] = 0;

		for (auto route : routes) {
			if (route->getPeriod() == t) {
				lockRoute(route);
				counter[route->getPeriod()]++;
			}
		}
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

	for (int r : Routes)
		delete[] travelRoute[r];

	delete[] travelRoute;

	for (auto node1 : Instance.AllNodes) {
		i = node1->getId();
		for (auto node2 : Instance.AllNodes) {
			if (node1->inArcSet(node2)) {
				j = node2->getId();
				delete[] loadPickup[i][j];
				delete[] loadDelivery[i][j];
			}

		}
		delete[] loadDelivery[i];
		delete[] loadPickup[i];
	}

	for (auto nodeDel : Instance.DeliveryNodes) {
		i = nodeDel->getId();
		delete[] inventory[i];
		delete[] delivery[i];
	}

	for (auto nodePick : Instance.PickupNodes) {
		i = nodePick->getId();
		delete[] inventory[i];
		delete[] pickup[i];
	}

	delete[] delivery;
	delete[] pickup;
	delete[] inventory;
	delete[] loadDelivery;
	delete[] loadPickup;

}

void RouteProblem::lockRoute(Route * route)
{
	int routeId = route->getId();
	int period = route->getPeriod();
	routeProblem.newCtr("RouteLock", travelRoute[routeId][period] == 1);
}

Solution * RouteProblem::solveProblem(Solution * sol)
{

	//routeProblem.print();
	routeProblem.mipOptimise();
	//If no solution, allocate a new solution
	if (sol == 0) {
		sol = Solution::allocateSolution(Instance);
	}



	//routeProblem.print();
	updateSolution(sol);
	if (MIP)
		updateEdges(sol);
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
