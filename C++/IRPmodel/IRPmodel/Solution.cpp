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

double Solution::getTransportationCost()
{
	double TravelCost = 0;
	for (int t : Instance.Periods)
		TravelCost += getTransportationCost(t);
	return TravelCost;
}

double Solution::getHoldingCost()
{
	double HoldingCost = 0;

	//Holding Cost
	for (int t : Instance.Periods) {
		HoldingCost += getHoldingCost(t);
	}

	return HoldingCost;
}

double Solution::getHoldingCost(int period)
{
	double HoldingCost = 0;
	for (NodeIRPHolder * n : Nodes) {
		if (n->getId() > 0) {
			HoldingCost += n->getHoldCost(period);
		}
	}
	return HoldingCost;
}


double Solution::getTransportationCost(int t)
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


Solution::Solution(const NodeInstanceDB & model, bool integer = false)
	:
	IntegerSolution(integer),
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
	Instance(cpSol.Instance)
{
	Nodes.resize(cpSol.Nodes.size());
	for (auto node : cpSol.Nodes) {
		Nodes[node->getId()] = new NodeIRPHolder(*node);
	}

	//Copy the edges
	for (int i = 0; i < Nodes.size(); i++)
		for (int t = 1; t < Nodes[i]->NodePeriods.size(); t++) {
			for (NodeIRP::EdgeIRP* edge : cpSol.Nodes[i]->NodePeriods[t]->getEdges()) {
				NodeIRP * nextNode = edge->getEndNode();
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

	return *this;
}

//Moves ownership of solution
void Solution::updateSolution(Solution &cpSol)
{
	//Release memory
	for (NodeIRPHolder* node : cpSol.Nodes) {
		delete Nodes[node->getId()];
		Nodes[node->getId()] = node;
	}
}



NodeIRP * Solution::getDepot(int period)
{

	for (NodeIRPHolder * node : Nodes) {
		if (node->getId() == 0) {
			return (NodeIRP *) node->NodePeriods[period];
	
		}
	}
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

void Solution::routeSearch()
{
	vector<Route*> RouteHolder;
	for (int i = 0; i < 1; i++) {
		RouteHolder.clear();
		generateRoutes(RouteHolder);
		
		//Solve route problem
		RouteProblem routeProb(Instance, RouteHolder);
		routeProb.formulateRouteProblem(ModelParameters::HIGHEST_TOTALCOST);
		routeProb.addChangeCtr();
		//Periods to lock routes for
		/*vector<int> periods;
		for (int t : Instance.Periods)
			if (period != t)
				periods.push_back(t);

		routeProb.lockRoutes(periods); */
		routeProb.solveProblem(this);

		//routeSol2->print("Solution/Heurestic" + to_string(i), ModelParameters::X);
		/*for (auto route : this->getAllRoutes()) {
			//route->printPlot("Solution/Sol2route" + to_string(route->getId()) + "Num" + to_string(i));
		}*/
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
	Solution * sol = new Solution(Instance, true);
	sol->SolID = SolutionCounter;
	return sol;
}

vector<Route*> Solution::getRoutes(int period)
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

//Count number of solution
int Solution::solCounter = 0;

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
	list <Route> routeHolder;
	int count = 0;
	//push back exisiting routes
	for (auto r : routes) {
		routeHolder.push_back(*r);
		r->setId(count);
		count++;
	}


	if (routes.size() >= 2) {
		//Iterations of merge
		for (int i = 1; i <= 1; i++) {
			int u = 0;
			for (Route * r1 : routes)
				for (Route * r2 : routes)
					if (r1 != r2 && r1->sameDirection(r2)) {
						r1->generateRoute(r2, routeHolder);
					}
		}

		routeHolder.sort();
		int k = 0;
		for (Route a : routeHolder) {
			cout << a.constructionCost << "\n";
			a.printPlot("Routes/bestRoutes" + to_string(k));
			k++;

		}

		auto pos = routeHolder.begin();
		int i = 0;
		do {
			if (pos->constructionCost > -500000) {
				routeHold.push_back(new Route(*pos));
				
			}
			pos++;
			i++;
		} while (i < 300 && !(pos == routeHolder.end()));

		//Plot merged routes

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

void Solution::plotPeriod(int t, string filename)
{
	vector<NodeIRP*> nodes;
	for (NodeIRPHolder * node : Nodes){
		nodes.push_back(node->NodePeriods[t]);
	}
	graphAlgorithm::printGraph(nodes, filename, ModelParameters::X);
}


bool Solution::isFeasible()
{
	//Check if too many vehicles
	for (int t : Instance.Periods) {
		if (getNumberOfRoutes(t) > ModelParameters::nVehicles) {
			return false;
		}
	}

	//Check if each route is feasible
	for (int t : Instance.Periods)
		for (Route* r : getRoutes(t)) {
			if (!isRouteFeasible(r))
				return false;
		}
	return true;
}


void Solution::clearEdges(int period)
{
	for (auto node : Nodes) {
		node->NodePeriods[period]->deleteEdges();
	}
}

//Returns the period shifted from
void Solution::shiftQuantity(int PeriodSelection, int ObjectiveSelection)
{
	vector <Route *> routes;
	Solution * solRoute = 0;
	double origQuantity;
	double newQuantity;
	double maxShift = 0;
	int shiftPeriod;
	Solution * shiftSolution = 0;
	Solution * tempSol;

	for (int t : Instance.Periods) {
		
		if (getNumberOfRoutes(t) >= 1) {
			tempSol = new Solution(*this);
			RouteProblem routeProb(Instance, getAllRoutes());
			routeProb.ShiftPeriod = t;
			routeProb.formulateRouteProblem(ObjectiveSelection);
			routeProb.lockRoutes(Instance.Periods);

			if (PeriodSelection == ModelParameters::MAX_SHIFT)
				origQuantity = getTotalDelivery(t) + getTotalPickup(t);

			routeProb.solveProblem(tempSol);


			if (PeriodSelection == ModelParameters::MAX_SHIFT) {
				newQuantity = tempSol->getTotalDelivery(t) + tempSol->getTotalPickup(t);

				if (origQuantity - newQuantity > maxShift) {
					maxShift = origQuantity - newQuantity;
					delete shiftSolution;
					shiftSolution = new Solution(*tempSol);
					shiftPeriod = t;
				}
				else
					delete tempSol;

			}
		}
	}


	//Solve a VRP for the shift period
	if (maxShift > 0) {

		shiftSolution->solveVRP(shiftPeriod);
	//	for (int t : Instance.Periods)
		//	cout << "Trans cost: " << t << " " << shiftSolution->getTransportationCost(t);
		//shiftSolution->printSolution();
		updateSolution(*shiftSolution);
		//printSolution();
	}

	//Minimize the inventory
	shiftSolution->solveInventoryProblem();
}

bool Solution::isRouteFeasible(Route * r)
{

	for (NodeIRP * n : r->route) {

		if (n->getEdge()->LoadDel + n->getEdge()->LoadPick> Instance.Capacity)
			return false;
	}
	return true;
}

double Solution::getNumberOfRoutes(int period)
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

double ** Solution::getVisitedMatrix()
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


NodeIRPHolder * Solution::getNode(int id)
{
	for (NodeIRPHolder * n : Nodes) {
		if (n->getId() == id)
			return n;
	}
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
vector<Route*> Solution::getAllRoutes()
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

double Solution::getObjective()
{
	return getHoldingCost() + getTransportationCost();
}

