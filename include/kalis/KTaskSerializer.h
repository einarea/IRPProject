// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KTaskSerializer.h                                         *
// ********************************************************************

#ifndef __TASK_SERIALIZER_H
#define __TASK_SERIALIZER_H

#include "Globals.h"
#include "KBranchingScheme.h"

/** 
    Abstract interface class for task selection heuristic
	
	A custom scheduling optimization strategy can be specified by using the KTaskSerializer branching scheme to select the task to be scheduled and value choice heuristics for its start and duration variables.

    \image html taskSerializer.png	
	
	@see KSmallestEarliestStartTime
	@see KSmallestEarliestCompletionTime
	@see KLargestEarliestStartTime
	@see KLargestEarliestCompletionTime
	@see KSmallestLatestStartTime
	@see KSmallestLatestCompletionTime
	@see KLargestLatestStartTime
	@see KLargestLatestCompletionTime
	  @version 2013.1
*/
class DLLIMPORTEXPORT KTaskSerializer : public KBranchingScheme { 	
public:
	/**
	* Variable branching orders for each task
	*/
	enum varOrder{
		AFF_DUR_START=0,	///< Variable branching order: <ol><li>affectations <li>duration <li>start time</ol>
		AFF_START_DUR=1,	///< Variable branching order: <ol><li>affectations <li>start time <li>duration</ol>
		DUR_START_AFF=2,	///< Variable branching order: <ol><li>duration <li>start time <li>affectations</ol>
		DUR_AFF_START=3,	///< Variable branching order: <ol><li>duration <li>affectations <li>start time</ol>
		START_DUR_AFF=4,	///< Variable branching order: <ol><li>start time <li>duration <li>affectations</ol>
		START_AFF_DUR=5		///< Variable branching order: <ol><li>start time <li>affectations <li>duration</ol>
	};		
	/// Primary constructor	
	KTaskSerializer(KTaskArray & tasks,int discrepancyLimit=MAX_INT,varOrder varOrder=DUR_START_AFF);	
	/// Constructor with task ,start and duration selector and discrepancy limit
	KTaskSerializer(const KTaskArray& tasks, const KTaskSelector& tsel, const KValueSelector& durationSelector, const KValueSelector& startSelector, int discrepancyLimit=MAX_INT, varOrder varOrder=DUR_START_AFF);	
	/// Constructor with KTaskSerializer_I*
	KTaskSerializer(KTaskSerializer_I* taskSerializer);	
	/// Copy constructor
	KTaskSerializer(const KTaskSerializer & taskSerializer);	
	/// Destructor
	virtual ~KTaskSerializer();
	/// Get a copy pointer
	virtual KBranchingScheme* getCopyPtr() const;
	
	virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

};



#endif
