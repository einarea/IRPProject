// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KBranchingSchemeGroup.h                                   *
// ********************************************************************

#ifndef __KBranchingSchemeGroup_H
#define __KBranchingSchemeGroup_H

#include "KBranchingSchemeArray.h"

class DLLIMPORTEXPORT KBranchingSchemeGroup
{
protected :
	KBranchingSchemeArray  _branchings;
	int _tag;
public :
	KBranchingSchemeGroup(KBranchingSchemeArray & branchings,int tag=0);
	KBranchingSchemeGroup(const KBranchingSchemeGroup & toCopy);
	KBranchingSchemeArray & getBranchings();
	virtual KBranchingSchemeGroup* getCopyPtr() const;
	bool isFixed();
	int getTag();

	/**
	 * Pretty printing of the task with a PrintFunctionPtr
	 */
	void print(void * ctx,PrintFunctionPtr*pfp);
	void print();
	void setSolver_I_ptr(KSolver_I * solver_I);
	double getPriority();

//	KBranchingSchemeGroup* getInstanceCopyPtr(const KProblem& problem) const;
};

EXTTEMPL template class DLLIMPORTEXPORT ArtelysList<KBranchingSchemeGroup>;

class DLLIMPORTEXPORT KBranchingSchemeGroupArray : public ArtelysList<KBranchingSchemeGroup> {

  public:
  // Constructors
  KBranchingSchemeGroupArray(); 
  // destructor
  ~KBranchingSchemeGroupArray();

}; // class KBranchingSchemeGroupArray : public ArtelysList<KBranchingScheme>

class DLLIMPORTEXPORT KBranchingSchemeGroupSelector {

public:
  
  KBranchingSchemeGroupSelector();
  KBranchingSchemeGroupSelector(const KBranchingSchemeGroupSelector& selectorToCopy);
  virtual ~KBranchingSchemeGroupSelector();
  virtual KBranchingSchemeGroup * selectNextGroup(KBranchingSchemeGroupArray* taskArray); 
  virtual KBranchingSchemeGroupSelector* getCopyPtr() const; 
  virtual void printName() { printf("BranchingSchemeGroupSelector\n");}
  virtual char * getName() { return "BranchingSchemeGroupSelector"; }
  
}; // class KBranchingSchemeGroupSelector

class DLLIMPORTEXPORT KBranchingSchemeGroupSerializer : public KBranchingScheme { 	
public: 
	
	/// Primary constructor	
	KBranchingSchemeGroupSerializer(const KBranchingSchemeGroupArray & groups,int discrepancyLimit=MAX_INT,KBranchingSchemeGroupSelector * groupSelector = NULL);		
	/// Constructor with KBranchingSchemeGroupSerializer_I*
	KBranchingSchemeGroupSerializer(KBranchingSchemeGroupSerializer_I* branchingSchemeGroupSerializer);	
	/// Copy constructor
	KBranchingSchemeGroupSerializer(const KBranchingSchemeGroupSerializer & taskSerializer);	
	/// Destructor
	virtual ~KBranchingSchemeGroupSerializer();
	/// Get a copy pointer
	virtual KBranchingScheme* getCopyPtr() const;
	
	virtual KBranchingScheme* getInstanceCopyPtr(const KProblem& problem) const;

};




#endif
