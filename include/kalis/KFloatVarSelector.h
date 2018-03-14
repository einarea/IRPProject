// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KFloatVarSelector.h                                       *
// * Description : SubClass of KVariableSelector                      *
// ********************************************************************

#ifndef __FLOATVARSELECTOR_H
#define __FLOATVARSELECTOR_H

#include "Globals.h"
#include "KVariableSelector.h"


class DLLIMPORTEXPORT KFloatVarSelector : public KVariableSelector {
public : 
	/// Constructor
	KFloatVarSelector();

	/// Copy constructor
	KFloatVarSelector(const KFloatVarSelector& floatvarselectorToCopy);

	// Destructor
	virtual ~KFloatVarSelector();

	//methods
	virtual KFloatVar* selectNextVariable(KNumVarArray* floatVarArray, double gap); // get Next Variable
	virtual KVariableSelector* getCopyPtr() const;

};// class KFloatVarSelector

#endif
