// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KDoubleArray.h                                            *
// * Description : array of double                                    *
// ********************************************************************

#ifndef __DOUBLEARRAY_H
#define __DOUBLEARRAY_H

#include "Globals.h"
#include "ArtelysValList.h"

EXTTEMPL template class DLLIMPORTEXPORT ArtelysValList<double>;
/**	 
  * This class implements an array of doubles
  @code
    Example :
   
	KDoubleArray doubleArray;
	doubleArray += 3.0;
	doubleArray += 5.0;
	// doubleArray = { 3.0 ,5.0 }
	doubleArray[0] = 2.2;
	// doubleArray = { 2.2 ,5.0 }
	
  @endcode	
  *	@see  KIntArray  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KDoubleArray : public ArtelysValList<double> {
public :
  // constructors
  KDoubleArray();
  #ifndef VARARGIGNORE
  KDoubleArray(int nbElements,...);
  #endif

  // destructor
  virtual ~KDoubleArray();
  
  // methods
  virtual ArtelysValList<double>* getCopyPtr() const;

}; // class KDoubleArray : public ArtelysValList<double>

#endif


