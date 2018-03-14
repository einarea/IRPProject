// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSolver.h                                                 *
// * Description : user class which is in charge of solving your      *
// * problem                                                          *
// ********************************************************************

#ifndef __SOLVER_H
#define __SOLVER_H

#include "Globals.h"
class KSolverEventListener;

/// KSolver is the main class for solving problems defined in a KProblem instance.
/**	 
 * 
 *	Once the problem has been fully built, we can begin to look for solutions.
 *	For this, the main class to be used is KSolver, which allows us to :
 *	<ul>
 *		<li> look for one solution    
 *		<li> look for all solutions
 *		<li> look for another solution when we already know some of them
 *		<li> look for the optimal solution according to the problem objective
 *	</ul>
 *	A KSolver object must be associated to a specific problem. Here is how we can declare and create a KSolver which will be associated to our problem :
 *	@code
 *	KSolver mySolver(myProblem);
 *	@endcode	
 *	When performing its solving functionalities, our object mySolver will store all solutions in the myProblem object.
 *	Retrieving these solutions and working on them is the subject of the next section.
 *	
 *	In order to find only one solution to our problem, we would write:
 *	@code
 *	mySolver.solve();
 *	@endcode	
 *	The solve() method looks for any valid solution and stores it in the associated KProblem object.
 *	
 *	In order to fine all solutions to the problem, we would write:
 *	@code
 *	mySolver.findAllSolutions();
 *	@endcode	
 *	The findAllSolutions() method searches for all solutions of the problem and stores them in the associated KProblem object.
 *
 *	When the problem is too large, it can be very time consuming to search for all solutions.
 *	If one needs to obtain more than one unique solution, then he should use the KSolver findNextSolution() method. For example :
 *	@code
 *	for ( int solutionIndex = 0; solutionIndex < 5; solutionIndex++)
 *		mySolver.findNextSolution(); 
 *	mySolver.endLookingForSolution();
 *	@endcode
 *	The findNextSolution() method searches for the next solution and stop in a restartable state.
 *	To go back to the state before search, it is necessary to call the endLookingForSolution() method after the successive calls to findNextSolution().
 *
 *	In order to find the optimal solution to the problem, we would write:
 *	@code
 *	mySolver.optimize();
 *	@endcode	
 *	The optimize() method searches for the optimal solutions according to the problem objective and stores it in the associated KProblem object.
 *
 *	In order to fine tune the search, one may set integer or double control parameters using the setIntControl() and setDblControl() methods.
 *
 *	Statistics on the search can be obtained using the getIntAttrib() and getDblAttrib() methods.
 *
 *	Multi-threaded search is automatically activated provided that the KProblem object holds multiple problem instances and that the KSolver::NumberOfThreads control parameter is greater than 1.
 *	@see KProblem::KProblem(...)
 *
 *	@version 2013.1
 */
class  DLLIMPORTEXPORT KSolver {

private:

	// Pointer to the internal solver
	KSolver_I* _solver_I_ptr;
	bool _manage_solver_I_ptr;
	// Pointer to the internal parallel solver
	KParallelSolver_I* _parallelSolver_I_ptr;
	bool _manage_parallelSolver_I_ptr;
	// Pointer to KProblem
	KProblem* _problem;

public:
	/**
	 * Default constructor
	 */
	KSolver();
	/**
	 * Constructor
	 * @param problem the problem to solve
	 */
	KSolver(KProblem& problem);
	/**
	 * Constructor
	 * @param problem the problem to solve
	 * @param branchingSchemeArray the resolution strategy used during branch and bound to solve the problem
	 */
	KSolver(KProblem& problem, KBranchingSchemeArray& branchingSchemeArray);

#ifdef ACK_LIB
	KSolver(KProblem& problem, KSolver_I* solver);
	KSolver(KProblem& problem, KParallelSolver_I* solver);
#endif

	/**
	 * CopyConstructor
	 */
	KSolver(const KSolver& toCopy);

	/**
	 * Destructor
	 */
	virtual ~KSolver();

	/**
	 * Get the KProblem instance
	 */
	KProblem* getProblem() const;

	/** 
	 * Search for a solution to the problem
	 * @return true when a solution was found
	 * @return false otherwhise
	 * @see IntControl
	 * @see DblControl
	 */
	int solve();
	/** 
	 * Search for all solutions to the problem
	 * @return true when a solution was found
	 * @return false otherwhise
	 * @see IntControls
	 * @see DblControls
	 */
	int findAllSolutions();
	/** 
	 * Start looking for a solution to the problem or look for a new one
	 * @return true if a solution was found
	 * @return false otherwhise		
	 * @see IntControls
	 * @see DblControls
	 */
	int findNextSolution();
	/**
	 * Stop looking for solutions and restore the state before search
	 */
	int endLookingForSolution();
	/**
	 * Search for an optimal solution to the problem.
	 * @param optimizeWithRestart boolean indicating if the search has to be restarted after finding a solution. 
	 * @see NumberOfSolutionBetweenRestarts
	 * @param dichotomicSearch boolean indicating the type of search (linear or dichotomic) to optimize the objective variable
	 * @see OptimizationAlgorithm
	 * @return true when a solution was found
	 * @return false otherwhise
	 * @see IntControls
	 * @see DblControls
	 */
	int optimize(const bool optimizeWithRestart=false, const bool dichotomicSearch=false);

	/**
	 * Search limit attributes
	 */
	enum SearchLimitAttrib {
		SearchLimitUnreached,					///< Search has not been limited
		SearchLimitedByNodes,					///< Search has been limited by maximum number of nodes explored.    @see MaxNumberOfNodes
		SearchLimitedBySolutions,				///< Search has been limited by maximum number of solutions found.   @see MaxNumberOfSolutions
		SearchLimitedByDepth,					///< Search has been limited by maximal tree search depth.           @see MaxDepth
		SearchLimitedByTime ,					///< Search has been limited by time.                                @see MaxComputationTime
		SearchLimitedByBacktracks ,				///< Search has been limited by maximum number of backtracks.        @see MaxNumberOfBackTracks
		SearchLimitedByNodesBetweenSolutions,	///< Search has been limited by maximum nodes between two solutions. @see MaxNumberOfNodesBetweenSolutions
		SearchLimitedByUser						///< Search has been limited by user.								
	};
	/**
	 * Tolerance limits attributes
	 */
	enum ToleranceLimitAttrib {
		ToleranceLimitUnreached,			///< Tolerance limit has not been reached.		
		OptimalityToleranceReached,			///< Optimality absolute tolerance has been reached. @see OptimalityTolerance
		OptimalityRelativeToleranceReached	///< Optimality relative tolerance has been reached. @see OptimalityRelativeTolerance
	};
	/**
	 * Integer attributes
	 */
	enum IntAttrib {
		NumberOfNodes,				///< Number of nodes explored.
		Depth,						///< Depth of the search tree.
		SearchLimitReached,			///< Limit reached during resolution.			@see SearchLimitAttrib
		ToleranceLimitReached,		///< Tolerance limit reached during resolution.	@see ToleranceLimitAttrib
		Backtracks,					///< Number of backtracks.
		TotalPropagationTime,		///< Total time elapsed during propagation.
		LastCallPropagationTime,	///< Time elapsed during last propagation.
		TotalPropagationIter,		///< Total fix point iterations for propagation.
		LastCallPropagationIter,	///< Fix point iterations during last propagation.
		MaxReachedDepth				///< Maximum depth reached during search.
	};
	static const int NbIntAttrib = 10;
	/**
	 * Double attributes
	 */
	enum DblAttrib {
		ComputationTime,			///< Total computation time.
		BestBound					///< Best bound on the optimal solution.
	};
	static const int NbDblAttrib = 2;
	/**
	 * Return a integer attribute of the solver.
	 * @param attrib the integer attribute to retrieve
	 * @see IntAttrib
	 */
	int  getIntAttrib(KSolver::IntAttrib attrib);
	/**
	 * Return a double attribute of the solver.
	 * @param attrib the double attribute to retrieve
	 * @see DblAttrib
	 */
	double getDblAttrib(KSolver::DblAttrib attrib);
	/**
	 * Integer controls
	 */
	enum IntControl {
		MaxNumberOfNodes,					///< Maximum number of nodes to explore.
		MaxNumberOfSolutions,				///< Maximum number of solutions to find.
		MaxDepth,							///< Maximum depth of the search tree.
		MaxNumberOfBackTracks,				///< Maximum number of backtracks during search.
		StatsPrinting,						///< Print search statistics each KSolver::StatsPrinting seconds (at max).
		CheckSolutionStatus,				///< Check each solution for validity.
		MaxNumberOfNodesBetweenSolutions,	///< Maximum number of nodes between two succesive solutions.
		NumberOfThreads,					///< Number of threads to be used during search. This control parameter is automatically limited by the number of instances in the KProblem object.
		OptimizationAlgorithm,				///< Algorithm used for optimization: less or equal to 0 (default) for branch and bound, 1 for binary search on objective interval, 2 for n-ary search on objective interval (available for multi-threaded optimization only).
		NumberOfSolutionBetweenRestarts		///< Number of solutions between search restarts : less or equal to 0 (default) for no restarts.
	};
	static const int NbIntControl = 10;
	/**
	 * Double controls
	 */
	enum DblControl {
		MaxComputationTime,				///< Maximum computation time.
		OptimalityTolerance,			///< Absolute optimality tolerance (default value: 0.000001 for continuous objective, 1 for integer objective).
		OptimalityRelativeTolerance		///< Relative optimality tolerance (default value: 0.000001 for continuous objective, 0 for integer objective).
	};
	static const int NbDblControl = 3;
	/**
	 * Return the value of an int control
	 * @param control integer control to retrieve
	 * @see IntControl
	 */
	int getIntControl(KSolver::IntControl control);
	/**
	 * Return the value of a double control
	 * @param control double control to retrieve
	 * @see DblControl
	 */
	double getDblControl(KSolver::DblControl control);
	/**
	 * Set the value of an int control
	 * @param control the int control to set
	 * @param value the value of the control
	 * @see IntControl
	 */
	void setIntControl(KSolver::IntControl control, int value);
	/**
	 * Set the value of a double control
	 * @param control tjhe double control to set
	 * @param value value of the control
	 * @see DblControl
	 */
	void setDblControl(KSolver::DblControl control, double value);

	/**
	 * \deprecated
	 * @see setSolverEventListener
	 * Set the node explored function ptr 
	 * @param ptr function pointer
	 * @param param user parameter passed to the function when called
	 */
	void setNodeFunctionPtr(KalisCallBackFunctionPtr ptr,void * param);

	/** 
	 * \deprecated
	 * @see setSolverEventListener
	 * Set the branch explored function ptr (called each time a branch is explored)
	 * @param ptr function pointer
	 * @param param user parameter passed to the function when called
	 */
	void setBranchFunctionPtr(KalisCallBackFunctionPtr ptr1,KalisCallBackFunctionPtr ptr2,void * param);
	/** 
	 * \deprecated
	 * @see setSolverEventListener
	 * Set the Branching scheme switch function ptr 
	 * @param ptr function pointer
	 * @param param user parameter passed to the function when called
	 */
	void setBranchingSchemeFunctionPtr(KalisCallBackFunctionPtr ptr1,void * param);
	// Internal use
	void setAsyncStopFunctionPtr(KalisCallBackFunctionPtr ptr,void * param);
	/** 
	 * \deprecated
	 * @see setSolverEventListener
	 * Set the solution function ptr (called each time a solution is found)
	 * @param ptr function pointer
	 * @param param user parameter passed to the function when called
	 */
	void setSolutionFunctionPtr(KalisCallBackFunctionPtr ptr,void * param);

	/**
	 * Set the solver event listener for tracking and controlling the search
	 * @see KSolverEventListener
	 */
	void setSolverEventListener(KSolverEventListener * listener);
	/*
	* Set the optimality tolerance for the optimization process
	* @param relativity Absolute or relative optimality tolerance
	* @param tolerance tolerance value
	*/
	//void setOptimalityTolerance(bool relativity=false, double tolerance=0);
	/**
	 * Set shaving activation flag
	 */ 
	void useShaving(bool use);
	/** 
	* @return shaving activation flag
	*/
	bool getUseShaving();
	/** 
	* Return the current branching scheme
	*/
	KBranchingScheme getCurrentBranchingScheme();
	/**
	* Return the current variable selector
	*/
	KVariableSelector getCurrentVariableSelector();
	/**
	* Return the current value selector
	*/
	KValueSelector getCurrentValueSelector();
	/**
	* Return a pointer to the current branching object
	*/
	void * getCurrentBranchingObject();
	/**
	* Pretty printing of resolution statistics
	*/
	void printStats();
	/** 
	* Pretty printing of resolution statistics
	*/
	void printStats(void * ctx,PrintFunctionPtr *pfp);
	/**
	* Sets the branching scheme array
	*/
	void setBranchingSchemeArray(KBranchingSchemeArray& branchingSchemeArray, int solverInstance=-1);
	/**
	* Return the default branching scheme array
	*/
	KBranchingSchemeArray * getDefaultBranchingSchemeArray();
	/**
	* Add a relaxation solver 
	*/
	void addRelaxationSolver (KLinearRelaxationSolver * solver,bool initDefaultBranchingScheme = false);
	/**
	* Use reducing cost fixing
	*/
	void setUseReducedCostFixing (bool flag);
	/**
	* Do a local optimization 
	*/
	bool localOptimization();

#ifdef ACK_LIB
	KSolver_I* getSolver_I_ptr() const {return _solver_I_ptr;};
	KParallelSolver_I* getParallelSolver_I_ptr() const {return _parallelSolver_I_ptr;};
#endif


}; // class KSolver

#endif


