// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KClpLinearRelaxationSolver.h							  *
// ********************************************************************

#ifndef KClpLinearRelaxationSolver_H
#define KClpLinearRelaxationSolver_H


#include "Globals.h"
#include "KalisHybrid.h"

#ifdef CLP_LINRELAX
#include "ClpSimplex.hpp"
#endif

class DLLIMPORTEXPORT KClpLinearRelaxationSolver : public KLinearRelaxationSolver {
protected:
	KHybridSolution * _hybridSol;
public:
	/** \brief Constructor (note: no default constructor).

	@param relax domain (a linear relaxation)
	@param obj the objective variable
	@param sense the sense for optimization
	@param storedBasisSize maximum depth for basis reload
	*/
	KClpLinearRelaxationSolver(KLinearRelaxation& relax, const KNumVar& obj, KProblem::Sense sense, int storedBasisSize = 1000);
	/// copy constructor
	KClpLinearRelaxationSolver(const KClpLinearRelaxationSolver&);
	/// destructor
	virtual ~KClpLinearRelaxationSolver();
	
	/** \brief Set objective variable.
	@param var the new objective variable
	*/
	void setObjective (const KNumVar& var);
	
	/** \brief Call the solver.
	Call (Clp solver) and return an error code (see class KLinearRelaxationSolver for its meaning).
	*/
	int solve ();

	// Get the bound computed by the solver (see class KLinearRelaxationSolver).
	virtual double getBound(void) const;
	
	//int getNumberGlobals(void) const;
	
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
	
#ifdef CLP_LINRELX
	/** \brief "Interface" with XPRS Optimizer.
	Allows the user to manipulate directly the internal XPRS problem.
	@return a pointer to the ClpSimplex object representing the problem
	*/
	ClpSimplex * getClpProblem (void);
#endif

	/*
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

	/** Cut generation */
	void generateCuts(KLinearRelaxation *);

	
	/*Note that LP solve must be complete.
	@param var the variable whose reduced cost in the current LP solution is retrieved
	@return reduced cost value
	*/
	double getReducedCost(const KNumVar& var) const;
	double getReducedCost(const KAuxVar& var) const;
	/// Activate or deactivate solver verbose output.

	void printSolverOutput (bool flag);
	void printSolverOutput (bool flag,void * callback,void * userPtr);
	/// Activate or deactivate presolve.
	void setPresolve(bool);
	//virtual void roundingSolution();
	virtual void instantiateNumVarsToCurrentSol(void);
	virtual void instantiateNumVarToCurrentSol(KNumVar* var);
};

#endif
