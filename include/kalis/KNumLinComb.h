// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNumLinComb.h                                             *
// ********************************************************************

#ifndef __KNumLinComb_H
#define __KNumLinComb_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
  * This class creates a  Sum(ai.Xi) { <= , != , == } C constraint
  @code
    Example :

	KNumVarArray X(...);
	
	...

	problem.post(2 * X[1] + 3 * X[2] + 5 * X[3] + ... + 7 * X[n] == 3);
	  or
	problem.post(2 * X[1] + 3 * X[2] + 5 * X[3] + ... + 7 * X[n] <= 3);
	  or 
	problem.post(2 * X[1] + 3 * X[2] + 5 * X[3] + ... + 7 * X[n] >= 3);
	  or
	problem.post(2 * X[1] + 3 * X[2] + 5 * X[3] + ... + 7 * X[n] != 3);


  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumLinComb : public KConstraint { 
  public :
	/**
	 Primary Constructor 
	 @param name name of the constraint
	 @param coeffs array of coefficients for the variables in the linear combination
	 @param vars array of variables involved in the linear combination
	 @param cste constant in the linear combination
	 @param linCombOperator operator of the linear constraint {<>,<=,>=,==}
	*/
	KNumLinComb(char * name, KDoubleArray& coeffs, KNumVarArray &vars , double cste, int linCombOperator);
	/// Copy Constructor 
	KNumLinComb(const KNumLinComb & toCopy);
	virtual ~KNumLinComb();	
    virtual void * getConstraintIPtr();
	enum LinCombOperator{
		Equal,
		GreaterOrEqual,
		NotEqual,
		LessOrEqual
	};
};



class DLLIMPORTEXPORT KConditionNumLinComb : public KConstraint { 
  public :
	/**
	 Primary Constructor 
	 @param name name of the constraint
	 @param coeffs array of coefficients for the variables in the linear combination
	 @param vars array of variables involved in the linear combination
	 @param cste constant in the linear combination
	 @param linCombOperator operator of the linear constraint {<>,<=,>=,==}
	*/
	KConditionNumLinComb(char * name, KDoubleArray& coeffs, KNumVarArray &vars , double cste, int linCombOperator);
	/// Copy Constructor 
	KConditionNumLinComb(const KConditionNumLinComb & toCopy);
	virtual ~KConditionNumLinComb();	
    virtual void * getConstraintIPtr();
	virtual int conditionTest(int varIndex) = 0;
	enum LinCombOperator{
		Equal,
		GreaterOrEqual,
		LessOrEqual
	};
};


#endif


