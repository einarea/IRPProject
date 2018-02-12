// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSolution.h                                               *
// * Description : user class representing a CSP solution             *
// ********************************************************************

#ifndef _SOLUTION_H
#define _SOLUTION_H

#include "Globals.h"

/**	 
 *	This class represents a solution of a KProblem.
@code
Example:

KProblem p(...);
KSolver solver(p);
solver.solve();	
KSolution sol = p.getSolution();
@endcode	
 *	@see KProblem
 *	@version 2013.1
 */
class DLLIMPORTEXPORT KSolution : public KAutoExternalObject<KSolution, KSolution_I> {
public:
	/// Constructor 
	KSolution();
	// Constructor with KSolution_I*
	KSolution(KSolution_I* solution);
	// Copy constructor
	KSolution(const KSolution &toCopy);
	// Destructor
	virtual ~KSolution();

	KSolution * getCopyPtr() const;

	KSolution& operator=(const KSolution& toCopy);

	/// Return the instantiation of a variable in the solution
	int getValue(KIntVar& intVar) const;
	/// Return the instantiation of a variable in the solution
	double getValue(KFloatVar& floatVar) const;
	/// Return the instantiation of the variable in the solution
	int getValue(KNumVar& numVar) const;

	/// Return the objective value of the solution if applicable
	double getObjectiveValue() const;

	void setAsTarget(void);
	/// Pretty print the solution
	void print() const;
	void print(void* ctx, PrintFunctionPtr* pfp) const;
	void printResume(void* ctx, PrintFunctionPtr* pfp) const;
	void printResume() const;
	void printProblem() const;

	// attributes
	int  getIntAttrib(int attrib);
	double getDblAttrib(int attrib);
	bool hasObjective(void) const;

	double getHashCode();

//#ifdef ACK_LIB
//	void setInternalObject(KSolution_I* solution); // should be removed
//#endif

}; // class KSolution

#endif


