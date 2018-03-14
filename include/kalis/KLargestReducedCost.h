// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KLargestReducedCost.h                                     *
// * Description : SubClass of KVariableSelector                      *
// * looks for variable with largest reduced cost in LP relaxation    *
// ********************************************************************

#ifndef __KLARGESTREDUCEDCOST_H
#define __KLARGESTREDUCEDCOST_H

#include "Globals.h"
#include "KVariableSelector.h"
#include "KNumVariableSelector.h"

/**	\class KLargestReducedCost
	\brief This variable selector selects the variable with biggest reduced cost in current LP solution
  of the provided linear relaxation solver.

  Note that it does NOT call the solve() method of the solver automatically.
  The current LP solution is simply read as it is.

   @version 2013.1
*/

class DLLIMPORTEXPORT KLargestReducedCost : public KVariableSelector {
public:
	/// Constructor
	KLargestReducedCost(KLinearRelaxationSolver*);
	/// Copy constructor
	KLargestReducedCost(const KLargestReducedCost& toCopy);
	/// Destructor
	virtual ~KLargestReducedCost();
	//methods reserved for internal use
	virtual KVariableSelector* getCopyPtr() const;
};

/**	\class KNumLargestReducedCost
	\brief This variable selector selects the variable with biggest reduced cost in current
	LP solution of the provided linear relaxation solver.

   @version 2013.1
*/

class DLLIMPORTEXPORT KNumLargestReducedCost : public KNumVariableSelector {
public:
	/// Constructor
	KNumLargestReducedCost(KLinearRelaxationSolver*);
	/// Copy constructor
	KNumLargestReducedCost(const KNumLargestReducedCost& toCopy);
	/// Destructor
	virtual ~KNumLargestReducedCost();
	//methods reserved for internal use
	virtual KNumVariableSelector* getCopyPtr() const;

};

#endif
