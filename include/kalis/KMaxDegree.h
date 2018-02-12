// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KMaxDegree.h                                              *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable with the largest degree            *
// ********************************************************************

#ifndef __KMAXDEGREE_H
#define __KMAXDEGREE_H

#include "Globals.h"
#include "KVariableSelector.h"

/**	 
  * This class implements a variable selector that selects first the variable involved in the maximum number of constraints.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KMaxDegree(),KMaxToMin();

	
  @endcode	
  *	@see  KVariableSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KMaxDegree : public KVariableSelector {
public:
  

  // Constructors
  KMaxDegree();

  // Copy constructor
  KMaxDegree(const KMaxDegree& maxDegreeToCopy);

  // Destructor
  virtual ~KMaxDegree();

  //methods
  /// return the uninstantiated variable with the maximum degree in the constraints graph
  //virtual KIntVar* selectNextVariable(KIntVarArray* intVarArray); // get Next Variable
  virtual KVariableSelector* getCopyPtr() const;
  
}; // class KMaxDegree

#endif


