// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KCopiableObject.h										  *
// * Auxiliary template class for allowing virtual copy constructor	  *
// * through the getCopyPtr() method								  *
// ********************************************************************

#ifndef __KCOPIABLEOBJECT_H
#define __KCOPIABLEOBJECT_H

#include "Globals.h"

template <class T>
class KCopiableObject {
public:
	// Virtual copy constructor
	virtual T* getCopyPtr() const;

}; // class KCopiableObject

template <class T>
T* KCopiableObject<T>::getCopyPtr() const
{
	return new T(*static_cast<const T* const>(this));
}

#endif
