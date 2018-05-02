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
	//Copy assignment operator
	Route& operator =(const Route&);
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
	void generateRoute(Route *, vector<Route*> & routeHolder);
	void setPeriod(int period);
	bool coincide(Route* r);
	void createSeperateRoute(Route *);
	bool inRoute(Node *);
	vector<Route*> getSubgraphs(int n) const;
	bool isDuplicate(const Route * r);
	double getTransCost() const;
	//int removeNode(NodeIRP*, Route *);
	//void insertSubRoute(vector<NodeIRP *>, NodeIRP * start, NodeIRP * end);

	Route * copyRoute();
	NodeIRP * front();
	NodeIRP * back();
//	vector<NodeIRP::EdgeIRP *> insertSubroute(vector<NodeIRP *> subroute);
	vector<NodeIRP*> cheapestInsertion(Route* subroute, double &minCost);
	vector<NodeIRP*> cheapestRemoval(int subroutesize, double &maxCost);
	int getTotalDelivery();
	int getTotalPickup();
	
	void insertSubgraph(Route* subgraph);
	void insert(NodeIRP * start, NodeIRP * end, Route* subroute);
	int getPosition(Node * node);
	int getId();
	int** getRouteMatrix(int gridSize);
	void printPlot(string filename) const;
	void printRoute();
	double getResidualCapacity(int capacity);
	void removeSubroute(int selection);
	void updateRoute();
	vector<NodeIRP*> getSubroute(int selection);
	



	//Graph 
	vector <NodeIRP*> route;
private:
	int Id;
	int Period;
};

