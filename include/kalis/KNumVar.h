// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNumVar.h                                                 *
// ********************************************************************

#ifndef __KNumVar_H
#define __KNumVar_H

#include "Globals.h"
#include "KPtrArray.h"

//EXTTEMPL template class DLLIMPORTEXPORT KPtrArray<KNumVar>;

/**
* Superclass of decision variables 
*/
class DLLIMPORTEXPORT KNumVar : public KPtrArray<KNumVar> {

protected :
	// Pointer to the problem
	KProblem * _problem;  
	// Pointer to the internal object implementing this KNumVar      
	KNumVar_I * _numVarIPtr;
public:  
	/// Default constructor
	KNumVar();
	/// main constructor
	KNumVar(KProblem& problem);    
	/// Constructor with KProblem and KNumVar_I
	KNumVar(KProblem* problem, KNumVar_I* numVar);    
	//#ifdef ACK_LIB
	/// Constructor with KNumVar_I
	KNumVar(KNumVar_I* numVar);
	//#endif
	/// Copy constructor
	KNumVar(const KNumVar& toCopy);
	/// Assignment copy constructor
	KNumVar& operator=(const KNumVar& toCopy);
	/// Destructor
	virtual ~KNumVar();
	/// Instantiate the variable to value
	virtual void instantiate(const double value);
	/// returns the KProblem associated with this variable
	KProblem * getProblem() const;
	KProblem * getKProblem() const;
	// get methods
	virtual char * getName() const;
	/// Set the name of the variable
	virtual void setName(char * name);
	/// returns lower bound of this variable
	virtual double getInf() const;    
	/// returns upper bound of this variable
	virtual double getSup() const;
	/// returns current instantiation of the variable (when the variable is not instantiated the returned value is undefined)
	virtual double getValue(void) const;
	/// returns true if the variable has been assigned a value, false otherwhise
	virtual bool getIsInstantiated() const; 
	/// returns the number of constraints where this variable appears
	virtual int getDegree() const; 
	/// get target value
	virtual double getTarget() const; 
	/// set the lower bound to value
	virtual void setInf(double value);
	/// set the upper bound to value
	virtual void setSup(double value);
	/// set the target value
	virtual void setTarget(double value); 
	/// Return true if this variable can be instantiated to 'value'
	virtual bool canBeInstantiatedTo(double value);    
	/// activate shaving Y/N
	virtual void useShaving(bool use);
	/// Hidden variable Y/N
	virtual void setHidden(bool hidden);
	/// Return true iff this variable is hidden
	virtual bool isHidden(void);
	/// Internal use
	virtual KNumVar_I *getNumVar_I_ptr() const;  
	KNumVar_I *getInternalObject() const;
	/// Internal use
	void setNumVar_I_ptr(KNumVar_I * numVar);
	/// Return a copy of this object
	KNumVar* getCopyPtr() const;
	/// Pretty printing
	virtual void print();
	/// Pretty printing
	virtual void print(void * ctx,PrintFunctionPtr*pfp) const;
	/**
	* Return the type of this variable 
	* @param KNumVar::IsKNumVar for an instance of the class KNumVar
	* @param KNumVar::IsKIntVar for an instance of the class KNumVar
	* @param KNumVar::IsKFloatVar for an instance of the class KNumVar
	*/
	virtual const int instanceof(void) const;

	/**
	* Variable types 
	*/
	enum Type {
		IsKNumVar,		///< Numeric variables 
		IsKIntVar,		///< Floating-point (continuous) variables
		IsKFloatVar		///< Integer variables
	};

}; // class KNumVar


#endif


