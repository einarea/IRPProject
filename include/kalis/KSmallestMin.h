// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSmallestMin.h                                            *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable with smallest domain               *
// ********************************************************************

#ifndef __KSMALLESTMIN_H
#define __KSMALLESTMIN_H

#include "Globals.h"
#include "KVariableSelector.h"

/**	 
  * This class implements a variable selector that selects the first uninstantiated variable with the smallest value in its domain.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KSmallestMin(),KMaxToMin();

  @endcode	
  *	@see  KVariableSelector
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KSmallestMin : public KVariableSelector {
public:
  
  // Constructors
  KSmallestMin();

  // Copy constructor
  KSmallestMin(const KSmallestMin& toCopy);

  // Destructor
  virtual ~KSmallestMin();

  //methods
  //virtual KIntVar* selectNextVariable(KIntVarArray* intVarArray); // get Next Variable
  virtual KVariableSelector* getCopyPtr() const;
  
}; // class KSmallestMin

#endif


