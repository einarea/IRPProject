// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KSchedule.h                                               *
// ********************************************************************

#ifndef __KSCHEDULE_H
#define __KSCHEDULE_H

#include "Globals.h"
#include "KTaskArray.h"
#include "KResourceArray.h"
#include "KSolver.h"
/** 
 *	Scheduling and planning problems are concerned with determining a plan for the execution of a given set of tasks. 
 *	The objective may be to generate a feasible schedule that satisfies the given constraints (such as sequence of 
 *	tasks or limited resource availability) or to optimize a given criterion such as the makespan of the schedule. 
 *	Artelys-Kalis defines several aggregate modeling objects to simplify the formulation of standard scheduling problems 
 *	like tasks,resources and schedule objects. by the types KUnaryResource and KDiscreteResource. When working with these 
 *	scheduling objects it is often sufficient to state the objects and their properties, such as task duration or resource use; 
 *	the necessary constraint relations are set up automatically by Artelys-Kalis (referred to as implicit constraints). 
 *	  @version 2013.1
 */
class DLLIMPORTEXPORT KSchedule {

protected :

	KProblem * _problem;
	KSchedule_I * _schedule_IPtr;
	KParallelSchedule_I* _parallelSchedule_IPtr;
	KResourceArray   _resourceArray;
	KTaskArray       _taskArray;
		
public :
	enum {Inconsistent=0, Suboptimal=1, Optimal=2};
	/**
	 * Constructor
	 * @param p the kalis problem
	 * @param name the name of this schedule
	 * @param timeMin minimal time horizon
	 * @param timeMax maximal time horizon
	 */
	KSchedule(KProblem &p,const char * name,int timeMin,int timeMax);
	/**
	 * Destuctor
	 */
	~KSchedule();

	/**
	 * Pretty printing of the schedule 
	 */
	void print();
	/**
	 * Pretty printing of the schedule
	 */
	void print(void * ctx,PrintFunctionPtr*pfp);
	/**
	 * Add a task to this schedule
	 * @param task the task to add to this schedule
	 */
	void addTask(KTask & task);
	/**
	 * Add a resource to this schedule
	 * @param resource the resource to add to this schedule
	 */
	void addResource(KResource & resource);
	/**
	 * Return the problem associated to this schedule
	 * @return the problem associated to this schedule
	 */
	KProblem * getProblem();
	/**
	 * Pretty printing of the solution of this schedule
	 */ 
	void printRessourcesGantt(KSolution & sol,int factor);
	

	void setFirstSolutionSearchStrategy(KBranchingSchemeArray & branchingSchemeArray);
	void setOptimalSolutionSearchStrategy(KBranchingSchemeArray & branchingSchemeArray);

	/**
	 * Return a pointer to the start dates array of all the tasks in this schedule 
	 * @return a pointer to the start dates array of all the tasks in this schedule 
	 */
	KIntVarArray * getStartDatesArray(void);
	/**
	 * Return a pointer to the end dates array of all the tasks in this schedule 
	 * @return a pointer to the end dates array of all the tasks in this schedule 
	 */
	KIntVarArray * getEndDatesArray(void);
	/**
	 * Return a pointer to the durations array of all the tasks in this schedule 
	 * @return a pointer to the durations array of all the tasks in this schedule 
	 */
	KIntVarArray * getDurationsArray(void);
	/**
	 * Return a pointer to the list of resource of this schedule
	 * @return a pointer to the list of resource of this schedule
	 */
	KResourceArray * getResourceArray();
	/**
	 * Return a pointer to the list of tasks of this schedule
	 * @return a pointer to the list of tasks of this schedule
	 */
	KTaskArray * getTaskArray();
	/**
	 * Return the minimal time horizon of this schedule
	 * @return the minimal time horizon of this schedule
	 */
	int getTimeMin();
	/**
	 * Return the maximal time horizon of this schedule
	 * @return the maximal time horizon of this schedule
	 */
	int getTimeMax();
	/**
	 *
	 **/
	void setTimeMin(int timemin);
	/**
	 *
	 **/
	void setTimeMax(int timemax);
	/**
	 * optimize the schedule
	 **/
	int optimize();	
	/**
	 * Find a initial heuristic solution for this schedule
	 * @return Inconsistent if this schedule has no solution
	 * @return Suboptimal if the heuristic solution is subobtimal
	 * @return Optimal if the heuristic solution is optimal
	 */ 
	int findInitialSolution();
	/**
	 * Find the optimal solution for this schedule
	 * @return Inconsistent if this schedule has no solution	 
	 * @return Optimal if the heuristic solution is optimal
	 */ 
	int findOptimalSolution();
	/**
	 * Find suboptimal solutions for this schedule using a local search algorithm
	 * @return Inconsistent if this schedule has no solution	 
	 * @return Suboptimal if the heuristic solution is suboptimal
	 */ 
	int localOptimization();
	/**
	 * Set the objective variable for this schedule as a KFloatVar
	 * @param obj the objective variable for this schedule as a KFloatVar
	 */
	void setObjective(KFloatVar & obj);
	/**
	 * Set the objective variable for this schedule as a KIntVar
	 * @param the objective variable for this schedule as a KIntVar
	 */
	void setObjective(KIntVar & obj);
	/**
	 * Return a reference to the objective variable of this schedule
	 * @return a reference to the objective variable of this schedule
	 */
	KNumVar& getObjective(void);
	/**
	 * Return a reference to the objective variable representing the makespan of this schedule
	 * @return a reference to the objective variable representing the makespan of this schedule
	 */
	KIntVar& getMakeSpan(void);
	/**
	 * Close this schedule (no tasks or resources can be added after this
	 */
	void close();
	/**
	 *
	 */
	bool computeLowerBounds();
	/**
	 * Set the callback functions to call when the schedule is optimized
	 * @param async the callback to call to stop the optimization process
	 * @param sol the callback to call when a solution has been found
	 * @param nodes the callback to call when a node is created
	 * @param goup the callback to call when a branch has been fully explored
	 * @param godown the callback to call when a branch is explored
	 */
	void setFunctionPointers(KalisCallBackFunctionPtr async,KalisCallBackFunctionPtr sol,KalisCallBackFunctionPtr nodes,KalisCallBackFunctionPtr goup,KalisCallBackFunctionPtr godown,void * param);
	/**
	 * Return the solver object used to optimize the schedule
	 */
	KSolver * getSolver();
	/**
	 * Return the number of tasks in this schedule
	 */
	int getNumberOfTasks(void);
	/**
	 * Return a pointer to the task number 'nbTask" in the input order
	 * @return a pointer to the task number 'nbTask" in the input order
	 */
	KTask * getTask(int nbTask);
	/**
	 * Return the number of resources in this schedule
	 * @return the number of resources in this schedule
	 */
	int getNumberOfResources(void);
	/**
	 * Return a pointer to the resource number 'nbResource' in this schedule in the input order
	 * @return a pointer to the resource number 'nbResource' in this schedule in the input order
	 */
	KResource * getResource(int nbResource);


    /**
 	 * return the value of an int control
	 * @param control integer control to retrieve
	 * @see IntControl
	 */
	int getIntControl(KSolver::IntControl control);
	/**
	 * return the value of a double control
	 * @param control double control to retrieve
	 * @see DblControl
	 */
	double getDblControl(KSolver::DblControl control);
	/**
	 * Set the value of an int control of the solver
	 * @param control the int control to set
	 * @param value the value of the control
	 * @see IntControl
	 */
	void setIntControl(KSolver::IntControl control, int value);
	/**
	 * Set the value of a double control
	 * @param control tjhe double control to set
	 * @param value value of the control
	 * @see DblControl
	 */
	void setDblControl(KSolver::DblControl control, double value);


	/**
	 * Add a relaxation solver to be used during the resolution process
	 */
    void addRelaxationSolver(KLinearRelaxationSolver * solver);

	void * getFirstTask();
	void * getNextTask();
	void * getFirstResource();
	void * getNextResource();

	KSchedule_I* getSchedule_IPtr() {
		return _schedule_IPtr;
	}

	KParallelSchedule_I* getParallelSchedule_IPtr() {
		return _parallelSchedule_IPtr;
	}

	KSchedule* getInstance(int pb);

	KSolverEventListener* getSolverEventListener() const;

	void setSolverEventListener(KSolverEventListener*);

};


#endif
