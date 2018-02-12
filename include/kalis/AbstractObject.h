#ifndef __ABSTRACT_OBJECT
#define __ABSTRACT_OBJECT

// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : AbstractObject.h                                          *
// ********************************************************************


#include "Globals.h"

/**
 * Root Superclass of all objects in Artelys Kalis
 */
class DLLIMPORTEXPORT AbstractObject {

protected :

	char * _name;

public :
	/// Constructor
	AbstractObject();
	/// Copy constructor
	AbstractObject(const AbstractObject & toCopy);
	/// Constructor with one parameter
	AbstractObject(const char * name);
	/// Constructor with one parameter
	AbstractObject( char * name);
	/// destructor
	virtual ~AbstractObject();
	/// return name of this AbstractObject
	virtual char * getName() const;
	/// Set the name of this AbstractObject
	virtual void setName(char * name);

};

#endif
