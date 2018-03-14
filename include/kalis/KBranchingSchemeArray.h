// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KBranchingSchemeArray.h                                   *
// * Description : array of branching schemes                         *
// ********************************************************************

#ifndef __BRANCHINGSCHEMEARRAY_H
#define __BRANCHINGSCHEMEARRAY_H

#include "Globals.h"
#include "ArtelysList.h"
#include "KBranchingScheme.h"


EXTTEMPL template class DLLIMPORTEXPORT ArtelysList<KBranchingScheme>;

/**	 
  * This class implements an array of KBranchingScheme
  @code
    Example :

	KBranchingSchemeArray myStrategy;
	
	// First solve all the disjunctions in the problem
	myStrategy += KSettleDisjunction();		
	// then assign each remaining non bound variable by assigning values in 
	// decreasing order to variables ordered by increasing size of domain 
	myStrategy += KAssignVar(KSmallestDomain(),KMaxToMin());
	
  @endcode	
  *	@see  KBranchingScheme
  *	@see  KValueSelector
  *	@see  KVariableSelector

  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KBranchingSchemeArray : public ArtelysList<KBranchingScheme> {

  public:
  // Constructors
  KBranchingSchemeArray();
  #ifndef VARARGIGNORE
  KBranchingSchemeArray(int nbElements,...);
  #endif 

  // destructor
  ~KBranchingSchemeArray();

}; // class KBranchingSchemeArray : public ArtelysList<KBranchingScheme>

#endif


