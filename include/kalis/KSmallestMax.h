// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSmallestMax.h                                            *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable with smallest upper bound in domain*
// ********************************************************************

#ifndef __SMALLESTMAX_H
#define __SMALLESTMAX_H

#include "Globals.h"
#include "KVariableSelector.h"

/**	 
  * This class implements a variable selector that selects first the variable with the smallest upperbound.
  @code
    Example:
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KSmallestMax(),KMaxToMin();

  @endcode	
  *	@see KVariableSelector
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KSmallestMax : public KVariableSelector {
public:
  
  /// Constructor
  KSmallestMax();

  /// Copy constructor
  KSmallestMax(const KSmallestMax& toCopy);

  // Destructor
  virtual ~KSmallestMax();

  //methods
  //virtual KIntVar* selectNextVariable(KIntVarArray* intVarArray); // get Next Variable
  virtual KVariableSelector* getCopyPtr() const;
  
}; // class SmallestMax

#endif


