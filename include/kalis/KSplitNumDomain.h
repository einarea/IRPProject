// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSplitNumDomain.h                                            *
// * Description : class implementing a branching scheme looking for a*
// * variable V. Makes one branch where the V <= X and another one    *
// * where V > X, where X is in the middle of the domain of V         *
// ********************************************************************

#ifndef __KSplitNumDomain_H
#define __KSplitNumDomain_H

#include "Globals.h"
#include "KBranchingScheme.h"

/**	 
  * SplitDomain Branching scheme  

  \image html SplitDomain.png  

  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KSplitNumDomain(KSmallestDomain(),KMaxToMin());

  @endcode	
  * @see KBranchingScheme
  *	@see KAssignVar
  *	@see KAssignAndForbid
  * @see KSettleDisjunction
  * @see KProbe
  * @see KSplitNumDomain
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KSplitNumDomain : public KBranchingScheme {

  public:

    /**
	 Constructor
	 @param lowerPartFirst explore the lower part of the domain first (default value = true)
	 @param minDomainSize under a size of 'minDomainSize' for the domain of the variable , no split occur	 
	*/
    KSplitNumDomain(const bool lowerPartFirst = true, const int minDomainSize = MIN_DOMAIN_SIZE_FOR_SPLIT_DOMAIN);
	/**
	 Constructor
	 @param varSel the variable selector
	 @param lowerPartFirst explore the lower part of the domain first (default value = true)
	 @param minDomainSize under a size of 'minDomainSize' for the domain of the variable , no split occur	 
	 @param 
	*/
    KSplitNumDomain(const KNumVariableSelector &varSel, const bool lowerPartFirst = true, const int minDomainSize = MIN_DOMAIN_SIZE_FOR_SPLIT_DOMAIN);
	/**
	 Constructor
	 @param varSel the variable selector
	 @param valSel the value selector
	 @param lowerPartFirst explore the lower part of the domain first (default value = true)
	 @param minDomainSize under a size of 'minDomainSize' for the domain of the variable , no split occur	 
	 @param 
	*/
    KSplitNumDomain(const KNumVariableSelector& varSel, const KNumValueSelector& valSel, const bool lowerPartFirst = true, const int minDomainSize = MIN_DOMAIN_SIZE_FOR_SPLIT_DOMAIN);
	/**
	 Constructor
	 @param varSel the variable selector
	 @param valSel the value selector
	 @param intVarArray the variable array to branch on
	 @param lowerPartFirst explore the lower part of the domain first (default value = true)
	 @param minDomainSize under a size of 'minDomainSize' for the domain of the variable , no split occur	 
	 @param 
	*/
    KSplitNumDomain(const KNumVariableSelector &varSel, const KNumValueSelector& valSel, const KNumVarArray &intVarArray, const bool lowerPartFirst = true, const int minDomainSize = MIN_DOMAIN_SIZE_FOR_SPLIT_DOMAIN);

    // Constructor with KSplitNumDomain_I*
    KSplitNumDomain(KSplitNumDomain_I* splitNumDomain);

	// Copy constructor
    KSplitNumDomain(const KSplitNumDomain& splitDomainToCopy);

    // Destructor
    virtual ~KSplitNumDomain();

    // Methods
    virtual KBranchingScheme* getCopyPtr() const;

	virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

}; // class KSplitNumDomain

#endif


