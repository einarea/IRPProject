// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KArithmeticConstraints.h                                  *
// ********************************************************************

#ifndef __KARITHMETIC_CONSTRAINTS_H
#define __KARITHMETIC_CONSTRAINTS_H

#include "Globals.h"
#include "KConstraint.h"

// ********************************************************************
//
//                UNARY ARITHMETIC CONSTRAINTS X op (int)
// 
// ********************************************************************

/**	 
  * This class creates a  X == C constraint
  @code
    Example :

	KIntVar X(...);
	...
	problem.post(X == 5);

	  or 

	problem.post(KEqualXc(X,5));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KEqualXc : public KConstraint { 
  public :
	/** Primary constructor of v1 == C 
	    @param v1 the variable 
		@param c the constant
    */	 
	KEqualXc(const KIntVar &v1,int c);
	/// Copy constructor
	KEqualXc(const KEqualXc & toCopy);
	// Destructor
	virtual ~KEqualXc();	
};

/**	 
  * This class creates a  X != C constraint
  @code
    Example :

	KIntVar X(...);	
	...
	problem.post(X != 5);

	  or 

	problem.post(KNotEqualXc(X,5));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNotEqualXc : public KConstraint { 
  public :
	/** Primary constructor of v1 != C 
	    @param v1 the variable 
		@param c the constant
    */	 
	KNotEqualXc(const KIntVar &v1,int c);
	/// Copy constructor
	KNotEqualXc(const KNotEqualXc & toCopy);
	// Destructor
	virtual ~KNotEqualXc();	

};

/**	 
  * This class creates a  X >= C  constraint
  @code
    Example :

	KIntVar X(...);
	...
	problem.post(X >= 3);

	  or 

	problem.post(KGreaterOrEqualXc(X,3));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KGreaterOrEqualXc : public KConstraint { 
  public :
	/** Primary constructor of v1 >= C 
	    @param v1 the variable 
		@param c the constant
    */	 	  
	KGreaterOrEqualXc(const KIntVar &v1,int c);
	/// Copy constructor
	KGreaterOrEqualXc(const KGreaterOrEqualXc & toCopy);
	// Destructor
	virtual ~KGreaterOrEqualXc();	
 
};

/**	 
  * This class creates a  X <= C  constraint
  @code
    Example :

	KIntVar X(...);
	...
	problem.post(X <= 3);

	  or 

	problem.post(KLessOrEqualXc(X,3));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KLessOrEqualXc : public KConstraint { 
  public :	
	/** Primary constructor of v1 <= C 
	    @param v1 the variable 
		@param c the constant
    */	 	  
	KLessOrEqualXc(const KIntVar &v1,int c);
	/// Copy constructor
	KLessOrEqualXc(const KLessOrEqualXc & toCopy);
	// Destructor
	virtual ~KLessOrEqualXc();	
 
};


// ********************************************************************
//
//            BINARY ARITHMETIC CONSTRAINTS X op Y + (int)
// 
// ********************************************************************


/**	 
  * This class creates a  X == Y + C constraint
  @code
    Example :

	KIntVar X(...);
	KIntVar Y(...);
	...
	problem.post(X == Y + 5);

	  or 

	problem.post(KEqualXyc(X,Y,5));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KEqualXyc : public KConstraint { 
  public :
	/** Primary constructor of v1 == v2 + C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param c the constant
    */	 
	KEqualXyc(const KIntVar &v1,const KIntVar &v2,int c);
	/// Copy constructor
	KEqualXyc(const KEqualXyc & toCopy);
	// Destructor
	virtual ~KEqualXyc();	

};

/**	 
  * This class creates a  X <> Y + C constraint
  @code
    Example :

	KIntVar X(...);
	KIntVar Y(...);
	...
	problem.post(X != Y + 5);

	  or 

	problem.post(KNotEqualXyc(X,Y,5));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNotEqualXyc : public KConstraint { 
  public :
	/** Primary constructor of v1 != v2 + C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param c the constant
    */	
	KNotEqualXyc(const KIntVar &v1,const KIntVar &v2,int c);
	/// Copy constructor
	KNotEqualXyc(const KNotEqualXyc & toCopy);
	// Destructor
	virtual ~KNotEqualXyc();	
 
};

/**	 
  * This class creates a  X >= Y + C  constraint
  @code
    Example :

	KIntVar X(...);
	KIntVar Y(...);
	...
	problem.post(X >= Y + 3);

	  or 

	problem.post(KGreaterOrEqualXyc(X,Y,3));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KGreaterOrEqualXyc : public KConstraint { 
  public :	
	/** Primary constructor of v1 >= v2 + C
	    @param v1 the v1 variable
		@param v2 the v2 variable
		@param c the constant
    */	
	KGreaterOrEqualXyc(const KIntVar &v1,const KIntVar &v2,int c);
	/// Copy constructor
	KGreaterOrEqualXyc(const KGreaterOrEqualXyc & toCopy);
	// Destructor
	virtual ~KGreaterOrEqualXyc();	
 
};

/**	 
  * This class creates a  abs(X-Y) <= C  constraint
  @code
    Example :

	KIntVar X(...);
	KIntVar Y(...);
	...
	problem.post(KDistanceLowerThanXyc(X,Y,3));	// |X-Y| <= 3

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KDistanceLowerThanXyc : public KConstraint { 
  public :	
	/** Primary constructor of abs(v1-v2) <= C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param c the constant
    */	
	KDistanceLowerThanXyc(const KIntVar &v1,const KIntVar &v2,int c);
	/// Copy Constructor
	KDistanceLowerThanXyc(const KDistanceLowerThanXyc & toCopy);
	// Destructor
	virtual ~KDistanceLowerThanXyc();	
 
};

/**	 
  * This class creates a  abs(X-Y) >= C  constraint
  @code
    Example :

	KIntVar X(...);
	KIntVar Y(...);
	...
	problem.post(KDistanceGreaterThanXyc(X,Y,3));	// |X-Y| >= 3

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KDistanceGreaterThanXyc : public KConstraint { 
  public :
	/** Primary constructor of abs(v1-v2) >= C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param c the constant
    */	  
	KDistanceGreaterThanXyc(const KIntVar &v1,const KIntVar &v2,int c);
	/// Copy constructor
	KDistanceGreaterThanXyc(const KDistanceGreaterThanXyc & toCopy);
	// Destructor
	virtual ~KDistanceGreaterThanXyc();	
};

/**	 
  * This class creates a  abs(X-Y) == C  constraint
  @code
    Example :

	KIntVar X(...);
	KIntVar Y(...);
	...
	problem.post(KDistanceEqualXyc(X,Y,3));	// |X-Y| == 3

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KDistanceEqualXyc : public KConstraint { 
  public :
	/** Primary constructor of abs(v1-v2) == C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param c the constant
    */	  
	KDistanceEqualXyc(const KIntVar &v1,const KIntVar &v2,int c);
	/// Copy constructor
	KDistanceEqualXyc(const KDistanceEqualXyc & toCopy);
	// Destructor
	virtual ~KDistanceEqualXyc();	
 
};

/**	 
  * This class creates a  abs(X-Y) != C  constraint
  @code
    Example :

	KIntVar X(...);
	KIntVar Y(...);
	...
	problem.post(KDistanceNotEqualXyc(X,Y,3));	// |X-Y| != 3

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/

class DLLIMPORTEXPORT KDistanceNotEqualXyc : public KConstraint { 
  public :
	/** Primary constructor of abs(v1-v2) != C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param c the constant
    */	  	  
	KDistanceNotEqualXyc(const KIntVar &v1,const KIntVar &v2,int c);
	/// Copy constructor
	KDistanceNotEqualXyc(const KDistanceNotEqualXyc & toCopy);
	// Destructor
	virtual ~KDistanceNotEqualXyc();	
 
};

// ********************************************************************
//
//          TERNARY ARITHMETIC CONSTRAINTS X op Y op Z + (int)
// 
// ********************************************************************

/**	 
  * This class creates a  X == Y - Z constraint
  @code
    Example :

	KIntVar X(...);
	KIntVar Y(...);
	KIntVar Z(...);
	...
	problem.post(X == Y - Z);

	  or 

	problem.post(KXEqualYMinusZ(X,Y,Z));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KXEqualYMinusZ : public KConstraint { 
  public :	  
	KXEqualYMinusZ(const KIntVar &v1,const KIntVar &v2,const KIntVar &v3);
	KXEqualYMinusZ(const KXEqualYMinusZ & toCopy);
	virtual ~KXEqualYMinusZ();	
    
};



#endif

