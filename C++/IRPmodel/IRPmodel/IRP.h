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
					//Arc variables
				//Customer visits variables
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
	class NodeIRPHolder;
	class Route;


	//Utility functions
	bool initializeSets();
	void generateMask();
	void generateMask(int ** mask);
	bool initializeVariables();
	bool initializeParameters();
	bool formulateProblem();
	void buildGraph(vector <Node*> &, int, bool includeDepot, double weight=0.01);

	void printGraph(vector <Node> &);

	int SolutionCounter; // Tracks number of solutions, solution's ID equals the counter value at the time added
	int getCounter();
	

	vector <Route *> routes;
	vector <Solution *> solutions;


public:
	XPRBvar ** y;
	XPRBvar *** x;
	vector<vector<vector<XPRBvar>>> subtourIndices;
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
		IRP & Instance;
		int getCost();
		int id;
		//int removeNode(NodeIRP*, IRP::Route *);
		//void insertSubRoute(vector<NodeIRP *>, NodeIRP * start, NodeIRP * end);
	

		int getPosition(Node * node);
		int getId();
		int ** getRouteMatrix(IRP * const instance);
		int period;
		double getResidualCapacity();
		Route(vector <NodeIRP*> & path, int id, IRP &instance, int t);
		//Graph 
		vector <NodeIRP*> route;
	}; //end class Route

	//Class to store solutions to the instance

	//Class that define a IRP node in a particular period
	class NodeIRP :
		public Node
	{
	public:
		class EdgeIRP : public Edge {
		public:
			EdgeIRP(Node *child, double loadDel, double loadPick, double value);
			NodeIRP * getEndNode();
			double LoadDel;
			double LoadPick;
		};

		NodeIRP(int id);
		~NodeIRP();
		//Override
		void addEdge(double loadDel, double loadPick, NodeIRP * child,  double value);
		EdgeIRP * getEdge();
		static NodeIRP * getNode(Node *);
		double Quantity;
		double Inventory;
		double TimeServed;
		vector <EdgeIRP*> getEdges();
		double getOutflow();
		double getHoldCost();
	};

	//Class to store nodes over all periods
	class NodeIRPHolder {
	public:
		int Id;
		int getId();
		NodeIRPHolder(int id, IRP &instance);
		IRP & Instance;
		vector<NodeIRP*> Nodes;

		//Changes
		void addEdge(double loadDel, double loadPic, NodeIRPHolder * child, int period, double value);

		//Information
		void propInvForw(int period);
		double getOutflow(int period);
		double quantity(int period);
		double timeServed(int period);

		vector <NodeIRP::EdgeIRP*> getEdges(int period);
		NodeIRP::EdgeIRP * getEdge(int period);
		double getHoldCost(int period);
		void changeQuantity(int period, int quantity);
		void updateInventory(int period);
		bool isDelivery();
	};

	class Solution {
	public:
		Solution(IRP &model, bool integer);
		Solution(IRP &model, vector<vector<IRP::Route*>> & r,bool integer);
		int SolID;
		double **pCapacity;
		vector<NodeIRPHolder *> NodeHolder;

		//The set of routes for each period
		vector<vector <IRP::Route*>> Routes;

		//void buildGraph(vector<Node*> &graph, int t);
		void print(string filname,int weight);

		vector <NodeIRP *> getVisitedNodes(int period);
		void sort(vector <NodeIRP>*);
		bool IntegerSolution;
		bool isFeasible();
		bool isRouteFeasible(IRP::Route *);
		double getNumberOfRoutes(int period);
		double getResidualCapacity(int period);
		double getNodeVisits(int period);
		double getDeliveryNodeVisits(int period);
		double getPickupNodeVisits(int period);
		double getDelivery(int period);
		double getPickup(int period);
		vector <IRP::Route *> getRoutes(int period);
		NodeIRPHolder * getNode(int id);
		int newRoute(vector <Node*> & route, int period);

		int getnPeriods();
		double getObjective();
		void printSolution();
		double getTransportationCost();
		double getHoldingCost();
		double getHoldingCost(int period);
		double getTransportationCost(int period);
		IRP &Instance;

		//Operators
		void removeVisit(IRP::Route * route, int selection = 1);
		IRP::Route * insertSubrouteInRoute(IRP::Route * subroute, int period);

		private:
		//Integer solutions
			vector<NodeIRP*> selectPair(IRP::Route *, int Selection);
			
		
	
	}; //End class solution

	

	

	IRP(CustomerIRPDB&, bool relaxed = false, bool maskOn = false, int ** VisitMask = 0);
	//void addSolution(int ** y, int ***x, int **d, int **pic, int ***loadDel, int ***loadPic, int **inv, int ** t);

	void addVisitConstraint(double ** VisitedMatrix);
	Route * getRoute(int id);
	vector<Route const*> getRoutes();
	int newRoute(vector <Node*> &path);
	void addValidIneq();

	//Route functions
	void printRouteMatrix();
	int getNumOfNodes();
	void addRoutesToVector();
	void printMatrix();
	int getCapacity();
	Map * getMap();
	void calculateExcess();
	void IRP::buildGraph(vector<NodeIRP*> &graph, int t, Solution * solution);
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

