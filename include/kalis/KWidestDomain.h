// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KWidestDomain.h                                           *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable with smallest domain               *
// ********************************************************************

#ifndef __KWidestDomain_H
#define __KWidestDomain_H


#include "Globals.h"
#include "KNumVariableSelector.h"

/**	 
  * This class implements a variable selector that selects the first uninstantiated variable with the widest domain.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KSplitNumDomain(KWidestDomain(),KMaxToMin();

  @endcode	
  *	@see  KNumVariableSelector
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KWidestDomain : public KNumVariableSelector {
public:
  
  /// Default constructor
  KWidestDomain();

  /// Copy constructor
  KWidestDomain(const KWidestDomain& widestDomainToCopy);

  // Destructor
  virtual ~KWidestDomain();

  //methods
 // return the uninstantiated variable with the smallest domain 
  virtual KNumVar * selectNextVariable(KNumVarArray* numVarArray);
  virtual KNumVariableSelector* getCopyPtr() const;
  
}; // class KWidestDomain


class DLLIMPORTEXPORT KNumSmallestDomain : public KNumVariableSelector {
public:
  
  /// Default constructor
  KNumSmallestDomain();

  /// Copy constructor
  KNumSmallestDomain(const KNumSmallestDomain& widestDomainToCopy);

  // Destructor
  virtual ~KNumSmallestDomain();

  //methods
 // return the uninstantiated variable with the smallest domain 
  virtual KNumVar * selectNextVariable(KNumVarArray* numVarArray);
  virtual KNumVariableSelector* getCopyPtr() const;
  
}; // class KNumSmallestDomain



#endif


