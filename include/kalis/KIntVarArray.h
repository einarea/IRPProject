// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KIntVarArray.h                                            *
// ********************************************************************

#ifndef __KINTVARARRAY_H
#define __KINTVARARRAY_H

#include "Globals.h"
#include "ArtelysList.h"

EXTTEMPL template class DLLIMPORTEXPORT ArtelysList<KIntVar>;

/**	 
  * This class implements an array of KIntVar  with enumerated (finite) domains
  @code
    Example :
   
	KProblem  p(...);

    // T is an array of KIntVar T0 T1 T2 T3 T4 with domain [0..10] 
	KIntVarArray T(p,5,0,10,"T");	
	
  @endcode	
  *	@see  KIntVar  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KIntVarArray : public ArtelysList<KIntVar> {
public:
	/// default constructor
	KIntVarArray();
	/// This constructor allows to build an array of variables with the same bounds
	KIntVarArray(KProblem& problem, int nbKIntVar_I, int minValue, int maxValue, const char*name=NULL);
	// This constructor allows to build an array of variables with the same domain
	// KKIntVar_IArray(Problem &problem, int nbKIntVar_I, const KIntArray &domain, char *name = 0);  
	virtual int check(KIntVar_I *KIntVar_I_ptr) const;
	// destructor
	~KIntVarArray();

}; 



#endif


