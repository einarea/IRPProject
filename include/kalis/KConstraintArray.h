// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KConstraintArray.h                                        *
// * Description : array of constraints                               *
// ********************************************************************

#ifndef __KCONSTRAINTARRAY_H
#define __KCONSTRAINTARRAY_H

#include "ArtelysList.h"
#include "KConstraint.h"

#ifdef WIN32
  #ifndef ACK_EXPORTS
    #pragma warning(disable : 4231)
  #endif
#endif

EXTTEMPL template class DLLIMPORTEXPORT ArtelysList<KConstraint>;

#ifdef WIN32
  #ifndef ACK_EXPORTS
    #pragma warning(default : 4231)
  #endif
#endif

/**	 
  * This class implements an array of KConstraint
  * @code
  *  Example :
  *
  *  KIntVarArray TAB(...)
  *  KIntVar X(...)
  *  KIntVar Y(...)
  *  KIntVar Z(...)
  *  KConstraintArray constraintArray;
  *	
  *  constraintArray += KAllDifferent("alldiff(TAB)",TAB);
  *  constraintArray += X == Y + 2:
  *  constraintArray += (Y < 6) || (Z + 4 == X)
  *	
  * @endcode	
  *	@see  KConstraint
  *	 @version 2013.1
  */
class DLLIMPORTEXPORT KConstraintArray : public ArtelysList<KConstraint> {

  public:
  /// Default Constructor
  KConstraintArray();
  /// Constructor
  KConstraintArray(int nbElements);
  /// destructor
  virtual ~KConstraintArray();

}; // class KConstraintArray : public ArtelysList<KConstraint>

#endif


