// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KMaxToMin.h                                               *
// * Description : SubClass of KValueSelector                         *
// * looks for next value in decreasing order                         *
// ********************************************************************

#ifndef __MAXTOMIN_H
#define __MAXTOMIN_H

#include "Globals.h"
#include "KValueSelector.h"

/**	 
  * This class implements a value selector that returns values in decreasing order
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KSmallestDomain(),KMaxToMin());

  @endcode	
  *	@see  KValueSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KMaxToMin : public KValueSelector {
public:
  
  /// Default Constructor
  KMaxToMin();

  /// Copy constructor
  KMaxToMin(const KMaxToMin& maxToMinToCopy);

  // Destructor
  virtual ~KMaxToMin();

  //methods
  /// get Next Value
  virtual int selectNextValue(KIntVar* intVar); 
  virtual KValueSelector* getCopyPtr() const;

  virtual char * getName() { return "MAX_TO_MIN"; }
  
}; // class KMaxToMin

#endif


