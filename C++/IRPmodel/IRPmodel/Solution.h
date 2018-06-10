#pragma once
#include <stdlib.h>
#include "stdafx.h"
#include "NodeIRPHolder.h"
#include "Route.h"
#include "RouteProblem.h"
#include "VRPmodel.h"
#include <list>


//Class to store solution of an IRP problem
class Solution
{
public:
	static int SolutionCounter; 
	//Constructors
	Solution(const NodeInstanceDB & model);
	Solution(const NodeInstanceDB & model, vector<NodeIRPHolder*> nodes);
	Solution(const Solution&);
	Solution& operator = (const Solution&);
	bool operator ==(const Solution&);

	void updateSolution(Solution&);
	//Destructors
	~Solution();



	int SolID;
	NodeIRP * getDepot(int period) const;

	//Solves an vrp for the period and update the solution
	void solveVRP(int period);

	vector<Route *> selectRoutesToMerge(vector < const Route *> Routes);
	void buildGraph(vector<NodeIRP*> &graph);

	void buildGraph(vector<NodeIRP*> &graph, int t);
	static Solution * allocateSolution(const NodeInstanceDB &);
	void routeSearch(int REQUIRE_CHANGE = ModelParameters::NO_CHANGE);
	void routeOptimize();
	double **pCapacity;
	int *** getRouteMatrix();

	//Database
	vector<NodeIRPHolder *> Nodes;

	vector<Route *> TabuRoutes;

	void addTabuRoutes(vector <Route*> routes, vector<Route*> origRoutes);

	int selectPeriod(int selection);
	vector<NodeIRPHolder *>& getNodes();

	//The set of routes for each period

	//void buildGraph(vector<Node*> &graph, int t);
	const NodeIRP* getLeastServed(int period) const;
	const NodeIRP* getLeastServed(vector<NodeIRP*>, int period) const;
	Route* removeNodeFromPeriod(int period, const NodeIRP* remNode);
	void print(string filname, int weight);
	int getPeriodWithMinExcess(const vector <int>& Periods);
	void mergeRoutes(int position, Route * route, vector<Route*> &Routes, vector<Route*> &newRoutes);
	void generateRoutes(vector<Route* >&routeHolder);
	bool Solution::sort_func(const Route* & lhs, const Route* & rhs);
	vector <NodeIRP *> getVisitedNodes(int period);
	vector <NodeIRP*> getNotVisitedNodes(int period);
	void plotPeriod(int t, string filename);
	void plot(string filename);
	void sort(vector <NodeIRP>*);
	static double ** Solution::getVisitDifference(const Solution & sol1, const Solution & sol2);
	int getnDivisible();
	int getnSingleService();
	int getTotalNodesServed();
	int getnSimultaneousVisits();
	int getnRoutes();
	bool isFeasible() const;
	void clearRoutes();
	void clearEdges(int period);
	void shiftQuantity(int SELECTION);
	void getStrongComponents();
	void shiftQuantityMIP(int PeriodSelection);
	int getMaxShiftPeriod();
	bool isRouteFeasible(Route *);
	double getNumberOfRoutes(int period) const;
	double getResidualCapacity(int period);
	vector<const NodeIRP*> getNodesIRP(int period);
	//Returns the amount of total product deliveried less the capacity x (number of vehicles used - 1)
	double getTotalDelivery(int period);
	//Returns the amount of total product pickup up less the capacity x (number of vehicles used - 1)
	double getTotalPickup(int period);
	double getNodeVisits(int period);
	double ** getVisitedMatrix() const;
	double getDeliveryNodeVisits(int period);
	double getPickupNodeVisits(int period);
	//Returns average delivery per node
	double getDelivery(int period);
	double getPickup(int period);
	vector <Route *> getRoutes(int period) const;
	int getInfeasiblePeriod();
	NodeIRPHolder * getNode(int id) const;
	int newRoute(vector <Node*> & route, int period);

	int getnPeriods();
	vector<Route*> getAllRoutes() const;
	double getObjective() const;
	void printSolution();
	double getTransportationCost() const;
	double getHoldingCost() const;
	double getHoldingCost(int period) const;
	double getTransportationCost(int period) const;
	Route * insertNodeInPeriod(int period, const NodeIRP * insNode);
	bool isVisited(const NodeIRP * node, int period);

	vector<Route*> SelectedRoutes;
	vector<Route*> GeneratedRoutes;

	//Operators
	void removeVisit(Route * route, int selection = 1);
	void solveInventoryProblem();
	void insertCustomer(int customerID, int period);
	void removeCustomer(int customerID, int period);
	vector<NodeIRP*> getCustomer(int id);
	void insertSubrouteInRoute(Route* subroute, int period);
	void createSubroute(vector<NodeIRP*> nodes);

	double SolutionTime;

private:
	void buildStrongGraph(vector<NodeStrong*>& graph, int t);
	void getSubtours(vector<Node*> result, int t);
	static int solCounter;
	list<int> TabuPeriods;
	//Integer solutions
	const NodeInstanceDB & Instance;
	vector<NodeIRP*> selectPair(Route *, int Selection);



};

