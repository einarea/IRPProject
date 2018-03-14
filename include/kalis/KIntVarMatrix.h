// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KIntVarMatrix.h                                           *
// * Description : Matrix of integer variable                         *
// ********************************************************************

#ifndef __KINTVARMATRIX_H
#define __KINTVARMATRIX_H

#include "Globals.h"

#undef _N
#undef _M
#undef _matrix

/**	 
  * This class implements an matrix of KIntVar
  @code
    Example :
   
	KProblem p(...);
	// mat is a matrix of KIntVar 
	// mat[0][0] mat[1][0]
	// mat[0][1] mat[1][1]
	// mat[0][2] mat[1][2] 
	// with domain [0..10]

	KIntVarMatrix  mat(p,2,3,0,10,"mat");
    
	
  @endcode	
  *	@see  KIntArray  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KIntVarMatrix  {

 KIntVar *** _matrix;
 int _N;
 int _M;

 public:
  /** 
	default constructor
	@param problem the problem
	@param N width of the matrix
	@param M length of the matrix
	@param lowerBound Lower bound for variables in the matrix
	@param upperBound Upper bound for variables in the matrix
	@param name name of the matrix
  */
  KIntVarMatrix(KProblem& problem,int N,int M,int lowerBound, int upperBound, char * name = 0);  
  ~KIntVarMatrix();
  /// return a pointer to the KIntVar at position (n,m) in the matrix
  KIntVar * getPtr(int n,int m);
  /// return the KIntVar at position (n,m) in the matrix
  KIntVar & getElt(int n,int m);
  /// put all the variables with row index m into the "row" KIntVarArray
  KIntVarArray & getRow(int m,KIntVarArray & row);
  /// put all the variables with column index m into the "col" KIntVarArray
  KIntVarArray & getCol(int n,KIntVarArray & col);
  /// put all the variables in the matrix into the "all" KIntVarArray 
  KIntVarArray & getAll(int n,KIntVarArray & all);
  /// pretty printing of the matrix
  void print(void);

}; 



#endif


