// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KMaxKMin.h                                                *
// ********************************************************************

#ifndef __KMAX_KMIN_H
#define __KMAX_KMIN_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
  * This class creates a vMax = max(X1,X2,...,Xn) constraint
  @code
    Example :

	KIntVarArray X(...);
	KIntVar maxOfX(...);
	...
	problem.post(KMax("maxOfX=max(X)",maxOfX,X));
	...
  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KMax : public KConstraint { 
  public :
	/** Primary constructor of vMax = max(X1,X2,...,Xn) 
	    @param vMax variable to be the max of vars
		@param vars variable array whose maximum is seeked
    */	  
	KMax(char * name,KIntVar & vMax,KIntVarArray &vars,bool boundConsistency=false);
	/** Primary constructor of vMax = max(X1,X2,...,Xn) 
	    @param vMax variable to be the max of vars
		@param vars variable array whose maximum is seeked
    */	  
	KMax(char * name,KNumVar & vMax,KNumVarArray &vars);
	/// Copy Constructor 
	KMax(const KMax & toCopy);
	// Destructor
	virtual ~KMax();	
    virtual void * getConstraintIPtr();
};

/**	 
  * This class creates a vMin = min(X1,X2,...,Xn) constraint
  @code
    Example :

	KIntVarArray X(...);
	KIntVar minOfX(...);
	...
	problem.post(KMin("minOfX=max(X)",minOfX,X));
	...
  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KMin : public KConstraint { 
  public :
	/** Primary constructor of vMin = min(X1,X2,...,Xn) 
	    @param vMin variable to be the max of vars
		@param vars variable array whose maximum is seeked
    */	    
	KMin(char * name,KIntVar & vMin,KIntVarArray &vars,bool boundConsistency=false);
	/** Primary constructor of vMin = min(X1,X2,...,Xn) 
	    @param vMin variable to be the max of vars
		@param vars variable array whose maximum is seeked
    */	    
	KMin(char * name,KNumVar & vMin,KNumVarArray &vars);
	/// Copy Constructor 
	KMin(const KMin & toCopy);
	// Destructor
	virtual ~KMin();	
    virtual void * getConstraintIPtr();
};





#endif


