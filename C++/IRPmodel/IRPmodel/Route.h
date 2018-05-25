#pragma once

#include "NodeIRP.h"
#include "NodeInstanceDB.h"
#include "graphAlgorithm.h"
#include <list>

class Route
{


	static int counter;
public:

	//How route was constructed
	static const int ORIG = 0;
	static const int SIMPLE_INSERTION = 1;
	static const int SIMPLE_REMOVAL = 7;
	static const int INSERTION_REMOVAL = 2;
	static const int DOUBLE_INSERTION_REMOVAL = 6;
	static const int LEAST_SERVED_REMOVAL = 3;
	static const int LEAST_SERVED_INSERTION = 4;
	static const int RESTRICTED_LEAST_SERVED_REMOVAL = 8;
	static const int MERGE = 5;

	void setId(int id);
	Route();
	Route(vector <NodeIRP*> & path);
	//Copy constructot
	Route(const Route & r);
	//Copy assignment operator
	Route& operator = (const Route&);
	Route & operator = (const Route * cpRoute);
	//Compares the construction cost of routes
	bool operator < (const Route&) const;
	bool operator ==(const Route&);
	bool operator !=(const Route& r) {
		return !(*this == r);
	}
	NodeIRP * operator [](int i);


	//Destructor
	~Route();
	void clearState();
	void insertCheapestNode(vector<const NodeIRP*> nodes);
	int getPeriod();
	void removeSubgraph(vector <NodeIRP*>);
	bool isFeasible();
	void resize(int size);
	int getSize();
	void generateRoute(const Route *, list<Route> & routeHolder, list<Route> & tabuRoutes);
	void reverseRoute();
	void setPeriod(int period);
	bool coincide(Route* r);
	int getDirection() const;
	bool sameDirection(const Route *);
	void createSeperateRoute(Route *);
	bool inRoute(const Node *);
	vector<Route*> getSubgraphs(int n) const;
	bool isDuplicate(const Route * r);
	double getTransCost() const;
	//int removeNode(NodeIRP*, Route *);
	//void insertSubRoute(vector<NodeIRP *>, NodeIRP * start, NodeIRP * end);

	NodeIRP * front();
	NodeIRP * back();
//	vector<NodeIRP::EdgeIRP *> insertSubroute(vector<NodeIRP *> subroute);
	vector<NodeIRP*> cheapestInsertion(Route* subroute, double &minCost);
	vector<NodeIRP*> cheapestRemoval(int subroutesize, double &maxCost);
	int getTotalDelivery();
	int getTotalPickup();
	
	void insertSubgraph(Route* subgraph);
	void insert(NodeIRP * start, NodeIRP * end, Route* subroute);
	void removeNode(NodeIRP * );
	void removeNodes();
	int getPosition(Node * node);
	int getId() const;
	int** getRouteMatrix(int gridSize);
	void printPlot(string filename) const;
	void printRoute();
	double getResidualCapacity(int capacity);
	double removeSubroute(int selection);
	void updateRoute();
	vector<NodeIRP*> getSubroute(int selection, double & minCost);
	
	double constructionCost;


	//Graph 
	vector <NodeIRP*> route;
	int State;
private:
	int Id;
	int Period;
};

