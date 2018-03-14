// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KAllDifferent.h          				                  *
// * Implementation of difference constraint                          *
// ********************************************************************

#ifndef __KALLDIFFERENT_H
#define __KALLDIFFERENT_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
  * This class creates a <pre>X1 <> X2 <> ... <></pre> Xn constraint
  @code
    Example :

	KIntVarArray X(...);
	...
	// Strong propagation 
	problem.post(KAllDifferent("allDiff(X)",X,KAllDifferent::GENERALIZED_ARC_CONSISTENCY));
	  or 
	// weak propagation but faster
	problem.post(KAllDifferent("allDiff(X)",X,KAllDifferent::FORWARD_CHECKING));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KAllDifferent : public KConstraint { 
  public :
	/** Primary constructor for the all different constraint
	    @param name for pretty printing of the constraint
		@param vars  array of variables that must take different values
		@param propagationLevel FORWARD_CHECKING for forward checking propagation 
		@param propagationLevel BOUND_CONSISTENCY for bound consistency propagation
		@param propagationLevel GENERALIZED_ARC_CONSISTENCY	for generalized arc consistency propagation
		@param propagationLevel USING_GCC for a similar propagation, internally using a Global Cardinality Constraint
    */	  
	KAllDifferent(char * name,KIntVarArray &vars,int propagationLevel=FORWARD_CHECKING);
	/// Copy constructor
	KAllDifferent(const KAllDifferent & toCopy);
	// Destructor
	virtual ~KAllDifferent();	
	// Differents level of propagation for the constraints
	enum PropagationLevel{
		FORWARD_CHECKING,
		BOUND_CONSISTENCY,
		GENERALIZED_ARC_CONSISTENCY,
		USING_GCC
	};
};

#endif

