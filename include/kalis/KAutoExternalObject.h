// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KAutoExternalObject.h									  *
// * Auxiliary template class for handling internal objects from	  *
// * external class													  *
// * The internal object is NOT deleted when the external object is	  *
// * deleted														  *
// ********************************************************************

// partial copy ctor
// partial dtor

#ifndef __KAUTOEXTERNALOBJECT_H
#define __KAUTOEXTERNALOBJECT_H

#include "Globals.h"

template <class Extern, class Intern>
class KAutoExternalObject {
public:
	class KInternalObject {
	private:
		// Defines how the pointer to internal objects are managed
		// _isManaged = true means that it is managed by external objects (similar to smart_ptr)
		// _isManaged = false means that it is managed somewhere else (e.g. by a KProblem_I object)
		bool _isManaged;
		// Pointer reference counter
		// This is only for member _intern of KExternalObject objects
		unsigned int _refCount;
		// Internal object
		// An object is used instead of a pointer to simplify and speed-up memory allocation/release
		Extern _extern;

	protected:
		// Default constructor
		KInternalObject();
		// Destructor (called just after the destructor of the derived class)
		virtual ~KInternalObject();
	public:
		// Release automatic management of the internal object
		void releaseAutoManagement();
		// Increment reference counter
		void incrementCounter();
		// Delete function using reference counter and management indicator
		void autoDelete();
		// Get a reference to the external object
		Extern& getExternalObject();
	};

private:
	// Internal object
	// A pointer is used to avoid member deletion when the class dtor is called
	KInternalObject* _intern;

protected:
	// Default constructor
	KAutoExternalObject();
	// Constructor with pointer to internal object
	KAutoExternalObject(KInternalObject* intern);
	// Copy constructor
	KAutoExternalObject(const KAutoExternalObject& toCopy);
	// Destructor (called just after the destructor of the derived class)
	virtual ~KAutoExternalObject();
	// Set the internal object
	void setInternalObject(KInternalObject* intern);

public:
	// Get a non-const pointer to the internal object
#ifdef ACK_LIB
	Intern* getInternalObject() const;
#endif

}; // class KExternalObject

template <class Extern, class Intern>
KAutoExternalObject<Extern, Intern>::KInternalObject::KInternalObject() :
	_extern(Extern((Intern*)NULL)),
	_isManaged(true),
	_refCount(0)
{
	_extern.setInternalObject((Intern*)this);
}

template <class Extern, class Intern>
KAutoExternalObject<Extern, Intern>::KInternalObject::~KInternalObject()
{}

template <class Extern, class Intern>
void KAutoExternalObject<Extern, Intern>::KInternalObject::releaseAutoManagement() {
	_isManaged = false;
}

template <class Extern, class Intern>
void KAutoExternalObject<Extern, Intern>::KInternalObject::incrementCounter() {
	if (_isManaged) {
		++_refCount;
	}
}

template <class Extern, class Intern>
void KAutoExternalObject<Extern, Intern>::KInternalObject::autoDelete() {
	if (_isManaged && (--_refCount == 1)) { // use 1 because of the '_extern' member of Intern object
		delete this;
	}
}

template <class Extern, class Intern>
Extern& KAutoExternalObject<Extern, Intern>::KInternalObject::getExternalObject() {
	return _extern;
}

template <class Extern, class Intern>
KAutoExternalObject<Extern,Intern>::KAutoExternalObject() :
	_intern(NULL)
{}

template <class Extern, class Intern>
KAutoExternalObject<Extern,Intern>::KAutoExternalObject(KInternalObject* intern) :
	_intern(intern)
{
	if (_intern != NULL) {
		_intern->incrementCounter();
	}
}

template <class Extern, class Intern>
KAutoExternalObject<Extern,Intern>::KAutoExternalObject(const KAutoExternalObject<Extern,Intern>& toCopy) :
	_intern(toCopy._intern)
{
	if(_intern != NULL) {
		_intern->incrementCounter();
	}
}

template <class Extern, class Intern>
KAutoExternalObject<Extern,Intern>::~KAutoExternalObject() {
	if (_intern != NULL) {
		_intern->autoDelete();
	}
}

#ifdef ACK_LIB
template <class Extern, class Intern>
Intern* KAutoExternalObject<Extern,Intern>::getInternalObject() const {
	return (Intern*)_intern;
}
#endif

template <class Extern, class Intern>
void KAutoExternalObject<Extern,Intern>::setInternalObject(KInternalObject* intern) {
	if (_intern != NULL) {
		_intern->autoDelete();
	}
	_intern = intern;
	if (_intern != NULL) {
		_intern->incrementCounter();
	}
}

#endif

/*
// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KAutoExternalObject.h									  *
// * Auxiliary template class for handling internal objects from	  *
// * external class													  *
// * The internal object is NOT deleted when the external object is	  *
// * deleted														  *
// ********************************************************************

// partial copy ctor
// partial dtor

#ifndef __KAUTOEXTERNALOBJECT_H
#define __KAUTOEXTERNALOBJECT_H

#include "Globals.h"

template <class Extern, class Intern>
class KAutoExternalObject {
public:
	class KInternalObject {
	private:
		// Defines how the pointer to internal objects are managed
		// _isManaged = true means that it is managed by external objects (similar to smart_ptr)
		// _isManaged = false means that it is managed somewhere else (i.e. by a KProblem_I object)
		bool _isManaged;
		// Pointer reference counter
		// This is only for member _intern of KExternalObject objects
		unsigned int _refCount;
		// Internal object
		// An object is used instead of a pointer to simplify and speed-up memory allocation/release
		Extern _extern;

	protected:
		// Default constructor
		KInternalObject();
		// Destructor (called just after the destructor of the derived class)
		virtual ~KInternalObject();
	public:
		// Release automatic management of the internal object
		void releaseAutoManagement();
		// Increment reference counter
		void incrementCounter();
		// Delete function using reference counter and management indicator
		void autoDelete();
		// Get a constant reference to the external object
		Extern& getExternalObject();
	};

private:
	// Internal object
	// A pointer is used to avoid member deletion when the class dtor is called
	Intern* _intern;

protected:
	// Default constructor
	KAutoExternalObject();
	// Constructor with pointer to internal object
	KAutoExternalObject(Intern* intern);
	// Copy constructor
	KAutoExternalObject(const KAutoExternalObject& toCopy);
	// Destructor (called just after the destructor of the derived class)
	virtual ~KAutoExternalObject();
	// Set the internal object
	void setInternalObject(Intern* intern);

public:
	// Get a non-const pointer to the internal object
#ifdef ACK_LIB
	Intern* getInternalObject() const;
#endif

}; // class KExternalObject

template <class Extern, class Intern>
KAutoExternalObject<Extern, Intern>::KInternalObject::KInternalObject() :
	_extern(Extern((Intern*)NULL)),
	_isManaged(true),
	_refCount(0)
{
	_extern.setInternalObject((Intern*)this);
}

template <class Extern, class Intern>
KAutoExternalObject<Extern, Intern>::KInternalObject::~KInternalObject() {
}

template <class Extern, class Intern>
void KAutoExternalObject<Extern, Intern>::KInternalObject::releaseAutoManagement() {
	_isManaged = false;
}

template <class Extern, class Intern>
void KAutoExternalObject<Extern, Intern>::KInternalObject::incrementCounter() {
	if (_isManaged) {
		++_refCount;
	}
}

template <class Extern, class Intern>
void KAutoExternalObject<Extern, Intern>::KInternalObject::autoDelete() {
	if (_isManaged && (--_refCount == 1)) { // use 1 because of _extern member of Intern object
		delete this;
	}
}

template <class Extern, class Intern>
Extern& KAutoExternalObject<Extern, Intern>::KInternalObject::getExternalObject() {
	return _extern;
}

template <class Extern, class Intern>
KAutoExternalObject<Extern,Intern>::KAutoExternalObject() :
	_intern(NULL)
{
}

template <class Extern, class Intern>
KAutoExternalObject<Extern,Intern>::KAutoExternalObject(Intern* intern) :
	_intern(intern)
{
	if (_intern != NULL) {
		_intern->incrementCounter();
	}
}

template <class Extern, class Intern>
KAutoExternalObject<Extern,Intern>::KAutoExternalObject(const KAutoExternalObject<Extern,Intern>& toCopy) :
	_intern(toCopy._intern)
{
	if(_intern != NULL) {
		_intern->incrementCounter();
	}
}

template <class Extern, class Intern>
KAutoExternalObject<Extern,Intern>::~KAutoExternalObject() {
	_intern->autoDelete();
}

#ifdef ACK_LIB
template <class Extern, class Intern>
Intern* KAutoExternalObject<Extern,Intern>::getInternalObject() const {
	return _intern;
}
#endif

template <class Extern, class Intern>
void KAutoExternalObject<Extern,Intern>::setInternalObject(Intern* intern) {
	if (_intern != NULL) {
		_intern->autoDelete();
	}
	_intern = intern;
	_intern->incrementCounter();
}

#endif

*/
