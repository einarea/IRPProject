// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNumVarArray.h                                            *
// ********************************************************************

#ifndef __KNumVarArray_H
#define __KNumVarArray_H

#include "Globals.h"
#include "ArtelysList.h"

EXTTEMPL template class DLLIMPORTEXPORT ArtelysList<KNumVar>;

/**	 
  * This class implements an array of KNumVar 
  @code
    Example :
   
	KProblem  p(...);

    // T is an array of KIntVar T0 T1 T2 T3 T4 with domain [0..10] 
	KNumVarArray T;	
	
  @endcode	
  *	@see  KIntVar  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumVarArray : public ArtelysList<KNumVar> {
 public:
  /// default constructor
  KNumVarArray();
  virtual int check(KNumVar_I *KIntVar_I_ptr) const;
  // destructor
  ~KNumVarArray();
  
}; 



#endif


