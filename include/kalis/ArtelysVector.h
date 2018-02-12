// ********************************************************************
// * Artelys Utilities												  *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : ArtelysVector.h                                           *
// * Desc : implement the vector<> template class STL replacement     *
// ********************************************************************

#ifndef ARTELYSVECTOR_H
#define ARTELYSVECTOR_H


#include <stdio.h>
#include <iostream>

namespace Artelys {

/** @doc

    vector<T>
  
	Description :

	A vector is a Sequence that supports :
	- random access to elements, 
	- constant time insertion and removal of elements at the end, and 
	- linear time insertion and removal of elements at the beginning or in the middle. 
	
	The number of elements in a vector may vary dynamically; memory management is automatic. 
	Vector is the simplest of the container classes, and in many cases the most efficient. 
	
	Example :
	
	@code

	vector<int> V;
	V.insert(V.begin(), 3);
	assert(V.size() == 1 && V.capacity() >= 1 && V[0] == 3);

	@endcode

*/

template <typename T> class vector {
  
  #define _ARTELYS_VECTOR_DEFAULT_ALLOCATION_SIZE 10

  protected:
	
	T *			 _array;
	unsigned int _size;
	unsigned int _allocatedSize;


   public :

	class iterator;
	friend class iterator;
   
    class iterator
    {
	  protected :
        
		unsigned int _currentIndex;
        
        /** Vector pointed
        */
        const vector<T> * _vect;
        
      public:
        
        /** Default constructor
        */
        iterator() {
			_currentIndex = 0;
	        _vect = NULL;
        }
        
        /** Contructor hooking to a list and pointing to a node
        *   @param aVect : the list for hooking
        *   @param node : the node for pointing to
        */
        iterator(const vector<T> * const aVect, int index) 
		{
	        _currentIndex = index;
	        _vect = aVect;
        }
        
        /** Copy-constructor
        *   @param iterator : iterator to copy
        */
        iterator(const iterator& iterator) {
			_currentIndex = iterator._currentIndex;
	        _vect = iterator._vect;	        
        }

        /** Destructor
        */
        ~iterator() {
            /* NOTHING TO DO */ ;
        }
                                        
        /** Tests equality
        *   @param iterator : 2th operand
        */
        bool operator==(  const iterator& iterator ) const {
        	return (iterator._currentIndex == _currentIndex);
        }		
        
        /** Tests disequality
        *   @param iterator : 2th operand
        */
        bool operator!=(  const iterator& iterator  ) const {
        	return (iterator._currentIndex != _currentIndex);
        }

        /** Returns the data of the pointed node
        */
        T& operator*() const {
	        return _vect->_array[_currentIndex];			
        }
		
        /** Prefixed ++ operator : the pointed node becomes
        *                          the successor of the currently
        *                          pointed node
        */
        iterator& operator++() {
	        ++_currentIndex;
	        return *this;
        }
        
        /** Postfixed ++ : the pointed node becomes
        *                  the successor of the currently
        *                  pointed node
        */
        const iterator operator++(int){
        	iterator temp = *this;
            ++_currentIndex;
	        return temp;
        }
       
        /** Prefixed -- operator : the pointed node becomes
        *                          the predecessor of the currently
        *                          pointed node
        */
        iterator& operator--()  {	        
	        --_currentIndex;
	        return (*this);
        }
        
        /** Postfixed -- operator : the pointed node becomes
        *                           the predecessor of the currently
        *                           pointed node
        */
        const iterator  operator--(int) {	
        	iterator temp;
            --_currentIndex;
	        return temp;
        }
    };

  public :
	
	/** Default contructor
	*/
	vector();
	
	vector(int size);
	
	vector(int size,const T&d);
	
	/** Copy-constructor
	*/
	vector( const vector<T>& aVect );
	//explicit vector( const vector<T>& aVect );

	/** Destructor
	*/
	~vector();
	 
    /** Insert a new node at the end of the list
    *   @param d : data to be stored in the newly inserted node
    */
	inline void push_back(const T& d);

    /** Removes the last node of the list
    */
	inline void pop_back(void);

	inline T & front(void) const;

	inline T &	back(void) const;

	inline unsigned int capacity(void) const;

	inline unsigned int size(void) const;

	/** Returns true if the vector is empty
    *   False else
    */
	inline bool empty(void) const; 

	void reserve(unsigned int rSize);

	void resize(unsigned int size,const T&d); 

	void print(void) const; 

	vector<T>& operator=(const vector<T>& aVect);
    
    /** Returns an iterator poiting to the first node
    *   of the list
    */
	inline iterator begin(void) const;

    /** Returns an iterator pointing to the head of the list
    *   WARNING : this node is not a valid node !        
    */
	inline iterator end(void) const;

	// To allow modification of vector's element, the operator[] returns non-const references
///	inline const T& operator[](unsigned int index) const;
	inline T& operator[](unsigned int index) const;
	
	iterator find(const iterator &pos1,const iterator &pos2,const T& d) const;

	inline void clear(void);


};	// Class definition

}  	// NAMESPACE Artelys


// *****************************************************************
// *                                                               *
// * IMPLEMENTATION                                                *
// *                                                               *
// *****************************************************************


using namespace Artelys;
namespace Artelys {
template <typename T> vector<T>::vector()
{
	_array = NULL;
	_array = new T[_ARTELYS_VECTOR_DEFAULT_ALLOCATION_SIZE];
	_allocatedSize = _ARTELYS_VECTOR_DEFAULT_ALLOCATION_SIZE;
	_size = 0;
}

template <typename T> vector<T>::vector(int size)
{
	_array = NULL;
	_array = new T[size];
	_allocatedSize = size;
	_size = size; 	
}

template <typename T> vector<T>::vector(int size,const T&d)
{
	_array = NULL;
	_array = new T[size];
	_allocatedSize = size;
	_size = size;
	for (unsigned int i=0;i<_size;i++) _array[i] = d;
}

template <typename T> vector<T>::vector( const vector<T>& aVect ) : _size(aVect._size)
{
	_array = NULL;
	_array = new T[aVect._size];
	memcpy(_array,aVect._array,aVect._size * sizeof(T));
	_size = aVect._size;
	_allocatedSize = _size;

}

template <typename T> vector<T>::~vector()
{
	if (_array != NULL)
		delete[] _array;
}

template <typename T> inline void vector<T>::push_back(const T& d)
{
	if (_size+1 >= _allocatedSize) {
		reserve((_size+1) * 2);
	}
	_array[_size++] = d;
} 

template <typename T> inline void vector<T>::pop_back()
{
	if (_size > 0) {
		--_size;	
	}
}

template <typename T> inline T & vector<T>::front(void) const 
{
	return (*begin());
}

template <typename T> inline T &	vector<T>::back(void) const 
{
	return *(--end());
}

template <typename T> inline unsigned int vector<T>::capacity(void) const 
{
	return _allocatedSize;
}

template <typename T> inline unsigned int vector<T>::size(void) const 
{
	return _size;
}	

template <typename T> inline bool vector<T>::empty(void) const 
{
	return (_size == 0);
}

template <typename T> void vector<T>::reserve(unsigned int rSize) 
{
	if (_allocatedSize < rSize) {
		T * _tmpArray;
		_tmpArray = new T[rSize];
		memcpy(_tmpArray,_array,_size * sizeof(T));
		delete [] _array;
		_array = _tmpArray;	
		_allocatedSize = rSize;
	}
}

template <typename T> void vector<T>::resize(unsigned int size,const T&d) 
{
	if (size <= _size) 
	{			
		_size = size;
		for (unsigned int i=0;i<_size;i++) _array[i] = d;
	} else 
	{						
		reserve(size);
		_size = size;
		for (unsigned int i=0;i<_size;i++) _array[i] = d;
	}
}

template <typename T> void vector<T>::print(void) const 
{
	if (_size > 0) {
		printf("[");
		for (unsigned int i=0;i<_size-1;i++)
			std::cout << _array[i] << ",";
		std::cout << _array[_size-1] << "]"<< std::endl;		
	}
}

template <typename T> vector<T>& vector<T>::operator=(const vector<T>& aVect) 
{
	if (_array != NULL) {
		delete [] _array;
	}
	_array = new T[aVect._size];
	memcpy(_array,aVect._array,aVect._size * sizeof(T));
	_size = aVect._size;
	_allocatedSize = _size;		
	return (*this);
}


template <typename T> inline typename vector<T>::iterator vector<T>::begin(void) const
{
	return iterator(this,0);
}

template <typename T> inline typename vector<T>::iterator vector<T>::end(void) const
{
	return iterator(this,_size);
}

template <typename T> inline T& vector<T>::operator[](unsigned int index) const
///template <typename T> inline const T& vector<T>::operator[](unsigned int index) const
{
	return _array[index];
}

template <typename T> typename vector<T>::iterator vector<T>::find(const iterator &pos1,const iterator &pos2,const T& d) const 
{
	iterator it;   
	for ( it = pos1; it != pos2; it++) {
		if ( *it == d )
			return (it);
	}    
	return end();
}

template <typename T> inline void vector<T>::clear(void) {
	_size = 0;
}
}

#endif	// #define



