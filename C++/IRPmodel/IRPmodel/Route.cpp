#include "stdafx.h"
#include "Route.h"


Route::~Route()
{
	for (NodeIRP* node : route)
		delete node;
}


void Route::setPeriod(int period)
{
	Period = period;
}

bool Route::coincide(Route* r)
{
	for (auto node : r->route) {
		for (auto thisNode : route)
			if (node->getId() == thisNode->getId())
				return true;
	}
	return false;
}

void Route::createSeperateRoute(Route * r)
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

bool Route::inRoute(Node * n)
{
	for (auto node : route)
		if (node->getId() == n->getId())
			return true;

	return false;
}

//Returns a copy of the subgraphs of a route
vector<Route*> Route::getSubgraphs(int n) const
{
	if (n > route.size() - 1){
		cout << "subgraph longer than route";
		exit(111);
	}

	vector<NodeIRP*> tempSubgraph;
	vector<Route*> subgraphs;
	int nGraphs = route.size() - n;
	subgraphs.resize(nGraphs);
	for (int i = 0; i < nGraphs; i++) {
		subgraphs[i] = new Route();
		subgraphs[i]->resize(n);
		tempSubgraph.resize(n);
		for (int j = 0; j < n; j++) {
			tempSubgraph[j] = new NodeIRP(*route[i + j + 1]);
		}
		subgraphs[i] = new Route(tempSubgraph);
	}

	return subgraphs;
}

int Route::getTotalDelivery()
{
	double totalDelivery = 0;
	for (auto node : route)
		if (node->isDelivery())
			totalDelivery += node->Quantity;

	return totalDelivery;
}

int Route::getTotalPickup()
{
	return 0;
}

void Route::insertSubgraph(Route* subgraph)
{
	double minCost = 100000;
	vector<NodeIRP*> holder = cheapestInsertion(subgraph, minCost);
	insert(holder.front(), holder.back(), subgraph);
}

void Route::insert(NodeIRP * start, NodeIRP * end, Route * subroute)
{
	NodeIRP* k = subroute->front();
	NodeIRP* l = subroute->back();
	//Insert edges
	start->Node::deleteEdge(end);
	start->Node::addEdge(k);
	l->Node::deleteEdges();
	l->Node::addEdge(end);

	//route[0]->deleteEdges();
	updateRoute();
	//Update route positions
}



int Route::getPosition(Node * node)
{
	auto nodeSearch = route.front();
	int position = 0;
	do {
		if (nodeSearch->getId() == node->getId())
			return position;

		nodeSearch = nodeSearch->getEdge()->getEndNode();
		position++;
	} while (nodeSearch->getId() != 0);
}

int Route::getId()
{
	return Id;
}

//Returns a Aij matrix, 1 if arc ij is traversed, 0 otherwise
int ** Route::getRouteMatrix(int gridSize)
{
	int ** routeMat = new int*[gridSize];
	for (int i = 0; i < gridSize; i++) {
		routeMat[i] = new int[gridSize];
		for (int j = 0; j < gridSize; j++) {
			routeMat[i][j] = 0;
		}
	}

	for (Node *node : this->route) {
		int i = node->getId();
		int j;
		if (i == 0)
			j = this->route[1]->getId();
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

void Route::printPlot(string filename) const
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

graphAlgorithm::printGraph(graph, filename, ModelParameters::X);
}

void Route::printRoute()
{
	cout << "\n";
	for (auto node : route)
		cout << (node->getId()) << " - ";

	cout << "0 \n";
}

double Route::getResidualCapacity(int capacity)
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
	return  capacity - maxLoad;
} 

void Route::removeSubroute(int size)
{
	vector<NodeIRP*> Nodes = getSubroute(size);

	if (route.size() - Nodes.size() >= 2) {

		//Delete edges and add edges

		Nodes.front()->deleteEdges();
		Nodes.front()->Node::addEdge(Nodes.back());

		updateRoute();

	}
}

void Route::updateRoute()
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


//Returns empty vector if it doesn't beat the minCost
vector<NodeIRP*> Route::cheapestInsertion(Route* subroute, double &minCost)
{
	vector<NodeIRP * > Nodes;
	Nodes.resize(2);
	//Nodes to store best insertion
	//First and last node in route to insert
	const NodeIRP * k;
	const NodeIRP * l;
	k = subroute->front();
	l = subroute->back();

	double tempCost = 0;
	NodeIRP * v = 0;
	double C_uk;
	double C_lv;
	double C_uv;

	for (auto u : route) {
		v = u->getEdge()->getEndNode();
		C_uk = u->getTransCost(k);
		C_lv = l->getTransCost(v);
		C_uv = u->getTransCost(v);

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



void Route::setId(int id)
{
	Id = id;
}

Route::Route()
{
}



int Route::getPeriod()
{
	return Period;
}

bool Route::isFeasible()
{
	double totalTime = 0;

	for (auto node : route) {
		NodeIRP *v = node->getNextNode();
		totalTime += node->getTravelTime(v);
	}

	return totalTime <= ModelParameters::maxTime;
}

void Route::resize(int size)
{
	route.resize(size);
}

Route * Route::generateRoute(const Route * r)
{
	//Target route
	const Route OrigRoute = *this;
	vector<Route > bestRoute;
	Route targetRoute;
	double minCost = 100000;
	double bestCost = 100000;
	int lowestSubroute = 3;
	int highestSubroute = r->route.size() - 1;
	int nRoutes = highestSubroute - lowestSubroute;
	bestRoute.resize(highestSubroute+1);

	vector<Route*> subgraphs;
	int i = 0;
	//For each subgraph of r OG size n, n-1, n-k, insert the subgraph
	for (int n = highestSubroute; n >= lowestSubroute; n--) {
		subgraphs = r->getSubgraphs(n);
	
		printPlot("Routes/targetRoute");
		r->printPlot("Routes/insertionRoute");
		for (Route * r : subgraphs)
			r->printPlot("Routes/sub" + to_string(i++));

		targetRoute = OrigRoute;

		for (auto subgraph : subgraphs) {
			targetRoute.insertSubgraph(subgraph);
			minCost = targetRoute.getTransCost();
			if (minCost <= bestCost) {
				bestRoute[n] = targetRoute;
				bestCost = minCost;
			}
			targetRoute = OrigRoute;
		}
		minCost = 200000;
		bestCost = 200000;
		bestRoute[n].printPlot("Routes/best" + to_string(n));
	}



	//newroute->printPlot("Routes/afterMerge" + to_string(rand()%100));

	//Remove random size of the new route
	int averge = (int)round((route.size() + r->route.size()) / 2);
	int averageDifference = OrigRoute.route.size() - (int)round((route.size() + r->route.size()) / 2);

	int max = min(averageDifference + 2, OrigRoute.route.size() - 2);
	int min = max(averageDifference - 2, 0);

	int remove = rand() % (max - min + 1) + min;
	//Route.removeSubroute(remove);

	//Ensure route is time feasible
	//while (!OrigRoute.isFeasible())
	//	OrigRoute.removeSubroute(1);

	return new Route();
}



vector<NodeIRP*> Route::getSubroute(int size)
{
	double minCost = 10000;
	vector<NodeIRP*> nodes = cheapestRemoval(size, minCost);
	return nodes;
}




int Route::counter = 1;

Route::Route()
	:
	Id(0)
{

}

Route::Route(vector<NodeIRP*>& path)
	:
	route(path),
	Id(counter++)
{
	//add edges
	for (int i = 0; i < route.size() - 1; i++)
		route[i]->Node::addEdge(route[i + 1]);
}

Route & Route::operator=(const Route & cpRoute)
{
	//Release memory of old route
	for (NodeIRP * node : route)
		delete node;

	//Copy nodes
	route.resize(cpRoute.route.size());
	for (int i = 0; i < cpRoute.route.size(); i++)
		route[i] = new NodeIRP(*cpRoute.route[i]);

	//Add edges
	for (int i = 0; i < cpRoute.route.size()-1; i++) {
		NodeIRP* v = cpRoute.route[i+1]->getNextNode();
		cpRoute.route[i+1]->Node::addEdge(v);
	}

	return *this;
}

NodeIRP * Route::operator[](int i)
{
	return route[i];
}

Route * Route::copyRoute()
{
	vector<NodeIRP*> path;
	path.resize(route.size());
	auto u = new NodeIRP();
	for (int i = 0; i < route.size(); i++) {
		path[i] = u;
		auto id = route[i]->getEdge()->getEndNode()->getId();
		auto v = new NodeIRP(*Instance.getNode(id));
		u->Node::addEdge(v);
		u = v;
	}
	return new Route(path);
}

NodeIRP * Route::front()
{
	return route.front();
}

NodeIRP * Route::back()
{
	return route.back();
}

void Route::insertSubroute(vector<NodeIRP *> subroute)
{
	/*printRoute();
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
	while (u->getId() != 0) {
		u = u->getEdge()->getEndNode();
		route[position] = u;
	}
	printRoute();*/
}

//Returns nodes to be connected for cheapest removal
vector<NodeIRP*> Route::cheapestRemoval(int subroutesize, double &minCost)
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
		C_uk = u->getTransCost(k);
		C_lv = l->getTransCost(v);
		C_uv = u->getTransCost(v);

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


bool Route::isDuplicate(Route * r)
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

double Route::getTransCost()
{
	double transCost = 0;
	for (NodeIRP * node : route)
		transCost += node->getTransCost(node->getNextNode());

	return transCost;
}








