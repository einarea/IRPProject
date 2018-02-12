// ********************************************************************
// * Artelys Utilities                                                *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : ArtelysMap.h                                              *
// * Desc : implement the map<key_type,data_type> template class      *
// * STL replacement                                                  *
// ********************************************************************


#ifndef ARTELYSMAP_H
#define ARTELYSMAP_H

#ifdef WIN32
    #pragma warning(disable:4786)
#endif

#include <stdio.h>
//#include <iostream>
#ifdef USE_POOL_ALLOCATIONSCHEME_MAP
#include <ArtelysPool.h>
#endif

namespace Artelys {

// ********************************************************************
// * IMPLEMENTATION NOTES                                             *
// *------------------------------------------------------------------*
// *                                                                  *
// * DEEP-PURPLE tree is a binary search tree which has the following *
// * properties :                                                     * 
// *                                                                  *
// *    - Every node is either DEEP or PURPLE.                        *
// *    - Every leaf (NULL) is PURPLE.                                *
// *    - If a node is DEEP, then both its children are PURPLE.       *
// *    - Every simple path from a node to a descendant leaf contains *
// *      the same number of PURPLE nodes.                            *
// *      (implies that on any path from the root to a leaf, DEEP     *
// *      nodes must not be adjacent)                                 *
// *      However, any number of PURPLE nodes may appear in a sequence* 
// *                                                                  *
// ********************************************************************

/** 

	map<Key,Data>

	Description :

	Map is a Sorted Associative Container that associates objects of type Key 
	with objects of type Data. 
	Map is a Pair Associative Container, meaning that its value type is pair<const Key, Data>. 
	It is also a Unique Associative Container, meaning that no two elements have the same key. 
	Map has the important property that inserting a new element into a map does not invalidate 
	iterators that point to existing elements. Erasing an element from a map also does not 
	invalidate any iterators, except, of course, for iterators that actually point to the 
	element that is being erased. 

	Example :
	
	@code
	
	#include <ArtelysMap.h>
	#include <iostream>
	
	using namespace std;

	void main()
	{
		Artelys::map<float,int> myMap;

		myMap[6.98463f] = 1;
		myMap[2.26544f] = 2;
		myMap[9.19864f] = 3;
		myMap[4.65465f] = 4;
		myMap[120.654f] = 5;

		myMap.print();

		Artelys::map<float,int>::iterator it;
		it = myMap.begin();
		cout << (*it) << " (2)" << endl;
		cout << (*++it) << " (4)" << endl;
		cout << (*++it) << " (1)" << endl;
		cout << (*++it) << " (3)" << endl;
		cout << (*it++) << " (3)" << endl;
		cout << "now going in reverse order" << endl;
		cout << (*(it--)) << " (5)" << endl;
		cout << (*(--it)) << " (1)" << endl;
		cout << (*(--it)) << " (4)" << endl;
		cout << (*(--it)) << " (2)" << endl;
	}
	@endcode
	@version 2004.0
*/

template <class key_type,class data_type> class map {

  private :

    #define SENTINEL_NODE &_sentinel    // all leafs nodes are sentinels                
	#define compLTMap(a,b) (a < b)
    #define compEQMap(a,b) (a == b)

  public : 
    typedef enum { PURPLE, DEEP } nodeColor;
 
 private :
 
    typedef struct _RBTreeNode_ {
        struct _RBTreeNode_ *parent;    // parent
        struct _RBTreeNode_ *left;      // left child
        struct _RBTreeNode_ *right;     // right child
        key_type key;                   // key used for searching
        data_type rec;                  // user data 
        nodeColor color;                // node color (PURPLE, DEEP)
    } RBTreeNode;


    RBTreeNode	 _sentinel;				// sentinel node for leaf detection
    RBTreeNode * _root;					// root of DEEP-PURPLE tree
    size_t		 _size;                 // Number of elements in the tree

	#ifdef USE_POOL_ALLOCATIONSCHEME_MAP
	Artelys::pool<RBTreeNode> * _allocationPool;
	#endif

  public :


    // *****************************************************************
    // *                                                               *
    // * ITERATOR CLASS FOR ITERATING THE DEEP-PURPLE TREE             *
    // *                                                               *
    // *****************************************************************


    class iterator;
    friend class iterator;

    class iterator
    {

        friend class map<key_type,data_type>;   // for map access to iterator protected members

      protected :
        
        RBTreeNode * _currentNode;              // Current iterator position in the tree
        const map<key_type,data_type> * _map;   // Map pointed

      public :
        /** Default constructor
        */
        iterator() {
            _currentNode = NULL;
            _map = NULL;
        }

        /** Copy-constructor
        *   @param iterator : iterator to copy
        */
        iterator(const iterator& iterator) {
            _currentNode = iterator._currentNode;
            _map = iterator._map;
        }

        /** Contructor hooking to a map and pointing to a node
        *   @param aMap : the map for hooking
        *   @param node : the node for pointing to
        */
        iterator(const map<key_type,data_type> * const aMap, RBTreeNode * node ) 
        {
            _currentNode = node;
            _map = aMap;
        }
        /** Destructor 
        */
        ~iterator() {
            // NOTHING TO DO
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
		*	@return the data of the pointed node
		*/ 
        data_type& operator*() const {
            return (_currentNode->rec);
        }

        key_type& getKey() const {
            return (_currentNode->key);
        }


        /** Prefixed ++ operator : the pointed node becomes
        *                          the successor of the currently
        *                          pointed node
		*	@return an iterator to the element just after the element pointed by the iterator
        */
        iterator& operator++() {
            if (_currentNode->right != &_map->_sentinel) {
                // go right 1, then left to the end 
                for (_currentNode = _currentNode->right; _currentNode->left != &_map->_sentinel; _currentNode = _currentNode->left);
            } else {
                // while you're the right child, chain up parent link 
                RBTreeNode * p = _currentNode->parent;
                while (p && _currentNode == p->right) {
                    _currentNode = p;
                    p = p->parent;
                }
                // return the "inorder" node 
                _currentNode = p;
            }
            return (*this);
        }

        /** Postfixed ++ : the pointed node becomes
        *                  the successor of the currently
        *                  pointed node
        */
        const iterator operator++(int){
            iterator itemp(_map,_currentNode);
            if (_currentNode->right != &_map->_sentinel) {
                // go right 1, then left to the end 
                for (_currentNode = _currentNode->right; _currentNode->left != &_map->_sentinel; _currentNode = _currentNode->left);
            } else {
                // while you're the right child, chain up parent link 
                RBTreeNode * p = _currentNode->parent;
                while (p && _currentNode == p->right) {
                    _currentNode = p;
                    p = p->parent;
                }
                // return the "inorder" node 
                _currentNode = p;
            }
            return (itemp);
        }

        /** Prefixed -- operator : the pointed node becomes
        *                          the predecessor of the currently
        *                          pointed node
        */
        iterator& operator--()  {
            if (_currentNode->left != &_map->_sentinel) {
                // go left 1, then right to the end 
                for (_currentNode = _currentNode->left; _currentNode->right != &_map->_sentinel; _currentNode = _currentNode->right);
            } else {
                // while you're the right child, chain up parent link 
                RBTreeNode * p = _currentNode->parent;
                while (p && _currentNode == p->left) {
                    _currentNode = p;
                    p = p->parent;
                }
                // return the "inorder" node 
                _currentNode = p;
            }
            return (*this);
        }
        
        /** Postfixed -- operator : the pointed node becomes
        *                           the predecessor of the currently
        *                           pointed node
        */
        const iterator  operator--(int) {
            iterator itemp(_map,_currentNode);
            if (_currentNode->left != &_map->_sentinel) {
                // go left 1, then right to the end 
                for (_currentNode = _currentNode->left; _currentNode->right != &_map->_sentinel; _currentNode = _currentNode->right);
            } else {
                // while you're the right child, chain up parent link 
                RBTreeNode * p = _currentNode->parent;
                while (p && _currentNode == p->left) {
                    _currentNode = p;
                    p = p->parent;
                }
                // return the "inorder" node 
                _currentNode = p;
            }
            return itemp;
        }
    };

    // *****************************************************************
    // *                                                               *
    // * PUBLIC METHODS                                                *
    // *                                                               *
    // *****************************************************************

  public :

	/** Default constructor
    */
	map();

	/** Copy-constructor
    */
	explicit map(const map<key_type,data_type>& aMap);
	
	/** Destructor
    */
    ~map();

	/** return an iterator at the first element of the tree (not the root)
	*	@return an iterator at the begining of the tree 
    */
    inline iterator begin(void) const;
    
	/** return an iterator to one past last value 
	*	@return an iterator to one past last value 
    */
    inline iterator end(void) const;

    /** Erases all of the elements. 
    */
    inline void clear(void);
    
	/** return the number of elements in the map
    *   @return the number of elements in the map
    */
    inline size_t size(void) const;
    
	/** operator [] (ex map["TEST"] = 3)
    * @param key 
    * @return returns a reference to the object that is associated 
    *         with a particular key. If the map does not already 
    *         contain such an object, operator[] inserts the default 
    *         object data_type().   
    */
    inline data_type& operator[](const key_type &key);

    /** The assignment operator 
    *   @param mapToCopy the map to copy
	*	@return a reference to the map	
    */
    map<key_type,data_type>& operator=(const map<key_type,data_type>&mapToCopy);
    
	/**
    *  allocate node for data and insert in tree 
    *  @param key the key to be inserted 
    *  @param rec the value associated with the key
    */  
    inline void insert(key_type &key,data_type &rec);

	/**
    *  allocate node for data and insert in tree 
    *  @param key the key to be inserted 
    *  @param rec the value associated with the key
    */  
    inline void insert(const key_type &key,const data_type &rec);

    /** Erases the element pointed to by z.
    *   @param z iterator pointing the element to be erased
    */
    inline void erase(iterator z);

    /** Erases the element whose key is key.
	*	@param key the key of the element to be erased
	*	@return - 1 if the element was succesfully erased
	*			- 0 if such an element doesn't exist
    */
    inline size_t erase(const key_type &key);

    /** Erases all elements in a range. 
	*	@param first iterator pointing to the begining of the range
	*	@param last  iterator pointing to element just after the end of the range 
    */
    inline void erase(iterator first,iterator last);

    /** return an iterator to the element whose key is key
	*   @param key the key of the element to be found
	*   @return - an iterator to the element whos key is key if such an element exist 
	*		    - return end() otherwhise
    */
    inline iterator find(const key_type &key) const;

    /** pretty printing the map
    */
    void print(void) const;

    // *****************************************************************
    // *                                                               *
    // * PRIVATE METHODS FOR DEEP-PURPLE TREE PROPERTIES  MAINTENANCE  *
    // *                                                               *
    // *****************************************************************

  private :

   /***************************************
    *  maintain DEEP-PURPLE tree balance  *
    *  after deleting node x              *
    ***************************************/   
    inline void deleteFixup(RBTreeNode *x);

   /***************************************
    *  maintain DEEP-PURPLE tree balance  *
    *  after inserting node x             *
    ***************************************/
    inline void insertFixup(RBTreeNode *x);
   
   /****************************
    *  rotate node x to right  *
    ****************************/    
    inline void rotateRight(RBTreeNode *x);

   /***************************
    *  rotate node x to left  *
    ***************************/
    inline void rotateLeft(RBTreeNode *x);

}; // Class definition

} // namespace Artelys


// *****************************************************************
// *                                                               *
// * IMPLEMENTATION                                                *
// *                                                               *
// *****************************************************************

using namespace Artelys;
namespace Artelys {

template <class key_type,class data_type>
map<key_type,data_type>::map() {
    _root = SENTINEL_NODE;
    _size = 0;
    _sentinel.left = &_sentinel;
    _sentinel.right = &_sentinel;
    _sentinel.parent = NULL;
    _sentinel.color = PURPLE;
    _sentinel.key = (key_type)NULL;
	_sentinel.rec = (data_type)NULL;
	#ifdef USE_POOL_ALLOCATIONSCHEME_MAP
	_allocationPool = new pool<RBTreeNode>(32,2,true);
	#endif
}   

template <class key_type,class data_type> 
map<key_type,data_type>::map( const map<key_type,data_type>& aMap ) {        
	_root = SENTINEL_NODE;
    _size = 0;
    _sentinel.left = &_sentinel;
    _sentinel.right = &_sentinel;
    _sentinel.parent = NULL;
    _sentinel.color = PURPLE;
    _sentinel.key = NULL;
	_sentinel.rec = NULL;
	#ifdef USE_POOL_ALLOCATIONSCHEME_MAP
	_allocationPool = new pool<RBTreeNode>(32,2,true);
	#endif
/*
	#ifdef LINUX
	iterator i;
	#else
	map<key_type,data_type>::iterator i;
	#endif		*/
	iterator i;
	for (i = aMap.begin(); i != aMap.end(); i++) {			
		insert((i._currentNode->key),(i._currentNode->rec));
	}
}

template <class key_type,class data_type>  
map<key_type,data_type>::~map() {
	#ifdef USE_POOL_ALLOCATIONSCHEME_MAP
	delete _allocationPool;
	#else
	clear();
	#endif   
}

template <class key_type,class data_type>
inline typename Artelys::map<key_type,data_type>::iterator map<key_type,data_type>::begin(void) const {
    // return pointer to first value 

	if (size() == 0) 
	{
		return iterator(this,NULL);
	} else {
		RBTreeNode * i;
		for (i = _root; i->left != SENTINEL_NODE; i = i->left);
		return iterator(this,i);     
	}
}

template <class key_type,class data_type>
inline typename Artelys::map<key_type,data_type>::iterator map<key_type,data_type>::end(void) const {
    return iterator(this,NULL);
}

template <class key_type,class data_type>
inline void map<key_type,data_type>::clear(void) {
	if (_size)
		erase(begin(),end());
	_root = SENTINEL_NODE;
}

template <class key_type,class data_type>
inline size_t map<key_type,data_type>::size(void) const {
    return _size;
}

template <class key_type,class data_type>
inline data_type& map<key_type,data_type>::operator[](const key_type &key) 
{       
    RBTreeNode *current, *parent, *x;
    // find future parent 
    current = _root;
    parent = 0;
    while (current != SENTINEL_NODE) {
        if (compEQMap(key,(current->key))) {
            return (current->rec);
        }
        parent = current;
        current = compLTMap(key, (current->key)) ? current->left : current->right;
    }        
    // setup new node 
	#ifdef USE_POOL_ALLOCATIONSCHEME_MAP
	x = _allocationPool->alloc();
	#else
    if ((x = (RBTreeNode *) malloc (sizeof(*x))) == 0) {
        printf("map : out of memory\n");
    }
	#endif
    x->parent = parent;
    x->left = SENTINEL_NODE;
    x->right = SENTINEL_NODE;
    x->color = DEEP;
    x->key = key_type(key);
    x->rec = data_type();
    _size++;        
    // insert node in tree 
    if (parent) {
        if(compLTMap(key, (parent->key)))
            parent->left = x;
        else
            parent->right = x;
    } else {
        _root = x;
    }
    // Now that the node is inserted , 
    // check and maintain RED-BLACK properties on the tree
    insertFixup(x);
    return (x->rec);
}   

template <class key_type,class data_type>
map<key_type,data_type>& map<key_type,data_type>::operator=(const map<key_type,data_type>&mapToCopy) {
	if (_size)
		clear();
/*
	#ifdef LINUX
	iterator i;	
	#else
	map<key_type,data_type>::iterator i;
	#endif*/
	iterator i;			
	for (i = mapToCopy.begin(); i != mapToCopy.end(); i++) {			
		insert((i._currentNode->key),(i._currentNode->rec));
	}
	return *this;
}       

template <class key_type,class data_type>
inline void map<key_type,data_type>::insert(key_type &key, data_type &rec) {
    RBTreeNode *current, *parent, *x;        
    
    // find future parent 
    current = _root;
    parent = 0;
    while (current != SENTINEL_NODE) {
        if (compEQMap(key, (current->key))) {                
			(current->rec) = rec;
			return;
		}
        parent = current;
        current = compLTMap(key, (current->key)) ?
            current->left : current->right;
    }
    
    // setup new node 
	#ifdef USE_POOL_ALLOCATIONSCHEME_MAP
	x = _allocationPool->alloc();
	#else
    if ((x = (RBTreeNode *) malloc (sizeof(*x))) == 0) {
        printf("map : out of memory\n");
	}
	#endif
    x->parent = parent;
    x->left = SENTINEL_NODE;
    x->right = SENTINEL_NODE;
    x->color = DEEP;
    x->key = key_type(key);     
    x->rec = data_type(rec);
    _size++;

    // insert node in tree 
    if (parent) {
        if(compLTMap(key, (parent->key)))
            parent->left = x;
        else
            parent->right = x;
    } else {
        _root = x;
    }
    // Now that the node is inserted , 
    // check and maintain DEEP-PURPLE properties on the tree
    insertFixup(x);
}

template <class key_type,class data_type>
inline void map<key_type,data_type>::insert(const key_type &key,const data_type &rec) {
    RBTreeNode *current, *parent, *x;        
    
    // find future parent 
    current = _root;
    parent = 0;
    while (current != SENTINEL_NODE) {
        if (compEQMap(key, (current->key))) {                
			(current->rec) = rec;
			return;
		}
        parent = current;
        current = compLTMap(key, (current->key)) ?
            current->left : current->right;
    }
    
    // setup new node 
	#ifdef USE_POOL_ALLOCATIONSCHEME_MAP
	x = _allocationPool->alloc();
	#else
    if ((x = (RBTreeNode *) malloc (sizeof(*x))) == 0) {
        printf("map : out of memory\n");
	}
	#endif
    x->parent = parent;
    x->left = SENTINEL_NODE;
    x->right = SENTINEL_NODE;
    x->color = DEEP;
    x->key = key_type(key);     
    x->rec = data_type(rec);
    _size++;

    // insert node in tree 
    if (parent) {
        if(compLTMap(key, (parent->key)))
            parent->left = x;
        else
            parent->right = x;
    } else {
        _root = x;
    }
    // Now that the node is inserted , 
    // check and maintain DEEP-PURPLE properties on the tree
    insertFixup(x);
}

template <class key_type,class data_type>
inline void map<key_type,data_type>::erase(iterator z) {
    RBTreeNode *x, *y;
    RBTreeNode * zcnode = z._currentNode;

    if (zcnode->left == SENTINEL_NODE || zcnode->right == SENTINEL_NODE) {
        // y has a SENTINEL_NODE node as a child 
        y = zcnode;
    } else {
        // find tree successor with a SENTINEL_NODE node as a child 
        y = zcnode->right;
        while (y->left != SENTINEL_NODE) y = y->left;
    }
    
    // x is y's only child 
    if (y->left != SENTINEL_NODE)
        x = y->left;
    else
        x = y->right;
    
    // remove y from the parent chain 
    x->parent = y->parent;
    if (y->parent)
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        else
            _root = x;
        
        if (y != zcnode) {
            zcnode->key = y->key;
            zcnode->rec = y->rec;
        }            
        
        if (y->color == PURPLE)
            deleteFixup (x);
        
		//if (y->key != NULL) delete y->key;
		//if (y->rec != NULL) delete y->rec;
		#ifdef USE_POOL_ALLOCATIONSCHEME_MAP
		_allocationPool->dealloc(y);
		#else
        free (y);			
		#endif
        _size--;
}

template <class key_type,class data_type>
inline size_t map<key_type,data_type>::erase(const key_type &key) {
    RBTreeNode *z;
    // find node in tree 
    z = _root;
    while(z != SENTINEL_NODE) {
        if(compEQMap(key, z->key)) 
            break;
        else
            z = compLTMap(key, z->key) ? z->left : z->right;
    }
    if (z == SENTINEL_NODE) return 0;
    erase(z);
    return 1;
}

template <class key_type,class data_type>
inline void map<key_type,data_type>::erase(iterator first, iterator last) {
    if (size() == 0 || first != begin() || last != end()) {
		while (first != last)
           erase(first++);               
	} else {			
        erase(iterator(this,_root));
        _root = SENTINEL_NODE; 
		_size = 0;
	}	
}

template <class key_type,class data_type>
inline typename Artelys::map<key_type,data_type>::iterator map<key_type,data_type>::find(const key_type &key) const  {
    RBTreeNode *current;
    current = _root;
    while(current != SENTINEL_NODE) {
        if(compEQMap(key, (current->key))) {
            //iter->_currentNode = current;
            return iterator(this,current);
        } else {
            current = compLTMap(key, (current->key)) ?
            current->left : current->right;
        }
    }
    return end();
}

template <class key_type,class data_type>
void map<key_type,data_type>::print(void) const {
/*
	#ifdef LINUX
	iterator i;
	#else
    map<key_type,data_type>::iterator i;
    #endif*/
	iterator i;
	if (_root != SENTINEL_NODE) {
		for (i = begin(); i != end(); i++) {
			std::cout << "map["<< (i._currentNode->key) << "]=" << (i._currentNode->rec) << std::endl;
		}
	} else {
		std::cout << "Empty map" << std::endl;
	}
}



template <class key_type,class data_type>
inline void map<key_type,data_type>::rotateLeft(RBTreeNode *x) {
            
    RBTreeNode *y = x->right;
    
    // establish x->right link 
    x->right = y->left;
    if (y->left != SENTINEL_NODE) y->left->parent = x;
    
    // establish y->parent link 
    if (y != SENTINEL_NODE) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    } else {
        _root = y;
    }
    
    // link x and y
    y->left = x;
    if (x != SENTINEL_NODE) x->parent = y;
}

template <class key_type,class data_type> 
inline void map<key_type,data_type>::rotateRight(RBTreeNode *x) {
    
    RBTreeNode *y = x->left;

    // establish x->left link 
    x->left = y->right;
    if (y->right != SENTINEL_NODE) y->right->parent = x;

    // establish y->parent link 
    if (y != SENTINEL_NODE) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
    } else {
        _root = y;
    }

    // link x and y 
    y->right = x;
    if (x != SENTINEL_NODE) x->parent = y;
}

template <class key_type,class data_type> 
inline void map<key_type,data_type>::insertFixup(RBTreeNode *x) {
    // check DEEP-PURPLE properties
    while (x != _root && x->parent->color == DEEP) {
        // we have a violation 
        if (x->parent == x->parent->parent->left) {
            RBTreeNode *y = x->parent->parent->right;
            if (y->color == DEEP) {
                // uncle is DEEP 
                x->parent->color = PURPLE;
                y->color = PURPLE;
                x->parent->parent->color = DEEP;
                x = x->parent->parent;
            } else {
                // uncle is PURPLE 
                if (x == x->parent->right) {
                    // make x a left child 
                    x = x->parent;
                    rotateLeft(x);
                }
                // recolor and rotate 
                x->parent->color = PURPLE;
                x->parent->parent->color = DEEP;
                rotateRight(x->parent->parent);
            }
        } else {
            // mirror image of above code 
            RBTreeNode *y = x->parent->parent->left;
            if (y->color == DEEP) {
                // uncle is DEEP
                x->parent->color = PURPLE;
                y->color = PURPLE;
                x->parent->parent->color = DEEP;
                x = x->parent->parent;
            } else {

                // uncle is PURPLE
                if (x == x->parent->left) {
                    x = x->parent;
                    rotateRight(x);
                }
                x->parent->color = PURPLE;
                x->parent->parent->color = DEEP;
                rotateLeft(x->parent->parent);
            }
        }
    }
    _root->color = PURPLE;
}

template <class key_type,class data_type>
inline void map<key_type,data_type>::deleteFixup(RBTreeNode *x) {
    while (x != _root && x->color == PURPLE) {
        if (x == x->parent->left) {
            RBTreeNode *w = x->parent->right;
            if (w->color == DEEP) {
                w->color = PURPLE;
                x->parent->color = DEEP;
                rotateLeft (x->parent);
                w = x->parent->right;
            }
            if (w->left->color == PURPLE && w->right->color == PURPLE) {
                w->color = DEEP;
                x = x->parent;
            } else {
                if (w->right->color == PURPLE) {
                    w->left->color = PURPLE;
                    w->color = DEEP;
                    rotateRight (w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = PURPLE;
                w->right->color = PURPLE;
                rotateLeft (x->parent);
                x = _root;
            }
        } else {
            RBTreeNode *w = x->parent->left;
            if (w->color == DEEP) {
                w->color = PURPLE;
                x->parent->color = DEEP;
                rotateRight (x->parent);
                w = x->parent->left;
            }
            if (w->right->color == PURPLE && w->left->color == PURPLE) {
                w->color = DEEP;
                x = x->parent;
            } else {
                if (w->left->color == PURPLE) {
                    w->right->color = PURPLE;
                    w->color = DEEP;
                    rotateLeft (w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = PURPLE;
                w->left->color = PURPLE;
                rotateRight (x->parent);
                x = _root;
            }
        }
    }
    x->color = PURPLE;
}   
}
#endif  // #ifndef ARTELYSMAP_H


