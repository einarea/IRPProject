#include "stdafx.h"
#include "Solution.h"


Solution::~Solution()
{
	//Delete nodeholder
	for (NodeIRPHolder * node : Nodes)
		delete node;
}

void Solution::printSolution()
{	
	printf("\n\n\nObjective value: %.2f\n", getObjective());
	printf("Transporationcost: %.2f\n", getTransportationCost());
	printf("Holding cost: %.2f\n", getHoldingCost());

	int j;
	const char * str;
	for (int t : Instance.Periods) {
		printf("\n\nPeriod %d:", t);
		for (NodeIRPHolder * node : Nodes) {
			printf("\n");
			if (node->getId() == 0) {
				printf("Depot\t");
				printf("y0: %.2f\t", node->getOutflow(t));

				for (NodeIRP::EdgeIRP * edge : node->getEdges(t))
					if (edge->getValue() > 0.01) {
						printf("x%d%d: %.2f\t", node->getId(), edge->getEndNode()->getId(), edge->getValue());
						printf("loadDel%d%d: %.2f\t", node->getId(), edge->getEndNode()->getId(), edge->LoadDel);
						printf("loadPick%d%d: %.2f\t", node->getId(), edge->getEndNode()->getId(), edge->LoadPick);
					}
			}
			else {

				node->isDelivery() ? str = "DEL\t" : str = "PICK\t";
				printf(str);
				printf("Inv_%d: %.2f\t", node->getId(), node->NodePeriods[t]->Inventory);

				for (auto edge : node->getEdges(t))
				{
					printf("y%d: %.2f\t", node->getId(), node->getOutflow(t));
					printf("t%d: %.2f\t", node->getId(), node->timeServed(t));

					if (node->isDelivery()) {
						printf("qD_%d: %.2f\t", node->getId(), node->quantity(t));
					}
					else {
						printf("qP_%d: %.2f\t", node->getId(), node->quantity(t));
					}

					int i = node->getId();
					j = edge->getEndNode()->getId();
					printf("x%d%d: %.2f\t", i, j, edge->getValue());
					printf("loadDel%d%d: %.2f\t", i, j, edge->LoadDel);
					printf("loadPick%d%d: %.2f\t", i, j, edge->LoadPick);
				}


			} // end if
		} // end t
	}
}

double Solution::getTransportationCost() const
{
	double TravelCost = 0;
	for (int t : Instance.Periods)
		TravelCost += getTransportationCost(t);
	return TravelCost;
}

double Solution::getHoldingCost() const
{
	double HoldingCost = 0;

	//Holding Cost
	for (int t : Instance.Periods) {
		HoldingCost += getHoldingCost(t);
	}

	return HoldingCost;
}

double Solution::getHoldingCost(int period) const
{
	double HoldingCost = 0;
	for (NodeIRPHolder * n : Nodes) {
		if (n->getId() > 0) {
			HoldingCost += n->getHoldCost(period);
		}
	}
	return HoldingCost;
}


double Solution::getTransportationCost(int t) const
{
	double TravelCost = 0;
	double TravelCost2 = 0;
	for (NodeIRPHolder * n : Nodes) {
		NodeIRP * node1 = n->NodePeriods[t];
		for (NodeIRP::EdgeIRP * edge : n->getEdges(t)) {
			NodeIRP * node2 = edge->getEndNode();
			double value = edge->getValue();
			TravelCost = TravelCost + node1->getTransCost(node2)*value;
		}
	}

	return TravelCost;

}

bool Solution::isVisited(const NodeIRP * nodeSearch, int period)
{
	for (NodeIRP * node : getVisitedNodes(period))
		if (*nodeSearch == *node)
			return true;

	return false;
}

Route * Solution::insertNodeInPeriod(int period, const NodeIRP * insNode)
{
	double minCost = 100000;
	double origCost;
	Route bestRoute;
	
	vector<const NodeIRP*> nodes;
	nodes.push_back(insNode);
	for (Route * r : getRoutes(period)) {

		origCost = r->getTransCost();
		r->insertCheapestNode(nodes);
		if (r->getTransCost() - origCost < minCost) {
			minCost = r->getTransCost() - origCost;
			bestRoute = (*r);
		}
		delete r;
		}

	if (minCost == 100000)
		return nullptr;
	else
		return new Route(bestRoute);
}

/*void Solution::removeVisit(Route * route, int selection)
{

vector<NodeIRP*> pair = selectPair(route, ModelParameters::HIGHEST_HOLDINGCOST);

for (NodeIRP * n : pair) {

//Removes node from the route
route->removeNode(n, route);
//Remove the service, the inventory is updated in this function as well
n->changeQuantity(route->period, 0);
}

}*/



vector<NodeIRP*> Solution::selectPair(Route * r, int Selection)
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
		NodeIRP * tempNode = 0;

		double minCost = 1000000;

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
			tempCost = u->Quantity * u->getHoldCost();
			tempPair[0] = u;
			//Check if next node is on the route is the same customer
			if (Instance.getNode(u->getId()) == Instance.getNode(v->getId())) {
				tempCost += v->Quantity * v->getHoldCost();
				i += 2;
				tempPair[1] = v;
			}
			else
				i += 1;
		}
		//Else the node is a pickup node
		else {
			tempCost += v->Quantity * v->getHoldCost();
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
			tempCost = u->Quantity * u->getHoldCost();

	}
	break;

	} // end switch
}


vector<NodeIRP*> Solution::getCustomer(int id)
{
	vector<NodeIRP * > customer;
	//Deliverynode
	int delId = id;
	int pickId = id + 1;
	NodeIRP * nodeDel = new NodeIRP(*Instance.getNode(delId));
	//Pickupnode
	NodeIRP * nodePick = new NodeIRP(*Instance.getNode(pickId));
	customer.push_back(nodeDel);
	customer.push_back(nodePick);
	return customer;
}

//Uses cheapest insertion heurestic for all routes in the period
void Solution::insertSubrouteInRoute(Route * subroute, int period)
{
	Route * path = 0;
	double minCost = 100000;
	double tempCost;
	//Nodes to store best insertion
	NodeIRP *start = 0;
	NodeIRP *end = 0;
	NodeIRP * u = 0;
	NodeIRP * v = 0;
	NodeIRP * k = subroute->front();
	NodeIRP * l = subroute->back();

	Route * route = 0;
	vector<NodeIRP * > nodes;
	//Go through all routes. Cheapest insertion

	for (auto R : getRoutes(period)) {
		if (!R->coincide(subroute)) {
			tempCost = minCost;
			nodes = R->cheapestInsertion(subroute, tempCost);

			//If new cheapest insertion
			if (tempCost < minCost) {
				u = nodes.front();
				v = nodes.back();
				minCost = tempCost;
				start = Nodes[u->getId()]->NodePeriods[period];
				end = Nodes[v->getId()]->NodePeriods[period];
			}

		}

	}

	//Insert edges
	start->Node::deleteEdge(end);
	start->Node::addEdge(k);
	l->Node::addEdge(end);

}

void Solution::createSubroute(vector<NodeIRP*> nodes)
{
	//Create route
	for (int u = 0; u < nodes.size() - 1; u++) {
		nodes[u]->deleteEdges();
		nodes[u]->Node::addEdge(nodes[u + 1]);
	}
}

void Solution::buildStrongGraph(vector<NodeStrong*>& graph, int t)
{
	int s;
	double edgeValue;
	int i, j;

	//Create nodes for each visited customer
	for (NodeIRPHolder * node : Nodes) {
		if (node->quantity(t) >= 0.001) {
			NodeStrong * node2 = new NodeStrong(node->getId());
			graph.push_back(node2);
		}
	}
	//Not robust, but faster arcset lookup, be aware of changing node set logic
	//Add outgoing edges from each visited node
	for (NodeStrong *node : graph) {
		s = node->getId();
		if (node->getEdge()->getValue() > ModelParameters::EDGE_WEIGHT) {
			node->addEdge(node->getEdge()->getValue(), node->getEdge()->getEndNode());
		}
	}
}

void Solution::getSubtours(vector<Node*>strongComp, int t)
{

}




Solution::Solution(const NodeInstanceDB & model)
	:
	Instance(model)
{
	//Initializa node hold
	//Create a route holder for each period
	Nodes.resize(Instance.AllNodes.size());
	//Create a nodes for all customers and depot, same id as in model

	for (auto node : Instance.AllNodes) {
		Nodes[node->getId()] = new NodeIRPHolder(*node);
	}
}



//Initilize solution from nodes
Solution::Solution(const NodeInstanceDB & model, vector<NodeIRPHolder*> nodes)
	:
	Instance(model),
	Nodes(nodes)
{

}

Solution::Solution(const Solution & cpSol)
	:
	Instance(cpSol.Instance),
	TabuPeriods(cpSol.TabuPeriods)
{
	Nodes.resize(cpSol.Nodes.size());
	for (auto node : cpSol.Nodes) {
		Nodes[node->getId()] = new NodeIRPHolder(*node);
	}

	//Copy the edges
	for (int i = 0; i < Nodes.size(); i++)
		for (int t = 1; t < Nodes[i]->NodePeriods.size(); t++) {
			for (NodeIRP::EdgeIRP* edge : cpSol.Nodes[i]->NodePeriods[t]->getEdges()) {
				NodeIRP * nextNode = Nodes[edge->getEndNode()->getId()]->NodePeriods[t];
				Nodes[i]->NodePeriods[t]->copyEdge(edge, nextNode);
			}
		}
}

Solution & Solution::operator=(const Solution & cpSol)
{
	//Delete temp
	for (NodeIRPHolder* node : Nodes)
		delete node;


	Solution * tempSol = new Solution(cpSol);
	Nodes.resize(tempSol->Nodes.size());

	for (int i = 0; i <= tempSol->Nodes.size()-1; i++)
		Nodes[i] = new NodeIRPHolder(*tempSol->Nodes[i]);

	//Add edges
	for (int i = 0; i < tempSol->Nodes.size(); i++)
		for (int t : Instance.Periods)
			for (NodeIRP::EdgeIRP* edge : tempSol->Nodes[i]->NodePeriods[t]->getEdges())
				Nodes[i]->NodePeriods[t]->copyEdge(edge, Nodes[edge->getEndNode()->getId()]->NodePeriods[t]);

	this->TabuPeriods = cpSol.TabuPeriods;
	return *this;
}

bool Solution::operator==(const Solution & sol)
{
	if (sol.getTransportationCost() == getTransportationCost() && sol.getHoldingCost() == getHoldingCost())
		return true;
	else
		return false;
	/*bool duplicate;
	for (Route * r : getAllRoutes()){
		duplicate = false;
		for (Route * r2 : sol.getAllRoutes())
			if (r->isDuplicate(r2))
				duplicate = true;

		if (!duplicate)
			return false;
	}

	return true; */
}

//Moves ownership of solution
void Solution::updateSolution(Solution &cpSol)
{
	//Release memory
	Solution * tempSol = new Solution(cpSol);
	for (NodeIRPHolder* node : tempSol->Nodes) {
		Nodes[node->getId()] = node;
	}
}



NodeIRP * Solution::getDepot(int period) const
{

	for (NodeIRPHolder * node : Nodes) {
		if (node->getId() == 0) {
			return (NodeIRP *) node->NodePeriods[period];
	
		}
	}

	exit(111);
}

void Solution::solveVRP(int period)
{
	VRPmodel model(Instance, getVisitedNodes(period), period);
	model.solveModel(this);
}


void Solution::buildGraph(vector<NodeIRP*> &graph)
{
	for (NodeIRPHolder * n : Nodes)
		graph.push_back(new NodeIRP(n->getData()));

	for (auto r : getAllRoutes()) {
		for (Node* node : r->route) {
			for (Node::Edge *edge : node->getEdges()) {
				if (!graph[node->getId()]->hasEdge(edge))
					graph[node->getId()]->Node::addEdge(edge->getEndNode());
			}
		}
	}
}

void Solution::buildGraph(vector<NodeIRP*>& graph, int t)
{
	//Add nodes from particular period
	for (NodeIRPHolder * n : Nodes) {
		NodeIRP * node = new NodeIRP(n->Instance);
		graph.push_back(node);
	}
}

void Solution::routeSearch(int REQUIRE_REQ_CHANGE)
{
	vector<Route*> RouteHolder;
	for (int i = 0; i < 1; i++) {
		RouteHolder.clear();
		generateRoutes(RouteHolder);
		
		//Solve route problem
		RouteProblem routeProb(Instance, RouteHolder);
		routeProb.formulateRouteProblem(ModelParameters::HIGHEST_TOTALCOST);

		//Require one change
	
		if (REQUIRE_REQ_CHANGE == ModelParameters::REQUIRE_CHANGE)
			routeProb.addChangeCtr();

		//Periods to lock routes for
		/*vector<int> periods;
		for (int t : Instance.Periods)
			if (period != t)
				periods.push_back(t);

		routeProb.lockRoutes(periods); */
		routeProb.solveProblem(this);
		routeProb.printRouteType();
	}
}

//Optimize over the routes in the current solution
void Solution::routeOptimize()
{
	RouteProblem routeProb1(Instance, getAllRoutes());
	routeProb1.formulateRouteProblem(ModelParameters::HIGHEST_TOTALCOST);
	routeProb1.solveProblem();
}



//Returns Aijt vector where 1 if arc ij is traversed in period t
int *** Solution::getRouteMatrix()
{
	int u;
	int v;
	int *** RouteMatrix = new int **[Nodes.size()];
	int i;
	int j;
	for (auto node1 : Instance.AllNodes) {
		i = node1->getId();
		RouteMatrix[i] = new int *[Instance.AllNodes.size()];
		for (auto node2 : Instance.AllNodes) {
			j = node2->getId();
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
int Solution::selectPeriod(int selection)
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

int Solution::SolutionCounter = 0;

Solution * Solution::allocateSolution(const NodeInstanceDB &Instance)
{
	SolutionCounter++;
	Solution * sol = new Solution(Instance);
	sol->SolID = SolutionCounter;
	return sol;
}


//Returns the difference in visits from sol2 to sol1
double ** Solution::getVisitDifference(const Solution &sol1, const Solution & sol2)
{
	int i;
	double ** changedMatrix;
	double ** newVisitedMatrix = sol1.getVisitedMatrix();
	double ** prevVisitedMatrix = sol2.getVisitedMatrix();
	cout << "\n";
	cout << "\nChangeMatrix: 1 added visit, -1 removed visit\n";
	changedMatrix = new double *[sol1.Instance.AllNodes.size()];
	for (NodeIRPHolder * node : sol1.Nodes) {
		i = node->getId();
		if (i != 0) {
			cout << "\n";
			changedMatrix[i] = new double[sol1.Instance.AllPeriods.size()];
			for (auto j : sol1.Instance.Periods) {
				double a = newVisitedMatrix[i][j];
				double b = prevVisitedMatrix[i][j];
				changedMatrix[i][j] = newVisitedMatrix[i][j] - prevVisitedMatrix[i][j];
				cout << changedMatrix[i][j] << "\t";
			}
		}
	}

	delete newVisitedMatrix;
	delete prevVisitedMatrix;

	return changedMatrix;
}

vector<Route*> Solution::getRoutes(int period) const
{
	NodeIRP* v;
	vector<Route*> routes;
	vector <NodeIRP*> path;
	//Perform a search from the depot. Travers edges until next node is depot
	for (NodeIRP::EdgeIRP * edge : getDepot(period)->getEdges()) {
		path.clear();

		NodeIRP* u = new NodeIRP(*getDepot(period));
		NodeIRP * origV = edge->getEndNode();
		v = new NodeIRP(*origV);
		//Depth first search 
		path.push_back(u);
		u = v;
		do {
			path.push_back(u);
			origV = origV->getNextNode();
			v = new NodeIRP(*origV);
			u = v;

		} while (!u->isDepot()); //while not depot

		Route * route = new Route(path);
		route->setPeriod(period);
		routes.push_back(route);
	}

	return routes;
}

//Returns infeasible period or -1
int Solution::getInfeasiblePeriod()
{
	for (int t : Instance.Periods) {
		if (getNumberOfRoutes(t) > ModelParameters::nVehicles + 0.5) {
			return t;
		}
	}

	return -1;
}

//Count number of solution
int Solution::solCounter = 0;

//Returns the least served node in a period which is stricly positive
const NodeIRP* Solution::getLeastServed(int period) const
{
	NodeIRP * minNode = 0;
	double minService = Instance.Capacity;
	for (Route * r : getRoutes(period))
		for (NodeIRP * node : r->route)
			if (node->Quantity < minService && !node->isDepot()) {
				minNode = node;
				minService = node->Quantity;
			}

	return minNode;
}

const NodeIRP * Solution::getLeastServed(vector<NodeIRP*> nodes, int period) const
{
	NodeIRP * minNode = nullptr;
	double minService = Instance.Capacity;
	for (NodeIRP * node : nodes)
			if (Nodes[node->getId()]->quantity(period) < minService && Nodes[node->getId()]->quantity(period) >= 0.1) {
				minNode = Nodes[node->getId()]->NodePeriods[period];
				minService = Nodes[node->getId()]->quantity(period);
			}

	return minNode;
}

Route* Solution::removeNodeFromPeriod(int period, NodeIRP const * remNode)
{

	for (Route *r : getRoutes(period)) {
		for (int i = 0; i < r->route.size(); i++)
			if (*r->route[i] == *remNode) {
				r->route[i - 1]->deleteEdges();
				if(i+1>= r->route.size())
					r->route[i - 1]->Node::addEdge(1, r->route[0]);
				else
					r->route[i - 1]->Node::addEdge(1, r->route[i + 1]);
				delete r->route[i];
				r->State = Route::LEAST_SERVED_REMOVAL;
				r->updateRoute();
				return r;
			}
		delete r;
	}

	exit(111);
}

void Solution::print(string filename, int load)
{
	solCounter++;
	vector<NodeIRP *> graph;
	for (int t : Instance.Periods) {
		buildGraph(graph, t);
		graphAlgorithm::printGraph(graph, filename + to_string(solCounter) + "t" + to_string(t), load);
		graph.clear();
	}
}

int Solution::getPeriodWithMinExcess(const vector<int>& Periods)
{
	if (Periods.size() == 0)
		exit(123);

	int shiftPeriod = -1;
	double oldDel;
	double oldPick;
	double deliveryExcess;
	double pickupExcess;
	double vehicleExcess;
	double minExcess = 10000;

	//Find the period with the lowest excess.
	for (int t : Periods) {
		deliveryExcess = max(getTotalDelivery(t) - Instance.Capacity*(getNumberOfRoutes(t) - 1), 0);
		pickupExcess = max(getTotalPickup(t) - Instance.Capacity*(getNumberOfRoutes(t) - 1), 0);
		vehicleExcess = max(deliveryExcess, pickupExcess);

		if (vehicleExcess < minExcess && vehicleExcess >= 1) {
			minExcess = vehicleExcess;
			shiftPeriod = t;
		}
	}
	if (shiftPeriod == -1)
	{
		for (int t : Periods) {
			deliveryExcess = max(getTotalDelivery(t) - Instance.Capacity*(getNumberOfRoutes(t) - 2), 0);
			pickupExcess = max(getTotalPickup(t) - Instance.Capacity*(getNumberOfRoutes(t) - 2), 0);
			vehicleExcess = max(deliveryExcess, pickupExcess);

			if (vehicleExcess < minExcess && vehicleExcess >= 1) {
				minExcess = vehicleExcess;
				shiftPeriod = t;
			}
		}
	}

	//Failsafe
	if (shiftPeriod == -1)
		shiftPeriod = rand() % (Periods.back() - Periods.front()) + Periods.front();

	return shiftPeriod;
}

/*void Solution::mergeRoutes(int position, Route * route, vector<Route*> &Routes, vector<Route*> &newRoutes)
{
	if (position < Routes.size() - 1) {
		mergeRoutes(position + 1, route, Routes, newRoutes);
	}

    route->generateRoute(Routes[position], newRoutes);

	//newroute->printPlot("Routes/afterRemoval" + to_string(rand() % 100));

}*/
bool sort_func(const Route* & lhs, const Route* & rhs)
{
	return *lhs < *rhs;
}

void Solution::generateRoutes(vector<Route* >&routeHold)
{
	vector < Route*> routes = getAllRoutes();
	list <Route> origRoutes;
	list <Route> routeHolder;
	int count = 0;
	//push back exisiting routes
	for (auto r : routes) {
		r->setId(count);
		r->State = Route::ORIG;
		routeHolder.push_back(*r);
		routeHold.push_back(new Route(*r));
		origRoutes.push_back(*r);
		count++;
	}

	int k = 0;
	for (Route r : origRoutes) {
		//r.printPlot("Routes/orig" + to_string(k++));
	}

	//Insert least served node in next period to the cheapest route in previous period
	const NodeIRP * node;
	bool duplicate = false;
	Route newRoute;
	Route *ptr = nullptr;
	for (int t : Instance.Periods) {
		if (t > 1) {
			
			node = getLeastServed(t);
			if (node != nullptr) {
				newRoute = *removeNodeFromPeriod(t, node);
				if (newRoute.getSize() >= 2) {
					for (Route r : routeHolder)
						if (newRoute.isDuplicate(&r))
							duplicate = true;

					//newRoute.printPlot("Routes/removal" + to_string(k++));


					if (!duplicate && newRoute.isFeasible()) {
						Route *r = new Route(newRoute);
						routeHold.push_back(r);
					}
				}
			}

			vector<NodeIRP*> nodes = getNotVisitedNodes(t-1);

				node = getLeastServed(nodes, t);

				if (node != nullptr) {
					cout << node->Quantity;
					duplicate = false;
					ptr = insertNodeInPeriod(t - 1, node);

					if (ptr != nullptr) {
						newRoute = *ptr;
						newRoute.State = Route::LEAST_SERVED_INSERTION;
						//newRoute.printPlot("Routes/insertion" + to_string(k++));
						for (Route r : routeHolder)
							if (newRoute.isDuplicate(&r))
								duplicate = true;

						if (!duplicate && newRoute.isFeasible()) {
							Route * r = new Route(newRoute);
							routeHold.push_back(r);
						}
					}
				}
		}
	}


	duplicate = false;
	vector < vector < const NodeIRP* >> nodePeriods;
	for (int t : Instance.Periods)
		nodePeriods.push_back(getNodesIRP(t));

	cout << "Merge\n";
	if (routes.size() >= 2) {
		//Iterations of merge

		for (int i = 1; i <= 1; i++) {
			int u = 0;
			for (Route * r1 : routes)
				for (Route * r2 : routes)
					if (r1 != r2 && r1->sameDirection(r2) && r1->State==Route::ORIG && r2->State == Route::ORIG) {
						r1->generateRoute(r2, origRoutes);
					}
		}

		origRoutes.sort();

		auto pos = origRoutes.begin();
		int i = 0;
		do {
			if (pos->State != Route::ORIG) {
				Route * r = new Route(*pos);
				r->State = Route::MERGE;
				if (r->isFeasible()) {
					routeHold.push_back(r);
				}

			}
			pos++;
			i++;
		} while (i < 100 && !(pos == origRoutes.end()));

	cout << "Insert\n";
	//insert single node in all routes
	int k = 0;

	Route newRoute;
	for (Route * r1 : routes) {
		duplicate = false;
		int t = r1->getPeriod();
		newRoute = *r1;
		newRoute.insertCheapestNode(nodePeriods[t-1]);

		for (Route r : routeHolder)
			if (newRoute.isDuplicate(&r))
				duplicate = true;
		
		if (!duplicate &&  newRoute.isFeasible()) {
			Route * r = new Route(newRoute);
			r->State = Route::SIMPLE_INSERTION;
			routeHold.push_back(r);
		}
	}

	k = 0;

	cout << "Insert and remove\n";
	//insert single node and remove other
	for (Route * r1 : routes) {
		duplicate = false;
		int t = r1->getPeriod();
		newRoute = *r1;
		newRoute.insertCheapestNode(nodePeriods[t - 1]);
		newRoute.removeSubroute(1);

		for (Route r : routeHolder)
			if (newRoute.isDuplicate(&r))
				duplicate = true;

		if (!duplicate &&  newRoute.isFeasible()) {
			Route * r = new Route(newRoute);
			r->State = Route::INSERTION_REMOVAL;
			routeHold.push_back(r);
		}
	}
	





		//Plot merged routes
		
		/*for (Route *r : routeHold) {
			string filename = "Routes/";
			switch (r->State) {
			case Route::ORIG: {
				filename = filename + "ORIG"+to_string(k++);
				break;
			}
			case Route::SIMPLE_INSERTION: {
				filename = filename + "Insertion"+to_string(k++);
				break;
			}
			case Route::INSERTION_REMOVAL: {
				filename = filename + "InsAndRem"+ to_string(k++);
				break;
			}
			case Route::MERGE: {
				filename = filename + "Merge"+ to_string(k++);
				break;
			}
			case Route::LEAST_SERVED_INSERTION: {
				filename = filename + "Least served insertion" + to_string(k++);
				break;
			}
			case Route::LEAST_SERVED_REMOVAL: {
				filename = filename + "Least served removal" + to_string(k++);
				break;
			}
		
			default: {
				filename = filename + "Error";
				break;
			}
			}
			r->printPlot(filename);
		}*/
	}
}

//Returns the visited nodes in a period
vector<NodeIRP*> Solution::getVisitedNodes(int period)
{
	vector<NodeIRP*> visitedNodes;
	//Push back depot
	visitedNodes.push_back(Nodes[0]->NodePeriods[period]);
	for (auto node : Nodes) {
		if (node->quantity(period) > 0.01) {
			NodeIRP * nodeIRP = (NodeIRP*) node->NodePeriods[period];
			visitedNodes.push_back(nodeIRP);
		}
	}
	return visitedNodes;
}

vector<NodeIRP*> Solution::getNotVisitedNodes(int period)
{
	vector<NodeIRP*> visitedNodes;
	//Push back depot

	for (auto node : Nodes) {
		if (node->quantity(period) < 0.01) {
			NodeIRP * nodeIRP = (NodeIRP*)node->NodePeriods[period];
			visitedNodes.push_back(nodeIRP);
		}
	}
	return visitedNodes;
}

void Solution::plotPeriod(int t, string filename)
{
	vector<NodeIRP*> nodes;
	for (NodeIRPHolder * node : Nodes){
		nodes.push_back(node->NodePeriods[t]);
	}
	graphAlgorithm::printGraph(nodes, filename, ModelParameters::X);
}

void Solution::plot(string filename)
{
	for (int t : Instance.Periods) {
		plotPeriod(t, filename + "_period_" + to_string(t));
	}
}


bool Solution::isFeasible() const
{
	//Check if too many vehicles
	for (int t : Instance.Periods) {
		if (getNumberOfRoutes(t) > ModelParameters::nVehicles+0.5) {
			return false;
		}
	}
	return true;
}


//Remove nodes that is not served

void Solution::clearEdges(int period)
{
	for (auto node : Nodes) {
		node->NodePeriods[period]->deleteEdges();
	}
}

//Returns the period shifted from
void Solution::shiftQuantity(int SELECTION)
{
	vector <Route *> routes;
	Solution * solRoute = 0;
	double origQuantity;
	double newQuantity;
	double maxShift = 0;
	int shiftPeriod;
	Solution * shiftSolution = 0;
	Solution tempSol(*this);



	//Alternative 1: Max shift
	if (SELECTION == ModelParameters::MAX_SHIFT){
		for (int t : Instance.Periods) {

			RouteProblem routeProb(Instance, getAllRoutes());

			if (getNumberOfRoutes(t) >= 1) {
				routeProb.ShiftPeriod = t;
				routeProb.formulateRouteProblem(ModelParameters::MIN_SERVICE);
				routeProb.lockRoutes(Instance.Periods);

				origQuantity = getTotalDelivery(t) + getTotalPickup(t);
				routeProb.solveProblem(&tempSol);


				//Select the period with the greatest shift
				newQuantity = tempSol.getTotalDelivery(t) + tempSol.getTotalPickup(t);

				if (origQuantity - newQuantity > maxShift) {
					maxShift = origQuantity - newQuantity;
					delete shiftSolution;
					shiftSolution = new Solution(tempSol);
					shiftPeriod = t;
				}

			}
		}

	}
	

	//Alternative 2: Restricted shift
	if (SELECTION == ModelParameters::RESTRICTED_SHIFT) {

		vector <int> Periods = Instance.Periods;
		bool FINISHED = false;

		while (FINISHED == false && Periods.size() >= 1) {
			RouteProblem routeProb(Instance, getAllRoutes());
			shiftPeriod = getPeriodWithMinExcess(Periods);
			double oldDel = getTotalDelivery(shiftPeriod);
			double oldPick = getTotalPickup(shiftPeriod);

			routeProb.ShiftPeriod = shiftPeriod;
			routeProb.formulateRouteProblem(ModelParameters::MIN_SERVICE);
			routeProb.lockRoutes(Instance.Periods);

			//Restrict the shift in the routeproblem
			routeProb.addRestrictedShiftCtr(getNumberOfRoutes(shiftPeriod), oldDel, oldPick);
			routeProb.solveProblem(&tempSol);

			//Check if able to move, if it is -> solve vrp, else find new period without the current shift period
			if (tempSol.getTotalDelivery(shiftPeriod) <= Instance.Capacity*(getNumberOfRoutes(shiftPeriod) - 1) &&
				tempSol.getTotalPickup(shiftPeriod) <= Instance.Capacity*(getNumberOfRoutes(shiftPeriod) - 1)) {
				shiftSolution = new Solution(tempSol);
				FINISHED = true;
			}

			else {
				int pos = 0;
				for (int t : Periods) {
					if (t != shiftPeriod)
						Periods[pos++] = t;
				}
				Periods.resize(Periods.size() - 1);
			}
		}
		//Return if not able to move
		if (!FINISHED)
			return;
	}

	if (SELECTION == ModelParameters::MINIMIZE_VISITS) {

		RouteProblem routeProb(Instance, getAllRoutes());
		routeProb.ShiftPeriod = getPeriodWithMinExcess(Instance.Periods);
		shiftPeriod = routeProb.ShiftPeriod;
		routeProb.formulateRouteProblem();
		routeProb.lockRoutes(Instance.Periods);
		routeProb.formulateMinVisitProblem();
		shiftSolution = routeProb.solveProblem(&tempSol);
	}


	//Solve a VRP for the shift period


	shiftSolution->solveVRP(shiftPeriod);
	updateSolution(*shiftSolution);
	//printSolution();
	

	//Minimize the inventory
	solveInventoryProblem();
}

void Solution::getStrongComponents()
{
	vector <vector<Node*>> result; //matrix to store strong components
	vector <NodeStrong*> graph;		//Graph to store nodes

	bool newCut = false;
	
	for (int t : Instance.Periods) {
		//buildStrongGraph(graph, t, false, EDGE_WEIGHT); //Do not include depot in graph
													//graphAlgorithm::printGraph(graph, *this, "Subtour/DepotGone" + to_string(t), ModelParameters::X);

		buildStrongGraph(graph, t); //Do not include depot in graph
		graphAlgorithm::sepByStrongComp(graph, result);
	}
}

void Solution::shiftQuantityMIP(int periodSelection)
{
	Solution tempSol(*this);
	RouteProblem routeProb(Instance, getAllRoutes());

	bool tabuInd;
	vector<int> Periods;
	for (int t : Instance.Periods) {
		tabuInd = false;
		for (int tabu : TabuPeriods)
			if (t == tabu) {
				tabuInd = true;
			}
		if (!tabuInd) {
			Periods.push_back(t);
		}
	}
	
	switch (periodSelection) {
	case ModelParameters::INFEASIBLE: {
		routeProb.ShiftPeriod = getInfeasiblePeriod();
		break;
	}

	case ModelParameters::RESTRICTED_SHIFT: {
		routeProb.ShiftPeriod = getPeriodWithMinExcess(Periods);
		break;
	}

	case ModelParameters::MAX_SHIFT: {
		routeProb.ShiftPeriod = getMaxShiftPeriod();
		break;
	}
	}

	if (routeProb.ShiftPeriod == -1)
		 exit(111);

	if (TabuPeriods.size() >= 2)
		TabuPeriods.pop_back();

	//Update tabu periods
	TabuPeriods.push_front(routeProb.ShiftPeriod);

	routeProb.formulateRouteProblem(ModelParameters::MIN_SERVICE);
	routeProb.formulateMIP();
	vector<int> Periods2;
	for (int per : Instance.Periods) {
		if (per != routeProb.ShiftPeriod) Periods2.push_back(per);
	}
	routeProb.lockRoutes(Periods2);
	routeProb.solveProblem(&tempSol);
	updateSolution(tempSol);
}

int Solution::getMaxShiftPeriod()
{
	Solution tempSol(*this);
	double origQuantity;
	double newQuantity;
	double maxShift = -1;
	int shiftPeriod = -1;
	for (int t : Instance.Periods) {
		RouteProblem routeProb(Instance, getAllRoutes());

		if (getNumberOfRoutes(t) >= 1) {
			routeProb.ShiftPeriod = t;
			routeProb.formulateRouteProblem(ModelParameters::MIN_SERVICE);
			routeProb.lockRoutes(Instance.Periods);

			origQuantity = getTotalDelivery(t) + getTotalPickup(t);
			routeProb.solveProblem(&tempSol);


			//Select the period with the greatest shift
			newQuantity = tempSol.getTotalDelivery(t) + tempSol.getTotalPickup(t);

			if (origQuantity - newQuantity > maxShift) {
				maxShift = origQuantity - newQuantity;
				shiftPeriod = t;
			}

		}
	}

	return shiftPeriod;
}



bool Solution::isRouteFeasible(Route * r)
{

	for (NodeIRP * n : r->route) {

		if (n->getEdge()->LoadDel + n->getEdge()->LoadPick > Instance.Capacity)
			return false;
	}
	return true;
}

double Solution::getNumberOfRoutes(int period) const
{
	NodeIRPHolder * depot = getNode(0);
	return depot->getOutflow(period);
}

double Solution::getResidualCapacity(int period)
{
	double residual = 0;
	for (auto node : Nodes) {
		residual += node->quantity(period);
	}
	return residual / getNumberOfRoutes(period);
}

vector<const NodeIRP*> Solution::getNodesIRP(int period)
{
	vector<const NodeIRP*> nodes;
	for (NodeIRPHolder * n : Nodes)
		if(n->getId() != 0)
			nodes.push_back(n->NodePeriods[period]);

	return nodes;
}

double Solution::getTotalDelivery(int period)
{
	double totalDelivery = 0;
	for (auto node : Nodes) {
		if (node->isDelivery() && node->getId() != 0)
			totalDelivery += node->quantity(period);
	}

	return totalDelivery;
}

double Solution::getTotalPickup(int period)
{
	double totalPickup = 0;
	for (auto node : Nodes)
		if (!node->isDelivery())
			totalPickup += node->quantity(period);

	return totalPickup;
}

double Solution::getNodeVisits(int period)
{
	return getDeliveryNodeVisits(period) + getPickupNodeVisits(period);
}

double ** Solution::getVisitedMatrix() const
{
	double ** VisitedMatrix;
	cout << "\n\nVisitMatrix: 1 = visited, 0 = not visited";
	VisitedMatrix = new double *[Instance.AllNodes.size()];
	for (auto node : Nodes) {
		if (node->getId() != 0) {
			cout << "\n";
			VisitedMatrix[node->getId()] = new double[Instance.getnPeriods() + 1];
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



double Solution::getDeliveryNodeVisits(int period)
{
	double nodeVisits = 0;
	for (auto node : Nodes)
		if (node->isDelivery() && node->quantity(period) >= 0.01)
			nodeVisits += 1;

	return nodeVisits;
}

double Solution::getPickupNodeVisits(int period)
{
	double nodeVisits = 0;
	for (auto node : Nodes)
		if (!node->isDelivery() && node->quantity(period) >= 0.01)
			nodeVisits += 1;

	return nodeVisits;
}

double Solution::getDelivery(int period)
{
	double service = 0;
	for (auto node : Nodes)
		if (node->isDelivery())
			service += node->quantity(period);
	if (service > 0.01)
		return service / getDeliveryNodeVisits(period);
	else
		return 0;
}

double Solution::getPickup(int period)
{
	double service = 0;
	for (auto node : Nodes)
		if (!node->isDelivery())
			service += node->quantity(period);
	if (service > 0.01)
		return service / getPickupNodeVisits(period);
	else
		return 0;
}


NodeIRPHolder * Solution::getNode(int id) const
{
	for (NodeIRPHolder * n : Nodes) {
		if (n->getId() == id)
			return n;
	}
	exit(111);
}

vector<NodeIRPHolder*>& Solution::getNodes()
{
	return Nodes;
}



void  Solution::solveInventoryProblem()
{
	RouteProblem routeProb(Instance, getAllRoutes());
	routeProb.formulateRouteProblem(ModelParameters::MinimumInventory);
	routeProb.lockRoutes(Instance.Periods);
	routeProb.solveProblem(this);
}

/*void Solution::insertCustomer(int customerID, int period)
{
	vector<NodeIRP *> nodes = getCustomer(customerID);
	createSubroute(nodes);

	cout << "Old routes in " << period << "\n";
	for (auto route : getRoutes(period)) {
		route->printRoute();
	}
	insertSubrouteInRoute(nodes, period);

	cout << "New routes in " << period << "\n";
	for (auto route : getRoutes(period)) {
		route->printRoute();

	}

	//Reoptimize inventory
	solveInventoryProblem();
}*/


int Solution::getnPeriods()
{
	return Instance.nPeriods;
}

//Returns a copy of the nodes in the solution. Hence any change to a route do not alter the solution.
vector<Route*> Solution::getAllRoutes() const
{
	vector<Route*> routes;
	for (int t : Instance.Periods) {
		for (Route* r : getRoutes(t)) {
			r->constructionCost = -100000;
			routes.push_back(r);
		}
	}
	
	return routes;
}

double Solution::getObjective() const
{
	return getHoldingCost() + getTransportationCost();
}

