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


	

	void printGraph(vector <Node> &);

	int SolutionCounter; // Tracks number of solutions, solution's ID equals the counter value at the time added
	int getCounter();
	





public:
	//Forward declaration
	class Solution;
	class NodeIRP;
	class NodeIRPHolder;
	class Route;

	void buildGraph(vector <Node*> &, int, bool includeDepot, double weight = 0.01);
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




	//Class to store solutions to the instance

	//Class that define a IRP node in a particular period
	

	
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
	void addHoldingCostCtr(double holdingCost);
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

