// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KAssignVar.h                                              *
// * Description : class implementing a branching scheme looking for  *
// * a variable and making one branch for each value of the variable  *
// ********************************************************************

#ifndef __KASSIGNVAR_H
#define __KASSIGNVAR_H

/**	 
  * AssignVar Branching scheme    
     
   \image html AssignVar.gif
   
 @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KSmallestDomain(),KMaxToMin());

  @endcode	
  
  * @see KBranchingScheme
  *	@see KAssignVar
  *	@see KAssignAndForbid
  * @see KSettleDisjunction
  * @see KProbe
  * @see KSplitDomain
  *	 @version 2013.1
*/
class  DLLIMPORTEXPORT KAssignVar : public KBranchingScheme  {
  public:

	// Constructors
	KAssignVar();
	/** Constructor
	    @param varSel the variable selector
    */	  
	KAssignVar(const KVariableSelector& vs);
	/** Constructor
	    @param varSel the variable selector
		@param valSel the value selector
    */	  
	KAssignVar(const KVariableSelector& varSel, const KValueSelector& valSel);
	/** Constructor
	    @param varSel the variable selector
		@param valSel the value selector
		@param intVarArray the array of variable to assign value
    */	  
	KAssignVar(const KVariableSelector& varSel, const KValueSelector& valSel, const KIntVarArray& intVarArray);

	// Constructor with KAssignVar_I*
	KAssignVar(KAssignVar_I* assignVar);

	// Copy constructor
	KAssignVar(const KAssignVar& assignVarToCopy);

	// Destructor
	virtual ~KAssignVar();

	// 'set' methods
//	int setVariableSelector(KVariableSelector_I* vs);

	//methods
	//virtual void* selectNextBranchingObject(); // get Next Branching Object
	//virtual bool finishedBranching(void* branchingObject,void* branchingInformation,int currentBranchNumber);
	//virtual void* getNextBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);
	//virtual void goDownBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);
	//virtual void goUpBranch(void* branchingObject,void* branchingInformation,int currentBranchNumber);
	//virtual void freeAllocatedObjectsForBranching(void* branchingObject,void* branchingInformation);
	//virtual void printName();
	//virtual char * getName();
	virtual KBranchingScheme* getCopyPtr() const;

	virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

	//#ifdef ACK_LIB
	//	KAssignVar_I * getAssignVar_I_ptr() const {return _assignVar_I_ptr;}  
	//	virtual void setSolver_I_ptr(KSolver_I* solver_I);
	//#endif  
}; // class KAssignVar

#endif

