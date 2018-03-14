// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KIntArray.h                                               *
// * Description : array of int                                       *
// ********************************************************************

#ifndef __INTARRAY_H
#define __INTARRAY_H


#include "Globals.h"
#include "ArtelysValList.h"

EXTTEMPL template class DLLIMPORTEXPORT ArtelysValList<int>;

/**	 
  * This class implements an array of integers
  @code
    Example :
   
	KIntArray intArray;
	intArray += 3;
	intArray += 5;
	// intArray = { 3,5 }
	intArray[0] = 2;
	// intArray = { 2,5 }
	
  @endcode	
  *	@see  KDoubleArray  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KIntArray : public ArtelysValList<int> {
public :
  // constructors
  KIntArray();

  #ifndef VARARGIGNORE
  KIntArray(int nbElements,...);
  #endif
 

  // destructor
  virtual ~KIntArray();
  
  // methods
  virtual ArtelysValList<int>* getCopyPtr() const;
  void print();

}; // class KIntArray : public ArtelysValList<int>

#endif


