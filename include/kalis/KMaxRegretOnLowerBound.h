// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KMaxRegretOnLowerBound.h                                  *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable with largest difference between the*
// * smallest and second smallest value in the domain                 *
// ********************************************************************

#ifndef __MAX_REGRET_ON_LOWER_BOUND_H
#define __MAX_REGRET_ON_LOWER_BOUND_H

#include "Globals.h"
#include "KVariableSelector.h"

/**	 
  * This class implements a variable selector that selects first the variable with maximum regret on its lowerbound.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KMaxRegretOnLowerBound(),KMaxToMin();

	
  @endcode	
  *	@see  KVariableSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KMaxRegretOnLowerBound : public KVariableSelector {
public:
  
  /// Constructor
  KMaxRegretOnLowerBound();

  /// Copy constructor
  KMaxRegretOnLowerBound(const KMaxRegretOnLowerBound& maxRegretOnLowerBoundToCopy);

  // Destructor
  virtual ~KMaxRegretOnLowerBound();

  //methods
  //virtual KIntVar* selectNextVariable(KIntVarArray* intVarArray); // get Next Variable
  virtual KVariableSelector* getCopyPtr() const;

  
}; // class KMaxRegretOnLowerBound

#endif


