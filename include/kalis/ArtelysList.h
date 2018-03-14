// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : ArtelysList.h                                             *
// * Description : represents Artelys general template for a list of  *
// * pointers to objects.                                             *
// * Pointed objects are not deleted unless explicit call to          *
// * "emptyAndDelete()"                                               *
// ********************************************************************

#ifndef __ARTELYS_LIST_H
#define __ARTELYS_LIST_H

#include <stdarg.h>
#include <stdlib.h>
#include "ArtelysException.h"
#include "KIntVar.h"
#include "KSolution.h"
#include <iostream>



//************
//*** class Declaration
//************
//
template <class T> class ArtelysList {
protected:
	bool *_toManage;
	T** _list;              // Array of pointers to objects
	int _size;              // array size
	int _nbOfElements;      // number of used elements
	int _increment;         // size increment

	static const int DEFAULT_SIZE;  // Default array size 

public:
	//constructor
	ArtelysList();
	ArtelysList(int size);
#ifdef ZWIN32
	ArtelysList(int nbElements, ...);
#endif
	ArtelysList(const ArtelysList& artelysList);

	//destructor
	virtual ~ArtelysList();

	// get 'methods'
	int getNumberOfElements() const; // returns the number of elements in the list
	T* getElt(int index) const; // returns a pointer on element at position index
	bool isEmpty() const; // returns true if the list is empty

	//methods
	void makeList(int size);
#ifdef WIN32
	void initializeListWithVa_list(int nbElements, va_list& arglist);
#endif
	int initializeAsEmptyList();
	int add(T* elt_ptr, const bool toManage = false);   // add a new element
	int add(const T& elt); // make a copy of the element and add it to the list
	int insert(T* elt_ptr, const int index, const bool toManage = false);   // insert a new element at position index
	int emptyAndDelete();  // it deletes all elements of _list and empty the list
	virtual int check(T* elt_ptr) const; // check if adding elt_ptr keeps list integrity
	virtual ArtelysList<T>* getCopyPtr() const { return new ArtelysList(*this);}
	void removeLastElement();
	void replace(const int index, T* const elt_ptr, const bool toManage = false);
	void print();

	//operators redefinition
	ArtelysList& operator+=(const T& elt); // call add(elt)
	T& operator[](int index); // return a pointer on element at position "index"
	const T& operator[](int index) const; // return a pointer on element at position "index"

}; // class ArtelysList

#ifdef ACK_LIB

/***************************************************************************/
/***                              Methods                               ****/
/***************************************************************************/

//*************
//*** initializeAsEmptyList() : make an empty list (this method is deprecated)
//*************
template <class T> int ArtelysList<T>::initializeAsEmptyList() {
	makeList(DEFAULT_SIZE); // Initialize list with default size
	return 0;
}

template <class T> void ArtelysList<T>::print() {
	for (int i=0;i<getNumberOfElements();i++) {
		std::cout << getElt(i) << ",";
	}
	std::cout << "\n";
}

//*************
//*** makeList(int size) : make an empty list of Size size
//*************
template <class T> void ArtelysList<T>::makeList(int size) {
	int eltIndex;

	// delete _list
	if (_list) {
		delete[] _list;
		_list = NULL;
	}
	if (_toManage) {
		delete [] _toManage;
		_toManage = NULL;
	}

	// New parameters
	_size = size;  // Initialize list with default size
	_increment = _size/10 + DEFAULT_SIZE;
	_nbOfElements = 0;

	//_list allocation;
	_list = new T*[_size];    

	if(!_list)
		throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4001,"Not enough memory\n"); 

	_toManage = new bool[_size];

	if(!_toManage)
		throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4001,"Not enough memory\n");

	for (eltIndex = 0; eltIndex < _size; eltIndex++) {
		_toManage[eltIndex] = false;
	}
}

//*************
//*** default constructor
//*************
template <class T> ArtelysList<T>::ArtelysList() {
	_list = NULL;
	_toManage = NULL;
	makeList(DEFAULT_SIZE); // Initialize list with default size
}


//*************
//*** constructor fo a specific size
//*************
template <class T> ArtelysList<T>::ArtelysList(int size) {
	_list = NULL;
	_toManage = NULL;
	makeList(size);
}


#ifdef ZWIN32
//*************
//*** constructor with a list of elements
//*************
template <class T> ArtelysList<T>::ArtelysList(int nbElements, ...) {
	_list = NULL;
	_toManage = NULL;
	makeList(nbElements);

	va_list arglist;
	va_start(arglist, nbElements);
	initializeListWithVa_list(nbElements,arglist);
	va_end(arglist);
}
#endif

//*************
//*** constructor with an artelysList
// Are the pointed elements copied or only pointers copied ?
//*************
template <class T> ArtelysList<T>::ArtelysList(const ArtelysList& artelysList) {
	int nbElements = artelysList.getNumberOfElements();
	_list = NULL;
	_toManage = NULL;
	makeList(nbElements);
	for(int index=0; index < nbElements; index++) {
		add(*(artelysList._list[index]));    
	}
}

//*************
//*** initialise a list with a va_list
//*************
#ifdef WIN32
template <class T> void ArtelysList<T>::initializeListWithVa_list(int nbElements, va_list& arglist) {
	for(int index=0; index < nbElements; index++)
	{
		T element = va_arg(arglist,T);
		add(element);
	}
}
#endif

//*************
//*** destructor
//*************
template <class T> ArtelysList<T>::~ArtelysList() {
	if(_list) {
		delete[] _list;
		_list = NULL;
	}
	if (_toManage) {
		delete [] _toManage;
		_toManage = NULL;
	}
}

//*************
//*** int getNumberOfElements() const; // returns the number of elements in the list
//*************
template <class T> int ArtelysList<T>::getNumberOfElements() const {
	return _nbOfElements;
}

//*************
//*** T* getElt(int index) const; // returns a pointer on element at position index
//*************
template <class T> T* ArtelysList<T>::getElt(int index) const {
	if ((index <0) || (index > _nbOfElements -1)) {
		//cout << "index = " << index << " - nbElts = " << _nbOfElements << " - size = " << _size << "\n";
		//cout << " sizeof T = " << sizeof(T) << "\n";
		//cout << _list[0] << "\n";
		return (T*) -1;
		throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4002,"Index out of range (a)");
	}
	return _list[index];
}

//*************
//*** bool isEmpty() const; // returns true if the list is empty
//*************
template <class T> bool ArtelysList<T>::isEmpty() const {
	return _nbOfElements == 0;
}


//*************
//*** add(T* elt_ptr) : it adds elt_ptr at the end of _list
//*************
template <class T> int ArtelysList<T>::add(T* elt_ptr, const bool toManage) { 
	int eltIndex;
	check(elt_ptr);

	if(_nbOfElements == _size) // it must be resized
	{
		T** tmpList = new T*[_size+_increment];   
		if(!tmpList)
			throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4001,"Not enough memory\n");
		bool* tmpToManage = new bool[_size+_increment];
		if(!tmpToManage)
			throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4001,"Not enough memory\n");
		memcpy(tmpList,_list,_size*sizeof(T*));
		memcpy(tmpToManage,_toManage,_size*sizeof(bool));
		delete[] _list;
		delete [] _toManage;
		_list = tmpList;
		_toManage = tmpToManage;

		for (eltIndex = _size; eltIndex < _size+_increment; eltIndex++) {
			_toManage[eltIndex] = false;
		}

		_size += _increment;
		_increment = _size/10 + DEFAULT_SIZE;
	}

	// add the element
	if (!toManage) {
		_list[_nbOfElements] = (T*)elt_ptr->getCopyPtr();
		_toManage[_nbOfElements] = true;
		_nbOfElements++;

	} else {
		_list[_nbOfElements] = (T*)elt_ptr;
		_toManage[_nbOfElements] = toManage;
		_nbOfElements++;
	}

	return (_nbOfElements -1);
}

//*************
//*** add(T elt) : it adds elt_ptr at the end of _list
//*************
template <class T> int ArtelysList<T>::add(const T& elt) {
	T* elt_copy;

	// make a copy of the element
	// We do not use 'elt_copy = new T(elt);' because we need a virtual copy function
	// This is important when 'elt' is actually an instance of a class derived from class T
	elt_copy = (T*) elt.getCopyPtr();
	if(!elt_copy)
		throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4001,"Not enough memory\n");

	// add the element
	return add(elt_copy,true); 
}



//*************
//*** insert(T* elt_ptr, const int index, const bool toManage = false) : inserts elt_ptr at position index of _list
//*************
template <class T> int ArtelysList<T>::insert(T* elt_ptr, const int index, const bool toManage) {
	int eltIndex;
	check(elt_ptr);

	if(_nbOfElements == _size) // it must be resized
	{
		T** tmpList = new T*[_size+_increment];   
		if(!tmpList)
			throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4001,"Not enough memory\n");
		bool* tmpToManage = new bool[_size+_increment];
		if(!tmpToManage)
			throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4001,"Not enough memory\n");
		memcpy(tmpList,_list,_size*sizeof(T*));
		memcpy(tmpToManage,_toManage,_size*sizeof(bool));
		delete[] _list;
		delete [] _toManage;
		_list = tmpList;
		_toManage = tmpToManage;

		for (eltIndex = _size; eltIndex < _size+_increment; eltIndex++) {
			_toManage[eltIndex] = false;
		}

		_size += _increment;
		_increment = _size/10 + DEFAULT_SIZE;
	}

	// insert the element
	// can do better using memcpy
	T* eltToBeInserted = elt_ptr;
	bool toManageToBeInserted = toManage;
	T* eltTmp;
	bool toManageTmp;
	eltIndex = index;
	while(eltIndex < _nbOfElements) {
		eltTmp = _list[eltIndex];
		toManageTmp = _toManage[eltIndex];
		_list[eltIndex] = eltToBeInserted;
		_toManage[eltIndex] = toManageToBeInserted;
		eltToBeInserted = eltTmp;
		toManageToBeInserted = toManageTmp;
		++eltIndex;
	}
	_list[_nbOfElements] = eltToBeInserted;
	_toManage[_nbOfElements] = toManageToBeInserted;
	_nbOfElements++;

	return (_nbOfElements - 1);
}


template <class T> void ArtelysList<T>::replace(const int index, T* const elt_ptr, const bool toManage) {
	if ((index < 0) || (index>_nbOfElements))
		throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4002,"Index out of range  (b)");

	if (_list[index] && _toManage[index])
		delete _list[index];

	_list[index] = elt_ptr;
	_toManage[index] = toManage;
}

//*************
//*** emptyAndDelete() : it deletes all elements of _list and empty the list
//*************
template <class T> int ArtelysList<T>::emptyAndDelete() {
	if(_list && _toManage) {
		int i=0;
		for (i=0;i<_nbOfElements;i++) if ( _toManage[i] && _list[i] ) {
			delete _list[i];
			_list[i]=NULL;
		}
		/*    delete[] _list;
		delete [] _toManage;
		_list = NULL;
		_toManage = NULL;*/
	}

	_nbOfElements = 0;
	return 0;
}

//*************
//*** removeLastElement()
//*************
template <class T> void ArtelysList<T>::removeLastElement() { 
	if (_list) {
		if (_toManage[_nbOfElements-1]) {
			delete(_list[_nbOfElements-1]);
			_list[--_nbOfElements] = NULL;    
		}
		else
			--_nbOfElements;
	}
}


//*************
//*** operator+=(const T& elt); // call add(elt)
//*************
template <class T> ArtelysList<T>& ArtelysList<T>::operator+=(const T& elt) {
	add(elt);
	return *this;
}


//*************
//*** T const* operator[](int index); // return a pointer on element at position "index"
//*************
template <class T> T&  ArtelysList<T>::operator[](int index) {
	if ((index <0) || (index > _nbOfElements -1))
		throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4002,"Index out of range (c)");
	return *_list[index];
}
template <class T> const T&  ArtelysList<T>::operator[](int index) const {
	if ((index <0) || (index > _nbOfElements -1))
		throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4002,"Index out of range (d)");
	return *_list[index];
}

template <class T> int ArtelysList<T>::check(T* elt_ptr) const {
	//elt_ptr = elt_ptr; // Dummy instruction : avoid a warning when compiling with g++ flags -W -Wall 
	return 0;
}

template <class T> const int ArtelysList<T>::DEFAULT_SIZE = ARTELYS_LIST_DEFAULTSIZE;

#endif // End of : #ifdef ACK_LIB

#endif // End of : #ifndef __ARTELYS_LIST_H

// End of file : ArtelysList.h

