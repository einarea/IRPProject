// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : kalis.h                                                   *
// * Description : header file for Artelys Kalis                      *
// ********************************************************************

/** \mainpage Artelys Kalis Reference Manual
 *
 *
 * \section install_sec Introduction
 * 
 * This reference manual documents the Artelys Kalis 2013 Callable Library routines.
 * 
 * (c) Copyright 2001-2013 Artelys S.A.
 * 
 * This document and the software described in this document are the property of Artelys S.A and are protected as Artelys trade secrets.
 * They are furnished under a license and non disclosure agreement, and may be used or copied only within the terms of such license and non disclosure agreement.
 * No part of this work may be reproduced or disseminated in any forms or by any means, without the prior written permission of Artelys S.A.
 * 
 * \subsection whatis What is the Artelys Kalis Callable Library ?
 *
 * Artelys Kalis is an open constraint programming environment for solving combinatorial problems through a C++ library.
 * It is based on constraint propagation techniques and powerful optimization strategies.
 * It support continuous and discrete (integer) variables as well as different types of constraints, including global constraints (constraints having a specific semantic and incorporating advanced propagation algorithms).
 * Artelys Kalis has been completely designed in an object-oriented manner.
 * Variables, constraints, solutions, branching schemes are provided as objects.
 * Several standard classes for solution search can be combined to form problem-specific solution search scheme.
 * 
 * The main features are :
 * 
 *     - Simple and rich syntax for problem declaration.
 *     - Strength of a generic constraint propagation engine.
 *     - Wide and extensible variety of objects for search algorithms.
 *     - Ability to integrate user-defined constraints with custom filtering algorithms.
 *     - Ability to integrate user-defined objects in complex search strategies.
 *     - Multi-threaded search algorithms.
 * 
 * Theses features yield to a number of benefits including :
 * 
 *     - Fast solution deductions.
 *     - Rapid application developments.
 *     - Reduced code size.
 *     - Facilities for search strategies comparison.
 *     - Scalability in a shared memory environment.
 * 
 * \subsection prereq Prerequisites
 * 
 * In order to develop applications with Artelys Kalis, this manual assumes you already know how to program in C++ and that you are able, if necessary, to find information about C++ in a specific programming guide.
 * It also assumes you are familiar with your operating system and development environment.
 * 
 * \subsection moreinf For More information
 * Artelys offers you more information about Artelys Kalis in different ways :
 * 
 * <ul>
 * 
 *     <li> Web-site (http://www.artelys.com): you will find product description, training course programs, recent news and examples. Consultants may provide you with additional information on your projects requirements and can be reached by e-mail at info-kalis@artelys.com.
 * 
 *     <li> Technical support: users under assistance may contact directly the support team for specific technical questions :
 *         <ul>
 *         <li> e-mail : support-kalis@artelys.com
 *         <li> phone : +33 1 44 77 89 00.
 *         <li> fax : +33 1 42 96 22 61.
 *         </ul>
 * </ul>
 * We recommend that you use e-mail for faster and more efficient service. 
 * 
 */

#ifndef __KALIS_H
#define __KALIS_H

#include "KIntSetIntAnnotation.h"
#include "KConstraint.h"

#include "ArtelysException.h"
#include "ArtelysTimer.h"
#include "ArtelysList.h"
#include "ArtelysValList.h"
#include "KIntVarArray.h"
#include "KNumVarArray.h"
#include "KIntVarMatrix.h"
#include "KIntMatrix.h"
#include "KIntArray.h"
#include "KDoubleArray.h"
#include "KSolution.h"
#include "KSolutionContainer.h"
#include "KSolutionArray.h"
#include "KIntVar.h"
#include "KFloatVar.h"
#include "KSession.h"
#include "KProblem.h"
#include "KVariableSelector.h"
#include "KValueSelector.h"
#include "KNumVariableSelector.h"
#include "KNumValueSelector.h"
#include "KBranchingScheme.h"
#include "KParallelBranchingScheme.h"
#include "KBranchingSchemeArray.h"
#include "KElement2D.h"
#include "KMaxToMin.h"
#include "KMinToMax.h"
#include "KNearestValue.h"
#include "KNearestRelaxedValue.h"
#include "KRandomValue.h"
#include "KInputOrder.h"
#include "KIntervalDomain.h"
#include "KSmallestDomain.h"
#include "KLargestDomain.h"
#include "KSmallestDomDegRatio.h"
#include "KSmallestMin.h"
#include "KSmallestMax.h"
#include "KLargestMax.h"
#include "KLargestMin.h"
#include "KRandomVariable.h"
#include "KMiddle.h"
#include "KNumMiddle.h"
#include "KMaxDegree.h"
#include "KMaxRegretOnLowerBound.h"
#include "KMaxRegretOnUpperBound.h"
#include "KMaxDegree.h"
#include "KLargestReducedCost.h"
#include "KAssignVar.h"
#include "KAssignAndForbid.h"
#include "KSplitDomain.h"
#include "KSplitNumDomain.h"
#include "KSettleDisjunction.h"
#include "KProbe.h"
#include "KConstraintArray.h"
#include "KDisjunctionArray.h"
#include "KGlobalCardinalityConstraint.h"
#include "KWidestDomain.h"
#include "KDisjunctionSelector.h"
#include "KFloatVarSelector.h"

#include "KOptimalityToleranceChecker.h"

#include "KTerm.h"
#include "KUnTerm.h"
#include "KBinTerm.h"
#include "KLinTerm.h"
#include "KNonLinearTerm.h"
#include "KNumNonLinearComb.h"
#include "KEltTerm.h"
#include "KEltTerm2D.h"
#include "KOccurTerm.h"
#include "KElement.h"

#include "KSolver.h"
#include "KSolverEventListener.h"
#include "KParallelSolverEventListener.h"
#include "KConstraint.h"
#include "KLinComb.h"
#include "KNumLinComb.h"
#include "KAllDifferent.h"
#include "KCycle.h"
#include "KUnaryResourceConstraint.h"
#include "KCumulativeResourceConstraint.h"
#include "KACBinConstraint.h"
#include "KACBinTableConstraint.h"
#include "KMaxKMin.h"
#include "KAbs.h"
#include "KOccurrence.h"
#include "KArithmeticConstraints.h"
#include "KNumArithmeticConstraints.h"
#include "KNumMiddle.h"
#include "KUserConstraint.h"
#include "KUserNumConstraint.h"
#include "KCompositeConstraints.h"
#include "KOperators.h"
#include "Console.h"
#include "Contradiction.h"

#include "KSchedule.h"
#include "KTask.h"
#include "KResource.h"
#include "KTaskArray.h"
#include "KResourceArray.h"
#include "KTaskSelector.h"
#include "KTaskSerializer.h"
#include "KBranchingSchemeGroup.h"

#include "KIntervalDomain.h"

#include "KMessageBox.h"
#include "KMessage.h"

DLLIMPORTEXPORT extern Console artelysConsole;

#endif

