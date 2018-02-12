// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KIntVar.h                                                 *
// ********************************************************************

#ifndef __KIntVar_H
#define __KIntVar_H

#include "Globals.h"
#include "KNumVar.h"
class KProblem;
class KIntArray;

/**	 
  * This class implements an integer variable with enumerated (finite) domain.
  Decision variables are the variable quantities that we are trying to instantiate in order to satisfy the constraints of our problem. In this version, Artelys Kalis works with integer variables : decision variables which are constrained to take only integer values. These integer variables are represented by instances of the class KIntVar.
  @code
    Example :
   
	KProblem  p(...);

    // X is an integer variable that can take value 0,1,2,3,4,5,6,7,8,9,10
	KIntVar X(p,"X",0,10);
	// Y is an integer variable that can take value 7,8,10 (3 different values)
	KIntVar Y(p,"Y",KIntArray(3,7,8,10));
	
	// Z is an integer variable that can take value 3,4,5
	KIntVar Z;
	Z = KIntVar(p,3,5);	
	
	
  @endcode	
  *	@see  KIntArray
  *	@see  KIntVarArray

  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KIntVar : public KNumVar {
 
 private :   
    // Pointer to the internal object implementing this KIntVar 
    KIntVar_I * _intVar_I_ptr;

  
 public:

  /// Default constructor
  KIntVar();
  /// Main constructor : minValue and maxValue are the bounds of the KIntVar's domain 
  KIntVar(KProblem &problem, char * name, const int minValue=0, const int maxValue=1,bool isBoundIntVar = false);
  /// Main constructor : enumerated domain 
  KIntVar(KProblem &problem, char * name, KIntArray &domain);
  /// Basic constructor
  KIntVar(KProblem &problem);
#ifdef ACK_LIB
  // Constructor with KNumVar_I
  KIntVar(KNumVar_I* numVar);
#endif
  /// Copy constructor
  KIntVar(const KIntVar& toCopy);
  /*
  /// Copy constructor for a specific instance
  KIntVar(const KIntVar& intVarToCopy, int pb);
  */
  // constructor using a KIntVar_I
  KIntVar(KIntVar_I& var_I) {_intVar_I_ptr = &var_I;};
  //destructor
  virtual ~KIntVar();

  KIntVar* getInstance(int pb) const;

  /// Instantiate the variable to value
  void instantiate(const int value);
  /// Remove value from the variable's domain
  void remVal(const int value);

  // get methods
  char * getName() const;
  /// Set the name of the variable
  void setName(const char * name);
  /// returns lower bound of this variable
  double getInf() const;  
  /// returns value in variable's domain and close to the middle
  double getMiddle() const; 
  /// returns upper bound of this variable
  double getSup() const;
  /// returns current domain size of the variable
  int getDomainSize() const; 
  /// returns current instantiation of the variable (when the variable is not instantiated the returned value is undefined)
  double getValue(void) const;
  /// returns true if the variable has been assigned a value, false otherwise
  bool getIsInstantiated() const; 
  /// returns the number of constraints where this variable appears
  int getDegree() const; 
  /// get target value
  double getTarget() const; 
  /// get a random value in the domain of the variable
  int getRandomValue() const; 
  /// get value immediatly after "next" in the domain of the variable and put it into next
  void getNextDomainValue(int &next);
  /// get value immediatly before "prev" in the domain of the variable and put it into prev
  void getPrevDomainValue(int &prev);
 
  /// shave lower bound of variable
  bool shaveFromLeft(void);   
  /// shave upper bound of variable
  bool shaveFromRight(void);
  /// shave the value 'val'
  bool shaveOnValue(int val);
  // set methods
  /// set the lower bound to value
  void setInf(int value);
  /// set the upper bound to value
  void setSup(int value);
  /// set the target value
  void setTarget(int value); 

  /// optimize the internal representation of the domain
  void optimizeDomainRepresentation();

  /// check if value is in the domain
  bool canBeInstantiatedTo(int value); 
  /// check if equal to x
  bool isEqualTo(KIntVar &x); 
  /**
   * Return a copy of this KIntVar object
   * @return a copy of this KIntVar object
   */
  KIntVar* getCopyPtr() const;
  /// overloaded operator =
  KIntVar& operator=(const KIntVar &toCopy);
  /// reserved for internal use
  KNumVar_I *getNumVar_I_ptr() const; 
  /// reserved for internal use
  KIntVar_I *getIntVar_I_ptr() const;  
  /// reserved for internal use
  void setIntVar_I_ptr(KIntVar_I * intVar);
  /// pretty printing of the variable
  void print();
  void print(void * ctx,PrintFunctionPtr*pfp);
  /// reserved for internal use
  const int instanceof(void) const;

  // Kalis hybrid
  // create indicators (KauxVar objects associated with a value in the domain of "*this")
  void createIndicators (void);
  // update the indicators with respect to the current domain of the KIntVar_I
  void updateIndicators (void) const;
  // get a pointer to the aux var associated with a value (NULL if undefined)
  KAuxVar * getIndicator (int value) const;

}; // class KIntVar

#endif


