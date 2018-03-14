// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KRandomValue.h                                            *
// * Description : SubClass of KValueSelector                         *
// * looks for a random value in the domain                           *
// ********************************************************************

#ifndef __KRANDOM_VALUE_H
#define __KRANDOM_VALUE_H

#include "Globals.h"
#include "KValueSelector.h"

/**	 
  * This class implements a value selector that selects a value at random in the domain of the variable.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KSmallestDomain(),KRandomValue());

  @endcode	
  *	@see  KValueSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KRandomValue : public KValueSelector {
public:
  
  // Constructors
  KRandomValue();

  // Copy constructor
  KRandomValue(const KRandomValue& toCopy);

  // Destructor
  virtual ~KRandomValue();

  //methods
  virtual int selectNextValue(KIntVar* intVar); // get Next Value
  virtual KValueSelector* getCopyPtr() const;
  
}; 

#endif


