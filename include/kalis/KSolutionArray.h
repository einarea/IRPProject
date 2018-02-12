// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSolutionArray.h                                          *
// * Description : array of solutions                                 *
// ********************************************************************

#ifndef _SOLUTIONARRAY_H
#define _SOLUTIONARRAY_H

#include "Globals.h"
#include "ArtelysList.h"

/**
 * An array of KSolution objects
 */
class DLLIMPORTEXPORT KSolutionArray : public ArtelysList<KSolution> {
  public:
	/// Constructor
    KSolutionArray();
	/// Variable arguments list constructor
	#ifndef VARARGIGNORE
    KSolutionArray(int nbOfElt, ...);
	#endif
	// Destructor
    virtual ~KSolutionArray();
}; // class KSolutionArray

#endif


