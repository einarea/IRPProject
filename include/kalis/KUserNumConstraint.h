// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KUserNumConstraint.h                                      *
// ********************************************************************

#ifndef __KUSERNUMCONSTRAINT_H
#define __KUSERNUMCONSTRAINT_H

#include "Globals.h"
#include "KNumVarArray.h"
#include "KConstraint.h"
#include "KNumVar.h"

/**
 * The KUserNumConstraint is the generic counterpart to the KUserConstraint for implementing user constraints when using numeric variables.
 */
class DLLIMPORTEXPORT KUserNumConstraint : public KConstraint {  
  public :	
	 KNumVarArray _nvars;
	/// Constructor for unary constraints
	 KUserNumConstraint(KNumVar &v1);
	/// Constructor for binary constraints
	KUserNumConstraint(KNumVar &v1,KNumVar &v2);
	/// Constructor for n-ary constraints
	KUserNumConstraint(KNumVarArray &vars);
	/// Copy constructor
	KUserNumConstraint(const KUserNumConstraint & toCopy);
	// destructor
    virtual ~KUserNumConstraint();
	
	// Internal use
	KConstraint* getInstance(int pb) const;

	/**
	 * Virtual copy method.
	 * Must be implemented by the user.
	 */
	virtual KConstraint* getCopyPtr() const;

	/**
	 * Virtual instance copy method.
	 * Each modeling elements stored (and used) in the user constraint must be copied using the KProblem::getCopyPtr() method.
	 * Must be implemented by the user when solving problems in parallel.
	 */
	virtual KConstraint* getInstanceCopyPtr(const KProblem& problem) const;

	/// Virtual method called when the domain of some or several variables has changed
    virtual void propagate(void);
	/// Virtual method called upon initialization of the constraint	
    virtual void awake(void);
	/// Virtual method called when the lower bound of var has been raised	
	virtual void awakeOnInf(KNumVar & var);
	/** Virtual method called when the upper bound of var has been lowered
		@param var the variable with modified domain
	*/
    virtual void awakeOnSup(KNumVar & var);
	/** Virtual method called when the variable var has been instantiated 
		@param var the variable with modified domain
	*/
    virtual void awakeOnInst(KNumVar & var);
	/** Virtual method called when the value removedValue has been removed from the domain of var
		@param var the variable with modified domain
		@param removedValue the value that has been removed from the domain of var
	*/
	virtual void awakeOnRem(KIntVar & var,int removedValue);
	/** Virtual method called when the domain of variable var has changed
		@param var the variable with modified domain		
	*/
    virtual void awakeOnVar(KNumVar & var);
	/** Virtual method for use within boolean connectors 
		@return CTRUE whenever the constraint is definitively satisfied
		@return CFALSE whenever the constraint is definitively violated
		@return CUNKNOWN otherwhise
	*/	

    virtual int askIfEntailed(void);
	
	void constAwake();	

	/**
	 * Pretty printing of the constraint
	 */
    virtual void print(void); 
	
	/**
	 * Pretty printing of the constraint with print function pointer
	 */
	virtual void print(void*ctx,PrintFunctionPtr*pfp);
	/*
	 *
	 */
	virtual int getTypeInfo(void); 

	/*Linear relaxation*/
	virtual KLinearRelaxation* getLinearRelaxation (int strategy);
	virtual bool isGetLinearRelaxationImplemented();

	/**
	 * askIfEntailed return values
	 */
    enum askRet {
		CUNKNOWN,
		CFALSE,
		CTRUE
	};
};

#endif


