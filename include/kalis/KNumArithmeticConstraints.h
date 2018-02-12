// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNumArithmeticConstraints.h                               *
// ********************************************************************

#ifndef __KNUMARITHMETIC_CONSTRAINTS_H
#define __KNUMARITHMETIC_CONSTRAINTS_H

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

	KNumVar X(...);
	...
	problem.post(X == 5);

	  or 

	problem.post(KNumEqualXc(X,5));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumEqualXc : public KConstraint { 
  public :
	/** Primary constructor of v1 == C 
	    @param v1 the variable 
		@param c the constant
    */	 
	KNumEqualXc(const KNumVar &v1,double c);
	/// Copy constructor
	KNumEqualXc(const KNumEqualXc & toCopy);
	// Destructor
	virtual ~KNumEqualXc();
};


/**	 
  * This class creates a  X >= C  constraint
  @code
    Example :

	KNumVar X(...);
	...
	problem.post(X >= 3);

	  or 

	problem.post(KNumGreaterOrEqualXc(X,3));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumGreaterOrEqualXc : public KConstraint { 
  public :
	/** Primary constructor of v1 >= C 
	    @param v1 the variable 
		@param c the constant
    */	 	  
	KNumGreaterOrEqualXc( const KNumVar &v1,double c);
	/// Copy constructor
	KNumGreaterOrEqualXc(const KNumGreaterOrEqualXc & toCopy);
	// Destructor
	virtual ~KNumGreaterOrEqualXc();	
};

/**	 
  * This class creates a  X <= C  constraint
  @code
    Example :

	KNumVar X(...);
	...
	problem.post(X <= 3);

	  or 

	problem.post(KNumLessOrEqualXc(X,3));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumLessOrEqualXc : public KConstraint { 
  public :	
	/** Primary constructor of v1 <= C 
	    @param v1 the variable 
		@param c the constant
    */	 	  
	KNumLessOrEqualXc(const KNumVar &v1,double c);
	/// Copy constructor
	KNumLessOrEqualXc(const KNumLessOrEqualXc & toCopy);
	// Destructor
	virtual ~KNumLessOrEqualXc();	
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

	KNumVar X(...);
	KNumVar Y(...);
	...
	problem.post(X == Y + 5);

	  or 

	problem.post(KNumEqualXyc(X,Y,5));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumEqualXYc : public KConstraint { 
  public :
	/** Primary constructor of v1 == v2 + C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param c the constant
    */	 
	KNumEqualXYc( const KNumVar &v1,const KNumVar &v2,double c);
	/// Copy constructor
	KNumEqualXYc(const KNumEqualXYc & toCopy);
	// Destructor
	virtual ~KNumEqualXYc();	
};


/**	 
  * This class creates a  X >= Y + C  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...
	problem.post(X >= Y + 3);

	  or 

	problem.post(KNumGreaterOrEqualXyc(X,Y,3));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumGreaterOrEqualXyc : public KConstraint { 
  public :	
	/** Primary constructor of v1 >= v2 + C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param c the constant
    */	
	KNumGreaterOrEqualXyc(const KNumVar &v1,const KNumVar &v2,double c);
	/// Copy constructor
	KNumGreaterOrEqualXyc(const KNumGreaterOrEqualXyc & toCopy);
	// Destructor
	virtual ~KNumGreaterOrEqualXyc();	
};

/**	 
  * This class creates a  X <= Y + C  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...
	problem.post(X <= Y + 3);

	  or 

	problem.post(KNumLowerOrEqualXyc(X,Y,3));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumLowerOrEqualXyc : public KConstraint { 
  public :	
	/** Primary constructor of v1 <= v2 + C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param  c the constant
    */	
	KNumLowerOrEqualXyc(const KNumVar &v1,const KNumVar &v2,double c);
	/// Copy constructor
	KNumLowerOrEqualXyc(const KNumLowerOrEqualXyc & toCopy);
	// Destructor
	virtual ~KNumLowerOrEqualXyc();	

};

/**	 
  * This class creates a  X = Y² constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXEqualsYSquared(X,Y));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumXEqualsYSquared : public KConstraint { 
  public :	
	/** Primary constructor of v1 = v2²
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param  c the constant
    */	
	KNumXEqualsYSquared(const KNumVar &v1,const KNumVar &v2);
	/// Copy constructor
	KNumXEqualsYSquared(const KNumXEqualsYSquared & toCopy);
	// Destructor
	virtual ~KNumXEqualsYSquared();	
 
};

/**	 
  * This class creates a  X = Y * C  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXEqualsYTimesC(X,Y,C));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumXEqualsYTimesC : public KConstraint { 
  public :	
	/** Primary constructor of v1 = v2 * c 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param  c the constant
    */	
	KNumXEqualsYTimesC(const KNumVar &v1,const KNumVar &v2,double c);
	/// Copy constructor
	KNumXEqualsYTimesC(const KNumXEqualsYTimesC & toCopy);
	// Destructor
	virtual ~KNumXEqualsYTimesC();	
};


/**	 
  * This class creates a  X = Y ^ C  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXEqualsYArithPowC(X,Y,C));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/                    
class DLLIMPORTEXPORT KNumXEqualsYArithPowC : public KConstraint { 
  public :	
	/** Primary constructor of v1 = v2 ^ c 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param  c the constant
    */	
	KNumXEqualsYArithPowC(const KNumVar &v1,const KNumVar &v2,int c);
	/// Copy constructor
	KNumXEqualsYArithPowC(const KNumXEqualsYArithPowC & toCopy);
	// Destructor
	virtual ~KNumXEqualsYArithPowC();	
 
};




/**	 
  * This class creates a  X = ln(Y)  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXEqualsLnY(X,Y));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/                    
class DLLIMPORTEXPORT KNumXEqualsLnY : public KConstraint { 
  public :	
	/** Primary constructor of v1 = ln( v2 ) 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param  c the constant
    */	
	KNumXEqualsLnY(const KNumVar &v1,const KNumVar &v2);
	/// Copy constructor
	KNumXEqualsLnY(const KNumXEqualsLnY & toCopy);
	// Destructor
	virtual ~KNumXEqualsLnY();	
 
};

/**	 
  * This class creates a  X = |Y|  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXEqualsAbsY(X,Y));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/                    
class DLLIMPORTEXPORT KNumXEqualsAbsY : public KConstraint { 
  public :	
	/** 
	 * Primary constructor of v1 = ln( v2 ) 
	 * @param v1 the v1 variable 
	 * @param v2 the v2 variable 		
     */	
	KNumXEqualsAbsY(const KNumVar &v1,const KNumVar &v2);
	/// Copy constructor
	KNumXEqualsAbsY(const KNumXEqualsAbsY & toCopy);
	// Destructor
	virtual ~KNumXEqualsAbsY();	
 
};


// ********************************************************************
//
//					Usual Trigonometric functions
// 
// ********************************************************************


/**	 
  * This class creates a  X {==,<=,>=} cos(Y)  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXOperatorCosY(X,Y,GEQ));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/                    
class DLLIMPORTEXPORT KNumXOperatorCosY : public KConstraint { 
  public :	
	/** Primary constructor of v1 = cos( v2 ) 
	 *  @param v1 the v1 variable 
     *	@param v2 the v2 variable 
     */	
	KNumXOperatorCosY(const KNumVar &v1,const KNumVar &v2,int OPERATOR);
	/// Copy constructor
	KNumXOperatorCosY(const KNumXOperatorCosY & toCopy);
	// Destructor
	virtual ~KNumXOperatorCosY();	
 
};


/**	 
  * This class creates a  X {==,<=,>=} sin(Y)  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXOperatorSinY(X,Y,GEQ));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/                    
class DLLIMPORTEXPORT KNumXOperatorSinY : public KConstraint { 
  public :	
	/** Primary constructor of v1 = sin( v2 ) 
	 *  @param v1 the v1 variable 
     *	@param v2 the v2 variable 
     */	
	KNumXOperatorSinY(const KNumVar &v1,const KNumVar &v2,int OPERATOR);
	/// Copy constructor
	KNumXOperatorSinY(const KNumXOperatorSinY & toCopy);
	// Destructor
	virtual ~KNumXOperatorSinY();	
 
};

/**	 
  * This class creates a  X {==,<=,>=} tan(Y)  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXOperatorTanY(X,Y,GEQ));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/                    
class DLLIMPORTEXPORT KNumXOperatorTanY : public KConstraint { 
  public :	
	/** Primary constructor of v1 = tan( v2 ) 
	 *  @param v1 the v1 variable 
     *	@param v2 the v2 variable 
     */	
	KNumXOperatorTanY(const KNumVar &v1,const KNumVar &v2,int OPERATOR);
	/// Copy constructor
	KNumXOperatorTanY(const KNumXOperatorTanY & toCopy);
	// Destructor
	virtual ~KNumXOperatorTanY();	
 
};



// ********************************************************************
//
//             Inverse trigonometric functions
// 
// ********************************************************************





/**	 
  * This class creates a  X {==,<=,>=} acos(Y)  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXOperatorACosY(X,Y,GEQ));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/                    
class DLLIMPORTEXPORT KNumXOperatorACosY : public KConstraint { 
  public :	
	/** Primary constructor of v1 = acos( v2 ) 
	 *  @param v1 the v1 variable 
     *	@param v2 the v2 variable 
     */	
	KNumXOperatorACosY(const KNumVar &v1,const KNumVar &v2,int OPERATOR);
	/// Copy constructor
	KNumXOperatorACosY(const KNumXOperatorACosY & toCopy);
	// Destructor
	virtual ~KNumXOperatorACosY();	
 
};


/**	 
  * This class creates a  X {==,<=,>=} asin(Y)  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXOperatorASinY(X,Y,GEQ));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/                    
class DLLIMPORTEXPORT KNumXOperatorASinY : public KConstraint { 
  public :	
	/** Primary constructor of v1 = asin( v2 ) 
	 *  @param v1 the v1 variable 
     *	@param v2 the v2 variable 
     */	
	KNumXOperatorASinY(const KNumVar &v1,const KNumVar &v2,int OPERATOR);
	/// Copy constructor
	KNumXOperatorASinY(const KNumXOperatorASinY & toCopy);
	// Destructor
	virtual ~KNumXOperatorASinY();	
 
};

/**	 
  * This class creates a  X {==,<=,>=} atan(Y)  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXOperatorATanY(X,Y,GEQ));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/                    
class DLLIMPORTEXPORT KNumXOperatorATanY : public KConstraint { 
  public :	
	/** Primary constructor of v1 = atan( v2 ) 
	 *  @param v1 the v1 variable 
     *	@param v2 the v2 variable 
     */	
	KNumXOperatorATanY(const KNumVar &v1,const KNumVar &v2,int OPERATOR);
	/// Copy constructor
	KNumXOperatorATanY(const KNumXOperatorATanY & toCopy);
	// Destructor
	virtual ~KNumXOperatorATanY();	
 
};


/**	 
  * This class creates a  X {==,<=,>=} ln(Y)  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXOperatorLnY(X,Y,GEQ));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/                    
class DLLIMPORTEXPORT KNumXOperatorLnY : public KConstraint { 
  public :	
	/** Primary constructor of v1 = ln( v2 ) 
	 *  @param v1 the v1 variable 
     *	@param v2 the v2 variable 
     */	
	KNumXOperatorLnY(const KNumVar &v1,const KNumVar &v2,int OPERATOR);
	/// Copy constructor
	KNumXOperatorLnY(const KNumXOperatorLnY & toCopy);
	// Destructor
	virtual ~KNumXOperatorLnY();	
 
};

/**	 
  * This class creates a  X {==,<=,>=} exp(Y)  constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	...	

	problem.post(KNumXOperatorExpY(X,Y,GEQ));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/                    
class DLLIMPORTEXPORT KNumXOperatorExpY : public KConstraint { 
  public :	
	/** Primary constructor of v1 = ln( v2 ) 
	 *  @param v1 the v1 variable 
     *	@param v2 the v2 variable 
     */	
	KNumXOperatorExpY(const KNumVar &v1,const KNumVar &v2,int OPERATOR);
	/// Copy constructor
	KNumXOperatorExpY(const KNumXOperatorExpY & toCopy);
	// Destructor
	virtual ~KNumXOperatorExpY();	
 
};








// ********************************************************************
//
//          TERNARY ARITHMETIC CONSTRAINTS X op Y op Z + (int)
// 
// ********************************************************************

/**	 
  * This class creates a  X == Y + Z constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	KNumVar Z(...);
	...
	problem.post(X == Y + Z);

	  or 

	problem.post(KNumEqualXYZ(X,Y,Z));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumEqualXYZ : public KConstraint { 
  public :	  
	KNumEqualXYZ(const KNumVar &v1,const KNumVar &v2,const KNumVar &v3);
	KNumEqualXYZ(const KNumEqualXYZ & toCopy);
	virtual ~KNumEqualXYZ();	
    
};

/**	 
  * This class creates a  X == Y * Z constraint
  @code
    Example :

	KNumVar X(...);
	KNumVar Y(...);
	KNumVar Z(...);
	...
	problem.post(X == Y * Z);

	  or 

	problem.post(KNumXEqualsYTimesZ(X,Y,Z));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumXEqualsYTimesZ : public KConstraint { 
  public :	  
	KNumXEqualsYTimesZ(const KNumVar &v1,const KNumVar &v2,const KNumVar &v3);
	KNumXEqualsYTimesZ(const KNumXEqualsYTimesZ & toCopy);
	virtual ~KNumXEqualsYTimesZ();	
    
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
class DLLIMPORTEXPORT KNumDistanceLowerThanXyc : public KConstraint { 
  public :	
	/** Primary constructor of abs(v1-v2) <= C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param c the constant
    */	
	KNumDistanceLowerThanXyc(const KNumVar &v1,const KNumVar &v2,double c);
	/// Copy Constructor
	KNumDistanceLowerThanXyc(const KNumDistanceLowerThanXyc & toCopy);
	// Destructor
	virtual ~KNumDistanceLowerThanXyc();	
 
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
class DLLIMPORTEXPORT KNumDistanceGreaterThanXyc : public KConstraint { 
  public :
	/** Primary constructor of abs(v1-v2) >= C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param c the constant
    */	  
	KNumDistanceGreaterThanXyc(const KNumVar &v1,const KNumVar &v2,double c);
	/// Copy constructor
	KNumDistanceGreaterThanXyc(const KNumDistanceGreaterThanXyc & toCopy);
	// Destructor
	virtual ~KNumDistanceGreaterThanXyc();	
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
class DLLIMPORTEXPORT KNumDistanceEqualXyc : public KConstraint { 
  public :
	/** Primary constructor of abs(v1-v2) == C 
	    @param v1 the v1 variable 
		@param v2 the v2 variable 
		@param c the constant
    */	  
	KNumDistanceEqualXyc(const KNumVar &v1,const KNumVar &v2,double c);
	/// Copy constructor
	KNumDistanceEqualXyc(const KNumDistanceEqualXyc & toCopy);
	// Destructor
	virtual ~KNumDistanceEqualXyc();	
 
};







#endif
