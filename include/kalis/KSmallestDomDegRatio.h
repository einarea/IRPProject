// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSmallestDomDegRatio.h                                    *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable with smallest ratio (dom / degree) *
// ********************************************************************

#ifndef __SMALLESTDOMDEGRATIO_H
#define __SMALLESTDOMDEGRATIO_H

#include "Globals.h"
#include "KVariableSelector.h"


/**	 
  * This class implements a variable selector that selects first the variable with the smallest ratio domain size / degree in the constraint graph.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KSmallestDomDegRatio(),KMaxToMin();

	
  @endcode	
  *	@see KVariableSelector
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KSmallestDomDegRatio : public KVariableSelector {
public:

  /// Constructor
  KSmallestDomDegRatio();

  /// Copy constructor
  KSmallestDomDegRatio(const KSmallestDomDegRatio& smallestDomDegRatioToCopy);

  // Destructor
  virtual ~KSmallestDomDegRatio();

  //methods
  // return the uninstantiated variable with the smallest domain size / degree in the constraint graph ratio
  //virtual KIntVar* selectNextVariable(KIntVarArray* intVarArray); // get Next Variable
  virtual KVariableSelector* getCopyPtr() const;
  
}; // class KSmallestDomDegRatio

#endif


