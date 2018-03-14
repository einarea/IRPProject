// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KCoinLinearRelaxationSolver.h							  *
// ********************************************************************

#ifndef KCoinLinearRelaxationSolver_H
#define KCoinLinearRelaxationSolver_H


#include "Globals.h"
#include "KalisHybrid.h"

#ifdef COIN_LINRELAX
//#define SOLVER_EXPORT // Allows to link statically to CoinMP.lib instead of dynamically to CoinMP.dll
#include "coinMP.h"
#endif

/**	\class KXPRSLinearRelaxationSolver
	\brief This linear relaxation solver relies on CoinMP to solve the LP/MIP problem.

	@code
    Example:
	...
	KProblem myProblem(mySession,"");
	...
	KLinearRelaxation * relax = myProblem.getLinearRelaxation();
	KCoinLinearRelaxationSolver mySolver(*relax, objectiveVar, KProblem::Minimize);
	@endcode	
	
	You must have the coinMP.lib and coinMP.dll to use this.	
	 @version 2013.1
*/

class DLLIMPORTEXPORT KCoinLinearRelaxationSolver : public KLinearRelaxationSolver {
protected:
	KHybridSolution * _hybridSol;
public:
	/** \brief Constructor (note: no default constructor).

	@param relax domain (a linear relaxation)
	@param obj the objective variable
	@param sense the sense for optimization
	@param storedBasisSize maximum depth for basis reload
	*/
	KCoinLinearRelaxationSolver(KLinearRelaxation& relax, const KNumVar& obj, KProblem::Sense sense, int storedBasisSize = 1000);
	/// copy constructor
	KCoinLinearRelaxationSolver(const KCoinLinearRelaxationSolver&);
	/// destructor
	virtual ~KCoinLinearRelaxationSolver();
	/** \brief Set objective variable.

	@param var the new objective variable
	*/
	void setObjective (const KNumVar& var);
	/** \brief Call the solver.

	Call (CoinMP) and return an error code (see class KLinearRelaxationSolver for its meaning).
	*/
	int solve ();
	/// Get the bound computed by the solver (see class KLinearRelaxationSolver).
	virtual double getBound(void) const;
	/** \brief Get the best bound in a branch and bound tree.

	Useful if search terminated before optimality.
	*/
	virtual double getBestBound(void) const ;
	/** \brief Print the internal state of the solver.

	Use is discouraged, use method writeLP to output the content of the solver.
	*/
	void print (void) const;

	// Write the current problem to a file in lp format.
	int writeLP (const char* filename) const;
	
#ifdef COIN_LINRELAX
	/** \brief "Interface" with CoinMP.
	
	Allows the user to manipulate directly the internal Coin problem.
	@return a pointer to the HPROB object representing the problem
	*/
	HPROB* getCoinProblem(void);
#endif
	/** \brief Generate cuts.

	If possible, cuts are added to the matrix of constraints to make
	the relaxation tighter and improve the bound.
	*/
	void generateCuts(KLinearRelaxation* relaxation);
	/** \brief Print the current solution.

	@param MIPflag to choose whether to print the current MIP solution or the current LP solution
	*/
	void printSolution (bool MIPflag);
	/** \brief Print variables name and their rank.

	This is useful to recover the meaning of the columns in the LP file produced by writeLP().
	*/
	void printVariables (void) const;
	/** \brief Get the current MIP solution for a KNumVar variable.

	@param var variable whose value is checked
	*/
	double getMIPSolution(const KNumVar& var) const;
	/** \brief Get the current MIP solution for a KAuxVar variable.

	@param var variable whose value is checked
	*/
	double getMIPSolution(const KAuxVar& var) const;
	/** \brief Get the current MIP solution for a KNumVar variable.

	@param var variable whose value is checked
	*/
	double getLPSolution(const KNumVar& var) const;
	/** \brief Get the current LP solution for a KAuxVar variable.

	@param var variable whose value is checked
	*/
	double getLPSolution(const KAuxVar& var) const;
	/** \brief Update the KHybridSolution object with the current MIP (MIPflag=true) or LP (MIPflag=false) solution.
	
	@param MIPflag true to get the current MIP solution, false for LP.
	*/
	void updateSolution (bool MIPflag);
	/** \brief Get a pointer to the solution contained in the solver.

	Method updateSolution() must be used before the call.
	*/
	KHybridSolution * getSolutionPtr (void) const;
	/** \brief Get the number of global variables.

	@return number of global variables
	*/
	int getNumberGlobals (void) const;
	/** \brief Get reduced costs.
	
	Note that LP solve is must be complete.
	@param var the variable whose reduced cost in the current LP solution is retrieved
	@return reduced cost value
	*/
	double getReducedCost(const KNumVar& var) const;
	/** \brief Get reduced costs.
	
	Note that LP solve must be complete.
	@param var the variable whose reduced cost in the current LP solution is retrieved
	@return reduced cost value
	*/
	double getReducedCost(const KAuxVar& var) const;
	/// Activate or deactivate solver verbose output.

	void printSolverOutput (bool flag);
	void printSolverOutput (bool flag,void * callback);
	/// Activate or deactivate presolve.
	void setPresolve(bool);
	/// Set MIPRELSTOP double control.
	void setMipRelStop (double);
	//virtual void roundingSolution();
	virtual void instantiateNumVarsToCurrentSol(void);
	virtual void instantiateNumVarToCurrentSol(KNumVar* var);
};

#endif
