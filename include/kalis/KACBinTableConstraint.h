// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KACBinTableConstraint.h                                   * 
// ********************************************************************

#ifndef __KAC_BIN_TABLE_CONSTRAINT_H
#define __KAC_BIN_TABLE_CONSTRAINT_H

#include "Globals.h"
#include "KConstraint.h"

/**	 
  * This class implements a generic class for propagation of any binary constraint by local 2-consistency (arc consistency) \\
    Two algorithms (AC3 and AC2001) are available for propagation of the constraint.	

  @code
	// Example : X == Y + 1
	
	KProblem p(...);
	KIntVar X(p,"X",0,4);
	KIntVar Y(p,"Y",0,4);

	//
	// truth table of constraint X == Y + 1 for X in [0..4] and Y in [0..4]

	//   |0|1|2|3|4|	
	// -------------
	// 0 |0|1|0|0|0|	
	// 1 |0|0|1|0|0|	
	// 2 |0|0|0|1|0|	
	// 3 |0|0|0|0|1|
	// 4 |0|0|0|0|0|
	// -------------
	
	bool ** truthTable;
	truthTable = new bool*[X.getSup()];
	for (int i=0;i<5;i++) {
	   truthTable[i] = new bool[Y.getSup()];
	   memset(truthTable[i],false,Y.getSup() * sizeof(bool));
	}
	truthTable[1][0] = true;	// if X = 1 and Y = 0 then X == Y + 1 is satisfied
	truthTable[2][1] = true;	// if X = 2 and Y = 1 then X == Y + 1 is satisfied
	truthTable[3][2] = true;	// if X = 3 and Y = 2 then X == Y + 1 is satisfied
	truthTable[4][3] = true;	// if X = 4 and Y = 3 then X == Y + 1 is satisfied	

	p.post(KACBinTableConstraint(X,Y,truthTable,KACBinTableConstraint::ALGORITHM_AC2001,"X == Y + 1"))
	
	...
	
  @endcode	
  * @see KACBinConstraint
  * @see KConstraint
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KACBinTableConstraint : public KConstraint { 
  public :		
	/** Constructor.
      This constructor takes four arguments 	        
	  @param v1 the first variable of the constraint
      @param v2 the second variable of the constraint
	  @param truthTable is the truth table of the constraint 
	  @param acAlgorithm ALGORITHM_AC2001 (default value) for propagation by the AC2001 algorithm 
	  @param acAlgorithm ALGORITHM_AC3 for propagation by the AC3 algorithm 
	  @param name label for pretty printing of the constraint
    */
	KACBinTableConstraint(KIntVar&v1,KIntVar&v2,bool**truthTable,int acAlgorithm=ALGORITHM_AC2001,char*name=0);
	/// Copy Constructor
	KACBinTableConstraint(const KACBinTableConstraint &toCopy);
	~KACBinTableConstraint();
	enum acAlgorithms{
		ALGORITHM_AC3,
		ALGORITHM_AC2001
	};
};

#endif


