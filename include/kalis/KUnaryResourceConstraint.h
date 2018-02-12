// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KUnaryResourceConstraint.h          				      *
// * Implementation of difference constraint                          *
// ********************************************************************

#ifndef __KUnaryResourceConstraint_H
#define __KUnaryResourceConstraint_H

#include "Globals.h"
#include "KConstraint.h"
#include "KResource.h"

/**	 
 * This constraint states that some tasks are not overlapping chronologically.
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
 *	 @version 2013.1
 */
class DLLIMPORTEXPORT KUnaryResourceConstraint : public KConstraint { 
  public :	
	/**
	 * Constructor 
	 * @param name name of this constraint
	 * @param startsp array of starts variables 	 
	 * @param propagation propagation scheme @see KUnaryResource::PropagationHint
	 */
	KUnaryResourceConstraint(char * name,KIntVarArray &startsp,int propagation = KUnaryResource::Disjunctions | KUnaryResource::TasksIntervals);
	/// Copy constructor
	KUnaryResourceConstraint(const KUnaryResourceConstraint & toCopy);
	// Destructor
	virtual ~KUnaryResourceConstraint();	
	/// Differents level of propagation for the constraints
	enum PropagationLevel{
		FORWARD_CHECKING,
		BOUND_CONSISTENCY,
		GENERALIZED_ARC_CONSISTENCY,
		USING_GCC
	};
	// internal use
	void addTask(KTask_I * task);
};


class DLLIMPORTEXPORT KTasksRankConstraint : public KConstraint { 
  public :	
	/**
	 * Constructor 
	 */
	KTasksRankConstraint(char * name,KIntVarArray&ranks, KTaskArray&tasks);
	/// Copy constructor
	KTasksRankConstraint(const KTasksRankConstraint & toCopy);
	// Destructor
	virtual ~KTasksRankConstraint();		
};


#endif

