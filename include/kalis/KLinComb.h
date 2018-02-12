// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KLinComb.h                                                *
// ********************************************************************

#ifndef __KLINCOMB_H
#define __KLINCOMB_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
  * This class creates a  Sum(ai.Xi) { <= , != , == } C constraint
  @code
    Example :

	KIntVarArray X(...);
	
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
class DLLIMPORTEXPORT KLinComb : public KConstraint { 
  public :
	/**
	 Primary Constructor 
	 @param name name of the constraint
	 @param coeffs array of coefficients for the variables in the linear combination
	 @param vars array of variables involved in the linear combination
	 @param cste constant in the linear combination
	 @param linCombOperator operator of the linear constraint {<>,<=,>=,==}
	*/
	KLinComb(char * name,const KIntArray& coeffs, const KIntVarArray &vars , int cste, int linCombOperator);
	/// Copy Constructor 
	KLinComb(const KLinComb & toCopy);
	virtual ~KLinComb();	
    virtual void * getConstraintIPtr();
	enum LinCombOperator{Equal, GreaterOrEqual, NotEqual};
};


#endif


