// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KTaskArray.h                                            *
// ********************************************************************

#ifndef __KTaskArray_H
#define __KTaskArray_H

#include "Globals.h"
#include "ArtelysList.h"
#include "KTask.h"

#ifdef WIN32
#pragma warning(disable:4231)
#endif

EXTTEMPL template class DLLIMPORTEXPORT ArtelysList<KTask>;

/**	 
  * This class implements an array of KTask
  @code
    Example :
   	
	KSchedule  s(...);

    // T is an array of KTask T0 T1 T2 T3 T4
	KTaskArray T(s,5,0,10,"T");	
	
  @endcode	
  *	@see  KIntVar  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KTaskArray : public ArtelysList<KTask> {
 public:
  /// default constructor
  KTaskArray();
  /// This constructor allows to build an array of variables with the same bounds
  KTaskArray(KSchedule & schedule, int nbTask, const char*name=NULL);
  /// reserved for internal use
  virtual int check(KTask_I *_ptr) const;
  // destructor
  ~KTaskArray();
  
}; 
#endif


