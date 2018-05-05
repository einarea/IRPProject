#pragma once

#include "NodeIRP.h"
#include "NodeInstanceDB.h"
#include "graphAlgorithm.h"
#include <list>

class Route
{
	static int counter;
public:
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

	int getPeriod();
	bool isFeasible();
	void resize(int size);
	void generateRoute(const Route *, list<Route> & routeHolder);
	void reverseRoute();
	void setPeriod(int period);
	bool coincide(Route* r);
	int getDirection() const;
	bool sameDirection(const Route *);
	void createSeperateRoute(Route *);
	bool inRoute(Node *);
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
private:
	int Id;
	int Period;
};

