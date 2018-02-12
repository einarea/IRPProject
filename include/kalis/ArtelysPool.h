// ********************************************************************
// * Artelys Utilities                                                *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : ArtelysPool.h                                             *
// * Desc : implement a generic pool allocator                        *
// ********************************************************************


#ifndef ARTELYSPOOL_H
#define ARTELYSPOOL_H

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <ArtelysVector.h>

namespace Artelys {
		
/**
 * 
 * 
 * Blocks of pool items are allocated in chunks, to decrease allocation
 * overhead.  Chunks are simply arrays with a pointer to the next chunk
 * (used for freeing a pool).
 */
template<class T> class chunk {
  public:
	chunk<T> *  nextp;
	T *			items;
	
	chunk(unsigned int n) { 
		items = new T[n]; 
	}
	~chunk() { 
		delete[] items; 
	} 
};	

/** @doc

  	pool<data_type>
	
	Description :
	
	Pooled Memory Allocation :

	Most large C++ programs make use of dynamically allocated memory. 
	This is especially true of applications where the designer can never 
	safely set an upper limit on application size. In many cases large amounts 
	of dynamically allocated memory is consumed by interconnected objects which 
	are not themselves very large. The time consumed allocating objects can be minimized, 
	but is unavoidable. A significant amount of processing time can also be consumed 
	traversing the dynamic data structures and returning them to the system using the 
	default C++ delete function. Time consuming memory recovery can be avoided by using 
	a pool based memory allocator. 

	A pool based memory allocator allocates large blocks of memory and then allocates smaller 
	objects from these blocks. When it is time to recover memory, the entire pool is deallocated 
	at once. This usually involves returning only a few large memory blocks to the system. 
	This greatly reduces the time consumed by memory recovery. 

    @version 2004.0

*/	
template<class T> class pool {
	
  private:

	chunk<T> *  chunkp;
	unsigned int chunk_size;
	vector<T *>  free_list;
	bool is_growable;
	unsigned int pool_size;
	unsigned int allocated_count;
	unsigned int _refCount;

  protected:
	
 
	
	void alloc_chunk(void) {
		chunk<T> *  new_chunkp = new chunk<T>(chunk_size);
		pool_size += chunk_size;
		unsigned int i;
		for(i=0; i<chunk_size; i++ )
			free_list.push_back( &(new_chunkp->items[i]) );
		new_chunkp->nextp = chunkp;
		chunkp = new_chunkp;
	}		
	
  public: 

	pool() { 
		int nnn = 4096;
		free_list.reserve(nnn*4);
		chunk_size = nnn;
		is_growable = true;
		pool_size = 0;
		allocated_count = 0;
		chunkp = NULL;
		unsigned int i;
		for(i=0; i<4; i++ )
			alloc_chunk();
		_refCount = 1;
	}
	/** default constructor
	*	@param chunkSize the size of
	*	@param numberOfChunks the number if chunks to allocate
	*	@param grow chunklist resizable ?
	*/
	pool(unsigned int chunkSize, unsigned int nbChunks, bool grow = true) { 
		free_list.reserve(chunkSize*nbChunks);
		chunk_size = chunkSize;
		is_growable = grow;
		pool_size = 0;
		allocated_count = 0;
		chunkp = NULL;
		unsigned int i;
		for(i=0; i<nbChunks; i++ )
			alloc_chunk();
		_refCount = 1;
	}
	
	/** destructor
	*/
	~pool() {
		while( chunkp != NULL )
		{
			chunk<T> *  oldp = chunkp;
			chunkp = chunkp->nextp;
			delete oldp;
		}
	}
	/** allocate and return a pointer of type T* (equivalent to new T();
	*	@return a pointer on a allocated instance of type T
	*/
	inline T * alloc(void) {
		if ( free_list.size() == 0 )
		{
			if ( !is_growable ) {				 
				printf("Warning pool allocation error!\n");
				return NULL;		// NB : should throw an allocation exception
			}			
			alloc_chunk();
		}
		allocated_count++;					
		T *element=free_list.back();		
		free_list.pop_back();
		return element;
	}
	/** desallocate and the pointer ptr previously allocated by alloc (equivalent to delete ptr);
	*/
	inline void dealloc(T *  ptr) {		
		allocated_count--;
		free_list.push_back(ptr);
	}
	/** reset the pool allocator
	*/
	inline void reset(void) {
		free_list.reset();
		chunk<T> *  cp = chunkp;
		while( cp != NULL )
		{
			unsigned int i;
			for(i=0; i<chunk_size; i++ )
				free_list.add(&cp->items[i]);
		}
		allocated_count = 0;
	}
	/** @return the number if allocated objects of type T
	*/
	inline int get_allocated_count(void) {
		return allocated_count;
	}

	inline void incRefCount(void) {
		++_refCount;
	}
	inline unsigned int getRefCount(void) {
		return _refCount;
	}
};
	
}

#endif

