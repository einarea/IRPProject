// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KLinTerm.h                                                *
// * Description : A class representing arithmetic expressions of the *
// * form a0 + a1X1 +…+ anXn, where the Xi are KIntVar's and ai are   *
// * integer values.                                                  *
// ********************************************************************

#ifndef __KLINTERM_H
#define __KLINTERM_H

#include "Globals.h"
#include "KTerm.h"

/**	 
  * This class represent a linear term of the form Sum(coeffs[i].lvars[i]) + cste 
  @code
    Example :

    KProblem p(...);
    KIntVarArray X(...);
	
	KLinTerm linTerm;
	
	linTerm = 3 * X[0];
	linTerm = linTerm + 5;
	linTerm = linTerm + 2 * X[1];
	linTerm = linTerm - 3 * X[2] + 5 * X[3];
	linTerm = linTerm - 7;
	// these lines are equivalent to : 
	// linTerm = 3 * X[0] + 2 * X[1] - 3 * X[2] + 5 * X[3] - 2
	//
		
	// posting the constraint  3 * X[0] + 2 * X[1] - 3 * X[2] + 5 * X[3] - 2 >= 5
	// will be converted into  -3 * X[0] - 2 * X[1] + 3 * X[2] - 5 * X[3] + 7 <= 0
	p.post(linTerm >= 5);


  @endcode	
  *	@see  KConstraint
  * @see KLinComb
  *	  @version 2013.1 
*/
class DLLIMPORTEXPORT KLinTerm : public KTerm {

  private:

    // _lvars is the list KIntVar, _coeffs the list of coefficients
    // They must be the same size
    KNumVarArray *_lvars;
    KDoubleArray *_coeffs;

  public:

    // Constructors
	/// Default constructor
    KLinTerm(); 
	//KLinTerm(const double cste); 
	/// Main constructor
    KLinTerm(const KNumVarArray &lvars, const KDoubleArray &coeffs, const double cste);
    /// Each KIntVar has a coefficient 1
	KLinTerm(const KNumVarArray &lvars, const double cste); 
	/// Copy constructor
    KLinTerm(const KLinTerm &linTermToCopy);
	/// Copy constructor for a specific instance
    KLinTerm(const KLinTerm &linTermToCopy, int pb);
	/// Cast a KIntVar into a KLinTerm
    KLinTerm(const KNumVar &intVar); 
	/// Cast a KNumUnTerm into a KLinTerm
    KLinTerm(const KUnTerm &unTerm); 
	/// Cast a KNumBinTerm into a KLinTerm
	KLinTerm(const KBinTerm &binTerm); 
    
    // Destructor
    ~KLinTerm();

    // Get methods
    KNumVarArray *getLvars() const;
    KDoubleArray *getCoeffs() const;

    /// pretty print method
    virtual void print() const;
    /// pretty print method
	virtual void print(void * ctx,PrintFunctionPtr*pfp);
	
	void add(const KNumVar & var ,double cste);

    // overloaded = operator
    const KLinTerm &operator=(const KLinTerm &linTermToCopy);
}; // class KNumLinTerm;


#endif


