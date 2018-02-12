// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KElement2D.h      				                          *
// ********************************************************************

#ifndef __KELEMENT2D_H
#define __KELEMENT2D_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
  * This class creates a X == Tab[I + cste1][J + cste2] constraint
  @code
    Example :

	KIntArray Tab(...);
	KIntVar X(...);
	KIntVar I(...);
	KIntVar J(...);	
	...
	problem.post(KElement2D(Tab,I,J,X,4,8,"X == Tab[I + 4][J+8]"));
	...
  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KElement2D : public KConstraint { 
  public :
	  
	/**
	 Primary Constructor
	 @param matrix the values
	 @param I the first index variable
	 @param J the second index variable
	 @param X the value variable
	 @param offset1 constant offset of first index variable
	 @param offset2 constant offset of second index variable	 
	 @param name name of the constraint
	*/
	KElement2D(KIntMatrix &matrix, const KIntVar &I,const KIntVar &J, const KIntVar &X, const int offset1,const int offset2,char * name=0);	
   	/**
	 Constructor with KEltTerm2D
	 @param eltTerm2D the two dimensional element term 
	 @param X the value variable
	 @param name name of the constraint
	*/
	KElement2D(const KEltTerm2D &eltTerm2D , const KIntVar &X,char * name=0);

	/**
	 * Copy constructor
	 */
	KElement2D(const KElement2D & toCopy);
	/**
	 * Destructor
	 */
	~KElement2D();

	/**
	 * Get the value for I = index1 and J = index2
	 */
	int getValueForIndex(int index1,int index2);

	/**
	 * Choose value method between Table and method "getValueForIndex"
	 */
	void setUseValueFunction(bool useValueFunction);

	/**
	 * Internal pointer
	 */
    virtual void * getConstraintIPtr();
};

#endif




