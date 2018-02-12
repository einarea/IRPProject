// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KEltTerm.h                                                *
// * Description : This class implements expressions like l[I] or     *
// * l[I + c] for a list of integers l, an integer c and an KIntVar I.*
// * Such expressions are used for stating KElement constraints.      *
// ********************************************************************

#ifndef __KELTERM_H
#define __KELTERM_H

#include "Globals.h"
#include "KTerm.h"

/**	 
  * This class represent an expression of type Tab[I] where Tab is an array of integer value and I is the indexing variable
  @code
    Example :

    KProblem p(...);
    KIntVar X(...);
	KIntVar I(...);
	KIntArray valuesArray(...);
	
	KEltTerm eltTerm(valuesArray,I);
			
	// posting the constraint X can take its values indexed by the I variable in the valuesArray
	p.post(X == eltTerm);
	// equivalent to 
	p.post(X == valuesArray[I]);


  @endcode	
  *	@see KConstraint
  * @see KElement
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KEltTerm : public KTerm {

 private:
  
  // indexed array of integers
  KIntArray *_lValues;
  // index
  KIntVar *_indexVar;
  // user pointer
  void * _userPointer;
  
 public:
  
  /// Default Constructor
  KEltTerm();
  /// Main constructor : implements the expression lValues[indexVar]
  KEltTerm(const KIntArray &lValues, const KIntVar &indexVar);
  /// implements the expression lValues[I+c]
  KEltTerm(const KIntArray &lValues, const KUnTerm &indexUnTerm);
	
  /// Main constructor : implements the expression lValues[indexVar]
  KEltTerm(void * ptr, const KIntVar &indexVar);
  /// implements the expression lValues[I+c]
  KEltTerm(void * ptr, const KUnTerm &indexUnTerm);

  /// Copy constructor for a specific instance
  KEltTerm(const KEltTerm &toCopy, int pb); 

  /// return the array of values indexed by the index variable
  KIntArray *getLValues() const;
  /// return the index variable
  KIntVar *getIndexVar() const;
  /// return the user pointer
  void * getUserPointer() const;
  
  // Destructor
  virtual ~KEltTerm();
}; // class KEltTerm


#endif


