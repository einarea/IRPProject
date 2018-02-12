// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KLargestMin.h                                             *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable with largest lower bound on domain *
// ********************************************************************

#ifndef __KLARGESTMIN_H
#define __KLARGESTMIN_H

#include "Globals.h"
#include "KVariableSelector.h"

/**	 
  * This class implements a variable selector that selects first the variable with the largest lower bound.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KLargestMin(),KMaxToMin();

	
  @endcode	
  *	@see  KVariableSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KLargestMin : public KVariableSelector {
public:
  
  // Constructors
  KLargestMin();

  // Copy constructor
  KLargestMin(const KLargestMin& smallestDomainToCopy);

  // Destructor
  virtual ~KLargestMin();

  //methods
  /// return the uninstantiated variable with the largest lower bound
  //virtual KIntVar* selectNextVariable(KIntVarArray* intVarArray); 
  virtual KVariableSelector* getCopyPtr() const;
  
}; // class KLargestMin

#endif


