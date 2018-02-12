// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KRelaxationSolverConfigurator.h 					  *
// *                                                                  *
// * This class defines methods to set properties of				  *
// * KLinearRelaxationSolver										  *
// ********************************************************************

#ifndef KRelaxationSolverConfigurator_H
#define KRelaxationSolverConfigurator_H

#include "Globals.h"
#include "KProblem.h"

class DLLIMPORTEXPORT KRelaxationSolverConfigurator :
	public KCopiableObject<KRelaxationSolverConfigurator>,
	public KAutoExternalObject<KRelaxationSolverConfigurator,KRelaxationSolverConfigurator_I>,
	public KPtrArray<KRelaxationSolverConfigurator>
{
protected:
	KProblem * _problem;

	/*Default constructor*/
	KRelaxationSolverConfigurator();

public:
	/*Constructor with KRelaxationSolverConfigurator_I*/
	KRelaxationSolverConfigurator(KRelaxationSolverConfigurator_I* configurator);
	/*Destructor*/
	virtual ~KRelaxationSolverConfigurator();

	KProblem* getProblem() const;

	/*Set events before the launch of linear relaxation solving*/
	virtual void beforeEvent();
	/*Set conditions in order to launch the linear relaxation*/
	virtual bool mustRelax();
	/*Set events after the launch of linear relaxation solving*/
	virtual void afterEvent(int status);
	
	/*Use solveAsMiP, default is LP*/
	virtual bool solveAsMIP();
	/*Use presolve*/
	virtual bool usePresolve();
	/* Return the resolution algorithm used to solve the relaxation */
	virtual int getAlgorithm(); 
	/** Set the algorithm flag */
	virtual void setAlgorithm(int alg);
	/** Set the minimal desired optimality tolerance (LP)**/
	virtual void setOptimalityTolerance(double optimalityTolerance);
	/** Set the minimal desired relative optimality tolerance (MIP) **/
	virtual void setMipRelativeOptimalityTolerance(double optimalityTolerance);
	/** Set the minimal desired absolute optimality tolerance (MIP) **/
	virtual void setMipAbsoluteOptimalityTolerance(double optimalityTolerance);

	/*Use reduced cost fixing*/
	virtual bool useReducedCostFixing() const;
	/* Set the use reduced cost fixing flag */
	void setuseReducedCostFixing(bool urcf);
	
	/** minimal desired optimality tolerance **/
	virtual double getOptimalityTolerance();
	/** minimal desired optimality tolerance **/
	virtual double getMipRelativeOptimalityTolerance();
	/** minimal desired optimality tolerance **/
	virtual double getMipAbsoluteOptimalityTolerance();

	/*Set the RelaxationSolver*/
	void setRelaxationSolver(KRelaxationSolver *relaxationSolver);
	/*Set the KSolver*/
	void setSolver(const KSolver& solver);
	/* Set the solve as MIP flag */
	void setsolveAsMIP(bool sam);
	/* Set the use presolve flag */
	void setusePresolve(bool up);
	/* Set an integer control (solver specific) */
	void setIntControl(int param,int value);
	/* Set a double control (solver specific) */
	void setDoubleControl(int param,double value);
	/* Get the number of integer controls definined (solver specific) */
	int getNumberOfIntegerControls();
	/* Get the number of double controls definined (solver specific) */
	int getNumberOfDoubleControls();
	/* Get the integer control identifier number 'numparam' */
	int getIntegerControlParam(int numparam);
	/* Get the integer control 'control' value */
	int getIntegerControlValue(int control);
	/* Get the double control identifier number 'numparam' */
	double getDoubleControlParam(int numparam);
	/* Get the double control 'control' value */
	double getDoubleControlValue(int control);

	/* Force a contradiction to be raised */
	void forceContradiction();	
};

#endif
