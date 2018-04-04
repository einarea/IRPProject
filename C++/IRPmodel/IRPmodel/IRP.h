#pragma once


#include "xprb_cpp.h"
#include "MapIRP.h"
#include <vector>
#include "CustomerDB.h"
#include "xprs.h"
#include <string.h>
#include "Node.h"
#include "ModelInstance.h"

using namespace ::dashoptimization;
using namespace::std;

class IRP : 
	public ModelInstance
{
private:

	//Problem
	ModelInstance Instance;
	CustomerIRPDB database;
	MapIRP map;
	XPRBprob prob;
	XPRSprob oprob;		//Express problem to use with cuts
	bool MaskOn;


	//Parameters
	bool ARC_RELAXED;
	int ** VisitNode; //Crossover mask. Which customer to visit.

	void addInventoryAndLoadingCtr(XPRBprob & problem);


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

	XPRBctr ** TabuMatrix;
	double ** CountMatrix;




	XPRBexpr objective;
	int ** t;


	//Forward declaration
	class Solution;
	class NodeIRP;
	class NodeIRPHolder;
	class Route;

	//
	XPRBctr VisitCtr;

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
	
	vector <Solution *> solutions;


public:
	XPRBvar ** y;
	XPRBvar *** x;

	vector<vector<vector<XPRBvar>>> subtourIndices;
	//Sets
	int startTime;
	

	void clearVariables();
	
	//For valid ineq
	double ** ExcessConsumption;
	double ** ExcessProd;

	//Solving the aggregated model
	class IRPproblem
	{


	};

	//*****************Inner classes************************//
	//Class to store routes and cost of routes
	class Route {
		public:
		IRP & Instance;
		int getCost();
		int id;
		//int removeNode(NodeIRP*, IRP::Route *);
		//void insertSubRoute(vector<NodeIRP *>, NodeIRP * start, NodeIRP * end);
	
		int getTotalDelivery();
		int getTotalPickup();
		int getPosition(Node * node);
		int getId();
		int ** getRouteMatrix();
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
		//
		int isInventoryFeasible();
		int isInventoryFeasible(int period);
		double moveQuantity(int from, int to, double quantity);
		double getMinQuantity();
		double getExcessQuantity();
		void removeMinQuantity();

		//Changes
		void addEdge(double loadDel, double loadPic, NodeIRPHolder * child, int period, double value);

		//Information
		void propInvForw(int period, double quantity);
		double getOutflow(int period);
		double quantity(int period);
		double inventory(int period);
		double timeServed(int period);

		vector <NodeIRP::EdgeIRP*> getEdges(int period);
		NodeIRP::EdgeIRP * getEdge(int period);
		double getHoldCost(int period);
		void changeQuantity(int period, double quantity);
		bool isDelivery();

	private:
		double getFeasibleServiceIncrease(int period);
		double getFeasibleServiceDecrease(int period);
		double getFeasibleServiceMove(int from, int to);
	};

	class Solution {
	public:
		Solution(IRP &model, bool integer);
		Solution(IRP &model, vector<vector<IRP::Route*>> & r,bool integer);
		Solution(IRP &instance, vector<NodeIRPHolder*> nodes);
		int SolID;
		double **pCapacity;
		int *** getRouteMatrix();
		vector<NodeIRPHolder *> NodeHolder;

		vector<NodeIRPHolder *> getNodes();
		//The set of routes for each period
		vector<vector <IRP::Route*>> Routes;

		vector<vector <IRP::Route*>> getRoutes();

		//void buildGraph(vector<Node*> &graph, int t);
		void print(string filname,int weight);

		vector <NodeIRP *> getVisitedNodes(int period);
		void sort(vector <NodeIRP>*);
		bool IntegerSolution;
		bool isFeasible();
		bool isRouteFeasible(IRP::Route *);
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

	
	class LocalSearch {
	public:
		LocalSearch(IRP & model, IRP::Solution *origSol);

		void ShiftQuantity(Solution *);


		//Returns period with the highest transportation costs
		int ChoosePeriod(int selection);
	private:
		IRP & Instance;
		IRP::Solution * OrigSol;
	};
	

	class RouteProblem{
	public:
		RouteProblem(IRP & Instance);
		void addRouteConstraints();
	
		void formulateRouteProblem(vector <Route*> routes, int objectiveSelection);
		void initializeRouteParameters(vector <Route*> routes);
		void initializeRouteVariables();
		void printRouteMatrix();
		void addRoutesToVector();
		Solution * solveProblem();
	private:
		XPRBprob routeProblem;
		Route * getRoute(int routeId);
		Solution * allocateSolution();
		//Holder of all routes
		vector <Route *> routes;
		IRP & Instance;
		//Parameters*********
		//Route matrix
		vector<int **>A;
		//Variables
		XPRBvar ** travelRoute;
		//Set of all routes
		vector <int> Routes;
	};

	IRP(CustomerIRPDB&, bool relaxed = false, bool maskOn = false, int ** VisitMask = 0);
	//void addSolution(int ** y, int ***x, int **d, int **pic, int ***loadDel, int ***loadPic, int **inv, int ** t);

	void addVisitConstraint(double ** VisitMatrix);
	Route * getRoute(int id);
	vector<Route const*> getRoutes();
	int newRoute(vector <Node*> &path);
	void addValidIneq();

	//Solution information
	double ** getVisitDifference(Solution * sol1, Solution * sol2);



	//Route functions

	void updateTabuMatrix(double ** changeMatrix);
	int getNumOfNodes();

	void printMatrix();
	int getCapacity();
	Map * getMap();
	void calculateExcess();
	void IRP::buildGraph(vector<NodeIRP*> &graph, int t, Solution * solution);
	Solution * allocateSolution();
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

