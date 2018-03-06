#pragma once


#include "xprb_cpp.h"
#include "MapIRP.h"
#include <vector>
#include "CustomerDB.h"
#include "Node.h"
#include "xprs.h"
#include <string.h>

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

	
	//Class to store routes and cost of routes
	class Route {
	public:
	int getCost();
	int id;
	int removeNode(Node*, IRP::Solution&);
	int getPosition(Node * node);
	int getId();
	int ** getRouteMatrix(IRP * const instance);
	int period;
	Route(vector <Node*> & path, int id, int t = 0);
	//Graph 
	vector <Node*> route;
	};

	//Class to store solutions to the instance
	class Solution {
	public:
		Solution(IRP &model, bool integer);
		Solution(IRP &model, bool integer, double ** y, double ***x, double **del, double **pic, double ***loadDel, double ***loadPic, double **inv, double ** time);
		int SolID;
		double **ySol;
		double ***xSol;
		double **delSol;
		double **pickSol;
		double	***loadDelSol;
		double ***loadPickSol;
		double **invSol;
		double **timeSol;
		double **pCapacity;

		vector <IRP::Route*> SolutionRoutes;

		void buildGraph(vector<Node*> &graph, int t);
		void print(string filname);

		bool IntegerSolution = false;
		bool isFeasible();
		bool isRouteFeasible(IRP::Route *);
		double getNumberOfRoutes();
		int newRoute(vector <Node*> & route, int period);

		double getObjective();
		void printSolution();
		double getTransportationCost();
		double getHoldingCost();

		//Operators
		double removeVisit(IRP::Route * route, int selection = 1);
		double fixInventory(Node *, int selection = 1, int period);

		private:
		//Integer solutions
			void propagateInventory(Node*, int period);
			double getTransInteger();
		IRP &instance;
	
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

