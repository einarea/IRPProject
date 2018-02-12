// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KTask.h                                                   *
// ********************************************************************

#ifndef __KTASK_H
#define __KTASK_H

#include "Globals.h"
//EXTTEMPL template class DLLIMPORTEXPORT KPtrArray<KTask>;


/** 
	Tasks (processing operations, activities) are represented by the class KTask. This object contains three variables :
	<UL>
		<LI>a start variable representing the starting time of the task</LI>
		<LI>an end variable representing the ending time of the task</LI>
		<LI>a duration variable representing the duration of the task</LI>
	</UL>

	These three structural variables are linked by Artelys-Kalis with the following constraint : 

		task + duration = end.

	The starting time variable represents two specific parameters of the task: 

	<UL>
		<LI>the Earliest Starting Time (EST represented by the start variable lower bound) </LI>
		<LI>and its Latest Starting Time (LST represented by the start variable upper bound) </LI>
	</UL>

	The end variable represents two specific parameters of the task : 

	<UL>
		<LI>the Earliest Completion Time (ECT represented by the end variable lower bound) </LI>
		<LI>and its Latest Completion Time (LCT represented by the end variable upper bound) </LI>
	</UL>

	The duration variable represents two specific parameters of the task : 

	<UL>
		<LI>the minimum task duration (Dmin represented by the duration variable lower bound) </LI>
		<LI>the maximum task duration (Dmax represented by the duration variable upper bound) </LI>
	</UL>

	The picture below illustrates this : 

     \image html task.png

	  @version 2013.1
*/
class DLLIMPORTEXPORT KTask : public KPtrArray<KTask> {

private : 

	KTask_I * task_IPtr;
	// Pointer to KSchedule instance
	KSchedule* _schedule;
			
public :

	/**
	 * Empty constructor
	 */
	KTask();
	/**
	 * Basic constructor
	 */
	KTask(KSchedule& schedule);
	/** 
	 * Primary constructor
	 * @param s the schedule
	 * @param name string representing the name of the task
	 */
	KTask(KSchedule &s,const char * name);
	/** 
	 * Constructor with min/max value for start time and duration
	 * @param s the schedule
	 * @param name string representing the name of the task
	 */
	KTask(KSchedule &s,const char * name,int smin,int smax,int dmin,int dmax);	
	/** 
	 * Copy constructor
	 * @param s the schedule
	 * @param name string representing the name of the task
	 */
	KTask(KSchedule &s,const char * name,int duration);	
	/** 
	 * Constructor with fixed duration and unary resource usage
	 * @param s the schedule
	 * @param name string representing the name of the task	
	 * @param duration fixed task duration
	 * @param r the resource required by the task
	 */
	KTask(KSchedule &s,const char * name,int duration,KResource & r);	
	/** 
	 * @param s the schedule
	 * @param name string representing the name of the task
	 */
	KTask(KSchedule &s,const char * name,KResource & r,int consumption);	
	/** 
	 * Copy constructor
	 */
	KTask(const KTask & toCopy);
	/** 
	 * Destructor
	 */
	virtual ~KTask();
	/**
	 * Pretty printing of the task to stdout
	 */

	KSchedule* getSchedule() const;

	void print();
	/**
	 * Pretty printing of the task with a PrintFunctionPtr
	 */
	void print(void * ctx,PrintFunctionPtr*pfp);
	/**
	 * TODO
	 */	
	void useResource(KResource & resource,int resourceConsumption);	
	/**
	 * set the sequence dependent setup time between the current task,and the one passed in parameters to before/after
	 */	
	void setupTime(KTask & task,int before, int after);	
	/**
	 * return the setup time between the current task and the one passed in parameter
	 */	
	int getSetupTime(KTask &task);
	/**
	 * add a resource usage requirement for this task
	 */	
	void requires(KResourceUsage &resusage);
	/**
	 * add a resource usage provision for this task
	 */	
	void provides(KResourceUsage &resusage);
	/**
	 * add a resource usage consumption for this task
	 */	
	void consumes(KResourceUsage &resusage);
	/**
	 * add a resource usage production for this task
	 */	
	void produces(KResourceUsage &resusage);
	/**
	 * add optional resources usages requirements for this task and ensure that between [min..max] of theses requirements are satisfied
	 */	
	void requires(KResourceUsageArray & resusagearray,int min=1,int max=1);
	/**
	 * add optional resources usages provisions for this task and ensure that between [min..max] of theses requirements are satisfied
	 */	
	void provides(KResourceUsageArray & resusagearray,int min=1,int max=1);
	/**
	 * add optional resources usages consumptions for this task and ensure that between [min..max] of theses requirements are satisfied
	 */	
	void consumes(KResourceUsageArray & resusagearray,int min=1,int max=1);
	/**
	 * add optional resources usages productions for this task and ensure that between [min..max] of theses requirements are satisfied
	 */	
	void produces(KResourceUsageArray & resusagearray,int min=1,int max=1);
	/**
	 * return the minimal resource requirement for this task and the resource in parameters at time step "tslot"
	 */	
	int getMinimalRequirement(KResource * resource,int tslot);
	/**
	 * return the minimal resource consumption for this task and the resource in parameters at time step "tslot"
	 */	
	int getMinimalConsumption(KResource * resource,int tslot);
	/**
	 * return the maximal resource provision for this task and the resource in parameters at time step "tslot"
	 */	
	int getMaximalProvision(KResource * resource,int tslot);
	/**
	 * return the maximal resource production for this task and the resource in parameters at time step "tslot"
	 */	
	int getMaximalProduction(KResource * resource,int tslot);

	/**
	 * State that this ressource requires ( renewable ) 'requirement' unit of resource 'resource'
	 * @param resource the resource
	 * @param requirement the resouce requirement
	 */	
	void requires(KResource & resource,int requirement);
	/**
	 * State that this ressource consumes ( non-renewable ) 'consumption' unit of resource 'resource'
	 * @param resource the resource
	 * @param requirement the resouce consumption
	 */	
	void consumes(KResource & resource,int consumption);
	/**
	 * State that this ressource provides ( renewable ) 'provision' unit of resource 'resource'
	 * @param resource the resource
	 * @param requirement the resouce provision
	 */	
	void provides(KResource & resource,int provision);
	/**
	 * State that this ressource produces ( non-renewable ) 'production' unit of resource 'resource'
	 * @param resource the resource
	 * @param requirement the resouce production
	 */	
	void produces(KResource & resource,int production);
	/**
	 * State that this ressource requires ( renewable ) between 'requirementmin' and 'requirementmax unit of resource 'resource'
	 * @param resource the resource
	 * @param requirementmin the minimal resouce requirement
	 * @param requirementmax the maximal resouce requirement
	 */	
	void requires(KResource & resource,int requirementmin,int requirementmax);
	/**
	 * State that this ressource consumes ( non-renewable ) between 'consumptionmin' and 'consumptionmax unit of resource 'resource'
	 * @param resource the resource
	 * @param consumptionmin the minimal resouce consumption
	 * @param consumptionmax the maximal resouce consumption
	 */	
	void consumes(KResource & resource,int consumptionmin,int consumptionmax);
	/**
	 * State that this ressource provides ( renewable ) between 'provisionmin' and 'provisionmax unit of resource 'resource'
	 * @param resource the resource
	 * @param requirementmin the minimal resouce provision
	 * @param requirementmax the maximal resouce provision
	 */	
	void provides(KResource & resource,int provisionmin,int provisionmax);
	/**
	 * State that this ressource produces ( non-renewable ) between 'productionmin' and 'productionmax unit of resource 'resource'
	 * @param resource the resource
	 * @param requirementmin the minimal resouce production
	 * @param requirementmax the maximal resouce production
	 */	
	void produces(KResource & resource,int productionmin,int productionmax);
	/**
	 * State that this ressource requires ( renewable ) one unit of resource 'resource'
	 * @param resource the resource
	 */	
	int requires(KResource & resource);
	/**
	 * State that this ressource consumes ( non-renewable ) one unit of resource 'resource'
	 * @param resource the resource
	 */	
	int consumes(KResource & resource);
	/**
	 * State that this ressource provides ( renewable ) one unit of resource 'resource'
	 * @param resource the resource
	 */	
	int provides(KResource & resource);
	/**
	 * State that this ressource produces ( non-renewable ) one unit of resource 'resource'
	 * @param resource the resource
	 */	
	int produces(KResource & resource);	
	/**
	 * State that this task starts 'delay' time unit after the completion of task 'task'
	 * @param task the task before the current task
	 * @param delay the time distance between the two tasks
	 */	
	void startsAfter(KTask & task,int delay=0);	
	/**
	 * State that the distance between the completion of this task and the completion of task 'task' cannot exceed 'Max' time units
	 * @param task the task
	 * @param Max the distance between the completion of this task and the completion of task 'task'
	 */	
	void postEndToEndMaxC(KTask &  task,int Max=0);
	/**
	 * State that the distance between the completion of this task and the start of task 'task' cannot exceed 'Max' time units
	 * @param task the task
	 * @param Max the distance between the completion of this task and the start of task 'task'
	 */	
	void postEndToStartMaxC(KTask &  task,int Max=0);
	/**
	 * State that the distance between the start of this task and the start of task 'task' must exceed 'Min' time units
	 * @param task the task
	 * @param Min the distance between the start of this task and the start of task 'task'
	 */	
	void postStartToStartMinC(KTask &  task,int Min=0);
	/**
	 * State that the distance between the completion of this task and the start of task 'task' must exceed 'Min' time units
	 * @param task the task
	 * @param Min the distance between the completion of this task and the start of task 'task'
	 */	
	void postEndToStartMinC(KTask &  task,int Min=0);
	/**
	 *
	 */		
	void setIndex(int index);
	/**
	 *
	 */	
	int getIndex();
	/**
	 * Return the name of this task
	 * @return the name of this task
	 */	
	const char * getName();
	/**
	 * Set the name of this task
	 * @param name the name of this task
	 */	
	void setName(const char * name);
	/**
	 *	Return a pointer to the KIntVar representing the starting date of this task
	 *  @return a pointer to the KIntVar representing the starting date of this task
	 */	
	KIntVar * getStartDateVar();
	/**
	 *	Return a pointer to the KIntVar representing the ending date of this task
	 *  @return a pointer to the KIntVar representing the ending date of this task
	 */	
	KIntVar * getEndDateVar();
	/**
	 *	Return a pointer to the KIntVar representing the duration of this task
	 *  @return a pointer to the KIntVar representing the duration of this task
	 */	
	KIntVar * getDurationVar();
	/**
	 *	Return a pointer to the KIntVar representing the resource requirement of this task for resource 'r' if any or NULL
	 *  @return a pointer to the KIntVar representing the resource requirement of this task for resource 'r' if any or NULL
	 */	
	KIntVar * getRequiresVar(KResource * r);
	/**
	 *	Return a pointer to the KIntVar representing the resource consumtion of this task for resource 'r' if any or NULL
	 *  @return a pointer to the KIntVar representing the resource consumtion of this task for resource 'r' if any or NULL
	 */	
	KIntVar * getConsumesVar(KResource * r);
	/**
	 *	Return a pointer to the KIntVar representing the resource provision of this task for resource 'r' if any or NULL
	 *  @return a pointer to the KIntVar representing the resource provision of this task for resource 'r' if any or NULL
	 */	
	KIntVar * getProvidesVar(KResource * r);
	/**
	 *	Return a pointer to the KIntVar representing the resource production of this task for resource 'r' if any or NULL
	 *  @return a pointer to the KIntVar representing the resource production of this task for resource 'r' if any or NULL
	 */	
	KIntVar * getProducesVar(KResource * r);
	/**
	 * Return a pointer to the KIntVar representing the product requirement * duraiton
	 */	
	KIntVar * getSizeVar(KResource * r);
	/**
	 *	Return a pointer to the KIntVar representing the resource requirement of this task for resource 'r' if any or NULL
	 *  @return a pointer to the KIntVar representing the resource requirement of this task for resource 'r' if any or NULL
	 */	
	KIntVar * getAffectationVar(KResource * r);
	/**
	 * Return the constant duration of this task or the lowerbound if duration is not constant
	 * @return the constant duration of this task or the lowerbound if duration is not constant
	 */	
	int getDurationValue();
	/**
	 * Internal use only
	 */	
	int getResourceConsumption();
	/**
	 * Return the earliest starting time of this task
	 * @return the earliest starting time of this task
	 */	
	int getEarliestStartTime();	
	/**
	 * Return the latest starting time of this task
	 * @return the latest starting time of this task
	 */	
	int getLatestStartTime();
	/**
	 * Return the earliest completion time of this task
	 * @return the earliest completion time of this task
	 */	
	int getEarliestCompletionTime();
	/**
	 * Return the latest completion time of this task
	 * @return the latest completion time of this task
	 */	
	int getLatestCompletionTime();	
	/**
	 * Return the minimum duration of this task
	 * @return the minimum duration of this task
	 */	
	int getMinimumDuration();
	/**
	 * Return the maximum duration of this task
	 * @return the maximum duration of this task
	 */	
	int getMaximumDuration();
	/**
	 * Set the earliest starting time of this task
	 * @parmam time the earliest starting time of this task
	 */	
	void setEarliestStartTime(int time);
	/**
	 * Set the latest starting time of this task
	 * @param time the latest starting time of this task
	 */	
	void setLatestStartTime(int time);
	/**
	 * Set the earliest completion time of this task
	 * @param time the earliest completion time of this task
	 */	
	void setEarliestCompletionTime(int time);
	/**
	 * Set the latest completion time of this task
	 * @param time the latest completion time of this task
	 */	
	void setLatestCompletionTime(int time);	
	/**
	 * Set the minimum duration of this task
	 * @param durationmin the minimum duration of this task
	 */	
	void setMinimumDuration(int durationmin);
	/**
	 * Set the maximum duration of this task
	 * @param durationmax the maximum duration of this task
	 */	
	void setMaximumDuration(int durationmax);
	/**
	 * Set the starting time of this task to 'startTime'
	 * @param startTime the starting time of this task to 'startTime'
	 */	
	void setStartTime(int startTime);
	/**
	 * Set the ending time of this task to 'endTime'
	 * @param endTime the ending time of this task to 'endTime'
	 */	
	void setEndTime(int endTime);
	/**
	 * Set the duration of this task to 'duration'
	 * @param duration the duration of this task to 'duration'
	 */	
	void setDuration(int duration);
	/**
	 * Internal use only
	 */	
	void constrainVars(void);
	/**
	 * Return a copy of this task object
	 * @return a copy of this task object
	 */	
	virtual KTask* getCopyPtr() const;
	/**
	 * Internal use only
	 */	
	KTask_I* getTaskIPtr();
	/**
	 * Internal use only
	 */	
	 void setTaskIPtr(KTask_I* taskIPtr);
	/**
	 * return <b>true</b> IFF this task is fixed (Start,End,Duration,and resource utilizations variables are instantiated)
	 */
	 bool isFixed();
		
};




#endif
