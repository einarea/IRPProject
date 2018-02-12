// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : Contradiction.h                                           *
// ********************************************************************

#ifndef __CONTRADICTION_H
#define __CONTRADICTION_H

#include "Globals.h"

/**
 * A contradiction is a c++ exception thrown whenever kalis deduce that the problem is inconsistent (i.e it has no solution)
 */
class DLLIMPORTEXPORT Contradiction
{
public:

	/**
	 * Constructor
	 */
	Contradiction(void) {};	
};

#endif


