// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSplitDomain.h                                            *
// * Description : class implementing a branching scheme looking for a*
// * variable V. Makes one branch where the V <= X and another one    *
// * where V > X, where X is in the middle of the domain of V         *
// ********************************************************************

#ifndef __KSPLITDOMAIN_H
#define __KSPLITDOMAIN_H

#include "Globals.h"
#include "KBranchingScheme.h"

/**	 
  * SplitDomain Branching scheme  

   \image html SplitDomain.png
     @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KSplitDomain(KSmallestDomain(),KMaxToMin());

  @endcode	
  * @see KBranchingScheme
  *	@see KAssignVar
  *	@see KAssignAndForbid
  * @see KSettleDisjunction
  * @see KProbe
  * @see KSplitDomain
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KSplitDomain : public KBranchingScheme {

  protected:

    KSplitDomain_I *_splitDomain_I_ptr;

  public:

    /**
	 Constructor
	 @param lowerPartFirst explore the lower part of the domain first (default value = true)
	 @param minDomainSize under a size of 'minDomainSize' for the domain of the variable , no split occur	 
	*/
    KSplitDomain(const bool lowerPartFirst = true, const int minDomainSize = MIN_DOMAIN_SIZE_FOR_SPLIT_DOMAIN,const int discrepancyLimit=MAX_INT);
	/**
	 Constructor
	 @param varSel the variable selector
	 @param lowerPartFirst explore the lower part of the domain first (default value = true)
	 @param minDomainSize under a size of 'minDomainSize' for the domain of the variable , no split occur	 
	 @param 
	*/
    KSplitDomain(const KVariableSelector &varSel, const bool lowerPartFirst = true, const int minDomainSize = MIN_DOMAIN_SIZE_FOR_SPLIT_DOMAIN,const int discrepancyLimit=MAX_INT );
	/**
	 Constructor
	 @param varSel the variable selector
	 @param valSel the value selector
	 @param lowerPartFirst explore the lower part of the domain first (default value = true)
	 @param minDomainSize under a size of 'minDomainSize' for the domain of the variable , no split occur	 
	 @param 
	*/
    KSplitDomain(const KVariableSelector& varSel, const KValueSelector& valSel, const bool lowerPartFirst = true, const int minDomainSize = MIN_DOMAIN_SIZE_FOR_SPLIT_DOMAIN,const int discrepancyLimit=MAX_INT );
	/**
	 Constructor
	 @param varSel the variable selector
	 @param valSel the value selector
	 @param intVarArray the variable array to branch on
	 @param lowerPartFirst explore the lower part of the domain first (default value = true)
	 @param minDomainSize under a size of 'minDomainSize' for the domain of the variable , no split occur	 
	 @param 
	*/
    KSplitDomain(const KVariableSelector &varSel, const KValueSelector& valSel, const KIntVarArray &intVarArray, const bool lowerPartFirst = true, const int minDomainSize = MIN_DOMAIN_SIZE_FOR_SPLIT_DOMAIN,const int discrepancyLimit=MAX_INT );

    // Constructor with KSplitDomain_I*
    KSplitDomain(KSplitDomain_I* splitDomain);

	// Copy constructor
    KSplitDomain(const KSplitDomain &splitDomainToCopy);

    // Destructor
    virtual ~KSplitDomain();

    // Methods
 //   virtual void* selectNextBranchingObject(); 
 //   virtual void* getNextBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);
 //   void goDownBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);
	//void goUpBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);
 //   virtual void freeAllocatedObjectsForBranching(void* branchingObject,void* branchingInformation);
	//virtual bool finishedBranching(void* branchingObject,void* branchingInformation,int currentBranchNumber);

 //   virtual void printName();
	//virtual char * getName();
    virtual KBranchingScheme* getCopyPtr() const;

	virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

  //  #ifdef ACK_LIB
		//KSplitDomain_I *getSplitDomain_I_ptr() const {return _splitDomain_I_ptr;};
		//virtual void setSolver_I_ptr(KSolver_I *solver_I_ptr);
  //  #endif


}; // class KSplitDomain

#endif


