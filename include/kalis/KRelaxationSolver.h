// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KRelaxationSolver.h 								  *
// ********************************************************************

#ifndef KRelaxationSolver_H
#define KRelaxationSolver_H

#include "Globals.h"
#include "KProblem.h"

/**	\class KRelaxationSolver
	\brief This class is intended as a superclass for linear relaxation solvers.

	Such a solver must be provided with
  - a linear relaxation (KLinearRelaxation)
  - an objective variable (KNumVar)
  - a sense for optimization (KProblem::Sense).

  It relies on a LP/MIP solver to provide the following information:
  - a value (a bound for the relaxed problem, cf method getBound())
  - a solution, possibly not feasible for the original problem, but which can be used to guide the search for a feasible solution
  - if the problem is LP, reduced costs (that can be used for instance in the "reduced cost fixing" procedure).
  *	 @version 2013.1
*/



class DLLIMPORTEXPORT KRelaxationSolver : public KAbstractPtrArray<KRelaxationSolver>
{
protected:
	KRelaxationSolver_I * _KRelaxationSolver_IPtr;
	KRelaxationSolverConfigurator_I* _configurator;
public:
	// destructor
	virtual ~KRelaxationSolver();
	// Get internal pointer (reserved for internal use)
	KRelaxationSolver_I * getInternalPtr (void);
	/** \brief Set objective variable.

	@param var the new objective variable
	*/
	virtual void setObjective (const KNumVar& var) = 0;
	/** \brief Solve the relaxed optimization problem.

	This methods returns the following error codes :
	- 0 : optimal
	- 1 : infeasible
	- 2 : search interrupted prematurely, a solution was found
	- 3 : search interrupted prematurely, no solution was found
	- 4 : other problem
	*/
	virtual int solve () = 0;
	/** \brief Get the (lower for minimization, upper for maximization) bound computed by solve().

	@return bound computed by solve()
	Note that:
	- solve() method must be called before the getBound() method
	- moreover, the return code provided by solve() must be checked before using the value
	returned by getBound().
	*/
	virtual double getBound(void) const = 0;
	virtual double getBestBound(void) const = 0;
	// Print the internal state of the solver (discouraged)
	virtual void print (void) const = 0;
	/** \brief Is a KIntVar set global ?

	@param var the variable whose status is checked
	*/
	bool isGlobal (const KIntVar&);
	/** \brief Is a KAuxVar set global ?

	@param var the variable whose status is checked
	*/
	bool isGlobal (const KAuxVar&);
	/** \brief Set (or unset) a KIntVar global.

	@param var variable to check
	@param isGlobal new global status
	*/
	void setGlobal (const KIntVar& var, bool isGlobal);
	/** \brief Set (or unset) a KAuxVar global.

	@param var variable to check
	@param isGlobal new global status
	*/
	void setGlobal (const KAuxVar& var, bool isGlobal);
	/** \brief (Un)set variables global.
	
	Set or unset as "global" all KIntVar and KAuxVar with global type (note that a 
	KFloatVar variables are not modified, since it would make little sense to set them global.
	@param isGlobal new global status
	*/
	void setAllGlobal (bool isGlobal);
	/** \brief Set all indicator auxiliary variables global.

	@param isGlobal new global status
	*/
	void setIndicatorsGlobal (bool isGlobal);
	/** \brief Set the sense of optimization (maximize, minimize).

	@param sense new sense for optimization
	*/
	void setSense(KProblem::Sense);
	/** \brief Get the current MIP solution for a given KNumVar variable.

	@param var variable whose solution is checked
	@return value of var in the current MIP solution
	*/
	virtual double getMIPSolution(const KNumVar& var) const = 0;
	/** \brief Get the current MIP solution for a given KAuxVar variable.

	@param var variable whose solution is checked
	@return value of var in the current MIP solution
	*/
	virtual double getMIPSolution(const KAuxVar& var) const = 0;
	/** \brief Get the current LP solution for a given KNumVar variable.

	@param var variable whose value is checked
	@return value of var in the current MIP solution
	*/
	virtual double getLPSolution(const KNumVar& var) const = 0;
	/** \brief Get the current relaxed solution for a given KAuxVar variable.

	@param var variable whose value is checked
	@return value of var in the current solution
	*/
	virtual double getLPSolution(const KAuxVar& var) const = 0;

	/** Get the total number of global variables.

	@return the number of global variables among the ones stored in the relaxation
	*/
	virtual int getNumberGlobals (void) const = 0;
	/** \brief Get the total amount of time spent in computations since the object was built.

	@return time spent in the solver since built
	*/
	double getTotalTimeSpentInComputation (void) const;
	/** \brief Get the amount of time spent during the last call to solve().

	@return time spent in the solver during the last call to solve()
	*/
	double getTimeSpentInLastComputation (void) const;
	/** \brief Set this flag to 0 if you want to solve as LP a linear relaxation containing
	global entities (1 is the default).

	@param DEPRECATED (the new way of doing this is to use a configurator); flag new flag value
	*/
	void setSolveAsMIP (bool flag);
	/** \brief Check wether the flag "solveAsMIP" is set.

	@return flag value
	*/
	bool solveAsMIP ();
	/** \brief Stop global search after maxMIPSol feasible solutions found.
	
	(0 for no limit, this is the default).
	If the limit is low, this is likely to cause optimization to end before optimality.
	*/
	void setMaxMIPSol (int);

	// Set the resolution algorithm
	void setAlgorithm(int alg);
	// Get the resolution algorithm
	int getAlgorithm();
	

	/** Instantiate variables to current solution obtained by linear relaxation solver*/
	virtual void instantiateNumVarsToCurrentSol (void) =0;

	/** Instantiate a variables to current solution obtained by linear relaxation solver*/
	virtual void instantiateNumVarToCurrentSol (KNumVar* var) =0;

	/** Cut generation */
	virtual void generateCuts(KLinearRelaxation* relaxation) =0;



	/*Set the configurator of a KRelaxationSolver*/
	void setConfigurator(KRelaxationSolverConfigurator* configurator);

	/*Get the configurator of a KRelaxationSolver*/
	KRelaxationSolverConfigurator* getConfigurator();
};

#endif
