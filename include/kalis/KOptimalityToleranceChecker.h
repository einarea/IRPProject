// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KOptimalityToleranceChecker.h                             *
// * Description : Optimality tolerance checkers					  *
// ********************************************************************

#ifndef __KOPTIMALITYTOLERANCECHECKER_I_H
#define __KOPTIMALITYTOLERANCECHECKER_I_H


#include "Globals.h"


/**
* This interface sets a framework for objects providing method to check if the current solution is close enough to
* the optimum, and, if not, to give a new bound to set on the objective variable
*/
class DLLIMPORTEXPORT KOptimalityToleranceChecker {
public:
	/**
	* Check for the optimality tolearance
	*
	* @param bestSolutionObj
	* @param bestBound
	* @return <tt>true</tt> is the best solution is close enough - for some criteria - to the optimum
	*/
	virtual bool isGoodEnough(double bestSolutionObj, double bestBound) const = 0;

	/**
	* Returns a bound to set on the objective, in order to look for solution which are not too close from the
	* current best known solution. This prevent from storing too many solutions which are very similar.
	*
	* @param bestSolutionObj the best objective value of already found solutions.
	* @return a bound to set on the objective.
	*/
	virtual double nextBoundToTry(double bestSolutionObj) const = 0;

	/**
	* Returns a bound to set on the objective, in order to look for solution which are much better from the
	* current best known solution. This allows to improve solution bounds much faster in some problems.
	*
	* @param bestSolutionObj the best objective value of already found solutions.
	* @param noBetterSolutionThanThis the best bound on the optimal solution
	* @return a bound to set on the objective.
	*/
	//virtual double nextDichotomicBoundToTry(double bestSolutionObj, double noBetterSolutionThanThis) const = 0;

};

/**
* An <tt>OptimalityToleranceChecker</tt> to use with integer objective only.
*/
class DLLIMPORTEXPORT KIntegerObjectiveOptimalityChecker : public KOptimalityToleranceChecker {

private :

	int increment;

public :

	KIntegerObjectiveOptimalityChecker(bool maximize);

	bool isGoodEnough(double bestSolutionObj, double bestBound) const;

	double nextBoundToTry(double bestSolutionObj) const;

	//double nextDichotomicBoundToTry(double bestSolutionObj, double noBetterSolutionThanThis) const;
};


/**
* An <tt>OptimalityToleranceChecker</tt> to use with any type of <tt>KNumVar</tt> objective, which use an absolute
* difference criteria
*/
class DLLIMPORTEXPORT KAbsoluteToleranceOptimalityChecker : public KOptimalityToleranceChecker {

private :
	double positiveTolerance;
	double nextBoundDelta;

public :

	KAbsoluteToleranceOptimalityChecker(bool maximize, double tolerance);

	bool isGoodEnough(double bestSolutionObj, double bestBound) const;

	double nextBoundToTry(double bestSolutionObj) const;
};

/**
* An <tt>OptimalityToleranceChecker</tt> to use with any type of <tt>KNumVar</tt> objective, which use a relative
* difference criteria
*/
class DLLIMPORTEXPORT KRelativeToleranceOptimalityChecker : public KOptimalityToleranceChecker {

private :
	double ratio; // always positive, and less than 1
	double nextBoundRatio;

public :

	KRelativeToleranceOptimalityChecker(bool maximize, double tolerance);

	bool isGoodEnough(double bestSolutionObj, double bestBound) const;

	double nextBoundToTry(double bestSolutionObj) const;
};

/**
* An <tt>OptimalityToleranceChecker</tt> to use with any type of <tt>KNumVar</tt> objective, which use both
* a relative and absolute difference criteria
*/
class DLLIMPORTEXPORT KNumObjectiveOptimalityChecker : public KOptimalityToleranceChecker {

private :
	bool _maximize;
	double ratio; // always positive, and less than 1
	double nextBoundRatio;
	double positiveTolerance;
	double nextBoundDelta;

public :

	KNumObjectiveOptimalityChecker(bool maximize, double absoluteTolerance, double relativeTolerance);

	bool isGoodEnough(double bestSolutionObj, double bestBound) const;

	double nextBoundToTry(double bestSolutionObj) const;
};


#endif
