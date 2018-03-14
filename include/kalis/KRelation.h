// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KRelation.h					              *
// * 'KRelation' stands for a super class for relation,		      *
// ********************************************************************


#ifndef KRelation_H
#define KRelation_H

#include "Globals.h"
class KRelation_I;

/**	\class KRelation
*/

class DLLIMPORTEXPORT KRelation :
	public KAbstractPtrArray<KRelation>
{
protected:

	KRelation_I * _KRelation_IPtr;

public:
	
	enum Comparator {LEQ, GEQ, EQ, RANGE, NONE};

	// ----- methods -----
	/** \brief Add all the terms of the given relation (no reduction).

	@param relation the relation to add
	*/
	void add(const KRelation& relation);

	/*Assign a KRelation*/
	void assign(const KRelation& relation);

	/*Multiply coeff of KRelation by a real*/	
	void mult(const KRelation& relation, double real);

	/** \brief Set the Comparator to <= and set the constant.

	@param cst constant term in the linar relation
	*/
	KRelation& operator<= (double cst);
	/** \brief Set the Comparator to == and set the constant.

	@param cst constant term in the linar relation
	*/
	KRelation& operator== (double cst);
	/** \brief Set the Comparator to >= and set the constant.

	@param cst constant term in the linar relation
	*/
	KRelation& operator>= (double cst);

	/** \brief Set comparator based on argument and discards the bounds that are no longer relevant.

	@param cmp comparator to be applied in the linar relation
	*/
	void setComparator (KRelation::Comparator cmp);

	/// Pretty-print the equation.
	void print() const;

	void print(void * ctx,PrintFunctionPtr*pfp) const;
	
	/** \brief Is the linear relation satisfied for this instantiation ?

	@param sol hybrid solution to check
	*/
	virtual bool isSatisfied (KHybridSolution* sol) const = 0;
	// Get a pointer to the internal object (reserved for internal use)
	KRelation_I* getInternalPtr (void) const {return _KRelation_IPtr;};

	// set the constant
	void setConstant(double);
	void setLB(double);
	void setUB(double);

	// get the constant
	double getConstant (void) ;

	// get the constant
	Comparator getComparator (void);

	//Clear store objects in KRelation
	void clear(void);
};
#endif
