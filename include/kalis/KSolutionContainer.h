// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSolutionContainer.h                                      *
// * Description : user class managing all solutions of a problem     *
// ********************************************************************

#ifndef _SOLUTIONCONTAINER_H
#define _SOLUTIONCONTAINER_H

#include "Globals.h"

/**	 
 *	This class represent a pool of solution of a KProblem.
@code
Example:

KProblem p(...);
KSolver solver(p);
solver.optimize();
KSolution sol = p.getSolutionContainer().getBestSolution();
@endcode	
 *	@see KProblem
 *	@version 2013.1
 */
class DLLIMPORTEXPORT KSolutionContainer : public KAutoExternalObject<KSolutionContainer, KSolutionContainer_I> {

public:
	// Constructors
	KSolutionContainer();
	KSolutionContainer(const KSolutionContainer& toCopy);
	KSolutionContainer(KSolutionContainer_I* solutionContainer);

	// Destructor
	virtual ~KSolutionContainer();

	/// Add a new solution of the solution container
	void add(const KSolution& solution);
	/// Return the number of solutions found
	int getNumberOfSolutions() const;
	/// Remove all solutions from the solution container
	void clear();
	/// Return true if the problem as at least one solution
	bool problemIsSolved() const;
	/// Return the last solution found
	KSolution& getLastSolution() const;
	/// Return the best solution found (if applicable)
	KSolution& getBestSolution() const;
	/// Return solution by index
	KSolution& getSolution(const int index) const;

};

#endif
