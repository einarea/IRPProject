// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNearestValue.h                                           *
// * Description : SubClass of KValueSelector                         *
// * looks for the nearest value of a target (middle value by default)*
// ********************************************************************

#ifndef __KNEARESTVALUE_H
#define __KNEARESTVALUE_H

#include "Globals.h"
#include "KValueSelector.h"
#include "KNumValueSelector.h"

/**	 
 * This class implements a value selector that selects the nearest value from target in the domain of the variable .
 * @code
 *  Example :
 * 
 *	KBranchingSchemeArray myBranchingSchemeArray;
 *	myBranchingSchemeArray += KAssignVar(KSmallestDomain(),KNearestValue());
 *
 * @endcode	
 *	@see  KValueSelector  
 *	 @version 2013.1
 */
class DLLIMPORTEXPORT KNearestValue : public KValueSelector {
public:
  
  // Default Constructor
  KNearestValue();

  // Copy constructor
  KNearestValue(const KNearestValue& nearestToCopy);

  // Destructor
  virtual ~KNearestValue();

  //methods
  virtual int selectNextValue(KIntVar* intVar); // get Next Value
  virtual KValueSelector* getCopyPtr() const;

   virtual char * getName() { return "NEAREST_VALUE"; }
  
}; // class KNearestValue


/**	 
 * This class implements a value selector that selects the nearest value from target in the domain of the variable .
 * @code
 *  Example :
 * 
 *	KBranchingSchemeArray myBranchingSchemeArray;
 *	myBranchingSchemeArray += KSplitDomain(KWidestDomain(),KNumNearestValue());
 *
 * @endcode	
 *	@see  KValueSelector  
 *	 @version 2013.1
 */
class DLLIMPORTEXPORT KNumNearestValue : public KNumValueSelector {
public:
  
  // Default Constructor
  KNumNearestValue();

  // Copy constructor
  KNumNearestValue(const KNumNearestValue& nearestToCopy);

  // Destructor
  virtual ~KNumNearestValue();

  //methods
  virtual double selectNextValue(KNumVar* intVar); // get Next Value
  virtual KNumValueSelector* getCopyPtr() const;
  virtual char * getName() { return "NUM_NEAREST_VALUE"; }
  
}; // class KNearestValue

class DLLIMPORTEXPORT KNearestNeighboor : public KValueSelector {
public:
  
  // Default Constructor
  KNearestNeighboor(KIntVarArray&succ,KIntMatrix& distanceMatrix);

  // Copy constructor
  KNearestNeighboor(const KNearestNeighboor& toCopy);

  // Destructor
  virtual ~KNearestNeighboor();

  //methods
  virtual int selectNextValue(KIntVar* intVar); // get Next Value
  virtual KValueSelector* getCopyPtr() const;
  virtual char * getName() { return "NEAREST_NEIGHBOOR"; }
  
}; // class KNearestNeighboor



#endif


