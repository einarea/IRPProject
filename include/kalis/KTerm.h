
// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KTerm.h                                                   *
// * Description : represent objects created by the methods creating  *
// * constraints                                                      *
// ********************************************************************

#ifndef __TERM_H
#define __TERM_H

#include "Globals.h"

/** 
 * Superclass of KUnTerm and KBinTerm
 */
class DLLIMPORTEXPORT KTerm {

 protected:
  
  // An integer constant
  double _cste;

 public:
  
  /// Default constructor
  KTerm(); 
  /// Main constructor
  KTerm(const double cste); 
  /// Copy constructor
  KTerm(const KTerm &termToCopy); 
  /// Destructor
  virtual ~KTerm(); 
  /// Get method
  double getCste() const;
  /// Set the constant
  void setCste(const double cste);
  /// pretty print method
  virtual void print() const;
  /// pretty print method
  virtual void print(void * ctx,PrintFunctionPtr*pfp);  
  /// overloaded operator =
  const KTerm &operator=(const KTerm &termToCopy);

}; // class KTerm

#endif
