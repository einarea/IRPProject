// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KIntervalDomain.h                                         *
// ********************************************************************

#ifndef __KINTERVALDOMAIN_H
#define __KINTERVALDOMAIN_H

#include "Globals.h"
//#include "KIntervalDomain_I.h"
#include "KBranchingScheme.h"

class  DLLIMPORTEXPORT KIntervalDomain : public KBranchingScheme  {

public:
	//constructor
	KIntervalDomain(const KNumVarArray& floatVarArray, double gap, bool order);
	// Constructor with KIntervalDomain_I*
	KIntervalDomain(KIntervalDomain_I* intervalDomain);
	//destructor
	virtual ~KIntervalDomain();

	virtual KBranchingScheme* getCopyPtr() const;

	virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

	
}; // class KIntervalDomain

#endif
