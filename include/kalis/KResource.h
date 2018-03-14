// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KResource.h                                               *
// ********************************************************************

#ifndef __KRESOURCE_H
#define __KRESOURCE_H

#include "Globals.h"
#include "ArtelysList.h"

/** 
 *	Resources (machines, raw material etc) can be of two different types : 
 *
 *	<UL>
 *		<LI>Disjunctive when the resource can process only one task at a time (represented by the class KUnaryResource).</LI>
 *		<LI>Cumulative when the resource can process several tasks at the same time (represented by the class KDiscreteResource).</LI>
 *	</UL>
 *
 *	Traditional examples of disjunctive resources are Jobshop problems, cumulative resources are heavily used for the Resource-Constrained Project Scheduling Problem (RCPSP). Note that a disjunctive resource is semantically equivalent to a cumulative resource with maximal capacity one and unit resource usage for each task using this resource but this equivalence does not hold in terms of constraint propagation. 
 *
 *	The following schema shows an example with three tasks A,B and C executing on a disjunctive resource and on a cumulative resource with resource usage 3 for task A, 1 for task B and 1 for task C :
 *
 *  \image html resources.png
 *
 *	Tasks may require, provide, consume and produce resources : 
 *	<UL>
 *		<LI>A task requires a resource if some amount of the resource capacity must be made available for the execution of the activity. The capacity is renewable which means that the required capacity is available after the end of the task. </LI>
 *		<LI>A task provides a resource if some amount of the resource capacity is made available through the execution of the task. The capacity is renewable which means that the provided capacity is available only during the execution of the task. </LI>
 *		<LI>A task consumes a resource if some amount of the resource capacity must be made available for the execution of the task and the capacity is non-renewable which means that the consumed capacity if no longer available at the end of the task. </LI>
 *		<LI>A task produces a resource if some amount of the resource capacity is made available through the execution of the task and the capacity is non-renewable which means that the produced capacity is definitively available after the starting of the task. </LI>
 *	</UL>
 *
 */
class DLLIMPORTEXPORT KResource : public KPtrArray<KResource> {	
	
protected :	

	KResource_I * _resource_IPtr;
	// Pointer to KSchedule instance
	KSchedule* _schedule;
	
public :
	
	/**
	 * Empty constructor
	 */
	KResource();
	/**
	 * Basic constructor
	 */
	KResource(KSchedule& schedule);
	/** 
	 * Copy constructor
	 */
	KResource(const KResource & toCopy);
	/** 
	 * Destructor
	 */
	virtual ~KResource();

	KSchedule* getSchedule() const;

	/** 
	 * Return a copy of this object
	 * @return a copy of this object
	 */
	virtual KResource * getCopyPtr() const;		
	/**
	 * Add a task to the resource (internal use only)
	 * @param task the task to add
	 */
	void addTask(KTask & task);	
	/**
	 * Pretty printing of this resource
	 */
	virtual void print();
	/**
	 * Pretty printing of this resource with a print function pointer
	 */
	virtual void print(void * ctx,PrintFunctionPtr*pfp);
	/**
	 * close this resource 
	 */
	virtual void close();
	/**
	 * @return the name of this resource
	 */
	const char * getName();
	/**
	 * Set the name of this resource
	 * @param name the name of this resource
	 */
	void setName(char * name);
	/**
	 * Pretty printing
	 */ 
	virtual void printResourceGantt(KSolution & s,int factor);
	/**
	 * Pretty printing
	 */ 
	virtual void printResourceGantt();
	/**
	 * Pretty printing
	 */ 
	virtual void printTaskGantt(KSolution & s,int factor);
	/**
	 * Pretty printing
	 */ 
	virtual void printTaskGantt();
	/** 
	 * Return the minimum tasks duration
	 * @return the minimum tasks duration
	 */
	int getMinimumTasksDuration();
	/**
	 * Return the KIntVar representing the slack for this resource
	 * @return the KIntVar representing the slack for this resource
	 */
	KIntVar * getSlackVar();
	/**
	 * Return the KIntVar representing the latest completion time of all the tasks executing on this resource
	 * @return the KIntVar representing the latest completion time of all the tasks executing on this resource
	 */
	KIntVar * getLCTVar();
	/**
	 * Return the KIntVar representing the earliest starting time of all the tasks executing on this resource
	 * @return the KIntVar representing the earliest starting time of all the tasks executing on this resource
	 */
	KIntVar * getESTVar();
	/**
	 * Return the KIntVar representing the difference between LST and EST variables
	 * @return the KIntVar representing the difference between LST and EST variables
	 */
	KIntVar * getDURVar();
	/**
	 * Return the number of tasks using this resource
	 * @return the number of tasks using this resource
	 */
	int getNumberOfTasks();
	/**
	 * Return task with index 'index' in this resource
	 * @param index the task index
	 * @return task with index 'index' in this resource	 
	 */
	KTask * getTask(int index);
	/**
	 * Return true if all the tasks on this resource are fixed
	 * @return true if all the tasks on this resource are fixed
	 */
	virtual bool getIsInstantiated();	
	/**
	 * Internal use only
	 * @return Internal use only
	 */
	virtual int getLocalSlack(KTask_I * &t1,KTask_I * &t2);
	/**
	 * Internal use only
	 * @return Internal use only
	 */
	virtual KDisjunction * getFirstDisjunctionWithUnknownStatus();
	/**
	 * Internal use only
	 * @return Internal use only
	 */
	virtual KDisjunction * getDisjunctionInvolving(KTask * t1,KTask * t2);	
	/**
	 * Internal use only
	 * @return Internal use only
	 */
	virtual int getInitialCapacity();
	/**
	 * Return the initial resource stock at time step t
	 * @return the initial resource stock at time step t
	 */
	virtual int getInitialCapacityAt(int t);
	/**
	 * Set the initial resource stock between time steps t0 and t1 to 'capa'
	 * @param t0 start of the interval
	 * @param t1 end of the interval
	 * @param capa initial resource stock	 
	 */
	virtual void setInitialCapacityBetween(int t0,int t1,int capa);	

	/**
	 * Internal use only
	 * @return Internal use only
	 */
	virtual int getMaxAvailability();
	/**
	 * Return the initial resource stock at time step t
	 * @return the initial resource stock at time step t
	 */
	virtual int getMaxAvailabilityAt(int t);
	/**
	 * Set the initial resource stock between time steps t0 and t1 to 'capa'
	 * @param t0 start of the interval
	 * @param t1 end of the interval
	 * @param capa initial resource stock	 
	 */
	virtual void setMaxAvailabilityBetween(int t0,int t1,int capa);		

	/**
	 * Internal use only
	 * @return Internal use only
	 */
	virtual int getMinUsage();
	/**
	 * Return the initial resource stock at time step t
	 * @return the initial resource stock at time step t
	 */
	virtual int getMinUsageAt(int t);
	/**
	 * Set the initial resource stock between time steps t0 and t1 to 'capa'
	 * @param t0 start of the interval
	 * @param t1 end of the interval
	 * @param capa initial resource stock	 
	 */
	virtual void setMinUsageBetween(int t0,int t1,int capa);		

	
	/**
	 * Add idle time steps to this resource. During "idle time steps", the resource
	 * does nothing, i.e. its usage (consumption, production, ...) for any task T is set to Zero and 
	 * delayed one time step after (if T is executed on this very time step).
	 */
	void addIdleTimeSteps(KIntArray &idleTimeSteps);
	
	/**
	 * Return true IFF 'timestep' is an idle timestep for this resource
	 */
	bool isIdleTimeStep(int timestep);
	
	/**
	 * Internal use only
	 * @return Internal use only
	 */
	KResource_I* getResourceIPtr();
	/**
	 * Internal use only
	 * @return Internal use only
	 */
	void setResourceIPtr(KResource_I*r);

	/**
	 * Return the timetable
	 */
	KTimeTable * getTimeTable();
	
	
	
};
/** 
 * 	Resources (machines, raw material etc) can be of two different types : 
 *
 *	<UL>
 *		<LI>Disjunctive when the resource can process only one task at a time (represented by the class KUnaryResource).</LI>
 *		<LI>Cumulative when the resource can process several tasks at the same time (represented by the class KDiscreteResource).</LI>
 *	</UL>
 *
 *	Traditional examples of disjunctive resources are Jobshop problems, cumulative resources are heavily used for the Resource-Constrained Project Scheduling Problem (RCPSP). Note that a disjunctive resource is semantically equivalent to a cumulative resource with maximal capacity one and unit resource usage for each task using this resource but this equivalence does not hold in terms of constraint propagation. 
 *
 *	The following schema shows an example with three tasks A,B and C executing on a disjunctive resource and on a cumulative resource with resource usage 3 for task A, 1 for task B and 1 for task C :
 *
 *  \image html resources.png
 *
 *	Tasks may require, provide, consume and produce resources : 
 *	<UL>
 *		<LI>A task requires a resource if some amount of the resource capacity must be made available for the execution of the activity. The capacity is renewable which means that the required capacity is available after the end of the task. </LI>
 *		<LI>A task provides a resource if some amount of the resource capacity is made available through the execution of the task. The capacity is renewable which means that the provided capacity is available only during the execution of the task. </LI>
 *		<LI>A task consumes a resource if some amount of the resource capacity must be made available for the execution of the task and the capacity is non-renewable which means that the consumed capacity if no longer available at the end of the task. </LI>
 *		<LI>A task produces a resource if some amount of the resource capacity is made available through the execution of the task and the capacity is non-renewable which means that the produced capacity is definitively available after the starting of the task. </LI>
 *	</UL>
 *
 */
class DLLIMPORTEXPORT KUnaryResource : public KResource {			
public :
	/**
     * Propagation Hint Attributes
     */
	enum PropagationHint {
	    TimeTabling    = 1,	  			    ///< TimeTabling propagation scheme		
		TasksIntervals = 2,					///< Tasks Intervals propagation scheme
		Disjunctions   = 4					///< Disjunction propagation scheme
	};

	/**
	 * Constructor
	 * @param s the schedule
	 * @param name name for this resource	 
	 * @param propagation propagation scheme @see KUnaryResource::PropagationHint
	 */
	KUnaryResource(KSchedule &s,const char * name,int propagation = (TimeTabling | TasksIntervals | Disjunctions));
	/**
	 * Destructor
	 */
	virtual ~KUnaryResource();			
			
};

/** 
 *	Resources (machines, raw material etc) can be of two different types : 
 *
 *	<UL>
 *		<LI>Disjunctive when the resource can process only one task at a time (represented by the class KUnaryResource).</LI>
 *		<LI>Cumulative when the resource can process several tasks at the same time (represented by the class KDiscreteResource).</LI>
 *	</UL>
 *
 * 	Traditional examples of disjunctive resources are Jobshop problems, cumulative resources are heavily used for the Resource-Constrained Project Scheduling Problem (RCPSP). Note that a disjunctive resource is semantically equivalent to a cumulative resource with maximal capacity one and unit resource usage for each task using this resource but this equivalence does not hold in terms of constraint propagation. 
 *
 *	The following schema shows an example with three tasks A,B and C executing on a disjunctive resource and on a cumulative resource with resource usage 3 for task A, 1 for task B and 1 for task C :
 *
 *   \image html resources.png
 *
 *	Tasks may require, provide, consume and produce resources : 
 *	<UL>
 *		<LI>A task requires a resource if some amount of the resource capacity must be made available for the execution of the activity. The capacity is renewable which means that the required capacity is available after the end of the task. </LI>
 *		<LI>A task provides a resource if some amount of the resource capacity is made available through the execution of the task. The capacity is renewable which means that the provided capacity is available only during the execution of the task. </LI>
 *		<LI>A task consumes a resource if some amount of the resource capacity must be made available for the execution of the task and the capacity is non-renewable which means that the consumed capacity if no longer available at the end of the task. </LI>
 *		<LI>A task produces a resource if some amount of the resource capacity is made available through the execution of the task and the capacity is non-renewable which means that the produced capacity is definitively available after the starting of the task. </LI>
 *	</UL>
 */
class DLLIMPORTEXPORT KDiscreteResource : public KResource {	
	
public :
	/**
     * Propagation Hint Attributes
     */
	enum PropagationHint {
	    TimeTabling		 = 1,		    ///< TimeTabling propagation scheme
		TasksIntervals	 = 2,			///< Tasks Intervals propagation scheme
		EdgeFinding 	 = 4,			///< Tasks Intervals + EdgeFinding propagation scheme
		MaxAvailMinUsage = 8,			///< Constrain and keep track of max availability,and minimum usage of the resource
		BoundConsistency = 16,			///< TimeTabling Bound consistency
		ArcConsistency   = 32			///< TimeTabling Arc Consistency
	};
	/**
	 * Constructor
	 * @param s the schedule
	 * @param name name for this resource
	 * @param stockInit Initial stock for the resource (positive or null)
	 * @param propagation propagation scheme @see KDiscreteResource::PropagationHint
	 */
	KDiscreteResource(KSchedule &s,const char * name,int stockInit,int propagation = TimeTabling | TasksIntervals | EdgeFinding);
	/**
	 * Destructor
	 */
	virtual ~KDiscreteResource();		


};

class DLLIMPORTEXPORT KResourceUsageArray : public ArtelysList<KResourceUsage> {
 public:
  /// default constructor
  KResourceUsageArray();  
  /// reserved for internal use
  virtual int check(KResourceUsage_I *_ptr) const;
  // destructor
  ~KResourceUsageArray();
  
}; 


/**
 *
 */
class DLLIMPORTEXPORT KResourceUsage : public KPtrArray<KResourceUsage> {

private : 

	KResourceUsage_I * _impPtr;
	// Pointer to KSchedule instance
	KSchedule* _schedule;

public :

	/**
	 * Constructor
	 */
	KResourceUsage(KResource & resource,KIntArray &pattern);

	/**
	 * Constructor
	 */
	KResourceUsage(KResource & resource,int value);

	/**
	 * Constructor
	 */
	KResourceUsage(KResource & resource,int usemin,int usemax);

	/**
	 * Copy constructor
	 */
	KResourceUsage(const KResourceUsage& toCopy);

	/**
	 * Destructor
	 */
	~KResourceUsage();


	/**
	 * Pretty printing
	 */
	void print(void * ctx,PrintFunctionPtr*pfp);

	/**
	 * Pretty printing
	 */
	void print();

	/**
	 * Get a copy pointer of this object
	 */
	KResourceUsage* getCopyPtr() const;

	/**
	 * Internal use only
	 * @return Internal use only
	 */
	KResourceUsage_I* getInternalPtr();


};




#endif
