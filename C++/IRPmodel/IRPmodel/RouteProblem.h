#pragma once
#include "Route.h"
#include "Solution.h"
#include "xprb_cpp.h"
#include "xprs.h"
#include "NodeInstanceDB.h"
#include "RouteAnalyzer.h"


using namespace ::dashoptimization;

class RouteProblem
{
public:

	friend class Solution;

	RouteProblem(const NodeInstanceDB & Instance, vector<Route*> routes);
	~RouteProblem();

	vector<Route *> getSelectedRoutes();
	void addRouteConstraints();
	void addRestrictedShiftCtr(double nRoutes, double oldDel, double oldPick);
	void formulateMIP();
	vector<NodeIRP*> getNodesInShiftPeriod();
	void formulateRouteProblem(int objectiveSelection = ModelParameters::HIGHEST_TOTALCOST);
	void initializeRouteParameters();
	void initializeRouteVariables();
	void addInventoryCtr();
	void addChangeCtr();
	void printRouteType();
	void saveRouteType();
	int getShiftPeriod();
	static void clearRouteCounter();
	static void printRouteTypeToFile(string filename, double improvement);
	void formulateMinVisitProblem();
	void shiftQuantityCtr(int quantity);
	void initializeRoutes();
	int getnRoutes();
	void updateEdges(Solution* sol);
	void printRouteMatrix();
	void addRoutesToVector();
	void updateSolution(Solution * sol);
	void lockRoutes(vector<int> Periods);


	int ShiftPeriod;
	time_t StartTime;
	double SolutionTime;

	//Route types
	static int Orig;
	static int Insertion;
	static int InsRem;
	static int Combination;
	static int LeastServedRemoval;
	static int LeastServedInsertion;
	static int DoubleInsertion;
	static vector<vector <int>> RouteCounter;
	static int Iteration;
	void countRouteTypes();

	Solution * solveProblem(Solution * currentSol = 0);
private:
	XPRBprob routeProblem;

	//Constraint holders
	vector<vector<XPRBctr>> loadingCtr;
	vector<vector <XPRBctr>> ArcCtr;
	vector<vector<XPRBctr>> VisitCtr;

	bool MIP = false;

	//Set holder for MIP
	vector<NodeIRP*> AllNodes;
	vector<NodeIRP*> Nodes;
	vector<NodeIRP*> DeliveryNodes;
	vector<NodeIRP *> PickupNodes;

	//Variables
	XPRBvar ** x;
	XPRBvar extraVehicle;
	XPRBvar * y;
	XPRBvar * timeVar;
	XPRBexpr objective;

	//For min visit
	XPRBvar * delta;

	//Variables
	XPRBvar ** inventory;
	XPRBvar ** delivery;
	XPRBvar ** pickup;
	XPRBvar *** loadDelivery;
	XPRBvar *** loadPickup;
	XPRBvar ** travelRoute;

	void clearVariables();
	void lockRoute(Route *);
	int getRoutePosition(int routeId);

	void fillRoutes(vector<vector<Route* >>& routes);
	void fillNodes(vector<NodeIRPHolder*> &nodeHolder);
	void fillLoad(vector<NodeIRPHolder*> &nodeHolder);
	//Holder of all routes
	vector <Route *> routes;
	const NodeInstanceDB & Instance;
	//Parameters*********
	vector<double> RouteCost;
	//Route matrix
	vector<int **>A;
	//Variables

	//Set of all routes
	vector <int> Routes;
};

