// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KParallelSolverEventListener_I.h                          *
// * Description : Default parallel solver event listener			  *
// ********************************************************************

#ifndef __KPARALLELSOLVEREVENTLISTENER_I_H
#define __KPARALLELSOLVEREVENTLISTENER_I_H

#include "Globals.h"
#include "KSolverEventListener.h"

class DLLIMPORTEXPORT KParallelSolverEventListener : public KSolverEventListener
{
protected:
	KParallelSolver_I* _parallelSolver;
	KSolver_I* _solver;
	bool _stopOnSolutionFound;
	bool _sendSolutionMessage;
	bool _stopComputations;

public:
	KParallelSolverEventListener();
	KParallelSolverEventListener(KParallelSolver_I* parallelSolver, KSolver_I* solver, bool stopOnSolutionFound = false, bool sendSolutionMessage = false);
	KParallelSolverEventListener(const KParallelSolverEventListener& toCopy);
	KParallelSolverEventListener& operator=(const KParallelSolverEventListener& toCopy);
	virtual ~KParallelSolverEventListener();

	void setStopComputations(bool stopComputations);
	void setStopOnSolutionFound(bool stopOnSolutionFound);
	void setSendSolutionMessage(bool sendSolutionMessage);

	virtual void processMessages();
	void nodeExplored(int thread);
	void branchGoUp(int thread);
	void branchGoDown(int thread);
	void solutionFound(const KSolution& solution, int thread);
	bool stopComputations();

}; // class KParallelSolverEventListener

class DLLIMPORTEXPORT KOptimizeListener : public KParallelSolverEventListener
{
protected:
	bool _dichotomic;
	double _optimalityBound;

public:
	KOptimizeListener();
	KOptimizeListener(KParallelSolver_I* parallelSolver, KSolver_I* solver, bool stopOnSolutionFound = false, bool dichotomic = false);
	KOptimizeListener(const KOptimizeListener& toCopy);
	KOptimizeListener& operator=(const KOptimizeListener& toCopy);
	~KOptimizeListener();

	void setOptimalityBound(double optimalityBound);
	void processMessages();
	void solutionFound(const KSolution& solution, int thread);
	void postObjectiveCut(double objectiveValue);

}; // class KOptimizeListener

class DLLIMPORTEXPORT KOptimizeWithISListener : public KParallelSolverEventListener
{
protected:
	bool _dichotomic;
	double _optimalityBound;

public:
	KOptimizeWithISListener();
	KOptimizeWithISListener(KParallelSolver_I* parallelSolver, KSolver_I* solver, bool stopOnSolutionFound = false);
	KOptimizeWithISListener(const KOptimizeWithISListener& toCopy);
	KOptimizeWithISListener& operator=(const KOptimizeWithISListener& toCopy);
	~KOptimizeWithISListener();

	void setDichotomic(bool dichotomic);
	void setOptimalityBound(double optimalityBound);
	void processMessages();
	void solutionFound(const KSolution& solution, int thread);
	void postObjectiveCut(double objectiveValue);

}; // class KOptimizeWithISListener

#endif
