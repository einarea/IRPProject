#pragma once
#include <stdlib.h>
#include "stdafx.h"
#include "NodeIRPHolder.h"
#include "Route.h"
#include "RouteProblem.h"
#include "VRPmodel.h"


//Class to store solution of an IRP problem
class Solution
{
public:
	static int SolutionCounter; 
	//Constructors
	Solution(const NodeInstanceDB & model, bool integer);
	Solution(const NodeInstanceDB & model, vector<NodeIRPHolder*> nodes);
	Solution(Solution&);
	void updateSolution(Solution&);
	//Destructors
	Solution::~Solution();

	int SolID;
	NodeIRP * getDepot(int period);

	//Solves an vrp for the period and update the solution
	void solveVRP(int period);

	void buildGraph(vector<Node*> &graph);

	void buildGraph(vector<Node*> &graph, int t);
	static Solution * allocateSolution(const NodeInstanceDB &);
	void routeSearch(int period);
	void routeOptimize();
	double **pCapacity;
	int *** getRouteMatrix();

	//Database
	vector<NodeIRPHolder *> Nodes;

	int selectPeriod(int selection);
	vector<NodeIRPHolder *>& getNodes();

	//The set of routes for each period

	//void buildGraph(vector<Node*> &graph, int t);
	void print(string filname, int weight);

	void mergeRoutes(int position, Route * route, vector<Route*> &Routes, vector<Route*> &newRoutes);
	void generateRoutes(vector<Route* >&routeHolder);
	vector <NodeIRP *> getVisitedNodes(int period);
	void sort(vector <NodeIRP>*);
	bool IntegerSolution;
	bool isFeasible();
	void clearRoutes();
	void clearEdges(int period);
	void shiftQuantity(int PeriodSelection, int ObjectiveSelection);
	bool isRouteFeasible(Route *);
	double getNumberOfRoutes(int period);
	double getResidualCapacity(int period);
	//Returns the amount of total product deliveried less the capacity x (number of vehicles used - 1)
	double getTotalDelivery(int period);
	//Returns the amount of total product pickup up less the capacity x (number of vehicles used - 1)
	double getTotalPickup(int period);
	double getNodeVisits(int period);
	double ** getVisitedMatrix();
	double getDeliveryNodeVisits(int period);
	double getPickupNodeVisits(int period);
	//Returns average delivery per node
	double getDelivery(int period);
	double getPickup(int period);
	vector <Route *> getRoutes(int period);
	NodeIRPHolder * getNode(int id);
	int newRoute(vector <Node*> & route, int period);

	int getnPeriods();
	vector<Route*> getAllRoutes();
	double getObjective();
	void printSolution();
	double getTransportationCost();
	double getHoldingCost();
	double getHoldingCost(int period);
	double getTransportationCost(int period);

	//Operators
	void removeVisit(Route * route, int selection = 1);
	void solveInventoryProblem();
	void insertCustomer(int customerID, int period);
	void removeCustomer(int customerID, int period);
	vector<NodeIRP*> getCustomer(int id);
	void insertSubrouteInRoute(vector<NodeIRP*> subroute, int period);
	void createSubroute(vector<NodeIRP*> nodes);

private:
	static int solCounter;
	//Integer solutions
	const NodeInstanceDB & Instance;
	vector<NodeIRP*> selectPair(Route *, int Selection);



};

