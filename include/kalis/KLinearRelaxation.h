// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KLinearRelaxation.h										  *
// * A linear relaxation stores (pointers to) variables (KNumVar and  *
// * KAuxVar) and linear relations (KLinRel) between them.			  *
// ********************************************************************

#ifndef KLinearRelaxation_H
#define KLinearRelaxation_H

#include "Globals.h"

/**	\class KLinearRelaxation
	\brief This class represents a linear relaxation of a domain.
	
	A linear relaxation consists of the following.
  - A set of involved variables
  - A type for each variable (either continuous or global). The type of a variable in
  a relaxation need not be the same as its "intrinsic" type. For instance, a KIntVar
  (which is a global variable) can be set continuous in a relaxation. On the contrary,
  making a KFloatVar global in a relaxation is forbidden since it would not make much sense
  to "relax" the domain of a variable by restricting it.
  - A set of linear relations (KLinRel) representing linear (in)equalities with these variables
  
   @version 2013.1
*/

class DLLIMPORTEXPORT KLinearRelaxation :
	public KCopiableObject<KLinearRelaxation>,
	public KPtrArray<KLinearRelaxation>
{
protected:
	KLinearRelaxation_I* _KLinearRelaxation_IPtr;
public:
	/// Constructor with a list of instances.
	KLinearRelaxation(NAMESPACE_STL::vector<KLinearRelaxation*>* instances);
	/// Constructor with a problem.
	KLinearRelaxation(const KProblem& problem);
	/// Constructor with a problem and a name.
	KLinearRelaxation(const KProblem& problem, const char* name);
	/// Copy constructor.
	KLinearRelaxation(const KLinearRelaxation&);
	// constructor with an internal object (reserved for internal use)
	KLinearRelaxation(KLinearRelaxation_I&);
	/// Destructor.
	virtual ~KLinearRelaxation();
	/** \brief Add a KLinRel (linear relation).

	This method adds a linear (in)equality (and all variables involved) to the relaxation object.
	@ param klr KLinRel to add.
	*/
	KLinearRelaxation& operator+= (const KLinRel& klr);
	/** \brief Merge two linear relaxations.

	@param relax the linear relaxation to add to the current object
	*/
	KLinearRelaxation& operator+= (const KLinearRelaxation& relax);

		/** \brief Add a KLinRel (linear relation).

	This method adds a linear (in)equality (and all variables involved) to the relaxation object.
	@ param klr KLinRel to add.
	*/
	void add(const KLinRel& klr);
	/** \brief Merge two linear relaxations.

	@param relax the linear relaxation to add to the current object
	*/
	void add(const KLinearRelaxation& relax);
	/** \brief Add a SOS of type 1 or 2.
	
	SOS are stored as KLinRel, the constant of the KLinRel being either 1 or 2 depending on the type of the SOS.

	A SOS1 (special ordered set of type 1) is a set of variables with the constraint that
	at most one variable in the set may be non-zero. Note that the comparator and the coefficients
	of the KLinRel plays no role.

	A SOS2 (special ordered set of type 2) is a set of variables with the constraint that
	at most two variables in the set may be non-zero, and if there are two non-zeros, they must be adjacent.
	Adjacency is defined by the weights (coefficients in the KLinRel), which must be unique. Note that the
	comparator of the KLinRel plays no role.

	@param sos the sos to add
	*/
	void addSOS(const KLinRel& sos);
	/** \brief Insert a KIntVar variable.
	
	Inserting a variable "manually" to the list of variables involved in the relaxation is not necessary
	in most cases, since variables are added automatically when a constraint in which they are involved
	is added to the Relaxation.

	Note: if the KIntVar has indicators, they are automatically inserted in the relaxation as well.
	@param var variable to add (with its indicators, if any)
	*/
	void insertVar (const KIntVar& var);
	/** \brief Get the rank of a KNumVar variable.

	Note that method close() must be called first, otherwise ranks are undefined.
	@param var variable to rank
	*/
	unsigned int getRank (const KNumVar* var) const;
	/** \brief Get the rank of a KAuxVar.

	Note that method close() must be called first, otherwise ranks are undefined.
	@param var variable to rank
	*/
	unsigned int getRank (const KAuxVar* var) const;
	/** \brief Check whether the relaxation is exact or not.

	A relaxation is said to be "exact" when it represents exactly the underlying set of constraints
	(constraints that were relaxed), so it is not an intrinsic property. This flag is meant to inform the
	user, not the solver ! (it is not used by the solver in any way).
	*/
	bool isExact (void) const;
	/** \brief Setter for isRelaxationExact.

	Same remark as for the previous getter.
	*/
	void isExact (bool);
	/// set object name
	void setName(char*);
	/** \brief Print method.

	This methods prints all variables, KLinRel and SOS involved in the relaxation to the default output
	*/
	void print(void) const;
	void print(void * ctx,PrintFunctionPtr*pfp);
	void printStats(void * ctx,PrintFunctionPtr*pfp);
	/// set or unset a KIntVar global
	void setGlobal (const KIntVar& var, bool global);
	void setGlobal (const KAuxVar& var, bool global);
	/** \brief Print statistics about the relaxation.
	Print only the number of variables (with their type), KLinRel and SOS involved.
	*/
	void printStat(void) const;
	/** \brief Convex hull method.

	Get a new linear relaxation which is the convex hull of the two arguments.
	Note: deleting it is user's responsibility.
	*/
	static KLinearRelaxation * convexHull(const KLinearRelaxation*, const KLinearRelaxation *);
	/** \brief  Convex hull method, with any number of arguments.

	Get a new linear relaxation which is the convex hull of the arguments.
	Note: deleting it is user's responsibility.
	*/
	static KLinearRelaxation * convexHull(const KLinearRelaxation**, int numberOfRelax);
	/** \brief Big-M method.

	Get a new linear relaxation which is the big-M disjunction of the two arguments.
	Note: deleting it is user's responsibility.
	*/
	static KLinearRelaxation * bigM(const KLinearRelaxation*, const KLinearRelaxation *);
	/** \brief Big-M method with any number of arguments.

	Get a new linear relaxation which is the big-M disjunction of the arguments.
	Note: deleting it is user's responsibility.
	*/
	static KLinearRelaxation * bigM(const KLinearRelaxation**, int numberOfRelax);
	// Get a pointer to the internal object (reserved for internal use).
	KLinearRelaxation_I* getInternalPtr (void) const {return _KLinearRelaxation_IPtr;};
	/** \brief Print KLinRel that are violated by an hybrid solution (if any).

	Useful to check whether a solution contained in a KHybridSolution object is valid.
	*/
	void printViolated (const KHybridSolution*) const ;
};

#endif
