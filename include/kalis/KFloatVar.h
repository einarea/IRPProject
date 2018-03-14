// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KFloatVar.h                                                 *
// ********************************************************************

#ifndef __KFloatVar_H
#define __KFloatVar_H

#include "Globals.h"
#include "KNumVar.h"

class KProblem;
class KIntArray;


/**	 
 * This class implements a variable with continuous real valued domain.
 * Conceptually the continuous variables can be represented the following way :
 *
 * \image html cpfloatvar.jpg
 *
 * @code
 * Example :
 *  
 *	KProblem  p(...);
 *
 *   // X is an continuous variable that can take real value between interval [0..10]
 *	KFloatVar X(p,"X",0,10);
 *	
 * @endcode	
 * @see  KNumVarArray
 * @see  KFloatVarArray
 *
 *  @version 2013.1
 */
class DLLIMPORTEXPORT KFloatVar : public KNumVar  {

 public :	 
	
	static double PINFINITY;
	static double NINFINITY;
	static double DEFAULT_PRECISION;
	static bool DEFAULT_RELATIVITY;
	enum { PABSOLUTE = false, PRELATIVE = true };
	
 
 private :   
    // Pointer to the internal object implementing this KFloatVar 
    KFloatVar_I * _floatVar_I_ptr;

	void construct(KProblem & problem, char * name, double minValue, double maxValue, bool relativity, double precision);
	
  
 public:

  /// Empty constructor
  KFloatVar();
  /// Main constructor : minValue and maxValue are the bounds of the KFloatVar's domain 
  KFloatVar(KProblem & pb, char * name);
  /**
   * Constructor
   * @param pb the problem
   * @param lowerBound domain lower bound
   * @param upperBound domain upper bound   
   */
  KFloatVar(KProblem & pb, char * name, double lowerBound, double upperBound);
  /**
   * Constructor
   * @param pb the problem
   * @param lowerBound domain lower bound
   * @param upperBound domain upper bound   
   */
  KFloatVar(KProblem & pb, char * name, double lowerBound, double upperBound, bool relativity, double precision);
#ifdef ACK_LIB
  /// Constructor with KNumVar_I
  KFloatVar(KNumVar_I* numVar);
#endif
  /**
   * Copy constructor
   */
  KFloatVar(const KFloatVar& toCopy);
  /*
  // Copy constructor for a specific instance
  KFloatVar(const KFloatVar& toCopy, int pb);
  */
  /**
   * Destructor
   */
  virtual ~KFloatVar();
  
  KFloatVar* getInstance(int pb) const;

  /// Instantiate the variable to value
  void instantiate(const double value);
  /// Remove value from the variable's domain
  //void remVal(const int value);

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
  /// returns true if the variable has been assigned a value, false otherwhise
  bool getIsInstantiated() const; 
  /// returns the number of constraints where this variable appears
  int getDegree() const; 
  /// get target value
  double getTarget() const; 
  /// get a random value in the domain of the variable
  double getRandomValue() const; 
  /// activate shaving Y/N
  void useShaving(bool use);
  /// shave lower bound of variable
  bool shaveFromLeft(void);   
  /// shave upper bound of variable
  bool shaveFromRight(void);
  /// shave the value 'val'
  bool shaveOnValue(int val);
  // set methods
  /// set the lower bound to value
  void setInf(double value);
  /// set the upper bound to value
  void setSup(double value);
  /// set the target value
  void setTarget(double value); 

  /// optimize the internal representation of the domain
  void optimizeDomainRepresentation();

  /// check if value is in the domain
  bool canBeInstantiatedTo(int value); 
  /// check if equal to x
  bool isEqualTo(KFloatVar &x); 

  KFloatVar* getCopyPtr() const;

  /// overloaded operator =
  void operator=(const KFloatVar &toCopy);

  KNumVar_I *  getNumVar_I_ptr() const;  
  KFloatVar_I *getFloatVar_I_ptr() const;  
  void setFloatVar_I_ptr(KFloatVar_I * floatVar);


  /// pretty printing of the variable
  void print();
  /// pretty printing of the variable
  void print(void * ctx,PrintFunctionPtr*pfp);

  static void setDefaultPrecisionParameters(bool relativity, double precision);
  /// Set the precision relativity (true for relative precision and false for absolute precision
  void setPrecisionRelativity(bool relativity);
  void setPrecisionValue(double precision);

  const int instanceof(void) const;

}; // class KFloatVar

#endif


