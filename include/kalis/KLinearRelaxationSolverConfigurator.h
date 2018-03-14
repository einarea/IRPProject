// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KLinearRelaxationSolverConfigurator.h 					  *
// *                                                                  *
// * This class defines methods to set properties of				  *
// * KLinearRelaxationSolver										  *
// ********************************************************************

#ifndef KLinearRelaxationSolverConfigurator_H
#define KLinearRelaxationSolverConfigurator_H

#include "Globals.h"
#include "KProblem.h"
#include "KRelaxationSolverConfigurator.h"

class DLLIMPORTEXPORT KLinearRelaxationSolverConfigurator :
	public KRelaxationSolverConfigurator
{
protected:
	/*Default constructor*/
	KLinearRelaxationSolverConfigurator();

public:
	/*Constructor with KProblem*/
	KLinearRelaxationSolverConfigurator(KProblem& problem);
	/*Constructor with KSolver*/
	KLinearRelaxationSolverConfigurator(const KSolver& solver);
	/*Constructor with KLinearRelaxationSolverConfigurator_I*/
	KLinearRelaxationSolverConfigurator(KLinearRelaxationSolverConfigurator_I* configurator);
	/*Destructor*/
	virtual ~KLinearRelaxationSolverConfigurator();

	/*Set the LinearRelaxationSolver*/
	void setLinearRelaxationSolver(KLinearRelaxationSolver *relaxationSolver);

	/* Set the use reload basis flag */
	void setUseReloadBasis(bool urb);
	/*Use reload basis*/
	virtual bool useReloadBasis();
};

#endif
