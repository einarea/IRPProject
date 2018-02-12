// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KRandomVariable.h                                         *
// * Description : SubClass of KVariableSelector                      *
// * looks for unassigned variable at random                          *
// ********************************************************************

#ifndef __KRANDOMVALUE_H
#define __KRANDOMVALUE_H

#include "Globals.h"
#include "KVariableSelector.h"

/**	 
  * This class implements a variable selector that selects an uninstantiated variable at random.
  @code
    Example :
   
	KBranchingSchemeArray myBranchingSchemeArray;
	myBranchingSchemeArray += KAssignVar(KRandomVariable(),KMaxToMin();

	
  @endcode	
  *	@see  KVariableSelector  
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KRandomVariable : public KVariableSelector {
public:

  // Constructors
  KRandomVariable();

  // Copy constructor
  KRandomVariable(const KRandomVariable& toCopy);

  // Destructor
  virtual ~KRandomVariable();

  //methods
  /// return an uninstantiated variable at random
  //virtual KIntVar* selectNextVariable(KIntVarArray* intVarArray); // get Next Variable
  virtual KVariableSelector* getCopyPtr() const;  
  
}; // class KRandomVariable

#endif


