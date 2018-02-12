// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KValueSelector.h                                          *
// * Description : class which is in charge of founding the order in  *
// * which value will be tested during Branch and Bound               *
// ********************************************************************

#ifndef __VALUE_SELECTOR_H
#define __VALUE_SELECTOR_H

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

class DLLIMPORTEXPORT KValueSelector :
	public KExternalObject<KValueSelector_I>,
	public KPtrArray<KValueSelector>
{
private:
	KProblem* _problem;

public:

	/// Constructor
	KValueSelector();

	/// Constructor with KProblem
	KValueSelector(KProblem* problem);

	/// Constructor with KValueSelector_I*
	KValueSelector(KValueSelector_I* valueSelector, bool manage = true);

	/// Copy constructor
	KValueSelector(const KValueSelector& toCopy);

	// Destructor
	virtual ~KValueSelector();

	KProblem* getProblem() const;

	//methods
	/** virtual method to overload with your own value selection heuristic
	@param intVar the variable to selects a value for
	*/
	virtual int selectNextValue(KIntVar* intVar); 
	virtual KValueSelector* getCopyPtr() const;
	virtual KValueSelector* getInstanceCopyPtr(const KProblem& problem) const;

	virtual void printName() {printf("VALUE_SELECTOR\n");}
	virtual char * getName() { return "VALUE_SELECTOR"; }
}; // class KValueSelector

#endif


