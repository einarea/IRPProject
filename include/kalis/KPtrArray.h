// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KPtrArray.h												  *
// * Auxiliary template class for handling multiple instances in a	  *
// * single object													  *
// ********************************************************************

#ifndef __KPTRARRAY_H
#define __KPTRARRAY_H

#include "Globals.h"

// ********************************************************************
// * Abstract Template Class KAbstractPtrArray                        *
// ********************************************************************

template <class T>
class KAbstractPtrArray {
protected: //might be better to use private...
	NAMESPACE_STL::vector<T*>* _instances;
	bool _toManage;

protected:
	// Default constructor
	KAbstractPtrArray();
	// Constructor with a list of instances
	KAbstractPtrArray(NAMESPACE_STL::vector<T*>* instances);
	// Destructor (called just after the destructor of the derived class)
	virtual ~KAbstractPtrArray();
#ifdef ACK_LIB
	// Initialize member _instances
	void initializeInstances();
#endif
public:
#ifdef ACK_LIB
	// Check if the object has multiple instances
	bool hasMultipleInstances() const;
	// Retrieve the number of handled instances
	int getNumberOfInstances() const;
#endif
	// Retrieve a specific instance by index
	virtual T* getInstance(unsigned int index) const;
#ifdef ACK_LIB
	// Reserve for n instances
	void reserve(int n);
	// Add a new pointer instance to the object
	void addInstance(T* instance);
	// Set a new pointer instance to the object
	void setInstance(T* instance, unsigned int index);
	// Release instances management
	void releasePtrManagement();
#endif

}; // class KAbstractPtrArray

#ifdef ACK_LIB

template<class T>
KAbstractPtrArray<T>::KAbstractPtrArray() :
	_instances(NULL),
	_toManage(true)
{}

template<class T>
KAbstractPtrArray<T>::KAbstractPtrArray(NAMESPACE_STL::vector<T*>* instances) :
	_instances(instances)
{}

template<class T>
KAbstractPtrArray<T>::~KAbstractPtrArray() {
	if (_instances != NULL) {
		if (_toManage) {
			for (unsigned int i = 0; i < _instances->size(); ++i) {
				if ((*_instances)[i] != NULL) {
					delete (*_instances)[i];
				}
			}
		}
		delete _instances;
		_instances = NULL;
	}
}

template<class T>
void KAbstractPtrArray<T>::initializeInstances() {
	_instances = new NAMESPACE_STL::vector<T*>();
}

template<class T>
bool KAbstractPtrArray<T>::hasMultipleInstances() const {
	return _instances != NULL && !_instances->empty();
}

template<class T>
int KAbstractPtrArray<T>::getNumberOfInstances() const {
	if (_instances == NULL) {
		return 0;
	}
	else {
		return _instances->size();
	}
}

template<class T>
T* KAbstractPtrArray<T>::getInstance(unsigned int index) const {
	if (_instances != NULL && index < _instances->size()) {
		return (*_instances)[index];
	}
	else {
		return NULL;
	}
}

template<class T>
void KAbstractPtrArray<T>::reserve(int n) {
	if (_instances == NULL) {
		initializeInstances();
	}
	_instances->reserve(n);
}

template<class T>
void KAbstractPtrArray<T>::addInstance(T* instance) {
	if (_instances == NULL) {
		initializeInstances();
	}
	_instances->push_back(instance);
}

template<class T>
void KAbstractPtrArray<T>::setInstance(T* instance, unsigned int index) {
	if (_instances == NULL) {
		initializeInstances();
	}
	while (_instances->size() <= index) {
		_instances->push_back(NULL);
	}
	if (_toManage && (*_instances)[index] != NULL) {
		delete (*_instances)[index];
	}
	(*_instances)[index] = instance;
}

template<class T>
void KAbstractPtrArray<T>::releasePtrManagement() {
	_toManage = false;
}
#endif

// ********************************************************************
// * Base Template Class KPtrArray                                    *
// ********************************************************************

template <class T>
class KPtrArray : public KAbstractPtrArray<T> {
protected:
	// Default constructor
	KPtrArray();
	// Constructor with a list of instances
	KPtrArray(NAMESPACE_STL::vector<T*>* instances);
	// Copy constructor (should be called by the copy constructor of the derived class)
	KPtrArray(const KPtrArray<T>& toCopy);
	// Destructor
	virtual ~KPtrArray();
public:
#ifdef ACK_LIB
	// Assignment operator
	KPtrArray<T>& operator=(const KPtrArray<T>& toCopy);
#endif
	// Retrieve a specific instance by index
	// Important: virtual member functions should never be placed inside #ifdef ACK_LIB .. #endif statements !
	virtual T* getInstance(unsigned int index) const;
#ifdef ACK_LIB
	// Add a new pointer instance to the object
	void addInstance(T* instance);
	// Add a new instance to the object (after calling the copy constructor)
	void addInstance(const T& instance);
#endif

}; // class KPtrArray

#ifdef ACK_LIB

template<class T>
KPtrArray<T>::KPtrArray()
{}

template<class T>
KPtrArray<T>::KPtrArray(NAMESPACE_STL::vector<T*>* instances) :
	KAbstractPtrArray<T>(instances)
{}

template<class T>
KPtrArray<T>::KPtrArray(const KPtrArray<T>& toCopy) :
	KAbstractPtrArray<T>()
{
	if (toCopy.hasMultipleInstances()) {
		this->initializeInstances();
		reserve(toCopy.getNumberOfInstances());
		for (int pb = 0; pb < toCopy.getNumberOfInstances(); ++pb) {
			addInstance(new T(*toCopy.getInstance(pb)));
		}
	}
}

template<class T>
KPtrArray<T>::~KPtrArray()
{}

#ifdef ACK_LIB
template<class T>
KPtrArray<T>& KPtrArray<T>::operator=(const KPtrArray<T>& toCopy) {
	if (this->_instances != NULL){
		if (this->_toManage) {
			for (unsigned int i = 0; i < this->_instances->size(); ++i) {
				if ((*this->_instances)[i] != NULL) {
					delete (*this->_instances)[i];
				}
			}
		}
		delete this->_instances;
		this->_instances = NULL;
	}
	if (toCopy.hasMultipleInstances()) {
		this->initializeInstances();
		reserve(toCopy.getNumberOfInstances());
		for (int pb = 0; pb < toCopy.getNumberOfInstances(); ++pb) {
//			addInstance(new T(*toCopy.getInstance(pb))); // Bug due to call to non-virtual copy constructor
			addInstance(toCopy.getInstance(pb)->getCopyPtr());
		}
	}
	return *this;
}

template<class T>
T* KPtrArray<T>::getInstance(unsigned int index) const {
	return KAbstractPtrArray<T>::getInstance(index);
}

template<class T>
void KPtrArray<T>::addInstance(T* instance) {
	KAbstractPtrArray<T>::addInstance(instance);
}

template<class T>
void KPtrArray<T>::addInstance(const T& instance) {
	KAbstractPtrArray<T>::addInstance(new T(instance));
}
#endif

#endif

// ********************************************************************
// * Derived Template Class KDerivedPtrArray                          *
// ********************************************************************

template <class T, class Base> // Base must itself derived from KAbstractPtrArray or KPtrArray
class KDerivedPtrArray : public Base {
protected:
	// Default constructor
	KDerivedPtrArray();
	//// Constructor with a list of instances
	//KDerivedPtrArray(NAMESPACE_STL::vector<T*>* instances);
	// Copy constructor (should be called by the copy constructor of the derived class)
	KDerivedPtrArray(const KDerivedPtrArray<T,Base>& toCopy);
	// Destructor
	virtual ~KDerivedPtrArray();
public:
#ifdef ACK_LIB
	// Assignment operator
	KDerivedPtrArray<T,Base>& operator=(const KDerivedPtrArray<T,Base>& toCopy);
	// Retrieve a specific instance by index
	//T* getInstance(int index) const;
	// Add a new pointer instance to the object
	void addInstance(T* instance);
	// Add a new instance to the object (after calling the copy constructor)
	void addInstance(const T& instance);
#endif

}; // class KDerivedPtrArray

#ifdef ACK_LIB

template<class T, class Base>
KDerivedPtrArray<T,Base>::KDerivedPtrArray() {
}

template<class T, class Base>
KDerivedPtrArray<T,Base>::KDerivedPtrArray(const KDerivedPtrArray<T,Base>& toCopy) {
	if (toCopy.hasMultipleInstances()) {
		this->initializeInstances();
		reserve(toCopy.getNumberOfInstances());
		for (int pb = 0; pb < toCopy.getNumberOfInstances(); ++pb) {
			addInstance(new T(*(T*)toCopy.getInstance(pb)));
		}
	}
}

template<class T, class Base>
KDerivedPtrArray<T,Base>::~KDerivedPtrArray() {
}

template<class T, class Base>
KDerivedPtrArray<T,Base>& KDerivedPtrArray<T,Base>::operator=(const KDerivedPtrArray<T,Base>& toCopy) {
	if (this->hasMultipleInstances()){
		if (this->_toManage) {
			for (unsigned int i = 0; i < this->_instances->size(); ++i) {
				if ((*this->_instances)[i] != NULL) {
					delete (*this->_instances)[i];
				}
			}
		}
		delete this->_instances;
		this->_instances = NULL;
	}
	if (toCopy.hasMultipleInstances()) {
		this->initializeInstances();
		reserve(toCopy.getNumberOfInstances());
		for (int pb = 0; pb < toCopy.getNumberOfInstances(); ++pb) {
			addInstance(new T(*(T*)toCopy.getInstance(pb)));
		}
	}
	return *this;
}

template<class T, class Base>
void KDerivedPtrArray<T,Base>::addInstance(T* instance) {
	KAbstractPtrArray<Base>::addInstance(instance);
}

template<class T, class Base>
void KDerivedPtrArray<T,Base>::addInstance(const T& instance) {
	KAbstractPtrArray<Base>::addInstance(new T(instance));
}

#endif

#endif
