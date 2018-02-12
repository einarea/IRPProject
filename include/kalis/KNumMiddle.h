// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNumMiddle.h                                              *
// * Description : SubClass of KValueSelector                         *
// * looks for the middle of current domain as next value             *
// ********************************************************************

#ifndef __KNUMMIDDLE_H
#define __KNUMMIDDLE_H

#include "Globals.h"
#include "KNumValueSelector.h"

/**	 
  * This class implements a value selector that selects the nearest value from the middle value in the domain of the variable.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KSmallestDomain(),KNumMiddle());

  @endcode	
  *	@see  KValueSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumMiddle : public KNumValueSelector {
public:
  
  /// Constructor
  KNumMiddle();

  /// Copy constructor
  KNumMiddle(const KNumMiddle& middleToCopy);

  // Destructor
  virtual ~KNumMiddle();

  //methods
  virtual double selectNextValue(KNumVar* numVar); 
  virtual KNumValueSelector* getCopyPtr() const;
  
}; // class KNumMiddle

#endif


