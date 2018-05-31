#pragma once

#ifndef IRP_H
#define IRP_H

#include "xprb_cpp.h"
#include "NodeInstanceDB.h"
#include <vector>
#include "xprs.h"
#include "Solution.h"
#include "NodeIRPHolder.h"
#include <time.h>
#include "SolutionInfo.h"
#include "ModelParameters.h"


using namespace ::dashoptimization;
using namespace::std;

class IRP
{	
private:
	//Problem
	const NodeInstanceDB &Database;
	XPRBprob prob;
	XPRSprob oprob;		//Express problem to use with cuts
	bool MaskOn;
	void getSubset(vector <NodeInstance*>  subset, int subsetSize, int node, vector <NodeInstance*> & IncludeNodes, int t1, int t2);

	//Parameters
	bool ARC_RELAXED;
	int ** VisitNode; //Crossover mask. Which customer to visit.

	void addInventoryAndLoadingCtr(XPRBprob & problem);

	int NumOfCustomers;
	double getTransportationCost();
	double getHoldingCost();
	double getnRoutes();
	int MaxTime;
	int Capacity;
	int NumOfPeriods;


	//Variables	
	XPRBvar * extraVehicle;
	XPRBvar ** delivery;
	XPRBvar ** pickup;
	XPRBvar *** loadDelivery;
	XPRBvar *** loadPickup;
	XPRBvar ** timeVar;
	XPRBvar ** actionDelivery;
	XPRBvar ** actionPickup;
	XPRBvar *** simAction;
	vector<XPRBvar>changeRoute;
	XPRBctr RouteChangeCtr;
	vector<XPRBctr> RouteCtr;
	double epsilon = 0.001;

	vector<vector<XPRBctr>> TabuMatrix;
	double ** CountMatrix;

	XPRBexpr objective;
	int ** t;

	XPRBctr VisitCtr;

	//Utility functions
	bool initializeSets();
	void generateMask();
	void generateMask(int ** mask);
	bool initializeVariables();
	bool initializeParameters();

	void IRP::copyGraph(vector<Node*>& newGraph, vector<NodeStrong*>& cpGraph);
	void IRP::copyGraph(vector<Node*>& newGraph, vector<Node*>& cpGraph);
	bool IRP::getSubtourGraph(vector<Node*>&subtour, int t);

	bool formulateProblem();

	void printGraph(vector <Node> &);

	int SolutionCounter; // Tracks number of solutions, solution's ID equals the counter value at the time added
	int getCounter();

public:

	//Clocks
	time_t lastSolutionFound;
	//Solution data
	double solutionTime = -1;
	bool SubtourElimination = false;
	int nNodes = -1;
	int CutType;
	int timeCounter = 0;
	int nVariables = -1;
	int nConstraints = -1;
	double bestBound = -1;
	double bestSol = -1;
	void printSolutionToFile(double lpOptimat, double trans, double hold, int version, int edge);
	void buildGraph(vector <Node*> &, int, bool includeDepot, double weight = 0.01);
	void buildStrongGraph(vector <NodeStrong*> &, int, bool includeDepot, double weight = -1);
	vector <Solution *> solutions;

	SolutionInfo::InstanceInfo * InstanceData = nullptr;

	XPRBvar ** inventory;
	XPRBvar ** y;
	XPRBvar *** x;

	void fillSolution(Solution * sol);

	//To store all routes that are generated
	vector <Route *> routes;

	vector<vector<int>> subtourIndices;
	vector<vector<int>> matrixValues;
	//Sets
	time_t startTime;
	vector <int> integers1;


	void clearVariables();
	void printStrongCompAlgorithm();
	//For valid ineq
	double *** ExcessConsumption;
	double *** ExcessProd;
	
	class LocalSearch {
	public:
		LocalSearch(IRP & model, Solution *origSol);

		void ShiftQuantity(Solution *);


		//Returns period with the highest transportation costs
		int ChoosePeriod(int selection);
	private:
		IRP & Instance;
		Solution * OrigSol;
	};
	

	IRP(const NodeInstanceDB&, bool relaxed = false, SolutionInfo::InstanceInfo * instance = nullptr, bool maskOn = false, int ** VisitMask = 0);
	void addVisitConstraint(double ** VisitMatrix, int selection, int relaxed = -1);
	Route * getRoute(int id);
	vector<Route const*>& getRoutes();
	int newRoute(vector <Node*> &path);
	void addValidIneq(int ValidInequality);


	//Route functions
	void updateTabuMatrix(double ** changeMatrix);
	int getNumOfNodes();
	void addHoldingCostCtr(double holdingCost);
	void addRouteCtr(vector<Route*> routes, int relaxed = -1);
	void printMatrix();
	int getCapacity();
	void useIPSubtourElimination(int cuttype);
	void useLPSubtourElimination();
	bool LPSubtour;
	void calculateExcess();
	
	Solution* allocateIRPSolution();
	bool sepStrongComponents(vector<XPRBcut> &);
	void addSubtourCut(vector<vector <Node *>> &, int t, bool &, vector<XPRBcut> &);
	bool addSubtourCtr(vector<vector <Node *>> &, int t);
	Solution * solveModel();
	Solution * solveLPModel();
	XPRBprob * getProblem();
	vector <XPRBbasis> SavedBasis;
	int nSubtourCuts;
	int getNumOfPeriods();
	int getNumOfCustomers();
	//void getVisitedCustomers(int period, vector <Customer *> &);
	//void getDemand(int period, vector<vector<double>> &, vector <Customer *> &);
	Solution * getSolution(int id);
	NodeInstanceDB const * getDB();
	//void printBounds();

	~IRP();
};

#endif

