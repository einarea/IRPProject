// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KOccurTerm.h                                              *
// * Description : class representing expressions occur(a, (X1 ,…, Xn)*
// * ) where the Xi are KIntVar's and a is an integer. Such           *
// * expressions are used for stating occurrence constraints.         *
// ********************************************************************

#ifndef __KOCCURTERM_H
#define __KOCCURTERM_H

#include "KTerm.h"
#include "KIntVarArray.h"

/**	 
  * This class represent an expression of type occur(target,lvars)  where target is the value for wich we want to restrict the number of occurence(s) in the lVars array of variables
  @code
    Example :

    KProblem p(...);    
	KIntVar countVar(...);
	KIntVarArray X(...);
	
	KOccurTerm occurTerm(3,X);
			
	// posting the constraint "the number of occurences of the value 3 in the 
	// X variable array must be less than the value of countVar
	p.post(occurTerm <= countVar);
	or
	p.post(occurTerm <= 5);


  @endcode	
  *	@see  KConstraint
  * @see KOccurrence
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KOccurTerm : public KTerm {

  private:

  // Number of occurences targeted
  int _target;
  
  // List of variables witch could take the value
  KIntVarArray *_lvars;
  
 public:
  
  // Constructors
  
  /// Default constructor
  KOccurTerm();  
  /// Main constructor
  KOccurTerm(const int target, const KIntVarArray &lvars);  
  /// Copy constructor for a specific instance
  KOccurTerm(const KOccurTerm & toCopy, int pb);  
  // Destructor
  virtual ~KOccurTerm();  
  /// return the target value
  int getTarget() const {return _target;};
  /// return the array of variables in wich we want to restrict the number of occurences of the target value
  KIntVarArray * getLvars() const {return _lvars;};
  
}; // class KOccurTerm

#endif


