#pragma once

#include "NodeIRP.h"
#include "NodeInstanceDB.h"
#include "graphAlgorithm.h"

class Route
{
	static int counter;
public:
	void setId(int id);

	//Destructor
	Route::~Route();

	double getTransportationCost();
	int getPeriod();
	bool isFeasible();
	Route * generateRoute(Route *);
	void setPeriod(int period);
	bool coincide(vector<NodeIRP*> r);
	void createSeperateRoute(Route *);
	bool inRoute(Node *);
	bool isDuplicate(Route * r);
	double getTransCost(NodeIRP * u, NodeIRP * v);
	//int removeNode(NodeIRP*, Route *);
	//void insertSubRoute(vector<NodeIRP *>, NodeIRP * start, NodeIRP * end);

	Route * copyRoute();
	void insertSubroute(vector<NodeIRP *> subroute);
	vector<NodeIRP*> cheapestInsertion(vector<NodeIRP*> subroute, double &minCost);
	vector<NodeIRP*> cheapestRemoval(int subroutesize, double &maxCost);
	int getTotalDelivery();
	int getTotalPickup();
	void mergeRoute(Route * mergeIn);
	void insert(NodeIRP * start, NodeIRP * end, vector<NodeIRP *> subroute);
	int getPosition(Node * node);
	int getId();
	int ** getRouteMatrix();
	void printPlot(string filename);
	void printRoute();
	double getResidualCapacity();
	void removeSubroute(int selection);
	void updateRoute();
	vector<NodeIRP*> getSubroute(int selection);
	Route(vector <NodeIRP*> & path, const NodeInstanceDB &instance);

	//Graph 
	vector <NodeIRP*> route;
private:
	int Id;
	const NodeInstanceDB & Instance;
	int Period;
};

