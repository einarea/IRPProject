// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KACBinConstraint.h                                        *
// ********************************************************************

#ifndef __KAC_BIN_CONSTRAINT_H
#define __KAC_BIN_CONSTRAINT_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
  * This class implements a generic class for propagation of any binary constraint by local 2-consistency (arc consistency) \\
    Two algorithms (AC3 and AC2001) are available for propagation of the constraint.	

  @code
    Example : X == Y + C

    class XEqualYC : public KACBinConstraint {
      int _C;
      public:
       XEqualYC(char * name,KIntVar & v1,KIntVar & v2,int cst) 
            : KACBinConstraint(v1,v2,KACBinConstraint::ALGORITHM_AC2001,"XEqualYC") {
          _C = cst;
       }
       virtual bool testIfSatisfied(int valX,int valY) {
          return (valX == valY + _C);	// the constraint is true if only iff valX == valY + C			
       }
    };
  @endcode	
  * @see KACBinTableConstraint
  * @see KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KACBinConstraint : public KConstraint {
public :
	/** Constructor.
     * This constructor takes threee arguments 	        
	 * @param v1 the first variable of the constraint
     * @param v2 the second variable of the constraint
	 * @param acAlgorithm ALGORITHM_AC2001 (default value) for propagation by the AC2001 algorithm , ALGORITHM_AC3 for propagation by the AC3 algorithm 
	 * @param name label for pretty printing of the constraint
     */
	KACBinConstraint(KIntVar& v1, KIntVar& v2, int acAlgorithm = ALGORITHM_AC2001, char * name=0);
	// Copy Constructor
	KACBinConstraint(const KACBinConstraint &toCopy);
	// Destructor
	virtual ~KACBinConstraint();
	// Internal use
	KConstraint* getInstance(int pb) const;
	/** 
	 * Abstract interface for generic propagation of any binary constraint
	 * @return true if and only if the constraint is satisfied when v1 == val1 & v2 == val2
	 */
	virtual bool testIfSatisfied(int val1,int val2);
	/**
	 * Virtual copy method.
	 * Each modeling elements stored (and used) in the binary constraint must be copied using the KProblem::getCopyPtr() method.
	 */
	virtual KACBinConstraint* getInstanceCopyPtr(const KProblem& problem) const;
	virtual void print();
	virtual void print(void*ctx,PrintFunctionPtr*pfp);
	enum acAlgorithms{
		ALGORITHM_AC3,
		ALGORITHM_AC2001
	};
};

/**	 
  * This class implements a generic class for propagation of any nary constraint by forward checking/arc consistency or generalized arc consistency \\    
  * @see KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KGeneralizedArcConsistencyConstraint : public KConstraint {  
  public :		
	/** Constructor.
      This constructor takes threee arguments 	        
	  @param vars an array of variables      
	  @param acAlgorithm ALGORITHM_AC2001 (default value) for propagation by the AC2001 algorithm , ALGORITHM_AC3 for propagation by the AC3 algorithm 
	  @param name label for pretty printing of the constraint
    */
	KGeneralizedArcConsistencyConstraint(KIntVarArray &vars,int acAlgorithm = GENERALIZED_ARC_CONSISTENCY,char * name=0);
	/// Copy Constructor
	KGeneralizedArcConsistencyConstraint(const KGeneralizedArcConsistencyConstraint &toCopy);
	/// Destructor
	virtual ~KGeneralizedArcConsistencyConstraint();
	// Internal use
	KConstraint* getInstance(int pb) const;
	/** Abstract Interface for generic propagation of any binary constraint
		@return true if and only if the constraint is satisfied when v1 == val1 & v2 == val2
	*/
	virtual bool testIfSatisfied(int * tuple);	// return true if the constraint holds 
	/// Pretty printing of the constraint
	virtual void print();
	virtual void print(void*ctx,PrintFunctionPtr*pfp);
	/// Possible propagation algorithms
	enum acAlgorithms{
		GENERALIZED_ARC_CONSISTENCY,
		ARC_CONSISTENCY,
		FORWARD_CHECKING
	};
};

#endif


