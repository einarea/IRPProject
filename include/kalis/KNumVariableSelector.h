// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNumVariableSelector.h                                    *
// * Description : class which is in charge of founding a variable    *
// * following specific parameteres (used in KBranchingScheme (mainly *
// * KAssignVar))                                                     *
// ********************************************************************

#ifndef __KNUMVARIABLE_SELECTOR_H
#define __KNUMVARIABLE_SELECTOR_H

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

class DLLIMPORTEXPORT KNumVariableSelector :
	public KExternalObject<KNumVariableSelector_I>,
	public KPtrArray<KNumVariableSelector>
{
private:
	KProblem* _problem;

public:

	/// Constructor
	KNumVariableSelector();

	/// Constructor with KProblem
	KNumVariableSelector(KProblem* problem);

	// Constructor with minDomainSize
	KNumVariableSelector(const int minDomainSize);

	// Constructor with KNumVariableSelector_I*
	KNumVariableSelector(KNumVariableSelector_I* numVariableSelector, bool manage = true);

	/// Copy constructor
	KNumVariableSelector(const KNumVariableSelector& toCopy);

	// Destructor
	virtual ~KNumVariableSelector();

	KProblem* getProblem() const;

	//methods
	void setMinDomainSize(const int minDomainSize);
	/** virtual interface method to overload for definition of your own variable selection heuristics
	@param intVarArray Array of variable from wich selecting  a variable
	*/
	virtual KNumVar * selectNextVariable(KNumVarArray* numVarArray); 
	virtual void getVariableRank(KNumVar * varToRank,double &best,KNumVar * &varResult,bool first);
	virtual KNumVariableSelector* getCopyPtr() const;
	virtual KNumVariableSelector* getInstanceCopyPtr(const KProblem& problem) const;

	virtual void printName() {printf("VARIABLE_SELECTOR\n");}
	virtual char * getName() { return "VARIABLE_SELECTOR"; }

}; // class KNumVariableSelector

#endif


