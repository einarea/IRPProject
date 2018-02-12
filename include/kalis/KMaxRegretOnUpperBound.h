// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KMaxRegretOnUpperBound.h                                  *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable with largest difference between the*
// * largest and second largest value in the domain                   *
// ********************************************************************

#ifndef __MAX_REGRET_ON_UPPER_BOUND_H
#define __MAX_REGRET_ON_UPPER_BOUND_H

#include "Globals.h"
#include "KVariableSelector.h"

/**	 
  * This class implements a variable selector that selects first the variable with maximum regret on its upperbound.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KMaxRegretOnUpperBound(),KMaxToMin();

	
  @endcode	
  *	@see  KVariableSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KMaxRegretOnUpperBound : public KVariableSelector {
public:
  
  /// Constructor
  KMaxRegretOnUpperBound();

  /// Copy constructor
  KMaxRegretOnUpperBound(const KMaxRegretOnUpperBound& maxRegretOnUpperBoundToCopy);

  // Destructor
  virtual ~KMaxRegretOnUpperBound();

  //methods  
    /// return the uninstantiated variable with the smallest domain size / degree in the constraint graph ratio
  //virtual KIntVar* selectNextVariable(KIntVarArray* intVarArray); // get Next Variable
  virtual KVariableSelector* getCopyPtr() const;

  
}; // class KMaxRegretOnUpperBound

#endif


