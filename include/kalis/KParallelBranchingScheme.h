// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KParallelBranchingScheme.h                                *
// * Description : class implementing a parallel branching scheme     *
// * allows splitting search work between several solvers             *
// ********************************************************************

#ifndef __KPARALLELBRANCHINGSCHEME_H
#define __KPARALLELBRANCHINGSCHEME_H

#include "Globals.h"
#include "KBranchingScheme.h"

/**	 
  * KParallelBranchingScheme Branching scheme  

   \image html KParallelBranchingScheme.png
     @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KParallelBranchingScheme(KSplitDomain(KSmallestDomain_I(),KMaxToMin_I()));

  @endcode	
  * @see KBranchingScheme
  *	@see KAssignVar
  *	@see KAssignAndForbid
  * @see KSettleDisjunction
  * @see KProbe
  * @see KSplitDomain
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KParallelBranchingScheme : public KBranchingScheme {

public:
    /**
	 Constructor
	 @param branchingScheme internal branching scheme used for parallel branching among multiple workers
	*/
    KParallelBranchingScheme(const KBranchingSchemeArray& branchingSchemeArray, int maxDepth);

    // Constructor with KSplitDomain_I*
    KParallelBranchingScheme(KParallelBranchingScheme_I* parallelBranchingScheme);

	// Copy constructor
    KParallelBranchingScheme(const KParallelBranchingScheme& toCopy);

    // Destructor
    virtual ~KParallelBranchingScheme();

    // Methods
    virtual KBranchingScheme* getCopyPtr() const;

	virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

}; // class KParallelBranchingScheme

#endif


