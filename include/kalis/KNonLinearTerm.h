// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNonLinearTerm.h                                          *
// ********************************************************************

#ifndef __KNONLINTERM_H
#define __KNONLINTERM_H

#include "Globals.h"
#include "KTerm.h"
class KHybridSolution;

/**	 
  * This class represent a non linear term 
  @code
    Example :
	X + 3 * Y ^ 3

  @endcode	
  *	@see  KConstraint
  * @see KNumLinComb
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNonLinearTerm : public KTerm 
{
  private :
	static int nbtmpvars;
	NAMESPACE_STL::vector<KConstraint*> _tmpConstraints;
	NAMESPACE_STL::vector<KNumVar*> _tmpVariables;
	bool manageVar;

  public:
	KNumVar * var;
	KLinTerm * linTerm;	
	int fct;
	int op;	
	int exp;
	enum {GEQ,GT,LEQ,LT,EQ};
	enum {OP_NONE,OP_PLUS,OP_MINUS,OP_MULT,OP_DIV};
	enum {FCT_NONE,FCT_LN,FCT_EXPONENTIAL,FCT_EXP,FCT_COS,FCT_SIN,FCT_TAN,FCT_ACOS,FCT_ASIN,FCT_ATAN,FCT_COSH,FCT_SINH,FCT_TANH,FCT_ABS};
	KNonLinearTerm *left,*right;
	KNonLinearTerm();
	KNonLinearTerm(double cste);
	KNonLinearTerm( KNonLinearTerm *LEFT, KNonLinearTerm *RIGHT,const int OPERATOR);
	KNonLinearTerm( KLinTerm *X);	
	KNonLinearTerm( KNumVar *X);
	KNonLinearTerm( KNonLinearTerm *LEFT,int FCT);
	KNonLinearTerm( KNonLinearTerm *LEFT,int EXP,const int FCT);
	//KNonLinearTerm(const KNonLinearTerm&);
	//KNonLinearTerm( KNonLinearTerm*nlt);
	KNonLinearTerm(const KNonLinearTerm& toCopy, int pb);
	KNonLinearTerm * getCopyPtr() const;
	
	~KNonLinearTerm();
	
	void print() const;
	void print(void * ctx,PrintFunctionPtr*pfp) const;
	void postNonLinearEpression(KProblem * prob);

};

#endif
