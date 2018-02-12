// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KBinTerm.h                                                *
// * Description : class representing arithmetic expressions made of  *
// * two variables and of the form aX + bY, where X and Y are         *
// * KIntVar's and a and b are -1/+1 values.                          *
// ********************************************************************

#ifndef __BINTERM_H
#define __BINTERM_H

#include "Globals.h"
#include "KTerm.h"

/**	 
  * This class represent an expression of the form X (+ , -) Y + cste where X and Y are variables and cste , an integer constant  
  * @see KUnTerm
  * @see KLinTerm
  *	 @version 2013.1
*/
/*
class DLLIMPORTEXPORT KBinTerm : public KTerm {

  private:

    KIntVar *_v1;
    bool _sign1;
    KIntVar *_v2;
    bool _sign2;

  public:
    /// Default Constructor
    KBinTerm();	
	/** Primary constructor
	    @param v1 the first variable
		@param sign1 sign of v1
	    @param v2 the second variable
		@param sign2 sign of v2
		@param cste the constant		
    */	  
   /* KBinTerm(const KIntVar &v1, const bool sign1, const KIntVar &v2, const bool sign2, const int cste);
	/// Copy constructor
    KBinTerm(const KBinTerm &binTermToCopy);
    // Destructor
    ~KBinTerm();      
    /// return true if the sign of the first variable is positive
    bool getSign1() const {return _sign1;};
	/// return true if the sign of the second variable is positive
    bool getSign2() const {return _sign2;};
	/// return a pointer to the first variable
    KIntVar *getV1() const {return _v1;};
	/// return a pointer to the second variable
    KIntVar *getV2() const {return _v2;};
    /// pretty print method
    int print() const;
	void print(void * ctx,PrintFunctionPtr*pfp);

    
    // overloaded = operator
    const KBinTerm &operator=(const KBinTerm &binTermToCopy);

};
*/

class DLLIMPORTEXPORT KBinTerm : public KTerm {

  private:

    KNumVar *_v1;
    bool _sign1;
    KNumVar *_v2;
    bool _sign2;

  public:
    /// Default Constructor
    KBinTerm();	
	/** Primary constructor
	 *  @param v1 the first variable
	 *	@param sign1 sign of v1
	 *  @param v2 the second variable
	 *	@param sign2 sign of v2
     *	@param cste the constant		
     */	  
    KBinTerm(const KNumVar &v1, const bool sign1, const KNumVar &v2, const bool sign2, const double cste);
	/// Copy constructor
    KBinTerm(const KBinTerm &binTermToCopy);
    // Destructor
    ~KBinTerm();      
    /// return true if the sign of the first variable is positive
    bool getSign1() const {return _sign1;};
	/// return true if the sign of the second variable is positive
    bool getSign2() const {return _sign2;};
	/// return a pointer to the first variable
    KNumVar *getV1() const {return _v1;};
	/// return a pointer to the second variable
    KNumVar *getV2() const {return _v2;};
    /// pretty print method
    virtual void print() const;
	virtual void print(void * ctx,PrintFunctionPtr*pfp);

    
    // overloaded = operator
    const KBinTerm &operator=(const KBinTerm &binTermToCopy);

};

#endif

