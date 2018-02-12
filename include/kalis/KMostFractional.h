// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KMostFractional.h                                         *
// * Description : SubClass of KVariableSelector                      *
// * looks for variable whose relaxed value is the most fractional    *
// ********************************************************************

#ifndef __KMOSTFRACTIONAL_H
#define __KMOSTFRACTIONAL_H

#include "Globals.h"
#include "KVariableSelector.h"

/**	\class KMostFractional
	\brief This variable selector selects the variable with biggest fractional part in the current solution
  held by the provided linear relaxation solver.

  Note that it does NOT call the "solve" method of the solver, so if you want the relaxation to be re-solved at each node,
  you must use method KSolver::setOtherNodesRelaxationSolver.
  *	 @version 2013.1
*/

class DLLIMPORTEXPORT KMostFractional : public KVariableSelector {
public:
	/// Constructor
	KMostFractional(KLinearRelaxationSolver*);
	/// Copy constructor
	KMostFractional(const KMostFractional& toCopy);
	/// Destructor
	virtual ~KMostFractional();
	//methods reserved for internal use
	virtual KVariableSelector* getCopyPtr() const;
};

#endif


