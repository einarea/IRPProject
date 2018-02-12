// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSolverEventListener.h                                    *
// * Description : listener class that allows to interact with the    *
// * solver during the search                                         *
// ********************************************************************

#ifndef __KSOLVEREVENTLISTENENER_H
#define __KSOLVEREVENTLISTENENER_H

#include "Globals.h"

class DLLIMPORTEXPORT KSolverEventListener
{
private:
	KProblem* _problem;

public :
	// Default constructor
	KSolverEventListener();

	// Constructor with KProblem
	KSolverEventListener(KProblem* problem);

	// Virtual destructor
	virtual ~KSolverEventListener();

	KProblem* getProblem() const;

	/** 
	* Called after constraint propagation in each node
	*/
	virtual void nodeExplored(int thread) {};

	/** 
	* Called after each branchGoDown event
	*/
	virtual void branchGoDown(int thread) {};

	/** 
	* Called after each branchGoUp event
	*/
	virtual void branchGoUp(int thread) {};

	/** 
	* Called after each bracnhing scheme switch
	*/
	virtual void branchingScheme() {};
	/** 
	* Ask user for termination at each node
	*/
	virtual bool stopComputations() { return false; };
	/** 
	* Called each time a solution is found
	*/
	virtual void solutionFound(const KSolution& solution) {}
	virtual void solutionFound(const KSolution& solution, int thread) {solutionFound(solution);}

	virtual KSolverEventListener* getInstanceCopyPtr(const KProblem& problem) const;
};

#endif
