// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KMinToMax.h                                               *
// * Description : SubClass of KValueSelector                         *
// * looks for next value in decreasing order                         *
// ********************************************************************

#ifndef __MINTOMAX_H
#define __MINTOMAX_H

#include "Globals.h"
#include "KValueSelector.h"

/**
  * This class implements a value selector that returns values in increasing order
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KSmallestDomain(),KMinToMax());

  @endcode	
  *	@see  KValueSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KMinToMax : public KValueSelector {
public:
  
  /// Constructor
  KMinToMax();

  /// Copy constructor
  KMinToMax(const KMinToMax& toCopy);

  // Destructor
  virtual ~KMinToMax();

  //methods
  /// get Next Value
  virtual int selectNextValue(KIntVar* intVar);
  virtual KValueSelector* getCopyPtr() const;

  virtual char * getName() { return "MIN_TO_MAX"; }
  
}; // class KMinToMax

#endif


