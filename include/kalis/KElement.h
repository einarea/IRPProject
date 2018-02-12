// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KElement.h      				                          *
// ********************************************************************

#ifndef __KELEMENT_H
#define __KELEMENT_H

#include "Globals.h"
#include "KConstraint.h"

typedef int (*KalisElement1DFunctionPtr)(int);

/**	 
  * This class creates a X == Tab[I + cste] constraint
  @code
    Example :

	KIntArray Tab(...);
	KIntVar X(...);
	KIntVar I(...);
	...
	problem.post(KElement(Tab,I,X,4,"X == Tab[I + 4]"));
	...
  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KElement : public KConstraint { 
  public :		
	/**
	 Constructor for X == Tab[I + cste]
	 @param Tab the values
	 @param I the index variable
	 @param X the value variable
	 @param offset constant offset of index 
	 @param name name of the constraint
	*/
	KElement(KIntArray& Tab, const KIntVar& I, const KIntVar& X, const int offset, char * name=0);

	/**
	 Constructor for x == Tab[I + cste]
	 @param Tab the values
	 @param I the index variable
	 @param x the value constant
	 @param offset constant offset of index 
	 @param name name of the constraint
	*/
    KElement(KIntArray& Tab, const KIntVar& I, const int x, const int offset, char * name=0);

	// Internal constructors
    KElement(const KEltTerm &e, const KIntVar &X, char * name=0);
    KElement(const KEltTerm &e, const int x, char * name=0);

	// Value function from index 
	virtual int getValueForIndex(int index);	

	// Use overloaded "getValueForIndex" function instead of integer array
	void setUseValueFunction(bool useValueFunction);

	// Copy constructor
	KElement(const KElement & toCopy);
	// Destructor
	~KElement();
	// Internal use only
    virtual void * getConstraintIPtr();
};

#endif




