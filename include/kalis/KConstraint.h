// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : AbstractConstraint.h                                      *
// ********************************************************************

#ifndef __KCONSTRAINT_H
#define __KCONSTRAINT_H

#include "Globals.h"
#include "AbstractObject.h"
#include "KLinearRelaxation.h"


EXTTEMPL template class DLLIMPORTEXPORT KPtrArray<KConstraint>;

/**   
    This class is an abstract interface for all constraints in Artelys Kalis
	 @version 2013.1
*/
class DLLIMPORTEXPORT KConstraint : public AbstractObject, public KPtrArray<KConstraint> {
  protected :
	void * _constraintIPtr;
	KProblem * _problem;
  public :
	KConstraint();
	KConstraint(KProblem * problem);
	KConstraint(const KConstraint & toCopy);
	virtual ~KConstraint();

	KConstraint& operator=(const KConstraint& toCopy);

	virtual KConstraint* getCopyPtr() const;   
	virtual KConstraint* getInstanceCopyPtr(const KProblem& problem) const;

	virtual KConstraint* getInstance(int pb) const;


   KProblem* getProblem() const;

   virtual void print(void);
   virtual void print(void * ctx,PrintFunctionPtr*pfp);

   virtual int getTypeInfo(void);
   void setName(char*);
   char* getName();
   virtual int getArity();
   virtual int getTag();
   virtual void setTag(int _tag);
   int getPriority();
   void setPriority(int priorite);

   #ifdef ACK_LIB
   	void setHidden(bool hidden);
	bool isHidden();
	void setConstraintIPtr(void * cstr);
	void * getConstraintIPtr() const;   
   #endif
   
   bool isGetLinearRelaxationImplemented();
   KLinearRelaxation * getLinearRelaxation (int strategy = 0);

   int askIfEntailed(void);
};

#endif


