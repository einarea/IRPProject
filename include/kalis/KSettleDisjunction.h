// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSettleDisjunction.h                                      *
// ********************************************************************

#ifndef __KSETTLEDISJUNCTION_H
#define __KSETTLEDISJUNCTION_H

#include "Globals.h"
#include "KBranchingScheme.h"

/**	 
  * KSettleDisjunction Branching scheme 

   \image html SettleDisjunction.png
  
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KSettleDisjunction();

  @endcode	 
  * @see KBranchingScheme
  *	@see KAssignVar
  *	@see KAssignAndForbid
  * @see KSettleDisjunction
  * @see KProbe
  * @see KSplitDomain
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KSettleDisjunction : public KBranchingScheme {

 public:
  
  /// Primary constructor
  KSettleDisjunction(KDisjunctionSelector * disjunctionSelector = NULL);
  /**
   Constructor
   @param disjunctionArray the disjunction array to branch on
  */
  KSettleDisjunction(const KDisjunctionArray &disjunctionArray,KDisjunctionSelector * disjunctionSelector = NULL);

  // Constructor with KSettleDisjunction_I*
  KSettleDisjunction(KSettleDisjunction_I* settleDisjunction);

  // Copy constructor
  KSettleDisjunction(const KSettleDisjunction& toCopy);

  // Destructor
  virtual ~KSettleDisjunction();

  //methods
  //virtual void* selectNextBranchingObject(); 
  //virtual void goDownBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);
  //virtual void goUpBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);
  //virtual bool finishedBranching(void* branchingObject,void* branchingInformation,int currentBranchNumber);

  virtual KBranchingScheme* getCopyPtr() const;

  virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

  //#ifdef ACK_LIB
  //KSettleDisjunction_I *getSettleDisjunction_I_ptr() const {return _settleDisjunction_I_ptr;};
  //virtual void setSolver_I_ptr(KSolver_I *solver_I_ptr);
  //#endif
  
}; // class KSettleDisjunction

#endif


