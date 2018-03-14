// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KVariableSelector.h                                       *
// * Description : class which is in charge of founding a variable    *
// * following specific parameteres (used in KBranchingScheme (mainly *
// * KAssignVar))                                                     *
// ********************************************************************

#ifndef __DISJUNCTION_SELECTOR_H
#define __DISJUNCTION_SELECTOR_H

#include "Globals.h"
/** 
* Abstract interface class for disjunction selection heuristic
*  @version 2013.1
*/
class DLLIMPORTEXPORT KDisjunctionSelector {
private:
	KProblem* _problem;
public:
	/// Constructor
	KDisjunctionSelector();
	/// Constructor with KProblem
	KDisjunctionSelector(KProblem* problem);
	/// Copy constructor
	KDisjunctionSelector(const KDisjunctionSelector& selectorToCopy);
	/// Destructor
	virtual ~KDisjunctionSelector();
	
	KProblem* getProblem() const;

	/** 
	* Virtual interface method to overload for definition of your own disjunction selection heuristics
	* @param intVarArray Array of variable from wich selecting  a variable
	*/
	virtual KDisjunction * selectNextDisjunction(KDisjunctionArray* disjunctionArray); 
	/**
	* Return a copy of this object
	*/
	virtual KDisjunctionSelector* getCopyPtr() const;  
	/// Print the name of this disjunction selector
	virtual void printName() { printf("KDisjunctionSelector\n");}
	/// Return the name of this disjunction selector
	virtual char * getName() { return "KDisjunctionSelector"; }  
}; // class KVariableSelector



/**	 
* This class implements a disjunction selector that selects the disjunction in the input order.
* @code
*  Example :
* 
*	KBranchingSchemeArray myBranchingSchemeArray;
*	myBranchingSchemeArray += KSettleDisjunction(new KDisjunctionInputOrder());
*
* @endcode	
* @see  KDisjunctionSelector  
*  @version 2013.1
*/
class DLLIMPORTEXPORT KDisjunctionInputOrder : public KDisjunctionSelector {
public:

	/// Constructor
	KDisjunctionInputOrder();

	/// Copy constructor
	KDisjunctionInputOrder(const KDisjunctionInputOrder& toCopy);

	// Destructor
	virtual ~KDisjunctionInputOrder();

	//methods
	virtual KDisjunction * selectNextDisjunction(KDisjunctionArray* disjunctionArray); 
	virtual KDisjunctionSelector* getCopyPtr() const;

}; 

/**	 
* This class implements a disjunction selector that selects first the disjunction ith the highest priority
* @code
* Example :
* 
*	KBranchingSchemeArray myBranchingSchemeArray;
*	myBranchingSchemeArray += KSettleDisjunction(new KDisjunctionPriorityOrder());
*
* @endcode	
* @see  KDisjunctionSelector  
*  @version 2013.1
*/
class DLLIMPORTEXPORT KDisjunctionPriorityOrder : public KDisjunctionSelector {
public:

	/// Constructor
	KDisjunctionPriorityOrder();

	/// Copy constructor
	KDisjunctionPriorityOrder(const KDisjunctionPriorityOrder& toCopy);

	// Destructor
	virtual ~KDisjunctionPriorityOrder();

	//methods
	virtual KDisjunction * selectNextDisjunction(KDisjunctionArray* disjunctionArray); 
	virtual KDisjunctionSelector* getCopyPtr() const;

}; 

#endif
