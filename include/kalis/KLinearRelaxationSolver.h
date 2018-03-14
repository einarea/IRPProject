// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KLinearRelaxationSolver.h 								  *
// ********************************************************************

#ifndef KLinearRelaxationSolver_H
#define KLinearRelaxationSolver_H

#include "Globals.h"
#include "KProblem.h"
#include "KRelaxationSolver.h"

/**	\class KLinearRelaxationSolver
	\brief This class is intended as a superclass for linear relaxation solvers.

	Such a solver must be provided with
  - a linear relaxation (KLinearRelaxation)
  - an objective variable (KNumVar)
  - a sense for optimization (KProblem::Sense).

  It relies on a LP/MIP solver to provide the following information:
  - a value (a bound for the relaxed problem, cf method getBound())
  - a solution, possibly not feasible for the original problem, but which can be used to guide the search for a feasible solution
  - if the problem is LP, reduced costs (that can be used for instance in the "reduced cost fixing" procedure).
  *	 @version 2013.1
*/

class DLLIMPORTEXPORT KLinearRelaxationSolver : 
	public KRelaxationSolver
{
public:
	/**
	 * Resolution algorithms
	 */
	enum SolveAlgorithm
	{
		ALG_PRIMAL,		///< Primal simplex algorithm
		ALG_DUAL,		///< Dual simplex algorithm
		ALG_NETWORK,	///< Network simplex algorithm
		ALG_BARRIER		///< Newton barrier method
	};

	// Destructor
	virtual ~KLinearRelaxationSolver();

	// get reduced cost (if solve complete)
	virtual double getReducedCost(const KNumVar& var) const = 0;
	virtual double getReducedCost(const KAuxVar& var) const = 0;

	/** \brief Writes the current problem to a file (in lp format).

	@param filename the path of the file to write (existing file is overwrited, if any)
	@return return code is reserved for future use (for now, errors are trapped by an exception)
	*/
	virtual int writeLP (const char* filename) const = 0;

	/** Cut generation */
	virtual void generateCuts(KLinearRelaxation* relaxation) =0;
};

#endif
