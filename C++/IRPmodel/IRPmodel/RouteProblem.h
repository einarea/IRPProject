#pragma once
#include "Route.h"
#include "Solution.h"
#include "xprb_cpp.h"
#include "xprs.h"
#include "NodeInstanceDB.h"


using namespace ::dashoptimization;

class RouteProblem
{
public:
	RouteProblem(const NodeInstanceDB & Instance, vector<Route*> routes);
	RouteProblem::~RouteProblem();

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
	void updateSolution(Solution * sol);
	void lockRoutes(vector<int> Periods);


	int ShiftPeriod;

	Solution * solveProblem(Solution * currentSol = 0);
private:
	XPRBprob routeProblem;

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

