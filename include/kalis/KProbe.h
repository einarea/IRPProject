// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KProbe.h                                                  *
// ********************************************************************

#ifndef __KPROBE_H
#define __KPROBE_H

#include "Globals.h"
#include "KBranchingScheme.h"

/**	 
 * KProbe Branching scheme 
 *	
 * \image html Probe.png
 *
 * @see KBranchingScheme
 * @see KAssignVar
 * @see KAssignAndForbid
 * @see KSettleDisjunction
 * @see KProbe
 * @see KSplitDomain
 *  @version 2013.1
 */
class  DLLIMPORTEXPORT KProbe : public KBranchingScheme  {

  public:

	/**
	 * Constructor (Default variable and value selection heuristics are used)
	 * @param discrepancyLimit number of differences from heuristic allowed
	 */
	KProbe(int discrepancyLimit=2,int breadthLimit=-1);
	/** 
	 * Constructor (default value selector is used)
	 * @param vs the variable selector	 
	 * @param discrepancyLimit number of differences from heuristic allowed
	 */
	KProbe(const KVariableSelector& vs,int discrepancyLimit=2,int breadthLimit=-1);
	/** 
	 * Constructor
	 * @param vs the variable selector
	 * @param valSel the value selector
	 * @param discrepancyLimit number of differences from heuristic allowed
	 */
	KProbe(const KVariableSelector& varSel, const KValueSelector& valSel,int discrepancyLimit=2,int breadthLimit=-1);
	/** 
	 * Constructor
	 * @param vs the variable selector
	 * @param valSel the value selector
	 * @param vars the list of variables to apply this branching scheme to
	 * @param discrepancyLimit number of differences from heuristic allowed
	 */
	KProbe(const KVariableSelector& varSel, const KValueSelector& valSel, const KIntVarArray& vars,int discrepancyLimit=2,int breadthLimit=-1);

	// Constructor with KProbe_I*
	KProbe(KProbe_I* probe);

	// Copy constructor
	KProbe(const KProbe& toCopy);

	// Destructor
	virtual ~KProbe();

	//// 'set' methods
	//int setVariableSelector(KVariableSelector* vs);

	//methods

	virtual KBranchingScheme* getCopyPtr() const;

	virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

}; // class KProbe


/**	 
  * KProbeDisjunction branching scheme
  *
  * \image html Probe.png
  *
  * @see KBranchingScheme
  *	@see KAssignVar
  *	@see KAssignAndForbid
  * @see KSettleDisjunction
  * @see KProbe
  * @see KSplitDomain
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT KProbeDisjunction : public KBranchingScheme {

 public:
  
  /**
   * Constructor
   * @param disjunctionSelector pointer to the disjunction selector heuristic
   * @param discrepancyLimit number of differences from heuristic allowed
   */
  KProbeDisjunction(KDisjunctionSelector * disjunctionSelector = NULL,int discrepancyLimit=2);
  /**
   * Constructor
   * @param disjunctionArray the disjunction array to branch on
   */
  KProbeDisjunction(const KDisjunctionArray &disjunctionArray, KDisjunctionSelector * disjunctionSelector = NULL,int discrepancyLimit=2);
  /// Constructor with KProbeDisjunction_I*
  KProbeDisjunction(KProbeDisjunction_I* probeDisjunction);
  /// Copy constructor
  KProbeDisjunction(const KProbeDisjunction& toCopy);
  /// Destructor
  virtual ~KProbeDisjunction();

  //methods
  virtual KBranchingScheme* getCopyPtr() const;

  virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

  
}; // class KProbeDisjunction

#endif


