// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KUserConstraint.h                                      *
// ********************************************************************

#ifndef __KUSERCONSTRAINT_H
#define __KUSERCONSTRAINT_H

#include "Globals.h"
#include "KIntVarArray.h"
#include "KConstraint.h"
#include "KIntVarArray.h"
#include "KIntVar.h"

/** 
    Abstract interface class for definition of user constraints

    To create your own constraints in Artelys Kalis, you must create a specific class
	that inherits from the KUserConstraint class. Then, you need to implement the pruning scheme corresponding to the semantic of your constraint
	by overloading the propagate method or awakeOnXXX methods for incremental propagation of the constraint
	
	<ul>
	<li> the awake method is launched one time upon initialization of the constraint    
	<li> the awakeOnInf method is launched when the lowerbound of a variable increase
	<li> the awakeOnSup method is launched when the upperbound of a variable decrease
	<li> the awakeOnRem method is lanched when a specific value has been removed from the domain of a variable
	<li> the awakeOnInst method is launched when a variable has been instantiated to a value
	<li> the awakeOnVar method is launched when the domain of one specific variable has changed
    <li> the propagate method is launched when one or more variables have seen their domain modified
	<li> askIfEntailed method is called when the constraints is used within a boolean connector (KGuard, KEquiv, KDisjunction, KConjunction). 
	     It should return:
		 <ul>
		 <li> CTRUE whenever the constraint is definitively verified
		 <li> CFALSE whenever the constraint is definitively violated
		 <li> CUNKNOWN otherwhise
		 </ul>
	</ul>
	The awake, propagate and print methods <u>must</u> be overloaded (pure virtual functions).
	The default behavior of the awakeOnInf,awakeOnSup,awakeOnInst,awakeOnVar and awakeOnRem is to call the propagate method.

	Additionally, it is necessary to implement the getInstanceCopyPtr(const KProblem& problem) that returns a copy pointer of the user constraint
	by calling the KProblem::getCopyPtr() on each modeling object used in the constraint.

	Here is an examples of user defined constraint definition : V1 == V2 % modulo
	@code
// ModuloConstraint class inherits from KUserConstraint class
class ModuloConstraint : public KUserConstraint {

private:
	// Modulo constant
	int _modulo;
	// Variables V1 and V2
	KIntVar* _v1;
	KIntVar* _v2;

public:
	// Primary constructor of the constraint V1 == V2 % modulo
	ModuloConstraint(KIntVar &v1, KIntVar &v2, const int modulo);
	// Destructor
	virtual ~ModuloConstraint();
	// Virtual copy method
	virtual KConstraint* getInstanceCopyPtr(const KProblem& problem) const;
	/////////////////////////
	// Propagation methods
	/////////////////////////
	virtual void awake(void);
	virtual void propagate(void);
	virtual void awakeOnInst(KIntVar & var);
	virtual void print();
	///////////////////////////////////////
	// For use within boolean connectors
	///////////////////////////////////////
	virtual int askIfEntailed(void);
};

// Main constructor
ModuloConstraint::ModuloConstraint(KIntVar &v1, KIntVar &v2, const int modulo) : KUserConstraint(v1,v2) {
	_modulo = modulo;
	_v1 = &v1;
	_v2 = &v2;
}

// Destructor
ModuloConstraint::~ModuloConstraint() {
}

// Virtual copy method
KConstraint * ModuloConstraintXYC::getInstanceCopyPtr(const KProblem& problem) const {
	return new ModuloConstraintXYC(*problem.getInstanceOf(_v1), *problem.getInstanceOf(_v2), _modulo);
}

///////////////////////////////////////////
// initial propagation of the constraint
///////////////////////////////////////////
void ModuloConstraint::awake() {
	propagate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Some variables of the constraints have seen their domain reduced... must propagate the changes
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModuloConstraint::propagate() {
	int v0, v1;
	printf("ModuloConstraint#Propagate()\n");
	// V1 is trivialy bounded by 0 <= V1 <= modulo - 1
	_v1.setInf(0);
	_v1.setSup(_modulo-1);
	//////////////////////////////
	// Arc consistent filtering
	//////////////////////////////
	/////////////////////////
	// First from V1 to V2
	/////////////////////////
	for ( v1 = _v2.getInf();v1<=_v2.getSup();v1 ++) {     
		bool supporte = false;
		for ( v0 = _v1.getInf();v0<=_v1.getSup();v0 ++) {
			// check if v0 == v1 % _modulo hold
			if ( v0 == v1 % _modulo)  {	
				// value v0 is a support for value v1 of variable V2
				supporte = true;
				break;
			}
		}
		if ( !supporte) {
		// no support was found for value v1 of variable V2 so we can safely remove it from its domain
		_v2.remVal(v1);
		}
	}
	////////////////////////
	// Then from V2 to V1
	////////////////////////
	for ( v0 = _v1.getInf(); v0 <= _v1.getSup(); v0++) {     
		bool supporte = false;
		for ( v1 = _v2.getInf(); v1 <= _v2.getSup(); v1++) {
			// check if v0 == v1 % _modulo hold
			if ( v0 == v1 % _modulo)  {
				// value v1 is a support for value v0 of variable V1
				supporte = true;
				break;
			}
		}
		if ( !supporte) {
		// no support was found for value v0 of variable V1 so we can safely remove it from its domain
		_v1.remVal(v0);
		}
	}
 
}

int ModuloConstraint::askIfEntailed() {
	if (_v1.getIsInstantiated() && _v2.getIsInstantiated() ) {
		if ( _v1.getValue() == _v2.getValue() % _modulo ) {	
			// The constraint is definitly verified
			return CTRUE;
		}
		else {
			// The constraint is definitly violated
			return CFALSE;
		}
	}
	else {
		// Don't know yet if the constraint is definitly violated or verified
		return CUNKNOWN;  
	}
}

////////////////////////////////////////////////////////////////
// The variable "var" has been instantiated to var.getValue()
////////////////////////////////////////////////////////////////
void ModuloConstraint::awakeOnInst(KIntVar & var) {
	int v;

	printf("ModuloConstraint#awakeOnInst\n");

	if ( var.isEqualTo(_v1) ) {	
		//  V1 was instantiated
		for ( v = _v2.getInf();v<=_v2.getSup();v ++) {
			if (_v1.getValue() != v % _modulo) 
				_v2.remVal(v);         
		}
	}
	else if ( var.isEqualTo(_v2) ) {
		//  V2 was instantiated
		for ( v = _v1.getInf();v<=_v1.getSup();v ++) {
			if ( v != _v2.getValue() % _modulo) 
				_v1.remVal(v);       
		}
	}
}

//////////////////////////////////
// For pretty printing purposes
//////////////////////////////////
void ModuloConstraint::print() {
	printf("ModuloConstraint");
}
@endcode

@code
KProblem problem(...);
KIntVar A(problem, "A", 0, 20);
KIntVar B(problem, "B", 0, 10);
// Now creating the constraint A == B % 7
ModuloConstraint modCst(A, B, 7);
// Now posting the constraint to the problem
problem.post(modCst);
...
@endcode

	@see KConstraint 
	@version 2013.1
*/
class DLLIMPORTEXPORT KUserConstraint : public KConstraint {  
  public :	
	KIntVarArray _vars;
	/// Constructor for unary constraints
	KUserConstraint(KIntVar &v1);
	/// Constructor for binary constraints
	KUserConstraint(KIntVar &v1,KIntVar &v2);
	/// Constructor for n-ary constraints
	KUserConstraint(KIntVarArray &vars);
	/// Copy constructor
	KUserConstraint(const KUserConstraint & toCopy);
	// destructor
    virtual ~KUserConstraint();

	// Internal use
	KConstraint* getInstance(int pb) const;

	/**
	 * Virtual copy method.
	 * Must be implemented by the user.
	 */
	virtual KConstraint* getCopyPtr() const;

	/**
	 * Virtual instance copy method.
	 * Each modeling elements stored (and used) in the user constraint must be copied using the KProblem::getInstanceOf() methods.
	 * Must be implemented by the user when solving problems in parallel.
	 */
	virtual KConstraint* getInstanceCopyPtr(const KProblem& problem) const;


	void constAwake();	

	/// Virtual method called when the domain of some or several variables has changed
    virtual void propagate();
	/// Virtual method called upon initialization of the constraint	
    virtual void awake();
	/// Virtual method called when the lower bound of var has been raised	
    virtual void awakeOnInf(KIntVar & var);
	/** Virtual method called when the upper bound of var has been lowered
	 *	@param var the variable with modified domain
	 */
    virtual void awakeOnSup(KIntVar & var);
	/** Virtual method called when the variable var has been instantiated 
	 *	@param var the variable with modified domain
	 */
    virtual void awakeOnInst(KIntVar & var);
	/** Virtual method called when the value removedValue has been removed from the domain of var
	 *	@param var the variable with modified domain
	 *	@param removedValue the value that has been removed from the domain of var
	 */
    virtual void awakeOnRem(KIntVar & var,int removedValue);
	/** Virtual method called when the domain of variable var has changed
	 *	@param var the variable with modified domain		
	 */
    virtual void awakeOnVar(KIntVar & var);
	/** Virtual method for use within boolean connectors 
		@return CTRUE whenever the constraint is definitively satisfied
		@return CFALSE whenever the constraint is definitively violated
		@return CUNKNOWN otherwhise
	*/	
    virtual int askIfEntailed(void);
	

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
	
	/*Linear Relaxation*/
	virtual KLinearRelaxation * getLinearRelaxation (int strategy);

	bool isGetLinearRelaxationImplemented();

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


