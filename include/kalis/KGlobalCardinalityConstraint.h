
#ifndef __KGCC
#define __KGCC

#include "Globals.h"
#include "KConstraint.h"


/**
 * @doc
 * This class implements a Global Cardinality Constraint.
 * 
 * A GCC (Global Cardinality Constraint) over a set of variables is defined by three arrays called
 * values,
 * lowerBound, and upperBound. The constraint is satisfied if and only if the number of
 * variables of the given set which are assigned to values[i] is greater or equal to lowerBound[i],
 * and lower or equal to upperBound[i] for all i, and if no variable of the given set is assigned
 * to a value which does not belong to values.
 * 
 * Posting a KGlobalCardinalityConstraint to a problem is equivalent, from a modelisation point of view,
 * to posting two instances of {@link KOccurence} for each value. But this is absolutely not equivalent from
 * a propagation point of view : GCC acquires a far better propagation, using the Regin algorithm.
 * 
 * Example.
 * A group of tourists have to be transported from a point to another one, using a fleet of buses. The objective
 * is to find the assignment which maximize a satisfaction of tourists, depending of their affinities.
 * The bus capacity constraint can me modelized by the following code :

 * @code
   Bus [] fleet = // something ;
   Tourist [] tourists = // something ;
   KIntVarArray assignment = new KIntVarArray (problem, tourists.length, 0, fleet.length-1, "TouristBusesAssignment");
   int [] capacity = new int [fleet.length]; // Capacities of the buses
   for (int i=0; i < fleet.length; i++)
        capacity[i] = fleet[i].capacity;
   KGlobalCardinalityConstraint gcc = new KGlobalCardinalityConstraint ("Buses Capacity constraint",
            assignment.getVars(), capacity);
   @endcode
 *
 * @see KCompleteAllDifferent
 * @see KOccurence
 */
class DLLIMPORTEXPORT KGlobalCardinalityConstraint : public KConstraint {

private:
	int * valsCopy;
	int * lowCopy;
	int * uppCopy;

public:
	/**
     * Builds a GCC
     *
     * @param name       The name of the constraint
     * @param vars       The set of variables involved by the GCC
     * @param vals       The set of values involved by the GCC
     * @param low        The minimum number of variables assigned to each value
     * @param upp        The maximum number of variables assigned to each value
	 * @param k			 The number of values. Arrays vals, low, upp, must be of length k.
     */
	KGlobalCardinalityConstraint(char * name, KIntVarArray & vars, int * vals, int * low, int * upp, int k);

	/**
	 * Copy constructor
	 */
	KGlobalCardinalityConstraint(const KGlobalCardinalityConstraint & toCopy);

	/**
	* Destructor.
	*/
	~KGlobalCardinalityConstraint ();


//#ifdef DEBUG
	int askIfEntailed (void);
//#endif 

};

#endif // __KGCC


