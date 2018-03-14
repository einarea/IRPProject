// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KOperators.h                                              *
// * Description : defined operators on Artelys Kalis objects         *
// ********************************************************************

#ifndef __OPERATORS_H
#define __OPERATORS_H


#include "Globals.h"

#ifdef WIN32
#define FASTCALL 
//_fastcall
#else
#define FASTCALL 
#endif

// ********************************************************************
//
//              Operators returning an KUnTerm object
// 
// ********************************************************************

KUnTerm  DLLIMPORTEXPORT FASTCALL operator+( const KUnTerm &left,  double right);
KUnTerm  DLLIMPORTEXPORT FASTCALL operator+( double left,  const  KUnTerm &right);
KUnTerm  DLLIMPORTEXPORT FASTCALL operator+( const  KNumVar &left,  double right);
KUnTerm  DLLIMPORTEXPORT FASTCALL operator+( double left,  const KNumVar &right);
KUnTerm  DLLIMPORTEXPORT FASTCALL operator-( const KUnTerm &right);
KUnTerm  DLLIMPORTEXPORT FASTCALL operator-( const  KNumVar &left,  double right);
KUnTerm  DLLIMPORTEXPORT FASTCALL operator-( const KUnTerm &left,  double right);
KUnTerm  DLLIMPORTEXPORT FASTCALL operator-( double left,  const KNumVar &right);
KUnTerm  DLLIMPORTEXPORT FASTCALL operator-( const KNumVar &right);

// ********************************************************************
//
//              Operators returning a KBdoubleerm object
// 
// ********************************************************************

KBinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KNumVar &left,  const KNumVar &right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KUnTerm &left,  const KNumVar &right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KNumVar &left,  const KUnTerm &right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KUnTerm &left,  const KUnTerm &right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KBinTerm &left,  double right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator+( double left,  const KBinTerm &right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KBinTerm &right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KBinTerm &left,  double right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KNumVar &left,  const KNumVar &right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KUnTerm &left,  const KNumVar &right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KNumVar &left,  const KUnTerm &right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KUnTerm &left,  const KUnTerm &right);
KBinTerm  DLLIMPORTEXPORT FASTCALL operator-( double left,  const KBinTerm &right);

 // ********************************************************************
//
//				Operators returning a KLinTerm object
//
// ********************************************************************

KLinTerm  DLLIMPORTEXPORT FASTCALL operator*( double left,  const KNumVar &right);
//KNumLinTerm  DLLIMPORTEXPORT FASTCALL operator*( double left,  const KNumVar &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator*( const KNumVar &left,  double right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KBinTerm &left,  const KNumVar &right);
//KNumLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KNumBinTerm &left,  const KNumVar &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KNumVar &left,  const KBinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KBinTerm &left,  const KUnTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KUnTerm &left,  const KBinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KBinTerm &left,   const KBinTerm &right);
//KNumLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KNumBinTerm &left,   const KNumBinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KLinTerm &left,  double right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( double left,  const KLinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KLinTerm &left,  const KNumVar &right);
//KNumLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KNumLinTerm &left,  const KNumVar &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KNumVar &left,  const KLinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KLinTerm &left,  const KUnTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KUnTerm &left,  const KLinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KLinTerm &left,  const KBinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KBinTerm &left,  const KLinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator+( const KLinTerm &left,  const KLinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KLinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KLinTerm &left,  double right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KBinTerm &left,  const KNumVar &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KLinTerm &left,  const KNumVar &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KLinTerm &left,  const KUnTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KNumVar &left,  const KBinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KUnTerm &left,  const KBinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KBinTerm &left,  const KBinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KLinTerm &left,  const KBinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( double left,  const KLinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KNumVar &left,  const KLinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KUnTerm &left,  const KLinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KBinTerm &left,  const KLinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KLinTerm &left,  const KLinTerm &right);
KLinTerm  DLLIMPORTEXPORT FASTCALL operator-( const KBinTerm &left,  const KUnTerm &right);

// ********************************************************************
//
//				Returning an KEqualXc / KNumEqualXc object object
//
// ********************************************************************

KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const  KNumVar &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( double left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KUnTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( double left,  const  KUnTerm &right);

// ********************************************************************
//
//				Returning a KLessOrEqualXc / KNumLessOrEqualXc object
//
// ********************************************************************

KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const  KNumVar &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const  KNumVar &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( double left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( double left,  const KNumVar &right);

// ********************************************************************
//
//				Returning a KGreaterOrEqualXc / KNumGreaterOrEqualXc object
//
// ********************************************************************

KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const  KNumVar &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const  KNumVar &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( double left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( double left,  const KNumVar &right);

// ********************************************************************
//
//					Returning a KNotEqualXc object
//
// ********************************************************************

KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const  KNumVar &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( double left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KUnTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( double left,  const  KUnTerm &right);

// ********************************************************************
//
//				Returning a KGreaterOrEqualXyc object
//
// ********************************************************************

KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KNumVar &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KNumVar &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KNumVar &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KNumVar &left,  const KNumVar &right);

// ********************************************************************
//
//				Returning a KNotEqualXyc object
//
// ********************************************************************

KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KNumVar &left,  const KNumVar &right);

// ********************************************************************
//
//					Returning an KEqualXyc object
//
// ********************************************************************

KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KNumVar &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KUnTerm &left,  const KUnTerm &right); 

// ********************************************************************
//
//				Returning a KXEqualYMinusZ object
//
// ********************************************************************

//KXEqualYMinusZ  DLLIMPORTEXPORT FASTCALL operator==( const KNumVar &left,  const KBinTerm &right);
//KXEqualYMinusZ  DLLIMPORTEXPORT FASTCALL operator==( const KBinTerm &left,  const KNumVar &right);

// ********************************************************************
//
//					Returning a KNumLinComb object
//
// ********************************************************************

KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KLinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KLinTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KLinTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KLinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KLinTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KBinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KBinTerm &left,  const KUnTerm &right); 
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KBinTerm &left,  const KNumVar &right); //!! for KXEqualYMinusZ constraint
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KNumVar &left,  const KBinTerm &right); //!! for KXEqualYMinusZ constraint
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( double left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KNumVar &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KUnTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KUnTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KBinTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KLinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KLinTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KLinTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KLinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KLinTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( double left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KNumVar &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KBinTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KBinTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KBinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KNumVar &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KUnTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KUnTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KBinTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( double left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KNumVar &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KNumVar &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KLinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KBinTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KLinTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KBinTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KLinTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KUnTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KBinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KLinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KUnTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KBinTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KLinTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( double left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KNumVar &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KNumVar &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KLinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KBinTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KLinTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KBinTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KLinTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KUnTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KBinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KLinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KUnTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KBinTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KLinTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( double left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KNumVar &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KNumVar &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KLinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KBinTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KLinTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KBinTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KLinTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KUnTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KBinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KLinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KUnTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KBinTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KLinTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KBinTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KBinTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KBinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KLinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KLinTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KLinTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KLinTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KLinTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KNumVar &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KUnTerm &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( double left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KNumVar &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KUnTerm &left,  const KLinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KBinTerm &left,  const KLinTerm &right);

// ********************************************************************
//
//				Returning an KConstraint object
//
// ********************************************************************

KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KUnTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KBinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( double left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator==( const KNumVar &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KUnTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KBinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( double &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KUnTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>=( const KNumVar &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( double &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KNumVar &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KBinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KUnTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator>( const KUnTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( double &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KNumVar &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KBinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KUnTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<=( const KUnTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( double &left,  const KBinTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KNumVar &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KBinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KUnTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator<( const KUnTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KUnTerm &left,  const KNumVar &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KUnTerm &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KBinTerm &left,  double right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( const KNumVar &left,  const KUnTerm &right);
KConstraint  DLLIMPORTEXPORT FASTCALL operator!=( double left,  const KBinTerm &right);

// ********************************************************************
//
// Returning an KUnIntraint object
//
// ********************************************************************
/*
KUnIntraint  DLLIMPORTEXPORT FASTCALL operator>=( const KUnTerm &left,  double right);
KUnIntraint  DLLIMPORTEXPORT FASTCALL operator>=( double left,  const  KUnTerm &right);
KUnIntraint  DLLIMPORTEXPORT FASTCALL operator>( double left,  const  KUnTerm &right);
KUnIntraint  DLLIMPORTEXPORT FASTCALL operator>( const KUnTerm &left,  double right);
KUnIntraint  DLLIMPORTEXPORT FASTCALL operator<=( double left,  const  KUnTerm &right);
KUnIntraint  DLLIMPORTEXPORT FASTCALL operator<=( const KUnTerm &left,  double right);
KUnIntraint  DLLIMPORTEXPORT FASTCALL operator<( double left,  const  KUnTerm &right);
KUnIntraint  DLLIMPORTEXPORT FASTCALL operator<( const KUnTerm &left,  double right);
*/

// ********************************************************************
//
//						Returning an KOccurrence
//
// ********************************************************************

KOccurrence  DLLIMPORTEXPORT FASTCALL operator==( const KNumVar &left, const KOccurTerm &right);
KOccurrence  DLLIMPORTEXPORT FASTCALL operator==( const KOccurTerm &left,  const KNumVar &right);
KOccurrence  DLLIMPORTEXPORT FASTCALL operator==( const KOccurTerm &left,  double right);
KOccurrence  DLLIMPORTEXPORT FASTCALL operator==( double left, const KOccurTerm &right);
KOccurrence  DLLIMPORTEXPORT FASTCALL operator>=( const KNumVar &left, const KOccurTerm &right);
KOccurrence  DLLIMPORTEXPORT FASTCALL operator>=( double left, const KOccurTerm &right);
KOccurrence  DLLIMPORTEXPORT FASTCALL operator>=( const KOccurTerm &left,  const KNumVar &right);
KOccurrence  DLLIMPORTEXPORT FASTCALL operator>=( const KOccurTerm &left,  double right);
KOccurrence  DLLIMPORTEXPORT FASTCALL operator<=( const KNumVar &left, const KOccurTerm &right);
KOccurrence  DLLIMPORTEXPORT FASTCALL operator<=( double left, const KOccurTerm &right);
KOccurrence  DLLIMPORTEXPORT FASTCALL operator<=( const KOccurTerm &left,  const KNumVar &right);
KOccurrence  DLLIMPORTEXPORT FASTCALL operator<=( const KOccurTerm &left,  double right);

// ********************************************************************
//
//						Returning an KElement
//
// ********************************************************************

KElement  DLLIMPORTEXPORT FASTCALL operator==( const KEltTerm &e,  const KNumVar &x);
KElement  DLLIMPORTEXPORT FASTCALL operator==( const KNumVar &x,  const KEltTerm &e);
KElement  DLLIMPORTEXPORT FASTCALL operator==( const KEltTerm &e,  double x);
KElement  DLLIMPORTEXPORT FASTCALL operator==( double x, const KEltTerm &e);

// ********************************************************************
// 
//						Returning a KConjunction
//
// ********************************************************************

KConjunction  DLLIMPORTEXPORT FASTCALL operator&&( const class KConstraint &c1, const class KConstraint &c2);

// ********************************************************************
//
//						Returning a KDisjunction
//
// ********************************************************************

KDisjunction  DLLIMPORTEXPORT FASTCALL operator||( const class KConstraint &c1, const class KConstraint &c2);


#endif


