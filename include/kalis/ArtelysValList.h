// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : ArtelysValList.h                                          *
// * Description : it represents Artelys general template for a list  *
// * of simple objects (int, real, ...).                              *
// ********************************************************************

#ifndef __ARTELYS_VAL_LIST_H
#define __ARTELYS_VAL_LIST_H

#include <stdarg.h>
#include <stdlib.h>
#include "ArtelysException.h"

//************
//*** class Declaration
//************
//
template <class T> class ArtelysValList {
protected:
  T* _list;              // Array of objects
  int _size;              // array size
  int _nbOfElements;      // number of used elements
  int _increment;         // size increment

  static const int DEFAULT_SIZE;  // Default array size 

public:
  //constructors
  ArtelysValList();
  ArtelysValList(int size);
#ifdef ZWIN32
  ArtelysValList(int nbElements, ...);
#endif
  ArtelysValList(const ArtelysValList& artelysValList);

  //destructor
  virtual ~ArtelysValList();

  // get 'methods'
  int getNumberOfElements() const; // returns the number of elements in the list

  //methods
  void makeList(int size);
  void initializeListWithVa_list(int nbElements, va_list& arglist);
  int initializeAsEmptyList();
  int add(const T& elt); // make a copy of the element and add it to the list
  int empty();  // empty the list
  void clear();
  virtual ArtelysValList<T>* getCopyPtr() const { return new ArtelysValList<T>(*this);}

  //operators redefinition
  ArtelysValList& operator+=(const T& elt); // call add(elt)
  T& operator[](int index) const; // return the element at position "index"
  T& getElt(int index) const;

}; // class ArtelysValList

#ifdef ACK_LIB

/***************************************************************************/
/***                              Methods                               ****/
/***************************************************************************/

//*************
//*** initializeAsEmptyList() : make an empty list (this method is deprecated)
//*************
template <class T> int ArtelysValList<T>::initializeAsEmptyList() {
  makeList(DEFAULT_SIZE); // Initialize list with default size
  return 0;
}

//*************
//*** makeList(int size) : make an empty list of Size size
//*************
template <class T> void ArtelysValList<T>::makeList(int size) {
  // delete _list
  if (_list) {
    delete[] _list;
    _list = NULL;
  }
  
  // New parameters
  _size = size;  // Initialize list with default size
  _increment = _size/10 + DEFAULT_SIZE;
  _nbOfElements = 0;

  //_list allocation;
  _list = new T[_size];  

  if(!_list)
    throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4001,"Not enough memory\n"); 
}

//*************
//*** default constructor
//*************
template <class T> ArtelysValList<T>::ArtelysValList() {
  _list = NULL;
  makeList(DEFAULT_SIZE); // Initialize list with default size
}


//*************
//*** constructor fo a specific size
//*************
template <class T> ArtelysValList<T>::ArtelysValList(int size) {
  _list = NULL;
  makeList(size);
}

/*
//*************
//*** constructor with a list of elements
//*************
template <class T> ArtelysValList<T>::ArtelysValList(int nbElements, ...) {
  _list = NULL;
  makeList(nbElements);

  va_list arglist;
  va_start(arglist, nbElements);
  initializeListWithVa_list(nbElements,arglist);
  va_end(arglist);
}*/

//*************
//*** constructor with an artelysList
// Are the pointed elements copied or only pointers copied ?
//*************
template <class T> ArtelysValList<T>::ArtelysValList(const ArtelysValList& artelysList) {
  int nbElements = artelysList.getNumberOfElements();
  _list = NULL;
  makeList(nbElements);
  for(int index=0; index < nbElements; index++)
    add(artelysList._list[index]);
}

//*************
//*** initialise a list with a va_list
//*************
template <class T> void ArtelysValList<T>::initializeListWithVa_list(int nbElements, va_list& arglist) {
  for(int index=0; index < nbElements; index++)
  {
    T element = va_arg(arglist,T);
    add(element);
  }
}

//*************
//*** destructor
//*************
template <class T> ArtelysValList<T>::~ArtelysValList() {
  if(_list) {
    delete[] _list;
    _list = NULL;
  }
}

//*************
//*** int getNumberOfElements() : returns the number of elements in the list
//*************
template <class T> int ArtelysValList<T>::getNumberOfElements() const {
  return _nbOfElements;
}


//*************
//*** add(T elt) : it adds elt_ptr at the end of _list
//*************
template <class T> int ArtelysValList<T>::add(const T& elt) {
  if(_nbOfElements == _size) // it must be resized
  {
	
//	printf("Resizing!!! _size+_increment = %i\n",(_size+_increment) * sizeof(T));
    T* tmpList = new T[_size+_increment];
    if(!tmpList)
      throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4001,"Not enough memory\n");
    memcpy(tmpList,_list,_size*sizeof(T));
    delete[] _list;
    _list = tmpList;
    _size += _increment;   
    _increment *= 2;// _size/10 + DEFAULT_SIZE;   
  }

  // add the element
  _list[_nbOfElements] = elt;
  _nbOfElements++;

  return 0;
}


//*************
//*** empty : empty the list
//*************
template <class T> void ArtelysValList<T>::clear() { 
  _nbOfElements = 0;  
}

//*************
//*** empty : empty the list
//*************
template <class T> int ArtelysValList<T>::empty() {
  if(_list) {
    delete[] _list;
    _list = NULL;
  }
  _nbOfElements = 0;
  return 0;
}


//*************
//*** operator+=(const T& elt); // call add(elt)
//*************
template <class T> ArtelysValList<T>& ArtelysValList<T>::operator+=(const T& elt) {
  add(elt);
  return *this;
}


//*************
//*** T const* operator[](int index); // return a pointer on element at position "index"
//*************
template <class T> T&  ArtelysValList<T>::operator[](int index) const {
  if ((index <0) || (index > _nbOfElements -1))
    throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4002,"Index out of range (e)");
  return _list[index];
}

template <class T> T&  ArtelysValList<T>::getElt(int index) const {
  if ((index <0) || (index > _nbOfElements -1))
    throw ArtelysException(__FILE__,__LINE__,__DATE__,__TIME__,4002,"Index out of range (e)");
  return _list[index];
}

template <class T> const int ArtelysValList<T>::DEFAULT_SIZE = ARTELYS_LIST_DEFAULTSIZE;

#endif // End of : #ifdef ACK_LIB

#endif // End of : #ifndef __ARTELYS_VAL_LIST_H

// End of file : ArtelysValList.h





