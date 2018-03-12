#pragma once


#include "xprb_cpp.h"
#include "MapIRP.h"
#include <vector>
#include "CustomerDB.h"
#include "xprs.h"
#include <string.h>
#include "Node.h"

using namespace ::dashoptimization;
using namespace::std;

class IRP
{
private:
	//Problem
	CustomerIRPDB database;
	MapIRP map;
	XPRBprob prob;
	XPRSprob oprob;		//Express problem to use with cuts
	bool MaskOn;


	//Parameters
	bool ARC_RELAXED;
	int ** VisitNode; //Crossover mask. Which customer to visit.

	int NumOfCustomers;
	int MaxTime;
	int Capacity;
	int NumOfPeriods;
	int ** Dist;
	int ** TransCost;
	int * HoldCost;
	int * InitInventory;
	int * UpperLimit;
	int * LowerLimit;
	int ** Demand;
	int ** TravelTime;
	//Map map;

	//Variables
	XPRBvar *** x;					//Arc variables
	XPRBvar ** y;					//Customer visits variables
	XPRBvar ** inventory;		
	XPRBvar ** delivery;
	XPRBvar ** pickup;
	XPRBvar *** loadDelivery;
	XPRBvar *** loadPickup;
	XPRBvar ** time;

	//Route matrix
	vector<int **>A;


	XPRBexpr objective;
	int ** t;


	//Forward declaration
	class Solution;
	class NodeIRP;
	class Route;


	//Utility functions
	bool initializeSets();
	void generateMask();
	void generateMask(int ** mask);
	bool initializeVariables();
	bool initializeParameters();
	bool formulateProblem();
	void buildGraph(vector <Node*> &, int, bool includeDepot);

	void printGraph(vector <Node> &);

	int SolutionCounter; // Tracks number of solutions, solution's ID equals the counter value at the time added
	int getCounter();
	

	vector <Route *> routes;
	vector <Solution *> solutions;


public:
	
	//Sets
	int startTime;
	vector  <int> Periods;
	vector  <int> AllPeriods;
	vector  <int> DeliveryNodes;
	vector <int> PickupNodes;
	vector  <int> Nodes;
	vector  <int> AllNodes;
	vector  <int> Depot;
	vector <int> integers1;
	
	//For valid ineq
	double ** ExcessConsumption;
	double ** ExcessProd;

	//*****************Inner classes************************//
	//Class to store routes and cost of routes
	class Route {
		public:
		int getCost();
		int id;
		//int removeNode(NodeIRP*, IRP::Route *);
		//void insertSubRoute(vector<NodeIRP *>, NodeIRP * start, NodeIRP * end);
	

		int getPosition(Node * node);
		int getId();
		int ** getRouteMatrix(IRP * const instance);
		int period;
		Route(vector <NodeIRP*> & path, int id, int t = 0);
		//Graph 
		vector <NodeIRP*> route;
	}; //end class Route

	//Class to store solutions to the instance
	class Solution {
	public:
		Solution(IRP &model, bool integer);
		Solution(IRP &model, vector<vector<IRP::Route*>> & r,bool integer);
		int SolID;
		double **pCapacity;

		vector<NodeIRP *> Nodes;

		//The set of routes for each period
		vector<vector <IRP::Route*>> Routes;

		//void buildGraph(vector<Node*> &graph, int t);
		void print(string filname);

		bool IntegerSolution;
		bool isFeasible();
		bool isRouteFeasible(IRP::Route *);
		double getNumberOfRoutes(int period);
		NodeIRP * getNode(int id);
		int newRoute(vector <Node*> & route, int period);

		int getnPeriods();
		double getObjective();
		void printSolution();
		double getTransportationCost();
		double getHoldingCost();
		double getHoldingCost(int period);
		double getTransportationCost(int period);

		//Operators
		void removeVisit(IRP::Route * route, int selection = 1);
		IRP::Route * insertSubrouteInRoute(IRP::Route * subroute, int period);

		private:
		//Integer solutions
			vector<NodeIRP*> selectPair(IRP::Route *, int Selection);

		IRP &instance;
	
	}; //End class solution

	//Class to store nodes
	class NodeIRP :
		public Node
	{
	public:
		IRP& Instance;

		class EdgeIRP : public Edge {
		public:
			EdgeIRP(Node *child, double loadDel, double loadPick, int t, double value);
			NodeIRP * getEndNode();
			double LoadDel;
			double LoadPick;
			int Period;
		};

		NodeIRP(int id, IRP & model);
		~NodeIRP();
		//Override
		void addEdge(double loadDel, double loadPick, NodeIRP * child, int period, double value);
		EdgeIRP * getEdge(int period);
		static NodeIRP * getNode(Node *);
		vector<double> Quantity;
		vector<double> Inventory;
		vector<double> TimeServed;
		vector <EdgeIRP*> getEdges(int period);
		double getOutflow(int period);
		double getHoldCost(int period);
		

		void propInvForw(int period);
		void changeQuantity(int period, int quantity);
		void updateInventory(int period);

		bool isDelivery();
	};

	IRP(CustomerIRPDB&, bool relaxed = false, bool maskOn = false, int ** VisitMask = 0);
	//void addSolution(int ** y, int ***x, int **d, int **pic, int ***loadDel, int ***loadPic, int **inv, int ** t);


	Route * getRoute(int id);
	vector<Route const*> getRoutes();
	int newRoute(vector <Node*> &path);
	void addValidIneq();

	//Route functions
	void printRouteMatrix();
	void addRoutesToVector();
	void printMatrix();
	int getCapacity();
	Map * getMap();
	void calculateExcess();
	void IRP::buildGraph(vector<Node*> &graph, int t, Solution * solution);
	int allocateSolution();
	int allocateIRPSolution();
	bool sepStrongComponents(vector<XPRBcut> &);
	void addSubtourCut(vector<vector <Node *>> &, int t, bool &, vector<XPRBcut> &);
	IRP::Solution * solveModel();
	XPRBprob * getProblem();
	CustomerDB * getDB();
	vector <XPRBbasis> SavedBasis;
	int nSubtourCuts;
	int getNumOfPeriods();
	int getNumOfCustomers();
	void getVisitedCustomers(int period, vector <Customer *> &);
	void getDemand(int period, vector<vector<double>> &, vector <Customer *> &);
	Solution * getSolution(int id);
	void printBounds();

	~IRP();
};

