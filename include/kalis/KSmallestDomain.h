// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSmallestDomain.h                                         *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable with smallest domain               *
// ********************************************************************

#ifndef __KSMALLESTDOMAIN_H
#define __KSMALLESTDOMAIN_H


#include "Globals.h"
#include "KVariableSelector.h"

/**	 
  * This class implements a variable selector that selects the first uninstantiated variable with the smallest domain.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KSmallestDomain(),KMaxToMin();

  @endcode	
  *	@see  KVariableSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KSmallestDomain : public KVariableSelector {
public:

  /// Default constructor
  KSmallestDomain();

  /// Copy constructor
  KSmallestDomain(const KSmallestDomain& smallestDomainToCopy);

  // Destructor
  virtual ~KSmallestDomain();

  //methods 
  virtual KVariableSelector* getCopyPtr() const;

 
}; // class KSmallestDomain

#endif


