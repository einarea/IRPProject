// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KAssignAndForbid.h                                        *
// * Description : class implementing a branching scheme looking      *
// * for a variable V and a value X.                                  *
// * Makes one branch where the V = X and another one where V != X    *
// ********************************************************************

#ifndef __KASSIGNANDFORBID_H
#define __KASSIGNANDFORBID_H

#include "KBranchingScheme.h"

/**	 
  * AssignAndForbid branching scheme   
  
	\image html AssignAndForbid.gif
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignAndForbid(KSmallestDomain(),KMaxToMin());

  @endcode	
  
  * @see KBranchingScheme
  *	@see KAssignVar
  *	@see KAssignAndForbid
  * @see KSettleDisjunction
  * @see KProbe
  * @see KSplitDomain
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KAssignAndForbid : public KBranchingScheme {
  public:

    /// Default Constructor
    KAssignAndForbid(const int discrepancyLimit=MAX_INT);
	/** 
		Constructor
	    @param vs the variable selector
    */	  
    KAssignAndForbid(const KVariableSelector& vs,const int discrepancyLimit=MAX_INT);
	/** 
		Constructor
	    @param vs the variable selector
		@param valS the value selector
    */	  
    KAssignAndForbid(const KVariableSelector& vs, const KValueSelector& valS,const int discrepancyLimit=MAX_INT);
	/** Constructor
	    @param vs the variable selector
		@param valS the value selector
		@param intVarArray the array of variable to assign value
    */	  
    KAssignAndForbid(const KVariableSelector& vs, const KValueSelector& valS, const KIntVarArray &intVarArray,const int discrepancyLimit=MAX_INT);

    /// Constructor with KAssignAndForbid_I*
    KAssignAndForbid(KAssignAndForbid_I* assignAndForbid);

	// Copy constructor
    KAssignAndForbid(const KAssignAndForbid &toCopy);

    // Destructor
    virtual ~KAssignAndForbid();

    // Methods
	//virtual bool finishedBranching(void* branchingObject,void* branchingInformation,int currentBranchNumber);
 //   virtual void* selectNextBranchingObject(); // get Next Branching Object    
 //   virtual void* getNextBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);
 //   virtual void goDownBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);    
	//virtual void goUpBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);    
 //   virtual void freeAllocatedObjectsForBranching(void* branchingObject,void* branchingInformation);
 //   virtual void printName();
	//virtual char * getName();
    virtual KBranchingScheme* getCopyPtr() const;

	virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

  //  #ifdef ACK_LIB
		//virtual void setSolver_I_ptr(KSolver_I *solver_I_ptr);
  //  #endif
}; // class KAssignAndForbid

#endif

