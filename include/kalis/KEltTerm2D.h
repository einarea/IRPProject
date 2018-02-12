// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KEltTerm2D.h                                              *
// * Description : This class implements expressions like l[I][J] or  *
// * l[I + c][J + d] for a list of integers l, two integers c and d,  *
// * andtwo  KIntVar I and J.                                         *
// * Such expressions are used for stating KElement2D constraints.    *
// ********************************************************************

#ifndef __KELTERM2D_H
#define __KELTERM2D_H

#include "Globals.h"
#include "KTerm.h"

/**	 
 * 
 * This class represent an expression of type Tab[I+a][J+b] where Tab is an array of integer value; I,J are the indexing variable and a and b indexing offsets
 * @code
 *  Example :
 *
 *  KProblem p(...);
 *  KIntVar X(...);
 *	KIntVar I(...);
 *	KIntVar J(...);
 *	KIntArray valuesArray(...);
 *	
 *	KEltTerm2D eltTerm(valuesArray,I,J);
 *			
 *	// posting the constraint X can take its values indexed by the I variable in the valuesArray
 *	p.post(X == eltTerm);
 *	// equivalent to 
 *	p.post(X == valuesArray[I]);
 *
 * @endcode	
 * @see KConstraint
 * @see KElement
 *  @version 2013.1
 */
class DLLIMPORTEXPORT KEltTerm2D : public KTerm {

 private:
  
  // indexed array of integers
  KIntMatrix *_lValues;
  // index variable of dimension one
  KIntVar *_indexVar1;
  // index variable of dimension two
  KIntVar *_indexVar2;
  // second constant
  int _cste2;
  
 public:
  
  /// Default Constructor
  KEltTerm2D();
  /// Main constructor : implements the expression lValues[indexVar1][indexVar2]
  KEltTerm2D(const KIntMatrix &lValues, const KIntVar &indexVar1,const KIntVar & indexVar2);
  /// implements the expression lValues[I+c][J+d]
  KEltTerm2D(KIntMatrix &lValues,const KUnTerm &index1UnTerm, const KUnTerm &index2UnTerm);
  /// return the array of values indexed by the index variable
  KIntMatrix *getLValues() const;
  /// return the index variable in dimension one
  KIntVar *getFirstIndexVar() const;
  /// return the index variable in dimension two
  KIntVar *getSecondIndexVar() const;
  int getCste2() const;
  
  virtual ~KEltTerm2D();
}; // class KEltTerm2D


#endif


