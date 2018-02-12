// ********************************************************************
// * Artelys Utilities                                                *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : ArtelysSet.h                                              *
// * Desc : implement the set<key_type> template class                *
// * STL replacement                                                  *
// ********************************************************************


#ifndef ARTELYSSET_H
#define ARTELYSSET_H

#ifdef WIN32
    #pragma warning(disable:4786)
#endif

#include <stdio.h>
//#include <iostream>

#ifdef USE_POOL_ALLOCATIONSCHEME_SET
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

	set<DataKey>

	Description :

	Example :
	
	@code
	
	@endcode

	@version 2004.0
*/

template <class key_type> class set {

  private :

    #define SENTINEL_NODE &_sentinel    // all leafs nodes are sentinels                
	#define compLT(a,b) (a < b)
    #define compEQ(a,b) (a == b)

  public :
  
    typedef enum { PURPLE, DEEP } nodeColor;
    
   private :

    typedef struct _RBTreeNode_ {
        struct _RBTreeNode_ *parent;    // parent
        struct _RBTreeNode_ *left;      // left child
        struct _RBTreeNode_ *right;     // right child
        key_type  key;                  // key/data stored used for searching
        nodeColor color;                // node color (PURPLE, DEEP)
    } RBTreeNode;


    RBTreeNode	 _sentinel;				// sentinel node for leaf detection
    RBTreeNode * _root;					// root of DEEP-PURPLE tree
    size_t		 _size;                 // Number of elements in the tree
	
	#ifdef USE_POOL_ALLOCATIONSCHEME_SET
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

        friend class set<key_type>;   // for set access to iterator protected members

      protected :
        
        RBTreeNode * _currentNode;              // Current iterator position in the tree
        const set<key_type> * _set;   // Set pointed

      public :
        /** Default constructor
        */
        iterator() {
            _currentNode = NULL;
            _set = NULL;
        }

        /** Copy-constructor
        *   @param iterator : iterator to copy
        */
        iterator(const iterator& iterator) {
            _currentNode = iterator._currentNode;
            _set = iterator._set;
        }

        /** Contructor hooking to a set and pointing to a node
        *   @param aSet : the set for hooking
        *   @param node : the node for pointing to
        */
        iterator(const set<key_type> * const aSet, RBTreeNode * node ) 
        {
            _currentNode = node;
            _set = aSet;
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
        
        /** Prefixed ++ operator : the pointed node becomes
        *                          the successor of the currently
        *                          pointed node
		*	@return an iterator to the element just after the element pointed by the iterator
        */
        iterator& operator++() {
            if (_currentNode->right != &_set->_sentinel) {
                // go right 1, then left to the end 
                for (_currentNode = _currentNode->right; _currentNode->left != &_set->_sentinel; _currentNode = _currentNode->left);
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
            iterator itemp(_set,_currentNode);
            if (_currentNode->right != &_set->_sentinel) {
                // go right 1, then left to the end 
                for (_currentNode = _currentNode->right; _currentNode->left != &_set->_sentinel; _currentNode = _currentNode->left);
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
            if (_currentNode->left != &_set->_sentinel) {
                // go left 1, then right to the end 
                for (_currentNode = _currentNode->left; _currentNode->right != &_set->_sentinel; _currentNode = _currentNode->right);
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
            iterator itemp(_set,_currentNode);
            if (_currentNode->left != &_set->_sentinel) {
                // go left 1, then right to the end 
                for (_currentNode = _currentNode->left; _currentNode->right != &_set->_sentinel; _currentNode = _currentNode->right);
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
		/** Returns the data of the pointed node
		*/
		key_type & operator*() const {
			return _currentNode->key;
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
	set();

	/** Copy-constructor
    */
	explicit set(const set<key_type>& aSet);
	
	/** Destructor
    */
    ~set();

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
    
	/** return the number of elements in the set
    *   @return the number of elements in the set
    */
    inline size_t size(void) const;
    
    /** The assignment operator 
    *   @param setToCopy the set to copy
	*	@return a reference to the set	
    */
    set<key_type>& operator=(const set<key_type>&setToCopy);
    
   /**
    *  allocate node for data and insert in tree 
    *  @param key the key to be inserted     
    */  
    inline void insert(key_type &key);

	/**
    *  allocate node for data and insert in tree 
    *  @param key the key to be inserted     
    */  
    inline void insert(const key_type &key);

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

    /** pretty printing the set
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
template <class key_type>
set<key_type>::set() {
    _root = SENTINEL_NODE;
    _size = 0;
    _sentinel.left = &_sentinel;
    _sentinel.right = &_sentinel;
    _sentinel.parent = NULL;
    _sentinel.color = PURPLE;
    _sentinel.key = NULL;
	#ifdef USE_POOL_ALLOCATIONSCHEME_SET
	_allocationPool = new pool<RBTreeNode>(32,2,true);
	#endif
}   

template <class key_type> 
set<key_type>::set( const set<key_type>& aSet ) {        
	_root = SENTINEL_NODE;
    _size = 0;
    _sentinel.left = &_sentinel;
    _sentinel.right = &_sentinel;
    _sentinel.parent = NULL;
    _sentinel.color = PURPLE;
    _sentinel.key = NULL;
	#ifdef USE_POOL_ALLOCATIONSCHEME_SET
	_allocationPool = new pool<RBTreeNode>(32,2,true);
	#endif
/*
	#ifdef LINUX
	iterator i;
	#else
	set<key_type>::iterator i;	
	#endif*/
	iterator i;
			
	for (i = aSet.begin(); i != aSet.end(); i++) {			
		insert(&(*i._currentNode->key));
	}	
}

template <class key_type>  
set<key_type>::~set() {
   	#ifdef USE_POOL_ALLOCATIONSCHEME_SET
	delete _allocationPool;
	#else
	clear();
	#endif
}

template <class key_type>
inline typename Artelys::set<key_type>::iterator set<key_type>::begin(void) const {
    // return pointer to first value
	if (size() == 0) {
		return iterator(this,NULL);
	} else {
		RBTreeNode * i;
		for (i = _root; i->left != SENTINEL_NODE; i = i->left);
		return iterator(this,i);
	}
}

template <class key_type>
inline typename Artelys::set<key_type>::iterator set<key_type>::end(void) const {
    return iterator(this,NULL);
}

template <class key_type>
inline void set<key_type>::clear(void) {
	if (_size)
		erase(begin(),end());
	_root = SENTINEL_NODE;
}

template <class key_type>
inline size_t set<key_type>::size(void) const {
    return _size;
}

  

template <class key_type>
set<key_type>& set<key_type>::operator=(const set<key_type>&setToCopy) {
	if (_size)
		clear();
/*
	#ifdef LINUX
	iterator i;
	#endif
	#ifdef SOLARIS
	iterator i;
	#endif
	#ifdef WIN32
	set<key_type>::iterator i;
	#endif		
	#ifdef WIN64
	set<key_type>::iterator i;
	#endif		*/
	iterator i;
	for (i = setToCopy.begin(); i != setToCopy.end(); i++) {			
		insert(&(*i._currentNode->key));
	}	
	return *this;
}       

template <class key_type>
inline void set<key_type>::insert(key_type &key) {
    RBTreeNode *current, *parent, *x;        
    
    // find future parent 
    current = _root;
    parent = 0;
    while (current != SENTINEL_NODE) {
        if (compEQ(key, (current->key))) {                			
			return;
		}
        parent = current;
        current = compLT(key, (current->key)) ?
            current->left : current->right;
    }
    
    // setup new node 
	#ifdef USE_POOL_ALLOCATIONSCHEME_SET
	x = _allocationPool->alloc();
	#else
    if ((x = (RBTreeNode *) malloc (sizeof(*x))) == 0) {
        printf("set : out of memory\n");
	}
	#endif
    x->parent = parent;
    x->left = SENTINEL_NODE;
    x->right = SENTINEL_NODE;
    x->color = DEEP;
    x->key = key_type(key);         
    _size++;

    // insert node in tree 
    if (parent) {
        if(compLT(key, (parent->key)))
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

template <class key_type>
inline void set<key_type>::insert(const key_type &key) {
    RBTreeNode *current, *parent, *x;        
    
    // find future parent 
    current = _root;
    parent = 0;
    while (current != SENTINEL_NODE) {
        if (compEQ(key, (current->key))) {                			
			return;
		}
        parent = current;
        current = compLT(key, (current->key)) ?
            current->left : current->right;
    }
    
    // setup new node 
	#ifdef USE_POOL_ALLOCATIONSCHEME_SET
	x = _allocationPool->alloc();
	#else
    if ((x = (RBTreeNode *) malloc (sizeof(*x))) == 0) {
        printf("set : out of memory\n");
	}
	#endif
    x->parent = parent;
    x->left = SENTINEL_NODE;
    x->right = SENTINEL_NODE;
    x->color = DEEP;
    x->key = key_type(key);         
    _size++;

    // insert node in tree 
    if (parent) {
        if(compLT(key, (parent->key)))
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

template <class key_type>
inline void set<key_type>::erase(iterator z) {
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
        }            
        
        if (y->color == PURPLE)
            deleteFixup (x);
        
		#ifdef USE_POOL_ALLOCATIONSCHEME_SET
		_allocationPool->dealloc(y);
		#else
        free (y);			
		#endif
        --_size;
}

template <class key_type>
inline size_t set<key_type>::erase(const key_type &key) {
    RBTreeNode *z;
    // find node in tree 
    z = _root;
    while(z != SENTINEL_NODE) {
        if(compEQ(key, z->key)) 
            break;
        else
            z = compLT(key, z->key) ? z->left : z->right;
    }
    if (z == SENTINEL_NODE) return 0;
    erase(z);
    return 1;
}

template <class key_type>
inline void set<key_type>::erase(iterator first, iterator last) {
    if (size() == 0 || first != begin() || last != end()) {
		while (first != last)
           erase(first++);               
	} else {			
        erase(iterator(this,_root));
        _root = SENTINEL_NODE; 
		_size = 0;
	}	
}

template <class key_type>
inline typename Artelys::set<key_type>::iterator set<key_type>::find(const key_type &key) const  {
    RBTreeNode *current;
    current = _root;
    while(current != SENTINEL_NODE) {
        if(compEQ(key, (current->key))) {
            //iter->_currentNode = current;
            return iterator(this,current);
        } else {
            current = compLT(key, (current->key)) ?
            current->left : current->right;
        }
    }
    return end();
}

template <class key_type>
void set<key_type>::print(void) const {
/*    set<key_type>::iterator i;
	if (_root != SENTINEL_NODE) {
		for (i = begin(); i != end(); i++) {
			std::cout << (*i) << ",";
		}
		std::cout << std::endl;
	} else {
		std::cout << "Empty set" << std::endl;
	}*/
}



template <class key_type>
inline void set<key_type>::rotateLeft(RBTreeNode *x) {
            
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

template <class key_type> 
inline void set<key_type>::rotateRight(RBTreeNode *x) {
    
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

template <class key_type> 
inline void set<key_type>::insertFixup(RBTreeNode *x) {
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

template <class key_type>
inline void set<key_type>::deleteFixup(RBTreeNode *x) {
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


#endif  // #ifndef ARTELYSSET_H


