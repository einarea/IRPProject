// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KDefaultLinearRelaxationSolverConfigurator.h              *
// ********************************************************************

#ifndef KLinearRelaxationSolverBiLevel_H
#define KLinearRelaxationSolverBiLevel_H

#include "Globals.h"
#include "KProblem.h"
#include "KNumVarArray.h"
#include "KLinearRelaxationSolver.h"
#include "KLinearRelaxationSolverConfigurator.h"
#include "KIntVarArray.h"


/*Bilevel relaxation solver*/
class DLLIMPORTEXPORT KBiLevelLinearRelaxationSolverConfigurator : public KLinearRelaxationSolverConfigurator {
	private :
		KBiLevelLinearRelaxationSolverConfigurator();
	public:
		KBiLevelLinearRelaxationSolverConfigurator(KProblem &problem);
		KBiLevelLinearRelaxationSolverConfigurator(const KNumVarArray &varArray);
		virtual ~KBiLevelLinearRelaxationSolverConfigurator();
};

/*Top node relaxation solver*/
class DLLIMPORTEXPORT KTopNodeLinearRelaxationSolverConfigurator : public KLinearRelaxationSolverConfigurator {
	public:
		KTopNodeLinearRelaxationSolverConfigurator(KProblem &problem);
		//KTopNodeLinearRelaxationSolverConfigurator(KSolver *solver);
		virtual ~KTopNodeLinearRelaxationSolverConfigurator();
};

/*Other node relaxation solver*/
class DLLIMPORTEXPORT KOtherNodeLinearRelaxationSolverConfigurator : public KLinearRelaxationSolverConfigurator{
	public:
		KOtherNodeLinearRelaxationSolverConfigurator(KProblem &problem);
		virtual ~KOtherNodeLinearRelaxationSolverConfigurator();
};

#endif
