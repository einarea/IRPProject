// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KAbs.h                                                    *
// ********************************************************************

#ifndef __KABS_H
#define __KABS_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
  * This class creates a X = abs(Y) constraint
  @code
    Example :

	KIntVar X(...);
	KIntVar Y(...);
	...
	problem.post(KAbs("X=|Y|",X,Y));	
	...
  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/

class DLLIMPORTEXPORT KAbs : public KConstraint { 
  public :		

    /** Constructor.
      This constructor takes threee arguments	  
      @param name label for pretty printing of the constraint
	  @param X the 'X' variable of the constraint
      @param Y the 'Y" variable of the constraint
    */
	KAbs(char * name,KIntVar & X,KIntVar & Y);

    /** Constructor.
      This constructor takes threee arguments	  
      @param name label for pretty printing of the constraint
	  @param X the 'X' variable of the constraint
      @param Y the 'Y" variable of the constraint
    */
	KAbs(char * name,KNumVar & X,KNumVar & Y);
	/// Copy Constructor 
	KAbs(const KAbs & toCopy);
	
	virtual ~KAbs();	

	// Propagation level of the constraint		
	enum PropagationLevel {
		BOUND_CONSISTENCY,
		ARC_CONSISTENCY
	};
};

#endif
