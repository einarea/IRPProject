#pragma once


#include "xprb_cpp.h"
#include "NodeInstanceDB.h"
#include <vector>
#include "xprs.h"
#include "Solution.h"
#include "NodeIRPHolder.h"


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

	//Variables	
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

	void buildGraph(vector <Node*> &, int, bool includeDepot, double weight = 0.01);
	vector <Solution *> solutions;

	XPRBvar ** inventory;
	XPRBvar ** y;
	XPRBvar *** x;

	void fillSolution(Solution * sol);

	//To store all routes that are generated
	vector <Route *> routes;

	vector<vector<vector<XPRBvar>>> subtourIndices;
	//Sets
	int startTime;
	vector <int> integers1;


	void clearVariables();
	
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
	

	IRP(const NodeInstanceDB&, bool relaxed = false, bool maskOn = false, int ** VisitMask = 0);
	void addVisitConstraint(double ** VisitMatrix, int selection);
	Route * getRoute(int id);
	vector<Route const*>& getRoutes();
	int newRoute(vector <Node*> &path);
	void addValidIneq(int ValidInequality);

	//Solution information
	double ** getVisitDifference(Solution * sol1, Solution * sol2);

	//Route functions
	void updateTabuMatrix(double ** changeMatrix);
	int getNumOfNodes();
	void addHoldingCostCtr(double holdingCost);
	void printMatrix();
	int getCapacity();
	void useIPSubtourElimination();
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

