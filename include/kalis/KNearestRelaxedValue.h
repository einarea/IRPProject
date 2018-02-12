// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KNearestRelaxedValue.h                                    *
// * Description : SubClass of KValueSelector                         *
// * rounds the relaxed value										  *
// ********************************************************************

#ifndef __KNEARESTRELAXEDVALUE_H
#define __KNEARESTRELAXEDVALUE_H

#include "Globals.h"
#include "KValueSelector.h"

/**	\class KNearestRelaxedValue
	\brief This value selector chooses the value closest to the relaxed solution contained in the provided solver.

  *	 @version 2013.1
*/

class DLLIMPORTEXPORT KNearestRelaxedValue : public KValueSelector {
public:
	/// Constructor.
	KNearestRelaxedValue(KLinearRelaxationSolver*);
	/// Copy constructor.
	KNearestRelaxedValue(const KNearestRelaxedValue& toCopy);
	/// Destructor.
	virtual ~KNearestRelaxedValue();
	/// get Next Value
	int selectNextValue(KIntVar* intVar);
	/// Reserved for internal use.
	virtual KValueSelector* getCopyPtr() const;
	/// Reserved for internal use.
	virtual char * getName() { return "NEAREST_RELAXED_VALUE"; }
}; 

#endif
