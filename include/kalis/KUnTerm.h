// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KUnTerm.h                                                 *
// * Description : class representing arithmetic expressions made of a*
// * variable with an integer constant and of the form aX+c where X is*
// * an KIntVar, a is a -1/+1 value and c is an integer.              *
// ********************************************************************

#ifndef __UNTERM_H
#define __UNTERM_H

#include "Globals.h"
#include "KTerm.h"

/**	 
  * This class represent an expression of the form X + cste where X is a variable and cste , an integer constant  
  * @see KBinTerm
  * @see KLinTerm
  *	 @version 2013.1
*/ 

class DLLIMPORTEXPORT KUnTerm : public KTerm {

  private:
  
  KNumVar * _v1;
  bool _sign1;
  
 public:
  
  /// Default constructor  
  KUnTerm();
 // KUnTerm(const KNumVar &v1);
  /// Primary constructor
  KUnTerm(const KNumVar &v1, const bool sign1, const double cste);
  /// Copy constructor
  KUnTerm(const KUnTerm &unTermToCopy);
  
  // Destructor
  ~KUnTerm();
  
  // 'Get' methods
  /// return true if the sign of the first variable is positive
  bool getSign1() const;
  /// return a pointer to the first variable
  KNumVar *getV1() const;

  /// Pretty print method
  virtual void print() const;

  // Overloaded affectation
  const KUnTerm &operator=(const KUnTerm &unTermToCopy);

}; // class KUnTerm

/*
class DLLIMPORTEXPORT KUnTerm : public KTerm {

  private:
  
  KIntVar * _v1;
  bool _sign1;
  
 public:
  
  /// Default constructor  
  KUnTerm();
  /// Primary constructor
  KUnTerm(const KIntVar &v1, const bool sign1, const int cste);
  /// Copy constructor
  KUnTerm(const KUnTerm &unTermToCopy);
  
  // Destructor
  ~KUnTerm();
  
  // 'Get' methods
  /// return true if the sign of the first variable is positive
  bool getSign1() const;
  /// return a pointer to the first variable
  KIntVar *getV1() const;

  /// Pretty print method
  int print() const;

  // Overloaded affectation
  const KUnTerm &operator=(const KUnTerm &unTermToCopy);

}; // class KUnTerm
*/
#endif



