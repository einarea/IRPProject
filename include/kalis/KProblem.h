// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KProblem.h                                                *
// * Description : represents one constraint programming problem. It  *
// * is also the main acces point to variables, constraints, solutions*
// ********************************************************************

#ifndef __KPROBLEM_H
#define __KPROBLEM_H

#include "Globals.h"
#include "KSolutionContainer.h"
#include "KNumVar.h"

/// Main class for storing Kalis problems
/**
 * Constraint satisfaction and optimization problems include variables, constraints ( modeling entities ) and might have solutions after search. Such problems are represented in Artelys Kalis by objects of the class KProblem. These objects are holding the modeling entities objects, the solutions objects  and the objective variable object and the sense of the optimization.
 * These elements are store in a KProblem object which is declared as follows:
 * @code
 * KProblem myProblem(mySession,"myProblem");
 * @endcode
 * This statement creates a KProblem object named <em>myProblem</em> and held by the KSession object <tt>mySession</tt>.
 *
 * For using Kalis parallel search algorithms, it is necessary to create multiple instances of the problem to be solved.
 * This can be done by using the following KProblem declaration:
 * @code
 * KProblem myProblem(mySession,"myProblem", n);
 * @endcode
 * This statement creates a KProblem object named <em>myProblem</em>, storing <tt>n</tt> problem instances and held by the KSession object <tt>mySession</tt>.
 *  @version 2013.1
*/
class DLLIMPORTEXPORT KProblem : public KPtrArray<KProblem> { 

protected :

	// Pointer to the internal KProblem_I object implementing this KProblem
	KProblem_I* _problem_I_ptr;
	// Integer representing the current KProblem instance
	int _currentInstance;
	// Solution container to be used after parallel search
	KSolutionContainer _solutionContainer;

public:

	/// Default constructor
	KProblem();
	/*
	Master problem constructor
	@param session the KSession
	@param name the name of the problem
	*/
	//  KProblem(const KSession &session, char * name);
	/*
	Problem constructor with specified number of instances
	@param session the KSession
	@param name the name of the problem
	@param n the number of problem instances
	*/
	//  KProblem(const KSession &session, char * name, int n);
	/**
	 * \brief Main problem constructor
	 *
	 * If <tt>n</tt> = 0, a multi-instance KProblem object is created with as many instances as the number of available cores
	 *
	 * If <tt>n</tt> < 0, a single-instance KProblem object is created
	 *
	 * @param session the KSession
	 * @param name the name of the problem
	 * @param n the number of problem instances
	 */
	KProblem(const KSession &session, char * name, int n=-1, unsigned int initialSize=1024*1024,unsigned int initialRegisterSize=1000);
	/*
	Problem constructor with specified number of instances
	@param session the KSession
	@param name the name of the problem
	@param n the number of problem instances
	*/
	//  KProblem(const KSession &session, char * name, int n=0, unsigned int initialSize=1024*1024,unsigned int initialRegisterSize=1000);
	//  KProblem(const KSession &session, char * name, int n, unsigned int initialSize,unsigned int initialRegisterSize);
	// Copy constructor
	KProblem(const KProblem &problemToCopy);  
	// Virtual copy constructor
	KProblem* getCopyPtr();
	// Destructor
	~KProblem();

	int getCurrentInstance() const;
	void setCurrentInstance(int pb);

	/**
	* Ouput log level
	*/
	enum LogLevel {
		NONE=0,			///< Display no information, except requested by user (e.g. call to print() method)
		LOW=1,			///< Display errors and basic search information
		MEDIUM=2,		///< Display errors, warnings and detailed search information
		HIGH=3,			///< Display all information
		INTERNALDEBUG=4	///< Display all information (including internal debug information)
	};
//	enum DebugLevel{VERBOSITY_NONE_LEVEL=0,VERBOSITY_MEDIUM_LEVEL=1,VERBOSITY_INFO_LEVEL=2,VERBOSITY_DEBUG_LEVEL=3};
	/// Set the output log level
	void setLogLevel(LogLevel logLevel);
	/// Set the print function pointer
	void setPrintFunctionPointer(void * ctx,PrintFunctionPtr*pfp);
	/// Trace 'printf' style function
	void trace(LogLevel logLevel,const char* format,...) const;
	/// return the number of variables in the problem
	int getNumberOfVariables();  
	/// return the number of constraints in the problem
	int getNumberOfConstraints();  

	// Return a pointer to the first variable
	void * getFirstVariable();
	// Return a pointer to the next variable (must call getFirstVariable first)
	void * getNextVariable();
	// Return the first constraint
	void * getFirstConstraint();
	// Return a pointer to the next constraint (must call getNextConstraint first)
	void * getNextConstraint();


	/// Collects all solutions from subproblem instances
	void collectAllSolutions();
	/// Get solution container
	KSolutionContainer& getSolutionContainer();

	/// return the number of solutions already found for this problem
	int getNumberOfSolutions() const;  
	/// returns true iff at least one solution was found for this problem
	bool problemIsSolved() const;

	/// returns best solution found if problem has an objective, last solution found otherwise
	KSolution & getBestSolution() const;
	/// returns last solution found
	KSolution & getSolution() const;
	/// returns the solution numbered 'index'
	KSolution & getSolution(const int index) const;

	// methods
	/// Post a constraint to the problem  
	void post(const KConstraint& constraint);
	/// Post a user constraint to the problem  
	void post(const KUserConstraint& constraint);
	/// Post a user num constraint to the problem  
	void post(const KUserNumConstraint& constraint);
	/// Post a constraint given as a pointer to the problem. Caution: the pointer must be managed by the user and remain alive during optimization.
	void post(const KConstraint* constraint); 

	/// Set the objective function to the problem
	void setObjective(KIntVar &objective); 



	/// Set the objective function to the problem as an continuous variable
	void setObjective(KFloatVar &objective); 

	/** Sets optimization sense
	 * @param sense Maximization or Minimization
	 */
	void setSense(int sense);
	int getSense() const;
	/// propagate changes in the problem , return true if the problem is proved inconsistent, else return false
	bool propagate(void); 
	bool shaveAndpropagate(void);

	/// pretty printing of the problem

	void print(bool satisfied = true, bool violated = true, bool unknown = true);
	void print(void * ctx,PrintFunctionPtr*pfp);
	/// pretty printing of the variables of the problem
	void printVariablesStates(void);
	/// pretty printing of the disjunctions involved in the problem
	void printDisjunctionsStates(void);
	/// Do some internal optimization to solve faster the problem
	void optimizeInternalRepresentation();

	// path and 3b consistency activation
	void setUse3BConsistency(bool use);
	bool getUse3BConsistency(void); 
	void setUsePathConsistency(bool use);
	bool getUsePathConsistency(void);

	// Get methods
#ifdef ACK_LIB
	KProblem_I *getInternalPtr() const {return _problem_I_ptr;};
	void setInternalPtr(KProblem_I * ptr)  {_problem_I_ptr = ptr;};
#endif

	// Get a copy pointer of a variable for a specific instance
	// The KProblem will insured that the copied variable will be deleted later
	KIntVar* getVariableCopyPtr(const KIntVar& intVar, int pb);
	KFloatVar* getVariableCopyPtr(const KFloatVar& floatVar, int pb);
	KNumVar* getVariableCopyPtr(const KNumVar& numVar, int pb);
	KConstraint* getInstanceCopyPtr(const KConstraint& constraint, int pb);
	KBranchingScheme* getInstanceCopyPtr(const KBranchingScheme& branchingScheme, int pb);

	/**
	* Returns mono-instance copy of multi-instance KIntVar object.
	* The copy is already managed.
	*/
	KIntVar* getInstanceOf(const KIntVar* intVar) const;

	/**
	* Returns mono-instance copy of multi-instance KFloatVar object.
	* The copy is already managed.
	*/
	KFloatVar* getInstanceOf(const KFloatVar* floatVar) const;

	/**
	* Returns mono-instance copy of multi-instance KIntVarArray object.
	* The copy is already managed.
	*/
	KIntVarArray* getInstanceOf(const KIntVarArray* intVarArray) const;

	/**
	* Returns mono-instance copy of multi-instance KNumVarArray object.
	* The copy is already managed.
	*/
	KNumVarArray* getInstanceOf(const KNumVarArray* numVarArray) const;

	/**
	* Returns mono-instance copy of multi-instance KTaskArray object.
	* The copy is already managed.
	*/
	KTaskArray* getInstanceOf(const KTaskArray* taskArray) const;

	/**
	* Returns mono-instance copy of multi-instance KDisjunctionArray object.
	* The copy is already managed.
	*/
	KDisjunctionArray* getInstanceOf(const KDisjunctionArray* disjunctionArray) const;

	/**
	* Returns mono-instance copy of multi-instance KBranchingSchemeGroupArray object.
	* The copy is already managed.
	*/
	KBranchingSchemeGroupArray* getInstanceOf(const KBranchingSchemeGroupArray* branchingSchemeGroupArray) const;

	/**
	* Sense for optimization
	*/
	enum Sense {
		Minimize,	///< Minimize objective variable
		Maximize	///< Maximize objective variable
	};
	
	/// constraint classes for which an automatic relaxation is available
	enum constraintClass {KAbs, KAllDifferent, KEqualXc, KNotEqualXc, KGreaterOrEqualXc, KLessOrEqualXc,
		KEqualXyc, KNotEqualXyc, KGreaterOrEqualXyc, KDistanceLowerThanXyc, 
		KDistanceGreaterThanXyc, KDistanceEqualXyc, KDistanceNotEqualXyc, KXEqualYMinusZ,
		KLinComb, KElement, KElement2D, KOccurenceVal, KOccurence, KGlobalCardinalityConstraint,
		KDisjunction, KConjunction, KGuard, KEquiv, KNumLinComb, OtherConstraints};

	void pushWorld();
	void popWorld();
	/** get an automatic relaxation of all the posted constraints (if relaxation available).
	See the reference manual page of a constraint to check the available options.
	All constraints for which a relaxation is available provide at least strategies 0 and 1 : "0" is supposed
	to be tighter and "1" lighter. Automatic relaxation 0 is LP by default
	and automatic relaxation 1 is MIP by default, but you can change the type (global / continuous) of
	the variables using setGlobal and setAllGlobal of KLinearRelaxation.
	@param strategy parameter to choose the relaxation you want to use. 
	*/
	KLinearRelaxation * getLinearRelaxation (int strategy = 0);
	/** get an automatic relaxation of all the constraints in the array provided as an argument
	@param constraintClassArray types of constraints you want to relax (must have nbElem elements)
	@param strategyArray strategy for each constraint (must have nbElem elements)
	@param nbElem number of elements in the arrays
	*/
	KLinearRelaxation * getLinearRelaxation (constraintClass constraintClassArray[], int strategyArray[], int nbElem);
	/** get an automatic relaxation of all the constraints in the array provided as an argument
	@param constraintArray constraints you want to relax (must have nbElem elements)
	@param strategyArray strategy for each constraint (must have nbElem elements)
	@param nbElem number of elements in the arrays
	*/
	KLinearRelaxation * getSimpleLinearRelaxation (int rtype);

	void setAllVariablesUseShaving(bool use);

	/** Returns true is the problem has an objective
	*/
	bool hasObjective() const;

	/** Return the Objective variable
	Throws an ArtelysException if the problem has no objective
	*/
	KNumVar getObjective() const;

	/** Compute a minimal conflict set for this problem 
	@param ctx printing context
	@param pfp pointer to a printing function
	@param minimalConflictSet a constraint array that will be filled with the minimal conflict set
	@verboseLevel verbosity level of the calcultations
	*/
	void computeMinimalConflictSet(void * ctx=NULL,PrintFunctionPtr*pfp=NULL,KConstraintArray * minimalConflictSet=NULL,int verboseLebel=1);
	/**
	* Return a measure of the problem size
	*/
	double getProblemSize();
}; // class KProblem


#endif


