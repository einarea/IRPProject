// ********************************************************************
// * Artelys Utilities                                                *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : ArtelysSingleLinkedList.h                                 *
// * Desc : implement the slist<data_type> template class             *
// * STL replacement                                                  *
// ********************************************************************


#ifndef ARTELYSSINGLELINKEDLIST_H
#define ARTELYSSINGLELINKEDLIST_H

#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
#include <ArtelysPool.h>
#include <iostream>
#endif

namespace Artelys 
{

/** @doc

	<b>slist<data_type> template</b>
  
	Description :

	An slist is a singly linked list: 
	a list where each element is linked to the next element, but not to the previous element. 
	
		[1] That is, it is a Sequence that supports forward but not backward traversal, 
	and (amortized) constant time insertion and removal of elements. 
	slists, like lists, have the important property that insertion and splicing do not invalidate 
	iterators to list elements, and that even removal invalidates only the iterators that point to 
	the elements that are removed. The ordering of iterators may be changed (that is, slist<T>::iterator 
	might have a different predecessor or successor after a list operation than it did before), but the 
	iterators themselves will not be invalidated or made to point to different elements unless that 
	invalidation or mutation is explicit. 
	
		[2] The main difference between slist and list is that list's iterators are bidirectional iterators, 	
	while slist's iterators are forward iterators. This means that slist is less versatile than list;
	frequently, however, bidirectional iterators are unnecessary. You should usually use slist unless you 
	actually need the extra functionality of list, because singly linked lists are smaller and faster than 
	double linked lists. 

	<b>Important note: </b>
		
	members function insert and erase and pop_back are <b>NOT DEFINED</b> because of performances issues

	Example :

    @code

	int main() {
	  slist<int> L;
	  L.push_front(0);
	  L.push_front(1);
	  L.insert_after(L.begin(), 2);
	  copy(L.begin(), L.end(),        // The output is 1 2 0
		   ostream_iterator<int>(cout, " "));
	  cout << endl;

	  slist<int>::iterator back = L.previous(L.end());
	  back = L.insert_after(back, 3); 
	  back = L.insert_after(back, 4);
	  back = L.insert_after(back, 5);
	  copy(L.begin(), L.end(),        // The output is 1 2 0 3 4 5
		   ostream_iterator<int>(cout, " "));
	  cout << endl;
	}
	
	@endcode
*/
 
template <class T> class slist
{
	
  protected :
	
	typedef struct _ListNode_ {
		struct _ListNode_ * next;    // next node in list
		T data;						 // user data 
	} ListNode;

	ListNode  *  _head;
	ListNode  *  _tail;		
	unsigned int _size;	
	
	#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
	Artelys::pool<ListNode> * _allocationPool;
	#endif
	
  public :
	
	class iterator;
	friend class iterator;        
	
	/** Iterateur de liste
	*/
	class iterator
	{
		friend class slist<T>; 
		
	  protected:
		
	    /** Node currently pointed
		*/
		ListNode *_currentNode;
		
		/** List pointed
		*/
		const slist<T> *_doubleList;
		
	  public:
		
	    /** Default constructor
		*/
		iterator() {
			_currentNode = NULL;
			_doubleList = NULL;
		}
		
		/** Contructor hooking to a list and pointing to a node
		*   @param list : the list for hooking
		*   @param node : the node for pointing to
		*/
		iterator(const slist<T> * const liste, ListNode * const node) {
			_currentNode = node;
			_doubleList = liste;
		}
		
		/** Copy-constructor
		*   @param iterator : iterator to copy
		*/
		iterator(const iterator& iterator) {
			_currentNode = iterator._currentNode;
			_doubleList = iterator._doubleList;
		}
		
		/** Destructor
		*/
		~iterator() {
			;
		}
		
		/** Tests equality
		*   @param iterator : 2th operand
		*/
		bool operator==(  const iterator& iterator ) const {
			return (iterator._currentNode == _currentNode);
		}		
		
		/** Tests disequality
		*   @param iterator : 2th operand
		*/
		bool operator!=(  const iterator& iterator  ) const {
			return (iterator._currentNode != _currentNode);
		}
		
		/** Returns the data of the pointed node
		*/
		T& operator*() const {
			return _currentNode->data;
		}
		
		/** operator +  : the pointed node becomes the Nth successor
		*                 of the currently pointed node
		*   @param dec  : N
		*/
		iterator operator+(const int dec) const {
			unsigned int decIndex;
			
			iterator temp(*this);  
			for ( decIndex = 0; decIndex < dec; decIndex++) 
				temp._currentNode = temp._currentNode->next;
			
			return temp;
		}
				
		/** += operator, matches the + operator
		*/
		iterator &operator+=(const int dec) const {
			unsigned int decIndex;
			
			for ( decIndex = 0; decIndex < dec; decIndex++) 
				_currentNode = _currentNode->next;
			
			return (*this);
		}
		
		/** Prefixed ++ operator : the pointed node becomes
		*                          the successor of the currently
		*                          pointed node
		*/
		iterator& operator++() {
			
			_currentNode = _currentNode->next;
			return *this;
		}
		
		/** Postfixed ++ : the pointed node becomes
		*                  the successor of the currently
		*                  pointed node
		*/
		const iterator operator++(int) {
			iterator temp = *this;
			_currentNode = _currentNode->next;	
			return temp;
		}		
		
	}; // end of iterator
	
  public:		
	
    /** Default contructor
	*/
	slist();
	
	#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
	/** Contructor with memory pool allocator argument
	*/
	slist(pool<ListNode> * memPoolAllocator);
	#endif
	
	/** Copy-constructor
	*/
	explicit slist( const slist<T>& liste);	  
	
	/** Destructor
	*/
	~slist() ;
	
	/** Overloaded = operator
	*   @param list : list to copy
	*/
	slist<T>& operator=(const slist<T>& liste);
	
	/** Returns true if the list is empty
	*   False else
	*/
	inline bool empty(void) const;
	
	/** Returns the number of nodes in the list
	*/
	inline unsigned int size(void) const;	
	
	/** Returns an iterator poiting to the first node
	*   of the list
	*/
	inline const iterator begin(void) const;
	
	/** Returns an iterator pointing to the head of the list
	*   WARNING : this node is not a valid node !        
	*/
	inline const iterator end(void) const;
	
	/** Returns a reference to the data stored
	*   at the beginning of the list
	*/
	inline T& front(void) const; 
	
	/** Returns a reference to the data stored
	*   at the end of the list
	*/
	inline T& back(void) const;
	
	/** Inserts a new node in the list        
	*   @param pos : iterator pointing to the node before which the new
	*                node has to be inserted
	*   @param d   : data to be stored in the new node
	*/
	inline void insert(const iterator &pos, const T& d);
	
	/** Inserts a specified sequence from another list in the list
	*   Constant time
	*   @param pos : iterator pointing to the node before which the new
	*                node has to be inserted  
	*   @param i   : iterator pointing to the first node to be inserted
	*   @param j   : iterator pointing to the first node to be NOT inserted
	*/
	inline void insert(const iterator &pos, const iterator &i, const iterator &j);
	
	/** Deletes a specified node from the list
	*   Constant time
	*   @param pos : iterator pointing to the node to be deleted
	*   WARNING : iterator pos is no longer valid after erasing the node !
	*/
	//inline void erase(const iterator &pos);
	
	/** Deletes a specified sequence of node from the list
	*   constant time
	*   @param pos1 : iterator pointing to the first node to be deleted
	*   @param pos2 : iterator pointing to the first node to be NOT deleted
	*/
	//inline void erase(const iterator &pos1,const iterator &pos2);
	
	/** Insert a new node at the beginning of the list        
	*   @param d : data to be stored in the newly inserted node
	*/
	inline void push_front(const T& d);
	
	/** Insert a new node at the end of the list
	*   @param d : data to be stored in the newly inserted node
	*/
	inline void push_back(const T& d);
	
	/** Removes the first node of the list
	*/
	inline void pop_front(void);
	
	/** Removes all elements of the list
	*/
	inline void clear(void);
	
	/** pretty print the list
	*/
	inline void print(void);
};

} // fin de namespace Artelys


// *****************************************************************
// *                                                               *
// * IMPLEMENTATION                                                *
// *                                                               *
// *****************************************************************

using namespace Artelys;

template <class T> slist<T>::slist() {	
	#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
	_allocationPool = new pool<ListNode>(32,2,true);	
	_head = _allocationPool->alloc();
	#else
	_head = new ListNode;//new ListNode;;
	#endif
	_head->next=NULL;
	_tail = _head;
	_size = 0;
}

#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
template <class T> slist<T>::slist(pool<ListNode> * memPoolAllocator) {
	_allocationPool = memPoolAllocator;
	_allocationPool->incRefCount();
	_head = _allocationPool->alloc();
	_head->next= NULL;
	_tail = _head;
	_size = 0;
}
#endif

template <class T> slist<T>::slist(const slist<T>& liste ) {
	#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
	_allocationPool = new pool<ListNode>(32,2,true);
	_head = _allocationPool->alloc();
	#else
	_head = new ListNode;;
	#endif
	_head->next = NULL;
	_tail = _head;
	iterator it;
	for (it = liste.begin();it != liste.end(); it++) 
		push_front(*it);
}

template <class T> slist<T>::~slist() {
	#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
	if (_allocationPool->getRefCount() == 1)
		delete _allocationPool;	
	#else
		clear();
		delete _head;
	#endif
}

template <class T> slist<T>& slist<T>::operator=(const slist<T>& liste) {
	clear();
    //erase(begin(),end());
    insert(end(),liste.begin(),liste.end());
    return (*this);
}

template <class T> inline bool slist<T>::empty(void) const {
	return (_size == 0);
}

template <class T> inline unsigned int slist<T>::size(void) const {
	return _size;
}

template <class T> inline const typename slist<T>::iterator slist<T>::begin(void) const {
	return iterator(this,_head->next);
}

template <class T> inline const typename slist<T>::iterator slist<T>::end(void) const {
	return iterator(this,NULL);
}

template <class T> inline T &slist<T>::front(void) const {
	return _head->next->data;
}

template <class T> inline T &slist<T>::back(void) const {
	return _tail->data;
}

template <class T> inline void slist<T>::insert(const iterator &position,const T& data) {
	#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
	ListNode * newNode = _allocationPool->alloc();
	#else
	ListNode * newNode = new ListNode;
	#endif
	newNode->next = position._currentNode;	
	newNode->data = data;
	position._currentNode->next = newNode;	
	++_size;	
}

template <class T> inline void slist<T>::insert(const iterator &pos,const iterator &i,const iterator &j) {    
    iterator temp = i;
    while ( temp != j ) {
        insert(pos,*(temp++));
    }
}

/*template <class T> inline void slist<T>::erase(const iterator &position) {
	printf("Call To ERASE\n");
}

template <class T> inline void slist<T>::erase(const iterator &pos1,const iterator &pos2) {
	printf("Call To ERASE\n");
	iterator it;
	it = pos1;
	while ( it != pos2 ) {
		erase(it++);
	} 
} */

template <class T> inline void slist<T>::push_front(const T& data) {
	#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
	ListNode * newNode = _allocationPool->alloc();
	#else
	ListNode * newNode = new ListNode;
	#endif
	newNode->next = _head->next;	
	newNode->data = data;

	_head->next = newNode;
	if (newNode->next == NULL)
		_tail = newNode;
	
	++_size;
}

template <class T> inline void slist<T>::push_back(const T& data) {
	#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
	ListNode * newNode = _allocationPool->alloc();
	#else
	ListNode * newNode = new ListNode;
	#endif
	newNode->next = NULL;
	newNode->data = data;		

	if (_tail == NULL)
		_head->next = newNode;
	else
		_tail->next = newNode;
	_tail = newNode;

	++_size;
}

template <class T> inline void slist<T>::pop_front(void) {
	ListNode * dnode = _head->next;
	_head->next = _head->next->next;
	#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
	_allocationPool->dealloc(dnode);
	#else
	delete dnode;
	#endif
	--_size;
	// Bug correction 2011/05/17
	if (_size == 0)
		_tail = _head;
	// Bug correction 2011/05/17
}

template <class T> inline void slist<T>::clear(void) {
	ListNode * cnode = _head->next;
	ListNode * tmp;
	while (cnode != NULL) {
		tmp = cnode->next;
		#ifdef USE_POOL_ALLOCATIONSCHEME_SLIST
		_allocationPool->dealloc(cnode);
		#else
		delete cnode;//free(cnode);
		#endif
		cnode = tmp;			  
	}
	_head->next = NULL;
	_tail = _head;
	_size = 0;
}

template <class T> inline void slist<T>::print(void) {
  ListNode * cnode = _head->next;
  while (cnode != NULL) {
	  std::cout << cnode->data << " ";
	  cnode = cnode->next;			  
  }
}

#endif // ARTELYSSINGLELINKEDLIST_H


