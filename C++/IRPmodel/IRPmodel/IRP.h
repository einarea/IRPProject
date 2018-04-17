#pragma once


#include "xprb_cpp.h"
#include "MapIRP.h"
#include <vector>
#include "CustomerDB.h"
#include "xprs.h"
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
	void getSubset(vector <int>  subset, int subsetSize, int node, vector <int> & IncludeNodes, int t1, int t2);

	//Parameters
	bool ARC_RELAXED;
	int ** VisitNode; //Crossover mask. Which customer to visit.

	void addInventoryAndLoadingCtr(XPRBprob & problem);

	//Subtour
	double EDGE_WEIGHT;
	double alpha;
	double ExcessParameter;

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
		
	XPRBvar ** delivery;
	XPRBvar ** pickup;
	XPRBvar *** loadDelivery;
	XPRBvar *** loadPickup;
	XPRBvar ** time;
	XPRBvar ** actionDelivery;
	XPRBvar ** actionPickup;
	XPRBvar *** simAction;
	double epsilon = 0.001;

	XPRBctr ** TabuMatrix;
	double ** CountMatrix;




	XPRBexpr objective;
	int ** t;




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
	





public:
	//Forward declaration
	class Solution;
	class NodeIRP;
	class NodeIRPHolder;
	class Route;

	vector <IRP::Solution *> solutions;

	XPRBvar ** inventory;
	XPRBvar ** y;
	XPRBvar *** x;

	IRP::Solution * allocateSolution(IRP&);
	void fillSolution(IRP::Solution * sol);

	//To store all routes that are generated
	vector <Route *> routes;

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


	void clearVariables();
	
	//For valid ineq
	double *** ExcessConsumption;
	double *** ExcessProd;

	//Solving the aggregated model
	class IRPproblem
	{


	};

	//*****************Inner classes************************//
	//Class to store routes and cost of routes
	class Route {
		static int counter;
		public:
		void setId(int id);
		IRP & Instance;
		double getTransportationCost();
		int getPeriod();
		bool isFeasible();
		void setPeriod(int period);
		bool coincide(vector<NodeIRP*> r);
		//int removeNode(NodeIRP*, IRP::Route *);
		//void insertSubRoute(vector<NodeIRP *>, NodeIRP * start, NodeIRP * end);
	
		Route * copyRoute();
		void insertSubroute(vector<NodeIRP *> subroute);
		vector<IRP::NodeIRP*> cheapestInsertion(vector<NodeIRP*> subroute, double &minCost);
		vector<IRP::NodeIRP*> cheapestRemoval(int subroutesize, double &maxCost);
		int getTotalDelivery();
		int getTotalPickup();
		void mergeRoute(IRP::Route * mergeIn);
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
		Route(vector <NodeIRP*> & path, IRP &instance);
	
		//Graph 
		vector <NodeIRP*> route;
		private:
			int Id;
			int Period;
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
		Solution(IRP &instance, vector<NodeIRPHolder*> nodes);
		int SolID;
		IRP::NodeIRP * getDepot(int period);
		
		double **pCapacity;
		int *** getRouteMatrix();
		vector<NodeIRPHolder *> NodeHolder;
		int selectPeriod(int selection);
		vector<NodeIRPHolder *>& getNodes();

		//The set of routes for each period

		//void buildGraph(vector<Node*> &graph, int t);
		void print(string filname,int weight);

		void generateRoutes(int period);
		vector <NodeIRP *> getVisitedNodes(int period);
		void sort(vector <NodeIRP>*);
		bool IntegerSolution;
		bool isFeasible();
		void clearRoutes();
		void clearEdges(int period);
		int shiftQuantity(int PeriodSelection, int ObjectiveSelection);
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
		vector<Route*> getAllRoutes();
		double getObjective();
		void printSolution();
		double getTransportationCost();
		double getHoldingCost();
		double getHoldingCost(int period);
		double getTransportationCost(int period);
		IRP &Instance;

		//Operators
		void removeVisit(IRP::Route * route, int selection = 1);
		void solveInventoryProblem();
		void insertCustomer(int customerID, int period);
		void removeCustomer(int customerID, int period);
		vector<NodeIRP*> getCustomer(int id);
		void insertSubrouteInRoute(vector<NodeIRP*> subroute, int period);
		void createSubroute(vector<NodeIRP*> nodes);

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
		RouteProblem(IRP & Instance, vector<IRP::Route*> routes);
		void addRouteConstraints();
		void formulateRouteProblem(int objectiveSelection);
		void initializeRouteParameters();
		void initializeRouteVariables();
		void addInventoryCtr();
		int getShiftPeriod();
		void shiftQuantityCtr(int quantity);
		void initializeRoutes();
		void printRouteMatrix();
		void addRoutesToVector();
		void updateSolution(IRP::Solution * sol);
		void lockRoutes();

		int ShiftPeriod;
	
		IRP::Solution * solveProblem(IRP::Solution * currentSol = 0);
	private:
		XPRBprob routeProblem;

		//Variables
		XPRBvar ** inventory;
		XPRBvar ** delivery;
		XPRBvar ** pickup;
		XPRBvar *** loadDelivery;
		XPRBvar *** loadPickup;
		XPRBvar ** travelRoute;

		void lockRoute(IRP::Route * );
		int getRoutePosition(int routeId);

		void fillRoutes(vector<vector<IRP::Route* >>& routes);
		void fillNodes(vector<NodeIRPHolder*> &nodeHolder);
		void fillLoad(vector<NodeIRPHolder*> &nodeHolder);
		//Holder of all routes
		vector <Route *> routes;
		IRP & Instance;
		//Parameters*********
		vector<double> RouteCost;
		//Route matrix
		vector<int **>A;
		//Variables
	
		//Set of all routes
		vector <int> Routes;
	};

	//Class that solves a VRP for a period while pushing quantity to other periods and its existing routes
	/*class RouteProblemWithVRP : public RouteProblem {
	public:
		RouteProblemWithVRP(IRP & Instance, int period, vector<IRP::Route*> routes);
		void initializeVaribles();
		void formulateProblem();
		void solveProblem();
		void addRoutingConstraints();
	private:
		int VRPperiod;
	};*/

	IRP(CustomerIRPDB&, bool relaxed = false, bool maskOn = false, int ** VisitMask = 0);
	//void addSolution(int ** y, int ***x, int **d, int **pic, int ***loadDel, int ***loadPic, int **inv, int ** t);

	void addVisitConstraint(double ** VisitMatrix, int selection);
	Route * getRoute(int id);
	vector<Route const*>& getRoutes();
	int newRoute(vector <Node*> &path);
	void addValidIneq(int ValidInequality);

	//Solution information
	double ** getVisitDifference(IRP::Solution * sol1, IRP::Solution * sol2);



	//Route functions

	void updateTabuMatrix(double ** changeMatrix);
	int getNumOfNodes();
	int solCounter;
	void printMatrix();
	int getCapacity();
	void useIPSubtourElimination();
	void useLPSubtourElimination();
	bool LPSubtour;
	Map * getMap();
	void calculateExcess();
	void IRP::buildGraphSol(vector<Node*> &graph, int t, Solution * solution);
	IRP::Solution* allocateIRPSolution();
	bool sepStrongComponents(vector<XPRBcut> &);
	void addSubtourCut(vector<vector <Node *>> &, int t, bool &, vector<XPRBcut> &);
	bool addSubtourCtr(vector<vector <Node *>> &, int t);
	IRP::Solution * solveModel();
	IRP::Solution * solveLPModel();
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

