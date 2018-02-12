// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KBranchingScheme.h                                        *
// * Description : user class which represents a branching scheme (the*
// * way branching could be done during resolution)                   *
// ********************************************************************

#ifndef __BRANCHING_SCHEME_H
#define __BRANCHING_SCHEME_H

#include "Globals.h"

/**	 
* Abstract class for  Branching scheme .Search is made thanks to a tree search algorithm. At each node, propagation is made and if no solution exists, Artelys Kalis needs to split your problem in smaller subproblems covering (or not) all the initial problem. This partition is made following a branching scheme. 
Here is a figure explaining that :  
\image html BranchingScheme.gif
Different types of branching schemes exist. For example, a classical way is to choose a variable which has not been instantiated so far and to build a sub-problem for each
remaining value in the variable’s domains, this sub-problem being the original problem where the variable has been instantiated to this value. And then, you can continue the search with these new nodes.
Choosing the right branching schemes to be used with your particular problem could greatly improve the performance of the tree search. Artelys Kalis allows you to choose between many classical branching schemes provided with the library and to easily program yourself the more specialized branching schemes that you suppose to be useful for your own problems.
* @see KBranchingScheme
*	@see KAssignAndForbidd
*	@see KSplitDomain
* @see KSettleDisjunction
* @see KProbe
*	 @version 2013.1
*/

class DLLIMPORTEXPORT KBranchingScheme : public KAutoExternalObject<KBranchingScheme,KBranchingScheme_I>, public KPtrArray<KBranchingScheme> {
private:
	KProblem* _problem;

public:
	/// Constructor
	KBranchingScheme();

	/// Constructor with KProblem
	KBranchingScheme(KProblem* problem);

	/// Constructor with KBranchingScheme_I*
	KBranchingScheme(KBranchingScheme_I* branchingScheme);

	/// Copy constructor
	KBranchingScheme(const KBranchingScheme& toCopy);

	// Destructor
	virtual ~KBranchingScheme();

	//methods  

	KProblem* getProblem() const;

	/**
	Select the next object (variable in general) to branch on when one branch has been explored
	*/
	virtual void * selectNextBranchingObject(); 
	/**
	return true IFF branching is completed on one specific branch of the branch and bound
	@param branchingObject the branching object
	@param branchingInformation the branching information
	@param currentBranchNumber the current branch number
	*/
	virtual bool finishedBranching(void* branchingObject,void* branchingInformation,int currentBranchNumber);
	/**
	return the next branch
	@param branchingObject the branching object
	@param branchingInformation the branching information
	@param currentBranchNumber the current branch number
	*/
	virtual void * getNextBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);
	/**
	this method is called once a branch has been selected and a decision must be taken
	@param branchingObject the branching object
	@param branchingInformation the branching information
	@param currentBranchNumber the current branch number
	*/
	virtual void goDownBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);
	/**
	this method is called upon backtrack on a specific branch
	@param branchingObject the branching object
	@param branchingInformation the branching information
	@param currentBranchNumber the current branch number
	*/
	virtual void goUpBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);  
	virtual void freeAllocatedObjectsForBranching(void* branchingObject,void* branchingInformation);
	/// Pretty printing of the branching scheme
	virtual void printName();
	virtual char* getName();

	virtual KBranchingScheme* getCopyPtr() const;
	virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

	//#ifdef ACK_LIB
	virtual void setSolver_I_ptr(KSolver_I* solver_I);
	//#endif
}; // class KBranchingScheme

#endif

