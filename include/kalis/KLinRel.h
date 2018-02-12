// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KLinRel.h					              *
// * 'KLinRel' stands for 'Linear Relation',			      *
// * ie linear equality or linear inequality of the form :    	      *
// *       "Sum_i(coeff_i * var_i) Comparator Constant"		      *
// * where coeff_i are double, var_i are either KNumVar or KAuxVar,   *
// * Comparator is <= or >= or == and Constant is a double.	      *
// ********************************************************************

#ifndef KLinRel_H
#define KLinRel_H

#include "Globals.h"
#include "KRelation.h"

/**	\class KLinRel
	\brief This class represents a linear relation (equality or inequality) between variables.
	
	Variables involved in the KLinRel object can be a mix of
	KNumVar and KAuxVar.
*/

class DLLIMPORTEXPORT KLinRel :
	public KRelation,
	public KCopiableObject<KLinRel>
{
public:

	/// Constructor with a problem.
	KLinRel (const KProblem& problem);
	/** \brief Constructor with a problem and aname.

	@param problem problem to which the KLinRel object is related
	@param name the name of the variable (used for printing)
	*/
	KLinRel (const KProblem& problem, const char* name);
	/// Copy constructor.
	KLinRel (const KLinRel&);
	/// Destructor.
	~KLinRel ();

	// ----- methods -----
	/** \brief Add a term (variable "times" coefficient) to the relation.

	@param var the variable involved
	@param coeff its coefficient
	*/
	void add(const KNumVar& var, double coeff = 1);
	/** \brief Add a term (variable "times" coefficient) to the relation.

	@param var the variable involved
	@param coeff its coefficient
	*/
	void add(const KAuxVar& var, double coeff = 1);
	
	/** \brief Add all the terms of the given relation (no reduction).

	@param relation the relation to add
	*/
	void add(const KLinRel& relation);
	

	/*Assign a KLinrel*/
	void assign(const KLinRel& relation);

	/*Invert coeff of KlinRel*/	
	void invert(const KLinRel& relation);

	/*Multiply coeff of KlinRel by a real*/	
	void mult(const KLinRel& relation, double real);

	/// Pretty-print the equation.
	void print() const;
	void print(void * ctx,PrintFunctionPtr*pfp) const;
	
	/// Print statistics about the equation.
	void printStat() const;
	/** \brief Is the linear relation satisfied for this instantiation ?

	@param sol hybrid solution to check
	*/
	bool isSatisfied (KHybridSolution* sol) const;

	//Clear store objects in KLinRel
	void clear(void);
};
#endif
