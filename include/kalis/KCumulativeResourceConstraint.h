// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KCumulativeResourceConstraint.h          				  *
// * Implementation of difference constraint                          *
// ********************************************************************

#ifndef __KCumulativeResourceConstraint_H
#define __KCumulativeResourceConstraint_H

#include "Globals.h"
#include "KConstraint.h"
#include "KResource.h"

/**	 
 * This constraint states that some tasks requiring a resource do not exceed the resource capacity. The primary use of this constraint is to express resource constraints.
 * 	Resources (machines, raw material etc) can be of two different types : 
 *
 *	<UL>
 *		<LI>Disjunctive when the resource can process only one task at a time (represented by the class KUnaryResource).</LI>
 *		<LI>Cumulative when the resource can process several tasks at the same time (represented by the class KDiscreteResource).</LI>
 *	</UL>
 *
 *	Traditional examples of disjunctive resources are Jobshop problems, cumulative resources are heavily used for the Resource-Constrained Project Scheduling Problem (RCPSP). Note that a disjunctive resource is semantically equivalent to a cumulative resource with maximal capacity one and unit resource usage for each task using this resource but this equivalence does not hold in terms of constraint propagation. 
 *
 *	The following schema shows an example with three tasks A,B and C executing on a disjunctive resource and on a cumulative resource with resource usage 3 for task A, 1 for task B and 1 for task C :
 *
 *  \image html resources.png
 *
 *  @version 2013.1
 */
class DLLIMPORTEXPORT KCumulativeResourceConstraint : public KConstraint { 
  public :
	/**
	 * Constructor for internal use only 	 
	 */
	  KCumulativeResourceConstraint(KProblem & p, KIntVarArray &starts,KResource_I * r,int propagation = KDiscreteResource::TasksIntervals);
	/**
	 * Constructor
	 * @param p the problem
	 * @param starts the array of variables representing the starts of the tasks
	 * @param ends the array of variables representing the ends of the tasks
	 * @param durs the array of variables representing the durations of the tasks
	 * @param uses the array of variables representing the durations of the tasks
	 * @param sizes the array of variables representing the durations of the tasks
	 * @param C initial resource stock capacity
	 * @param propagation propagation scheme @see KDiscreteResource::PropagationHint
	 */
	KCumulativeResourceConstraint(KProblem & p, KIntVarArray &starts,KIntVarArray &ends,KIntVarArray &durs,KIntVarArray &uses,KIntVarArray &sizes,int C,int propagation = KDiscreteResource::TasksIntervals);
	/**
	 * Constructor
	 * @param p the problem
	 * @param starts the array of variables representing the starts of the tasks
	 * @param ends the array of variables representing the ends of the tasks
	 * @param durs the array of variables representing the durations of the tasks
	 * @param uses the array of variables representing the durations of the tasks
	 * @param sizes the array of variables representing the durations of the tasks
	 * @param C initial resource stock capacity
	 * @param propagation propagation scheme @see KDiscreteResource::PropagationHint
	 */
	KCumulativeResourceConstraint(KProblem & p, KIntVarArray &starts,KIntVarArray &ends,KIntVarArray &durs,KIntArray &prodsidx,KIntVarArray &prods,KIntVarArray &sizesp,KIntArray &consosidx,KIntVarArray &consos,KIntVarArray &sizesc,int C,int propagation = KDiscreteResource::TasksIntervals);
	/**
	 * Copy constructor
	 */
	KCumulativeResourceConstraint(const KCumulativeResourceConstraint & toCopy);
	// internal use
	void addTask(KTask_I * task);
	// Destructor
	virtual ~KCumulativeResourceConstraint();		
};

class KTimeTable;

class DLLIMPORTEXPORT KCumulativeResourceConstraintResourceUsage : public KConstraint { 
  public :
	/**
	 * Constructor for internal use only 	 
	 */
	KCumulativeResourceConstraintResourceUsage(KProblem & p, KIntVarArray &starts,KResource_I * r,int propagation = KDiscreteResource::TasksIntervals);
	/**
	 * Copy constructor
	 */
	KCumulativeResourceConstraintResourceUsage(const KCumulativeResourceConstraintResourceUsage & toCopy);
	
	// internal use
	void addTask(KTask_I * task);
	
	// Destructor
	virtual ~KCumulativeResourceConstraintResourceUsage();	

	KTimeTable * getTimeTable();

	void setTimeTable(KTimeTable * timeTable);
};



class DLLIMPORTEXPORT KTimeTable :
	public AbstractObject,
	public KCopiableObject<KTimeTable>,
	public KPtrArray<KTimeTable>
{
protected :

	KProblem * _problem;
	void * _timeTableIPtr;

public :

	KTimeTable(char * name,KProblem * problem,int timeIndexMin,int timeIndexMax);

	KTimeTable(KProblem * problem);

	KTimeTable(const KTimeTable & toCopy);

	virtual ~KTimeTable();

	void * getInternalPtr() const;
	void setInternalPtr(void * timeTableIPtr);

	KProblem * getProblem() const;

	KIntVar * getUsageVar(int timeIndex);

	KIntVar * getAvailableVar(int timeIndex);

	int getIndexMin();

	int getIndexMax();

};

#endif

