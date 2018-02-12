// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KInputOrder.h                                             *
// * Description : SubClass of KVariableSelector                      *
// * looks for first unassigned variable                              *
// ********************************************************************

#ifndef __KINPUTORDER_H
#define __KINPUTORDER_H

#include "Globals.h"
#include "KVariableSelector.h"
#include "KNumVariableSelector.h"

/**	 
  * This class implements a variable selector that selects the first uninstantiated variable in the input order.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KInputOrder(),KMaxToMin());

  @endcode	
  *	@see  KVariableSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KInputOrder : public KVariableSelector {

public:
  
  
  /// Constructor
  KInputOrder();

  /// Copy constructor
  KInputOrder(const KInputOrder& inputOrderToCopy);

  // Destructor
  virtual ~KInputOrder();

  //methods
  /// return the first uninstantiated variable in the order of creation
  virtual KIntVar* selectNextVariable(KIntVarArray* intVarArray);   
  virtual KVariableSelector* getCopyPtr() const;
  
}; // class KInputOrder


/**	 
  * This class implements a variable selector that selects the first uninstantiated variable in the input order.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KInputOrder(),KMaxToMin());

  @endcode	
  *	@see  KVariableSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KNumInputOrder : public KNumVariableSelector {
public:
  
  
  
  
  /// Constructor
  KNumInputOrder();

  /// Copy constructor
  KNumInputOrder(const KNumInputOrder& toCopy);

  // Destructor
  virtual ~KNumInputOrder();

  //methods
  /// return the first uninstantiated variable in the order of creation
  virtual KNumVariableSelector*  getCopyPtr() const;
  
}; // class KNumInputOrder

class DLLIMPORTEXPORT KPathOrder : public KVariableSelector {
	
public:

  KPathOrder();
    
  KPathOrder(KIntVarArray& successors);
  /// Constructor
  KPathOrder(const KIntVarArray& successors, const KIntMatrix& distanceMatrix);

  /// Copy constructor
  KPathOrder(const KPathOrder& toCopy);

  // Destructor
  virtual ~KPathOrder();

  //methods
  /// return the first uninstantiated variable in the order of creation
  virtual KVariableSelector* getCopyPtr() const;  

}; // class KPathOrder

/*
class DLLIMPORTEXPORT KVariableSelectorComposer : public KVariableSelector {
protected :
	NAMESPACE_STL::vector<KVariableSelector *> _varSelectors;
	KDoubleArray _parameters;
	KDoubleArray _coefficients;
public:
    
  KVariableSelectorComposer();

  KVariableSelectorComposer(KVariableSelector * vselector1,double coeff1,KVariableSelector * vselector2,double coeff2);  

  KVariableSelectorComposer(KVariableSelector * vselector1,double coeff1,KVariableSelector * vselector2,double coeff2,KVariableSelector * vselector3,double coeff3);  

  void addVariableSelector(KVariableSelector * vselector,double coeff);

  /// Copy constructor
  KVariableSelectorComposer(const KVariableSelectorComposer& toCopy);

  // Destructor
  virtual ~KVariableSelectorComposer();

  //methods
  /// return the first uninstantiated variable in the order of creation
  virtual KIntVar* selectNextVariable(KIntVarArray* intVarArray);   
  virtual KVariableSelector* getCopyPtr() const;  

  virtual double evaluate();
  
}; // class KInputOrder
*/

#endif


