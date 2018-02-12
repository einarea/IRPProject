// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KMiddle.h                                                 *
// * Description : SubClass of KValueSelector                         *
// * looks for the middle of current domain as next value             *
// ********************************************************************

#ifndef __MIDDLE_H
#define __MIDDLE_H

#include "Globals.h"
#include "KValueSelector.h"

/**	 
  * This class implements a value selector that selects the nearest value from the middle value in the domain of the variable.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KSmallestDomain(),KMiddle());

  @endcode	
  *	@see  KValueSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KMiddle : public KValueSelector {
public:
  
  /// Constructor
  KMiddle();

  /// Copy constructor
  KMiddle(const KMiddle& middleToCopy);

  // Destructor
  virtual ~KMiddle();

  //methods
  virtual int selectNextValue(KIntVar* intVar); // get Next Value
  virtual KValueSelector* getCopyPtr() const;
  
}; // class KMiddle


class DLLIMPORTEXPORT KMinMaxConflict : public KValueSelector {

protected :
  /**
   * True for min conflict heuristic , false for max propagate
   */
  bool _minconflict;

public:
  
  /**
   * Primary Constructor
   @ param minconflict : true for min conflict heuristic , false for max propagate
   */
  KMinMaxConflict(bool minconflict=true);

  /// Copy constructor
  KMinMaxConflict(const KMinMaxConflict& toCopy);

 

  //methods
  virtual int selectNextValue(KIntVar* intVar); // get Next Value
  virtual KValueSelector* getCopyPtr() const;
  
}; // class KMinMaxConflict



class DLLIMPORTEXPORT KBestBoundValue : public KValueSelector {

protected :
  /**
   * True for min conflict heuristic , false for max propagate
   */
  KNumVar * _boundVar;
  /**
   * True for bound value on lower bound, False for bound value on upper bound
   */
  bool _lowerbound;

public:
  
  /**
   * Primary Constructor
   * @param lowerbound : True for bound value on lower bound, False for bound value on upper bound
   */
  KBestBoundValue(bool lowerbound);
  /**
   * Primaty Constructor
   * @param boundVar : the bounding variable 
   * @param minconflict : True for bound value on lower bound, False for bound value on upper bound
   */
  KBestBoundValue(KNumVar * boundVar = NULL,bool lowerbound=true);

  /**
   * Copy constructor
   */
  KBestBoundValue(const KBestBoundValue& toCopy);

 

  //methods
  virtual int selectNextValue(KIntVar* intVar); // get Next Value
  virtual KValueSelector* getCopyPtr() const;
  
}; // class KBestBoundValue

#endif


