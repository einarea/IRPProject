// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNumValueSelector.h                                       *
// * Description : class which is in charge of founding the order in  *
// * which value will be tested during Branch and Bound               *
// ********************************************************************

#ifndef __KNUMVALUE_SELECTOR_H
#define __KNUMVALUE_SELECTOR_H

#include "Globals.h"

/**
Abstract interface class for value selection heuristic
@see KMaxToMin
@see KMinToMax
@see KMiddle
@see KRandomValue
@see KNearestValue
@version 2013.1
*/

class DLLIMPORTEXPORT KNumValueSelector :
	public KExternalObject<KNumValueSelector_I>,
	public KPtrArray<KNumValueSelector>
{
private:
	KProblem* _problem;

public:

	/// Constructor
	KNumValueSelector();

	/// Constructor with KProblem
	KNumValueSelector(KProblem* problem);

	/// Constructor with KNumValueSelector_I*
	KNumValueSelector(KNumValueSelector_I* numValueSelector, bool manage = true);

	/// Copy constructor
	KNumValueSelector(const KNumValueSelector& toCopy);

	// Destructor
	virtual ~KNumValueSelector();

	KProblem* getProblem() const;

	//methods
	/** virtual method to overload with your own value selection heuristic
	@param intVar the variable to selects a value for
	*/
	virtual double selectNextValue(KNumVar* intVar); 
	virtual KNumValueSelector* getCopyPtr() const;
	virtual KNumValueSelector* getInstanceCopyPtr(const KProblem& problem) const;

	virtual void printName() {printf("VALUE_SELECTOR\n");}
	virtual char * getName() { return "VALUE_SELECTOR"; }
}; // class KNumValueSelector

#endif


