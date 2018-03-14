// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KLargestMax.h                                             *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable with smallest domain               *
// ********************************************************************

#ifndef __KLARGESTMAX_H
#define __KLARGESTMAX_H

#include "Globals.h"
#include "KVariableSelector.h"

/**	 
  * This class implements a variable selector that selects first the variable with the largest upperbound in its domain.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KLargestMax(),KMaxToMin();

	
  @endcode	
  *	@see  KVariableSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KLargestMax : public KVariableSelector {
public:
  /// Default Constructor
  KLargestMax();

  /// Copy constructor
  KLargestMax(const KLargestMax& toCopy);

  // Destructor
  virtual ~KLargestMax();

  //methods
  /// return the uninstantiated variable with the largest upper bound
  //virtual KIntVar* selectNextVariable(KIntVarArray* intVarArray);
  virtual KVariableSelector* getCopyPtr() const;
  
}; // class KLargestMax

#endif


