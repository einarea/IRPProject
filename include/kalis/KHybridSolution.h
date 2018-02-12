// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KHybridSolution.h										  *
// ********************************************************************

#ifndef KHybridSolution_H
#define KHybridSolution_H

#include "Globals.h"

/**	 \class KHybridSolution
	\brief This class represents a solution of a relaxation solver, that is, a mapping from variables
  (KNumVar and/or KAuxVar) to their value.

  @code
    Example :
	...
	KXPRSLinearRelaxationSolver mySolver(...);
	mySolver.solve();
	solverMIP.updateSolution();
	KHybridSolution * mySol = solverMIP.getSolutionPtr();
	mySol->print();
	...
	@endcode	
  *	 @version 2013.1
*/

class DLLIMPORTEXPORT KHybridSolution {
protected:
	KHybridSolution_I * _InternalPtr;
public:
	/// Default constructor.
	KHybridSolution();
	// Constructor reserved for internal use.
	KHybridSolution(KHybridSolution_I*);
	/// Copy constructor (forbidden).
	KHybridSolution(const KHybridSolution&);
	/// Destructor.
	~KHybridSolution();

	// ----- methods -----
	/// Print solution.
	void print (void) const;

	// ----- accessors -----
	// Reserved for internal use.
	KHybridSolution_I* getInternalPtr (void) const;
	/** \brief Set the value of a KNumVar.

	@param variable whose value is modified
	@param new value
	*/
	void setVal(const KNumVar* var, double val);
	/** \brief Set the value of a KAuxVar.

	@param var varaible to modify
	@param new value
	*/
	void setVal(const KAuxVar* var, double val);
	/** \brief Get the value of a KNumVar.

	@param variable to read
	@return value
	*/
	double getVal(const KNumVar*) const;
	/** \brief Get the value of a KAuxVar.

	@param variable to read
	@return value
	*/
	double getVal(const KAuxVar*) const;
};

#endif
