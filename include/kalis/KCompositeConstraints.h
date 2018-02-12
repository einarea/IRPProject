// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KArithmeticConstraints.h                                  *
// ********************************************************************

#ifndef __KCOMPOSITE_CONSTRAINTS_H
#define __KCOMPOSITE_CONSTRAINTS_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
  * This class creates a  Binary disjunction on two constraints C1 or C2 

  @code

	C1     C2       C1 \/ C2   
	-------------------------- 
	false  false    false      
	false  true     true       
	true   false    true       
	true   true     true       

	Example :

	KIntVar START(...);	
	...
	problem.post(START + 10 < 4 || START + 10 >= 4);

	  or 

	problem.post(KDisjunction(START + 10 < 4,START + 10 >= 4));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KDisjunction : public KConstraint { 
  public :
	KDisjunction();
	/** Primary constructor 
	    @param c1 the first constraint of the disjunction c1 or c2
		@param c2 the second constraint of the disjunction c1 or c2
    */	  	  
	KDisjunction(const KConstraint & c1,const KConstraint &c2);
	/// Copy constructor
	KDisjunction(const KDisjunction & toCopy);
	// Basic constructor
	KDisjunction(KProblem * problem);
	// Destructor
	~KDisjunction();	
	// methods
	/**
		@return true if status of disjunction is proven at current point of the branch and bound
		@return false if status of disjunction is unknown at current point of the branch and bound
	*/
    bool knownStatus(void); 
	/** fix status of one part of the disjunction
		@param branchNumber 0 for c1 , 1 for c2
		@param status true if corresponding constraint must be true 
	*/
    void setStatus(int branchNumber,bool status); 
	void setConstraint_I_ptr(void * ptr);
	/** 
	 * 
	 */	
	void setPrefferedBranch(int branch);
	int getPrefferedBranch();
	int getActiveBranch();
	void setRessource(int ressource);
	int getRessource();
	virtual int getTypeInfo(void);
	virtual char * getName() const;

};

/**	 
  * This class creates a  Binary conjunction on two constraints C1 and C2

  @code

	C1     C2       C1 /\ C2  
	-------------------------- 
	false  false    false      
	false  true     false      
	true   false    false      
	true   true     true 
	
    Example :

	KIntVar START0(...);	
	KIntVar START1(...);	

	...
	problem.post(START0 + 10 < 4 && START1 + 10 > 10);

	  or 

	problem.post(KConjunction(START0 + 10 < 4,START1 + 10 > 10));

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KConjunction : public KConstraint { 
  public :
	/** Primary constructor 
	    @param c1 the first constraint of the conjunction c1 and c2
		@param c2 the second constraint of the conjunction c1 and c2
    */	  
	KConjunction(const KConstraint & c1,const KConstraint &c2);
	/// Copy constructor
	KConjunction(const KConjunction & toCopy);
	// Destructor
	~KConjunction();	
};

/**	 
  * This class creates an implication on two constraints C1 ==> C2

  @code

	C1     C2       C1 ==> C2  
	-------------------------- 
	false  false    true       
	false  true     true       
	true   false    false      
	true   true     true       

    Example :

	KIntVar X(...);	
	KIntVar Y(...);	
	KIntVar Z(...);	

	problem.post( KGuard( X <= Y + 3 , Z > 4 )  );


  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KGuard : public KConstraint { 
  public :
	/** Primary constructor 
	    @param c1 the left part constraint in c1 ==> c2
		@param c2 the right part constraint in c1 ==> c2
    */	  
	KGuard(const KConstraint & c1,const KConstraint &c2);
	/// Copy constructor
	KGuard(const KGuard & toCopy);
	// Destructor
	~KGuard();	
};

/**	 
  * This class creates an Equivalence on two constraints C1 <==> C2

  @code

	C1     C2       C1 <==> C2 
	-------------------------- 
	false  false    true       
	false  true     false      
	true   false    false      
	true   true     true       

    Example :

	KIntVar X(...);	
	KIntVar Y(...);	
	KIntVar Z(...);	

	problem.post( KEquiv( X <= Y + 3 , Z > 4 )  );

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KEquiv : public KConstraint { 
  public :	
	/** Primary constructor 
	    @param c1 the left part constraint in c1 <==> c2
		@param c2 the right part constraint in c1 <==> c2
    */
	KEquiv(const KConstraint & c1,const KConstraint &c2);
	/// Copy constructor
	KEquiv(const KEquiv & toCopy);
	// Destructor
	~KEquiv();	
};


#endif


