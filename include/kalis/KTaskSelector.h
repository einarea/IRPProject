// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KTaskSelector.h                                           *
// * Description : class which is in charge of founding a task        *
// * following specific parameteres (used in KBranchingScheme (mainly *
// * KTaskSerializer))                                                *
// ********************************************************************

#ifndef __TASK_SELECTOR_H
#define __TASK_SELECTOR_H

#include "Globals.h"
#include "KIntArray.h"
#include "KResourceArray.h"
/** 
*  Abstract interface class for task selection heuristic
*	A custom scheduling optimization strategy can be specified by using the KTaskSerializer branching scheme to select the task to be scheduled and value choice heuristics for its start and duration variables.
*
*  \image html taskSerializer.png	
*
*	@see KSmallestEarliestStartTime
*	@see KSmallestEarliestCompletionTime
*	@see KLargestEarliestStartTime
*	@see KLargestEarliestCompletionTime
*	@see KSmallestLatestStartTime
*	@see KSmallestLatestCompletionTime
*	@see KLargestLatestStartTime
*	@see KLargestLatestCompletionTime
*	  @version 2013.1
*/
class DLLIMPORTEXPORT KTaskSelector {
private:
	KProblem* _problem;

public:

	/**
	* Empty constructor
	*/
	KTaskSelector();
	/**
	* Constructor with KProblem
	*/
	KTaskSelector(KProblem* problem);
	/**
	* Copy constructor
	*/
	KTaskSelector(const KTaskSelector& selectorToCopy);
	/// Destructor
	virtual ~KTaskSelector();

	KProblem* getProblem() const;

	/** 
	* virtual interface method to overload for definition of your own task selection heuristics
	* @param intVarArray Array of variable from wich selecting  a variable
	*/
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const; 
	/**
	* Pretty printing 
	*/
	virtual void printName() { printf("KTaskSelector\n");}
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName() { return "KTaskSelector"; }

}; // class KTaskSelector

/**
* Largest domain duration task selection heuristic
*/
class DLLIMPORTEXPORT KLargestDurationDomain : public KTaskSelector {
public:
	/**
	* Empty constructor
	*/
	KLargestDurationDomain();
	/**
	* Copy constructor
	*/
	KLargestDurationDomain(const KLargestDurationDomain & toCopy);
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const;
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Pretty printing 
	*/
	virtual void printName();
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName();
};

/**
* Smallest Earliest Start time task selection heuristic
*/
class DLLIMPORTEXPORT KSmallestEarliestStartTime : public KTaskSelector {
public:
	/**
	* Empty constructor
	*/
	KSmallestEarliestStartTime();
	/**
	* Copy constructor
	*/
	KSmallestEarliestStartTime(const KSmallestEarliestStartTime & toCopy);
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const;
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Pretty printing 
	*/
	virtual void printName();
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName();
};

/** 
* Smallest Earliest Completion time task selection heuristic
*/
class DLLIMPORTEXPORT KSmallestEarliestCompletionTime : public KTaskSelector {
public:
	/**
	* Empty constructor
	*/
	KSmallestEarliestCompletionTime();
	/**
	* Copy constructor
	*/
	KSmallestEarliestCompletionTime(const KSmallestEarliestCompletionTime & toCopy);
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const;
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Pretty printing 
	*/
	virtual void printName();
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName();
};

/** 
* Largest Earliest Start time task selection heuristic
*/
class DLLIMPORTEXPORT KLargestEarliestStartTime : public KTaskSelector {
public:
	/**
	* Empty constructor
	*/
	KLargestEarliestStartTime();
	/**
	* Copy constructor
	*/
	KLargestEarliestStartTime(const KLargestEarliestStartTime & toCopy);
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const;
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Pretty printing 
	*/
	virtual void printName();
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName();
};
/**
* Largest Earliest Completion time task selection heuristic
*/
class DLLIMPORTEXPORT KLargestEarliestCompletionTime : public KTaskSelector {
public:
	/**
	* Empty constructor
	*/
	KLargestEarliestCompletionTime();
	/**
	* Copy constructor
	*/
	KLargestEarliestCompletionTime(const KLargestEarliestCompletionTime & toCopy);
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const;
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Pretty printing 
	*/
	virtual void printName();
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName();
};
/**
* Smallest Latest Start time task selection heuristic
*/
class DLLIMPORTEXPORT KSmallestLatestStartTime : public KTaskSelector {
public:
	/**
	* Empty constructor
	*/
	KSmallestLatestStartTime();	
	/**
	* Copy constructor
	*/
	KSmallestLatestStartTime(const KSmallestLatestStartTime & toCopy);
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const;
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Pretty printing 
	*/
	virtual void printName();
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName();
};
/**
* Smallest Latest Completion time task selection heuristic
*/
class DLLIMPORTEXPORT KSmallestLatestCompletionTime : public KTaskSelector {
public:
	/**
	* Empty constructor
	*/
	KSmallestLatestCompletionTime();
	/**
	* Copy constructor
	*/
	KSmallestLatestCompletionTime(const KSmallestLatestCompletionTime & toCopy);
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const;
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Pretty printing 
	*/
	virtual void printName();
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName();
};
/**
* Largest Latest Start time task selection heuristic
*/
class DLLIMPORTEXPORT KLargestLatestStartTime : public KTaskSelector {
public:
	/**
	* Empty constructor
	*/
	KLargestLatestStartTime();
	/**
	* Copy constructor
	*/
	KLargestLatestStartTime(const KLargestLatestStartTime & toCopy);
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const;
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Pretty printing 
	*/
	virtual void printName();
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName();
};
/**
* Largest Latest Completion time task selection heuristic
*/
class DLLIMPORTEXPORT KLargestLatestCompletionTime : public KTaskSelector {
public:
	/**
	* Empty constructor
	*/
	KLargestLatestCompletionTime();
	/**
	* Copy constructor
	*/
	KLargestLatestCompletionTime(const KLargestLatestCompletionTime & toCopy);
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const;
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Pretty printing 
	*/
	virtual void printName();
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName();
};

/**
* Tasks input order selection heuristic
*/
class DLLIMPORTEXPORT KTaskInputOrder : public KTaskSelector {
public:
	/**
	* Empty constructor
	*/
	KTaskInputOrder();
	/**
	* Copy constructor
	*/
	KTaskInputOrder(const KTaskInputOrder & toCopy);
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const;
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Pretty printing 
	*/
	virtual void printName();
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName();
};

/**
* Tasks input order selection heuristic
*/
class DLLIMPORTEXPORT KTaskRandomOrder : public KTaskSelector {
public:
	/**
	* Empty constructor
	*/
	KTaskRandomOrder();
	/**
	* Copy constructor
	*/
	KTaskRandomOrder(const KTaskRandomOrder & toCopy);
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KTaskSelector* getCopyPtr() const;
	virtual KTask * selectNextTask(KTaskArray* taskArray); 
	/**
	* Pretty printing 
	*/
	virtual void printName();
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName();
};


class DLLIMPORTEXPORT KResourceSelector {

private:
	KProblem* _problem;

public:

	/**
	* Empty constructor
	*/
	KResourceSelector();
	/**
	* Constructor with KProblem
	*/
	KResourceSelector(KProblem* problem);
	/**
	* Copy constructor
	*/
	KResourceSelector(const KResourceSelector& selectorToCopy);
	/// Destructor
	virtual ~KResourceSelector();

	KProblem* getProblem() const;

	/** 
	* virtual interface method to overload for definition of your own task selection heuristics
	* @param intVarArray Array of variable from wich selecting  a variable
	*/
	virtual KResource * selectNextResource(KResourceArray* resArray); 
	/**
	* Return a copy of this task selector
	* @return a copy of this task selector
	*/
	virtual KResourceSelector* getCopyPtr() const; 
	/**
	* Pretty printing 
	*/
	virtual void printName() { printf("KResourceSelector\n");}
	/**
	* Return the name of this task selector
	* @return the name of this task selector
	*/
	virtual char * getName() { return "KResourceSelector"; }

}; // class KResourceSelector

#endif
