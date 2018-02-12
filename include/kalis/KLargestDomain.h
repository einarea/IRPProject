// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KLargestDomain.h                                         *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable with smallest domain               *
// ********************************************************************

#ifndef __KLARGESTDOMAIN_H
#define __KLARGESTDOMAIN_H


#include "Globals.h"
#include "KVariableSelector.h"

/**	 
  * This class implements a variable selector that selects the first uninstantiated variable with the smallest domain.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KLargestDomain(),KMaxToMin();

  @endcode	
  *	@see  KVariableSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KLargestDomain : public KVariableSelector {
public:

  /// Default constructor
  KLargestDomain();

  /// Copy constructor
  KLargestDomain(const KLargestDomain& toCopy);

  // Destructor
  virtual ~KLargestDomain();

  //methods 
  virtual KVariableSelector* getCopyPtr() const;

 
}; // class KLargestDomain

#endif


