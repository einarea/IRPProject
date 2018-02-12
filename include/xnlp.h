#ifndef XNLP_H_INCLUDED
#define XNLP_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/* xnlp.h - Xpress NonLinear header file
   
   (c) 1983-2013 Fair Isaac Corporation. All rights reserved. 
   Permission to use this software and its documentation is governed by
   the software license agreement between the licensee and Fair Isaac Corporation. 
   Portions of the program copyright various authors and are licensed under certain open 
   source licenses identified in the software itself in the
   <XPRESSDIR>/licenses/readme.txt file.
*/

#include <xprs.h>
#include <xslp.h>

#define XNLP_CC XPRS_CC

typedef struct xnlp_prob_struct * XNLPprob;
typedef void (XNLP_CC * XNLPmessagecallback)(XNLPprob prob, const char *msg, void *user_data);

#define XNLP_OK    0
#define XNLP_ERROR 32

/* Standard error statuses */
#define XNLP_PARAMETER_ERROR 1
#define XNLP_MEMORY_ERROR    2
#define XNLP_ACCESS_ERROR    3
#define XNLP_INTERNAL_ERROR  4
#define XNLP_OS_ERROR        5
#define XNLP_TOKEN_ERROR     6
#define XNLP_STRING_ERROR    7
#define XNLP_BAD_PROB_ERROR  8

/* Potential values for error source in XNLPgetlasterror */
#define XNLP_SOURCE_NONE       0
#define XNLP_SOURCE_XNLP       1
#define XNLP_SOURCE_OS         2
#define XNLP_SOURCE_OPTIMIZER 20
#define XNLP_SOURCE_SLP       21

/* Optimization sense */
#define XNLP_MINIMIZE       0
#define XNLP_MAXIMIZE       1

/* Special values */
#define XNLP_PLUS_INFINITY  XPRS_PLUSINFINITY
#define XNLP_MINUS_INFINITY XPRS_MINUSINFINITY

/* Generic controls suitable for all solvers */
#define XNLP_FEASIBILITY_TOLERANCE    15000
#define XNLP_OPTIMALITY_TOLERANCE     15001
#define XNLP_INTEGRALITY_TOLERANCE    15002
#define XNLP_SOLVER                   15003

/* Generic attributes suitable for all solvers */
#define XNLP_COLUMNS                  17501
#define XNLP_ROWS                     17502
#define XNLP_OBJECTIVE_VALUE          17503
#define XNLP_STATUS                   17504
#define XNLP_SOLVER_SELECTED          17505

/* Potential values status for XNLP_STATUS */
#define XNLP_STATUS_UNSTARTED          XSLP_NLPSTATUS_UNSTARTED
#define XNLP_STATUS_LOCALLY_OPTIMAL    XSLP_NLPSTATUS_LOCALLY_OPTIMAL
#define XNLP_STATUS_OPTIMAL            XSLP_NLPSTATUS_OPTIMAL
#define XNLP_STATUS_LOCALLY_INFEASIBLE XSLP_NLPSTATUS_LOCALLY_INFEASIBLE
#define XNLP_STATUS_INFEASIBLE         XSLP_NLPSTATUS_INFEASIBLE
#define XNLP_STATUS_UNBOUNDED          XSLP_NLPSTATUS_UNBOUNDED
#define XNLP_STATUS_UNSOLVED           XSLP_NLPSTATUS_UNFINISHED

/* Solvers supported by the XNLP run-time */
#define XNLP_SOLVER_KNITRO             XSLP_SOLVER_KNITRO
#define XNLP_SOLVER_SLP                XSLP_SOLVER_XSLP
#define XNLP_SOLVER_AUTO               XSLP_SOLVER_AUTO
#define XNLP_SOLVER_OPTIMIZER          XSLP_SOLVER_OPTIMIZER

/* Supported file formats */
#define XNLP_FILE_FORMAT_MPS           0
#define XNLP_FILE_FORMAT_LP            1

/* Supported token array formats */
#define XNLP_TOKEN_FORMAT_INFIX        0
#define XNLP_TOKEN_FORMAT_POSTFIX      1
#define XNLP_TOKEN_FORMAT_UNPARSED     XNLP_TOKEN_FORMAT_INFIX
#define XNLP_TOKEN_FORMAT_PARSED       XNLP_TOKEN_FORMAT_POSTFIX

/* Index of the objective row */
#define XNLP_OBJECTIVE_ROW           -1

int XNLP_CC XNLPinit        (const char *xpress);
int XNLP_CC XNLPfree        (void);
int XNLP_CC XNLPcreateprob  (XNLPprob *prob, int optimization_sense);
int XNLP_CC XNLPdestroyprob (XNLPprob *prob);
int XNLP_CC XNLPwriteprob   (XNLPprob  prob, const char *file, int format, int options);
int XNLP_CC XNLPgetlasterror(XNLPprob  prob, char *message_buffer, int buffer_size, int *source, int *code);
int XNLP_CC XNLPoptimize    (XNLPprob  prob, int *status);

int XNLP_CC XNLPsetoptimizationsense(XNLPprob prob, int  sense);
int XNLP_CC XNLPgetoptimizationsense(XNLPprob prob, int *sense);

int XNLP_CC XNLPsetinitialvalues(XNLPprob prob, int count, const int col_index[], const double value[]);
int XNLP_CC XNLPsetcolumntype   (XNLPprob prob, int count, const int col_index[], const char   type[]);
int XNLP_CC XNLPsetrowtype      (XNLPprob prob, int count, const int row_index[], const char   type[]);
int XNLP_CC XNLPsetbounds       (XNLPprob prob, int count, const int col_index[],
                                 const double lower[],     const double upper[]);
int XNLP_CC XNLPsetcolumnnames  (XNLPprob prob, int count, const int col_index[], const char * const name[]);
int XNLP_CC XNLPsetrownames     (XNLPprob prob, int count, const int row_index[], const char * const name[]);

int XNLP_CC XNLPgetintattribute   (XNLPprob prob, int attr, int    *val);
int XNLP_CC XNLPsetintcontrol     (XNLPprob prob, int ctrl, int     val);
int XNLP_CC XNLPgetintcontrol     (XNLPprob prob, int ctrl, int    *val);
int XNLP_CC XNLPgetdoubleattribute(XNLPprob prob, int attr, double *val);
int XNLP_CC XNLPsetdoublecontrol  (XNLPprob prob, int ctrl, double  val);
int XNLP_CC XNLPgetdoublecontrol  (XNLPprob prob, int ctrl, double *val);

int XNLP_CC XNLPgetsolverintattribute   (XNLPprob prob, int attr, int    *val);
int XNLP_CC XNLPsetsolverintcontrol     (XNLPprob prob, int ctrl, int     val);
int XNLP_CC XNLPgetsolverintcontrol     (XNLPprob prob, int ctrl, int    *val);
int XNLP_CC XNLPgetsolverdoubleattribute(XNLPprob prob, int attr, double *val);
int XNLP_CC XNLPsetsolverdoublecontrol  (XNLPprob prob, int ctrl, double  val);
int XNLP_CC XNLPgetsolverdoublecontrol  (XNLPprob prob, int ctrl, double *val);

int XNLP_CC XNLPcreaterows       (XNLPprob prob, int count, 
                                  const char * const name[], const char type[]);
int XNLP_CC XNLPcreatecolumns    (XNLPprob prob, int count,
                                  const char * const name[],
                                  const char    type[],
                                  const double  lower_bound[],
                                  const double  upper_bound[]);
int XNLP_CC XNLPclearrows        (XNLPprob prob, int count, const int row_index[]);
int XNLP_CC XNLPaddconstantterms (XNLPprob prob, int count,
                                  const int    row_index[], 
                                  const double value[]);
int XNLP_CC XNLPaddrhsterms      (XNLPprob prob, int count, 
                                  const int    row_index[],
                                  const double value[]);
int XNLP_CC XNLPaddlinearterms   (XNLPprob prob, int count,
                                  const int    row_index[],
                                  const int    row_offset[],
                                  const int    col_index[], 
                                  const double value[]);
int XNLP_CC XNLPaddquadraticterms(XNLPprob prob, int count,
                                  const int    row_index[], 
                                  const int    row_offset[],
                                  const int    col_index1[],
                                  const int    col_index2[],
                                  const double value[]);
int XNLP_CC XNLPaddtokenizedterms(XNLPprob prob, int format, int count,
                                  const int    row_index[],
                                  const int    row_offset[],
                                  const int    token_type[],
                                  const double token_value[]);

int XNLP_CC XNLPtokenizeexpression(XNLPprob prob, const char *expr, int max_size,
                                   int *size, int token_type[], double token_value[]);
int XNLP_CC XNLPdescriberow       (XNLPprob prob, int row, int max_size, int *size, char *expr);

int XNLP_CC XNLPregistermessagecallback  (XNLPprob             prob, 
                                          XNLPmessagecallback  callback,
                                          void                *user_data,
                                          int                 *handle);
int XNLP_CC XNLPderegistermessagecallback(XNLPprob prob, int handle, void **user_data);
int XNLP_CC XNLPclearmessagecallbacks    (XNLPprob prob);

int XNLP_CC XNLPopenlogfile  (XNLPprob prob, const char *file);
int XNLP_CC XNLPappendlogfile(XNLPprob prob, const char *file);
int XNLP_CC XNLPcloselogfile (XNLPprob prob, const char *file);
int XNLP_CC XNLPcloselogfiles(XNLPprob prob);

int XNLP_CC XNLPenableconsoleoutput (XNLPprob prob);
int XNLP_CC XNLPdisableconsoleoutput(XNLPprob prob);

int XNLP_CC XNLPgetsolution   (XNLPprob prob, int count, 
                               const int col_indices[], double value[]);
int XNLP_CC XNLPgetreducedcost(XNLPprob prob, int count,
                               const int col_indices[], double value[]);
int XNLP_CC XNLPgetslack      (XNLPprob prob, int count,
                               const int row_indices[], double value[]);
int XNLP_CC XNLPgetdual       (XNLPprob prob, int count,
                               const int row_indices[], double value[]);

#ifdef __cplusplus
}
#endif

#endif
