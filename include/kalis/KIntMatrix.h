// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KIntMatrix.h                                              *
// * Description : Matrix of integers                                 *
// ********************************************************************

#ifndef __KINTMATRIX_H
#define __KINTMATRIX_H

#include "Globals.h"
#include "ArtelysMatrix.h"

EXTTEMPL template class DLLIMPORTEXPORT ArtelysMatrix<int>;

/**	 
 * This class implements an matrix of integers
 * @code
 *   
 *	KProblem p(...);
 *	// mat is a matrix of integer 
 *	// mat[0][0] mat[1][0]
 *	// mat[0][1] mat[1][1]
 *	// mat[0][2] mat[1][2] 
 *	// with domain [0..10]
 *	KIntMatrix  mat(p,2,3,0,10,"mat");
 	
 * @endcode	
 *  @version 2013.1
 */
class DLLIMPORTEXPORT KIntMatrix : public ArtelysMatrix<int> {
 public:

  /** 
   * Constructor
   * @param problem the problem
   * @param N width of the matrix
   * @param M length of the matrix
   * @param lowerBound Lower bound for variables in the matrix
   * @param upperBound Upper bound for variables in the matrix
   * @param name name of the matrix
  */
  KIntMatrix(int N,int M,int value, char * name = 0);  
  /**
   * Copy constructor
   */
  KIntMatrix(const KIntMatrix &matrixToCopy); 
  /**
   * Destructor
   */
  ~KIntMatrix();  
  /**
   * Get a pointer to a copy of this object
   */
  virtual ArtelysMatrix<int>* getCopyPtr() const;  
  /** 
   * Pretty printing of the matrix
   */
  void print(void);
  

}; 


#endif


