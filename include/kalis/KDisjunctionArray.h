// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KDisjunctionArray.h                                       *
// * Description : array of disjunctions                              *
// ********************************************************************

#ifndef __KDISJUNCTIONARRAY_H
#define __KDISJUNCTIONARRAY_H

#include "ArtelysList.h"
#include "KCompositeConstraints.h"

#ifdef WIN32
  #ifndef ACK_EXPORTS
    #pragma warning(disable : 4231)
  #endif
#endif

EXTTEMPL template class DLLIMPORTEXPORT ArtelysList<KDisjunction>;

#ifdef WIN32
  #ifndef ACK_EXPORTS
    #pragma warning(default : 4231)
  #endif
#endif

/**	 
  * This class implements an array of KDisjunction
  @code
    Example :
    
    KIntVar TASK0(...)
    KIntVar TASK1(...)
    KIntVar TASK2(...)
    KDisjunctionArray disjunctionArray;
	
    disjunctionArray += (TASK0 + 10 < TASK1) || (TASK1 + 4 < TASK0);
    disjunctionArray += (TASK1 + 4 < TASK2) || (TASK2 + 7 < TASK1);
    disjunctionArray += (TASK2 + 7 < TASK0) || (TASK0 + 10 < TASK2);

	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KSettleDisjunction(disjunctionArray);

	
  @endcode	
  * @see ArtelysList
  *	@see KDisjunction
  * @see KBranchingScheme
  * @see KSettleDisjunction

  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KDisjunctionArray : public ArtelysList<KDisjunction> {

  public:
  /// constructor : creates an empty KDisjunctionArray
  KDisjunctionArray();
  #ifndef VARARGIGNORE
  KDisjunctionArray(int nbElements,...);
  #endif

  // destructor
  virtual ~KDisjunctionArray();

}; // class KDisjunctionArray : public ArtelysList<KDisjunction>

#endif


