// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KAllDifferent.h          				                  *
// * Implementation of difference constraint                          *
// ********************************************************************

#ifndef __KCYCLE_H
#define __KCYCLE_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
 * The cycle constraint ensures that the graph implicitly represented by a set of variables and their domain contains no sub-tours (tour visiting a partial number of nodes). The constraint can take a second set of variables Preds, representing the inverse relation of the Succ function and ensure the following equivalences : succ(i) = j <==> pred(j) = i for all i and j. The third parameter of the cycle constraint allow to take into account an accumulated quantity along the tour such as distance, time or weight. More formally it ensure the following constraint : quantity = sum(i,j) M(i,j) for all edges i->j belonging to the tour.
 *  @version 2013.1
*/
class DLLIMPORTEXPORT KCycle : public KConstraint { 
  public :
	/** 
	 * Primary constructor the all different constraint
	 * @param succ the list of successors variables
	 * @param preds the list of predecessors variables
	 * @param quantity  the accumulated quantity variable
	 * @param matrix a (node x node) matrix of integers representing the quantity to add to the accumulated quantity variable when an edge (i,j) belongs to the tour. 
     */	  
	KCycle(KIntVarArray *succ,KIntVarArray *preds = NULL,KIntVar*quantity = NULL,KIntMatrix *matrix = NULL);
	/// Copy constructor
	KCycle(const KCycle & toCopy);
	// Destructor
	virtual ~KCycle();	
	
};

#endif

