// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KVariableSelector.h                                       *
// * Description : class which is in charge of choosing a variable    *
// * following specific parameteres (used in KBranchingScheme (mainly *
// * KAssignVar))                                                     *
// ********************************************************************

#ifndef __VARIABLE_SELECTOR_H
#define __VARIABLE_SELECTOR_H

#include "Globals.h"

/** 
Abstract interface class for variable selection heuristic
@see KSmallestDomain
@see KMaxDegree
@see KSmallestMin
@see KSmallestMax
@see KLargestMin
@see KLargestMax
@see KRandomVariable
@see KSmallestDomDegRatio
@see KMaxRegretOnLowerBound
@see KMaxRegretOnUpperBound
@version 2013.1
*/
class DLLIMPORTEXPORT KVariableSelector :
	public KExternalObject<KVariableSelector_I>,
	public KPtrArray<KVariableSelector>
{
private:
	KProblem* _problem;

public:

	/// Constructor
	KVariableSelector();

	/// Constructor with KProblem
	KVariableSelector(KProblem* problem);

	/// Constructor with KVariableSelector_I*
	KVariableSelector(KVariableSelector_I* variableSelector, bool manage = true);

	/// Copy constructor
	KVariableSelector(const KVariableSelector& toCopy);

	// Destructor
	virtual ~KVariableSelector();

	KProblem* getProblem() const;

	//methods
	/** virtual interface method to overload for definition of your own variable selection heuristics
	@param intVarArray Array of variable from wich selecting  a variable
	*/
	virtual KIntVar * selectNextVariable(KIntVarArray* intVarArray); 
	virtual double getVariableRank(KIntVar * varToRank,double &best,KIntVar * &varResult,bool first);
	virtual KVariableSelector* getCopyPtr() const;
	virtual KVariableSelector* getInstanceCopyPtr(const KProblem& problem) const;

	virtual void printName() { printf("KVariableSelector\n");}
	virtual char* getName() { return "KVariableSelector"; }

}; // class KVariableSelector

#endif


