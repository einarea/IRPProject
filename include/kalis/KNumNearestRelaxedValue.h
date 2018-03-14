// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNumNearestRelaxedValue.h                                 *
// * Description : SubClass of KValueSelector                         *
// * looks for the middle of current domain as next value             *
// ********************************************************************

#ifndef __KNUMNEARESTRELAXEDVALUE_H
#define __KNUMNEARESTRELAXEDVALUE_H

#include "Globals.h"
#include "KNumValueSelector.h"

/**	\class KNumNearestRelaxedValue
	\brief This value selector chooses the value closest to the relaxed solution contained in the provided solver.

	If the relaxed value for a KFloatVar variable is within its bounds, the selected value
	is simply the relaxed value. Otherwise, it is the upper or lower bound of the KFloatVar.

	 @version 2013.1
*/

class DLLIMPORTEXPORT KNumNearestRelaxedValue : public KNumValueSelector {
protected:
	KLinearRelaxationSolver * _solver;
public:
  
  /// Constructor
  KNumNearestRelaxedValue(KLinearRelaxationSolver*);

  /// Copy constructor
  KNumNearestRelaxedValue(const KNumNearestRelaxedValue& middleToCopy);

  // Destructor
  virtual ~KNumNearestRelaxedValue();

  //methods
  virtual KNumValueSelector* getCopyPtr() const;
  
}; // class KNumNearestRelaxedValue

#endif


