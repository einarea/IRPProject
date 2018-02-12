// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KExternalObject.h										  *
// * Auxiliary template class for handling internal objects from	  *
// * external class													  *
// * The internal object is automatically deleted when the external	  *
// * object is deleted												  *
// ********************************************************************

// full copy ctor
// full delete

#ifndef __KEXTERNALOBJECT_H
#define __KEXTERNALOBJECT_H

#include "Globals.h"

template <class Intern>
class KExternalObject {
private:
	// Internal object
	Intern* _intern;
	// Set management status of internal object
	bool _manage;

protected:
	// Default constructor
	KExternalObject();
	// Constructor with pointer to internal object
	KExternalObject(Intern* intern, bool manage = true);
	// Copy constructor
	KExternalObject(const KExternalObject& toCopy);
	// Destructor (called just after the destructor of the derived class)
	virtual ~KExternalObject();
	// Set the internal object
	void setInternalObject(Intern* intern, bool manage = true);

public:
	// Get a non-const pointer to the internal object
	Intern* getInternalObject() const;

}; // class KExternalObject

template <class Intern>
KExternalObject<Intern>::KExternalObject() :
	_intern(NULL),
	_manage(true)
{}

template <class Intern>
KExternalObject<Intern>::KExternalObject(Intern* intern, bool manage) :
	_intern(intern),
	_manage(manage)
{}

template <class Intern>
KExternalObject<Intern>::KExternalObject(const KExternalObject<Intern>& toCopy) :
	_intern(NULL),
	_manage(true) // by default
{
	if (toCopy._intern != NULL) {
//		_intern = new Intern(*(toCopy._intern));
		_intern = toCopy._intern->getCopyPtr();
	}
}

template <class Intern>
KExternalObject<Intern>::~KExternalObject() {
	if (_manage && _intern != NULL) {
		delete _intern;
	}
}

template <class Intern>
Intern* KExternalObject<Intern>::getInternalObject() const {
	return _intern;
}

template <class Intern>
void KExternalObject<Intern>::setInternalObject(Intern* intern, bool manage) {
	if (_manage && _intern != NULL) {
		delete _intern;
	}
	_intern = intern;
	_manage = manage;
}

#endif
