// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNumNonLinearComb.h                                       *
// ********************************************************************

#ifndef __KNumNonLinearComb_H
#define __KNumNonLinearComb_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
  * This class represents a constraint to propagate any non linear constraint of the form
  * KNonLinearTerm COMPARATOR KNonLinearTerm  
  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumNonLinearComb : public KConstraint { 
  public :
	/**
	 Primary Constructor 
	 @param prob the problem
	 @param left the left non linear term
	 @param right the right non linear term	 
	 @param NonlinCombOperator operator of the non linear constraint {<=,>=,==}
	*/
	KNumNonLinearComb(KProblem & prob,KNonLinearTerm & left, KNonLinearTerm &right ,int NonlinCombOperator);	
	/**
	 * Constructor with a non linear term on the left and a variable on the right
	  @param prob the problem
	  @param left the left non linear term
	  @param right the variable on the right 
	  @param NonlinCombOperator operator of the non linear constraint {<=,>=,==}
	 */
	KNumNonLinearComb(KProblem & prob,KNonLinearTerm & left, KNumVar &right ,int NonlinCombOperator);
	/**
	 * Constructor with a non linear term on the left and a real on the right
	  @param prob the problem
	  @param left the left non linear term
	  @param right the value on the right 
	  @param NonlinCombOperator operator of the non linear constraint {<=,>=,==}
	 */
	KNumNonLinearComb(KProblem & prob,KNonLinearTerm & left, double right ,int NonlinCombOperator);
	/// Copy Constructor 
	KNumNonLinearComb(const KNumNonLinearComb & toCopy);
	/**
	 * Destructor
	 */
	virtual ~KNumNonLinearComb();	
    virtual void * getConstraintIPtr();
	/// Available operators for the constraint
	enum NonLinearCombOperator{
		Equal,
		GreaterOrEqual,
		LessOrEqual
	};
};


#endif


