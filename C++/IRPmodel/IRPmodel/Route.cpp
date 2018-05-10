#include "stdafx.h"
#include "Route.h"


Route::~Route()
{
	for (NodeIRP* node : route)
		delete node;
}

void Route::insertCheapestNode(vector<const NodeIRP*> nodes)
{
	double minCost = 100000;
	double tempCost = 100000;

	NodeIRP * start = 0;
	NodeIRP * end = 0;
	Route bestNode;
	vector<NodeIRP*> insertionPoints;
	vector<NodeIRP *> n;
	n.resize(1);

	printPlot("Routes/beforeInsertion");
	for (const NodeIRP * node : nodes) {
		if (!inRoute(node)) {
			n[0] = new NodeIRP(*node);
			Route singleNode(n);
			insertionPoints = cheapestInsertion(&singleNode, tempCost);
			if (tempCost < minCost) {
				bestNode = singleNode;
				minCost = tempCost;
				start = insertionPoints.front();
				end = insertionPoints.back();
			}
		}
	}

	//constuct the new route
	insert(start, end, new Route(bestNode));
	printPlot("Routes/afterInsertion");			
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

int Route::getDirection() const
{
	//vector product
	if (route.front() != route.back()->getNextNode())
		exit(104);

	NodeIRP * u = route.front();
	NodeIRP *	v = u->getNextNode();
	NodeIRP * w = route.back();

	//calculate vectors
	int x1 = v->getPosX()-u->getPosX();
	int y1 = v->getPosY() - u->getPosY();
	
	int x2 = u->getPosX()- w->getPosX();
	int y2 = u->getPosY() - w->getPosY();

	double degrees = atan2(x1*y2 - y1*x2, x1*x2 + y1*y2) * 180 / 3.14159;
	if (degrees >= 0) {
		printPlot("Routes/r_clockwise");
		return ModelParameters::CLOCKWISE;
	
	}
	else {
		printPlot("Routes/r_counterclockwise");
		return ModelParameters::COUNTER_CLOCKWISE;
		
	}

}

bool Route::sameDirection(const Route * r)
{
	return (getDirection() == r->getDirection());
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

bool Route::inRoute(const Node * n)
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
		exit(105);
	}

	vector<NodeIRP*> tempSubgraph;
	vector<Route*> subgraphs;
	int nGraphs = route.size() - n;
	subgraphs.resize(nGraphs);
	for (int i = 0; i < nGraphs; i++) {
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


//Returns the edges added
void Route::insertSubgraph(Route* subgraph)
{
	double minCost = 100000;
	vector<NodeIRP*> holder = cheapestInsertion(subgraph, minCost);
	insert(holder.front(), holder.back(), subgraph);
}

void Route::insert(NodeIRP * start, NodeIRP * end, Route * subroute)
{
	vector<Node::Edge*> edges;
	NodeIRP* k = subroute->front();
	NodeIRP* l = subroute->back();
	//Insert edges
	start->Node::deleteEdge(end);
	start->Node::addEdge(k);
	start->setState(Node::TABU_EDGE);
	l->Node::deleteEdges();
	l->Node::addEdge(end);
	l->setState(Node::TABU_EDGE);

	updateRoute();
}

void Route::removeNode(NodeIRP * remNode)
{
	for (NodeIRP * node : route) {
		if (node->getNextNode() == remNode) {
			node->deleteEdges();
			node->Node::addEdge(remNode->getNextNode());
			delete remNode;
			updateRoute();
			return;
		}
	
	}

	exit(106);
}

void Route::removeNodes()
{
	vector<NodeIRP*> remove;
	NodeIRP* remNode;
	NodeIRP * connectNode;
	for (NodeIRP * node : route) {
		if (node->getState() != Node::REMOVE) {
			remNode = node->getNextNode();
			if (remNode->getState() == Node::REMOVE) {
				node->deleteEdges();

				do {
					remove.push_back(remNode);
					remNode = remNode->getNextNode();
				} while (remNode->getState() == Node::REMOVE);

				node->Node::addEdge(remNode);
			}
		}
	}

	for (NodeIRP * node : remove) {
		delete node;
	}
	updateRoute();
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

int Route::getId() const
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

	*/vector<NodeIRP*> graph;

	//graph.push_back(delnode);
	//graph.push_back(pickNode);*/

for (NodeIRP* node : route)
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

double Route::removeSubroute(int size)
{
	double minCost = 100000;
	vector<NodeIRP*> Nodes = cheapestRemoval(size, minCost);

	if (route.size() - Nodes.size() >= 2) {

		//Delete edges and add edges

		removeSubgraph(Nodes);
	

	}
	return minCost;
}

void Route::updateRoute()
{
	route.resize(1);
	auto u = route.front()->getEdge()->getEndNode();

	while (u->getId() != 0) {
		route.push_back(u);
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

	for (NodeIRP* u : route) {
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



int Route::getPeriod()
{
	return Period;
}

//Connect the nodes in a route and delete the rest, must be nodes in the route
void Route::removeSubgraph(vector<NodeIRP*> nodes)
{
	//delete subgraph
	NodeIRP * u = nodes.front();
	NodeIRP * v = 0;  
	v = u->getNextNode();
	while (*v != *nodes.back()){
		u = v;
		v = u->getNextNode();
		delete u;
	}

	nodes.front()->deleteEdges();
	nodes.front()->Node::addEdge(nodes.back());
	updateRoute();
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

int Route::getSize()
{
	return route.size();
}

//Generate, cannot take routes that visit the same customer. Use selectRoutes first
void Route::generateRoute(const  Route * r, list<Route> & RouteHolder)
{
	//Copy insertion route
	Route tempRoute2(*r);

	int counter = 0;
	//Make unique routes
	for (const NodeIRP * node1 : route)
		if (!node1->isDepot()) {
			for (NodeIRP * node2 : tempRoute2.route)
				if (*node1 == *node2) {
					node2->setState(Node::REMOVE);
					counter++;
				}
		}


	tempRoute2.removeNodes();
	const Route tempRoute = tempRoute2;
	int lowestSubroute = 3;
	int highestSubroute = 5;
	//Only continue if subgraphs are larger or equal to lowest size and not to modified route
	if (lowestSubroute <= tempRoute.route.size() - 1 && counter < 0.3*r->route.size()) {

		//Target route
		const Route OrigRoute = *this;
		list <Route*> bestRoutes;
		Route bestRoute;
		Route targetRoute;
		double minCost = 100000;
		double bestCost = 100000;
		double removalCost;
		double origTransCost = OrigRoute.getTransCost();


		vector<Route*> subgraphs;
		int i = 0;

		int u = 0;
		OrigRoute.printPlot("Routes/target" + to_string(u));
		//For each subgraph of r OG size n, n-1, n-k, insert the subgraph
		for (int n = min(highestSubroute, tempRoute.route.size() - 1); n >= lowestSubroute; n--) {

			//Subgraphs are copies, the temp route is not affected by changes to subgraphs
			subgraphs = tempRoute.getSubgraphs(n);

			//printPlot("Routes/targetRoute");
			//tempRoute.printPlot("Routes/insertionRoute");

			for (Route * subgraph : subgraphs) {
				//Copy the orig route
				targetRoute = OrigRoute;
				//subgraph->printPlot("Routes/subInsert");
				targetRoute.insertSubgraph(subgraph);
				//targetRoute->printPlot("Routes/targeInsert");
				minCost = targetRoute.getTransCost() - origTransCost;
				if (minCost <= bestCost) {
					bestRoute = targetRoute;
					bestCost = minCost;

					//bestRoute.printPlot("Routes/insertSub" + to_string(u++));
				}
			}

			origTransCost = bestRoute.getTransCost();
			bestRoute.printPlot("Routes/bestinsertion" + to_string(u++));
			int a = max(1, n - ceil(0.3*n));
			int b = min(n + ceil(0.3*n), max(bestRoute.route.size() - 2, 0));
			for (int nRem = a; a <= b; a++) {
				targetRoute = bestRoute;
				removalCost = targetRoute.removeSubroute(nRem);
				targetRoute.constructionCost = bestCost + removalCost;

				targetRoute.printPlot("Routes/bestremoval" + to_string(u++));
				/*for (int j = n - ceil(0.3*n); j <= n + ceil(0.3*n); j++) {
				for (int i = 1; i <= j; i++) {
					removalCost = targetRoute.removeSubroute(1);
					targetRoute.constructionCost = bestCost + removalCost;
					//targetRoute.printPlot("Routes/remove" + to_string(u++));
				}*/

				//Ensure route is time feasible
				while (!targetRoute.isFeasible())
					targetRoute.removeSubroute(1);

				bool Duplicate = false;
				for (Route r : RouteHolder) {
					if (targetRoute.isDuplicate(&r))
						Duplicate = true;
				}


				//Insert route
				if (!Duplicate) {
					Route r = *new Route(targetRoute);
					r.State = MERGE;
					RouteHolder.push_back(r);
				}

			}

			minCost = 200000;
			bestCost = 200000;
		}



		//newroute->printPlot("Routes/afterMerge" + to_string(rand()%100));
		//RouteHolder.push_back(bestRoutes.front());
		//for (Route* r : RouteHolder)
			//r->printPlot("Routes/rr" + to_string(u++));

	}
}

void Route::reverseRoute()
{
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

	//make loop if depot
	if(route[0]->isDepot())
		route[route.size() - 1]->Node::addEdge(route[0]);

}

Route::Route(const Route & cpRoute)
{
	vector<NodeIRP*> path;

	for (NodeIRP * node : cpRoute.route) {
		path.push_back(new NodeIRP(*node));
		int id = cpRoute.getId();
	}
	route = path;
	constructionCost = cpRoute.constructionCost;
	State = cpRoute.State;
	Id = cpRoute.Id;

	//add edges
	for (int i = 0; i < route.size() - 1; i++)
		route[i]->Node::addEdge(route[i + 1]);

	//make loop if depot
	if (route[0]->isDepot())
		route[route.size() - 1]->Node::addEdge(route[0]);
}

Route & Route::operator=(const Route & cpRoute)
{
	//Release memory of old route
	for (NodeIRP * node : route)
		delete node;

	constructionCost = cpRoute.constructionCost;
	State = cpRoute.State;
	Id = cpRoute.Id;

	//Copy nodes
	route.resize(cpRoute.route.size());
	for (int i = 0; i < cpRoute.route.size(); i++)
		route[i] = new NodeIRP(*cpRoute.route[i]);

	//Add edges
	for (int i = 0; i < route.size(); i++) {
		if(i == route.size()-1)
			route[i]->Node::addEdge(route[0]);
		else
			route[i]->Node::addEdge(route[i+1]);
	}

	return *this;
}

Route & Route::operator=(const Route * cpRoute)
{
	//Release memory of old route
	for (NodeIRP * node : route)
		delete node;

	//Copy nodes
	route.resize(cpRoute->route.size());
	for (int i = 0; i < cpRoute->route.size(); i++)
		route[i] = new NodeIRP(*cpRoute->route[i]);

	//Add edges
	for (int i = 0; i < route.size(); i++) {
		if (i == route.size() - 1)
			route[i]->Node::addEdge(route[0]);
		else
			route[i]->Node::addEdge(route[i + 1]);
	}

	return *this;
}

bool Route::operator<(const Route & compRoute) const
{
	return constructionCost < compRoute.constructionCost;
}

bool Route::operator==(const Route & r)
{
	return isDuplicate(&r);
}

NodeIRP * Route::operator[](int i)
{
	return route[i];
}


NodeIRP * Route::front()
{
	return route.front();
}

NodeIRP * Route::back()
{
	return route.back();
}

//Returns the edges added to the route
/*vector<NodeIRP::EdgeIRP*> Route::insertSubroute(vector<NodeIRP *> subroute)
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
	printRoute();
}*/

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
	NodeIRP * v;
	double C_uk;
	double C_lv;
	double C_uv;

	for (int i = 0; i < route.size() - subroutesize; i++) {
		u = route[i];
		k = u->getNextNode();
		l = route[i + subroutesize];
		//Dont remove the recently added edges
		if (!(u->getState() == Node::TABU_EDGE && l->getState() == Node::TABU_EDGE)) {
			v = l->getNextNode();
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
	}

	return Nodes;
}


bool Route::isDuplicate(const Route * r)
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

double Route::getTransCost() const
{
	double transCost = 0;
	for (NodeIRP * node : route)
		transCost += node->getTransCost(node->getNextNode());

	return transCost;
}








