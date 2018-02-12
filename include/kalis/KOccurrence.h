// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KOccurrence.h          				                      *
// * occurence of a value in a list of variables                      *
// ********************************************************************

#ifndef __KOCCURRENCE_H
#define __KOCCURRENCE_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
  * This class creates an occurence constraint of a value in a list of variables
  @code
    Example :

	KIntVarArray Tab(...);
	KIntVar countVar(...);
	
	problem.post( KOccurTerm(0,Tab) <= 5 );		// No more than 5 occurence of 0 in Tab
	problem.post( KOccurTerm(1,Tab) < 5 );		// No more than 4 occurence of 1 in Tab
	problem.post( KOccurTerm(2,Tab) >= countVar );		// No Less than countVar occurence of 2 in Tab
	problem.post( KOccurTerm(3,Tab) > countVar );		// No Less than countVar occurence of 3 in Tab

  @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KOccurrence : public KConstraint { 
  public :	
	/**
	 Constructor
	 @param oc the occurence term
	 @param v1 the counting variable
	 @param atLeast constrain on inf the number of occurence Yes/No
	 @param atMost constrain on sup the number of occurence Yes/No
	*/
	KOccurrence(const KOccurTerm &oc, const KIntVar &v1, const bool atLeast, const bool atMost);
	/**
	 Constructor
	 @param oc the occurence term
	 @param cste the constant number of occurence
	 @param atLeast constrain on inf the number of occurence Yes/No
	 @param atMost constrain on sup the number of occurence Yes/No
	*/
    KOccurrence(const KOccurTerm &oc, const int cste, const bool atLeast, const bool atMost);

	/**
	 Constructor
	 @param variables the variables array
	 @param targets the targets value to count
	 @param minOccur minimal number of occurrences
	 @param maxOccur maximal number of occurrences
	*/
	KOccurrence(KIntVarArray &variables, KIntArray& targets,int minOccur,int maxOccur);
	/// Copy constructor
	KOccurrence(const KOccurrence & toCopy);
	/// Destructor
	virtual ~KOccurrence();	
};

#endif


