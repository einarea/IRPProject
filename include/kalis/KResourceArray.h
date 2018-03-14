// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KResourceArray.h                                          *
// ********************************************************************

#ifndef __KResourceArray_H
#define __KResourceArray_H

#include "Globals.h"
#include "ArtelysList.h"
#include "KResource.h"

#ifdef WIN32
#pragma warning(disable:4231)
#endif

EXTTEMPL template class DLLIMPORTEXPORT ArtelysList<KResource>;

/**	 
  * This class implements an array of KResource
  @code
    Example :
   	
	KSchedule  s(...);

    // R is an array of KResource R0 R1 R2 R3 R4
	KResourceArray R(s,5,0,10,"T");	
	
  @endcode	
  *	@see  KResource  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KResourceArray : public ArtelysList<KResource> {
 public:
  /// default constructor
  KResourceArray();
  /// This constructor allows to build an array of variables with the same bounds
  KResourceArray(KSchedule & schedule, int nbResource, const char*name=NULL);
  
  virtual int check(KResource_I *_ptr) const;
  // destructor
  ~KResourceArray();
  
}; 



#endif


