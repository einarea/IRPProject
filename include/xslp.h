/***************************************************************************\
 *                                                                         *
 * xslp.h                                                                  *
 *                                                                         *
 * Declarations neccesary for using Xpress-XSLP from C                     *
 *                                                                         *
 * (c) 1984-2015 Fair Isaac Corporation                                    *
 * For FICO Xpress XSLP v28.01.04                                          *
 * All rights reserved                                                     *
 *                                                                         *
\***************************************************************************/

#ifndef XSLP_H
#define XSLP_H


/***************************************************************************\
 * calling conventions for Windows                                         *
\**************************************************************************/
#ifdef _WIN32
#define XSLP_CC __stdcall
#else
#define XSLP_CC
#endif


#ifndef XSLP_CC
#define XSLP_CC XPRS_CC
#endif


typedef struct tagXSLPproblem *XSLPprob;
typedef int (XPRS_CC *XSLPcbFunc)(XSLPprob,...);



/***************************************************************************\
  * Integer problem attributes                                             *
\**************************************************************************/
#define XSLP_ITER                     12001       //  SLP - NLP iteration count
#define XSLP_STATUS                   12002       //  Bitmap holding the problem convergence status
#define XSLP_UNCONVERGED              12003       //  Number of unconverged values
#define XSLP_SBXCONVERGED             12004       //  Number of step-bounded variables converged only on extended criteria
#define XSLP_CVS                      12005       //  Number of character variables
#define XSLP_XVS                      12006       //  Number of extended variable arrays
#define XSLP_UFS                      12007       //  Number of user functions
#define XSLP_IFS                      12008       //  Number of internal functions
#define XSLP_PENALTYDELTAROW          12009       //  Index of equality row holding the penalties for delta vectors
#define XSLP_PENALTYDELTACOLUMN       12010       //  Index of column costing the penalty delta row
#define XSLP_PENALTYERRORROW          12011       //  Index of equality row holding the penalties for penalty error vectors
#define XSLP_PENALTYERRORCOLUMN       12012       //  Index of column costing the penalty error row
#define XSLP_EQUALSCOLUMN             12013       //  Index of the reserved '=' column
#define XSLP_VARIABLES                12014       //  Number of nonlinear variables
#define XSLP_IMPLICITVARIABLES        12015       //  Number of nonlinear variables appearing only in coefficients
#define XSLP_COEFFICIENTS             12016       //  Number of nonlinear coefficients
#define XSLP_PENALTYDELTAS            12017       //  Number of penalty delta vectors
#define XSLP_PENALTYERRORS            12018       //  Number of penalty error vectors
#define XSLP_PLUSPENALTYERRORS        12019       //  Number of positive penalty error vectors
#define XSLP_MINUSPENALTYERRORS       12020       //  Number of negative penalty error vectors
#define XSLP_UCCONSTRAINEDCOUNT       12021       //  Number of unconverged variables with coefficients in constraining rows
#define XSLP_MIPNODES                 12022       //  Number of nodes explored in MISLP
#define XSLP_MIPITER                  12023       //  Total number of SLP iterations in MISLP
#define XSLP_VERSION                  12024       //  Xpress-SLP major version number
#define XSLP_MINORVERSION             12025       //  Xpress-SLP minor version number
#define XSLP_NONLINEARCONSTRAINTS     12026       //  Number of nonlinear constraints in the problem
#define XSLP_TOLSETS                  12028       //  Number of tolerance sets
#define XSLP_INTERNALFUNCCALLS        12030       //  Number of calls made to internal functions
#define XSLP_USERFUNCCALLS            12031       //  Number of calls made to user functions
#define XSLP_UFINSTANCES              12034       //  Number of user function instances
#define XSLP_ECFCOUNT                 12035       //  Number of infeasible constraints found at the point of linearization
#define XSLP_USEDERIVATIVES           12037       //  Indicates whether numeric or analytic derivatives were used to create the linear approximations and solve the problem 
#define XSLP_PRESOLVEPASSES           12038       //  Number of passes made by the SLP nonlinear presolve procedure
#define XSLP_DELTAS                   12041       //  Number of delta vectors created during augmentation
#define XSLP_KEEPBESTITER             12042       //  If KEEPBEST is set and returned, the iteration where it was achieved
#define XSLP_NLPSTATUS                12044       //  Status of the problem being solved
#define XSLP_ZEROESRESET              12046       //  Number of placeholder entries set to zero
#define XSLP_ZEROESTOTAL              12047       //  Number of potential zero placeholder entries
#define XSLP_ZEROESRETAINED           12048       //  Number of potentially zero placeholders left untouched
#define XSLP_PRESOLVEFIXEDSLPVAR      12051       //  Number of SLP variables fixed by XSLPpresolve
#define XSLP_PRESOLVEFIXEDDR          12052       //  Number of determining rows fixed by XSLPpresolve
#define XSLP_PRESOLVEFIXEDCOEF        12053       //  Number of SLP coefficients fixed by XSLPpresolve
#define XSLP_PRESOLVEFIXEDZCOL        12054       //  Number of variables fixed at zero by XSLPpresolve
#define XSLP_PRESOLVEFIXEDNZCOL       12055       //  Number of variables fixed to a nonzero value by XSLPpresolve
#define XSLP_PRESOLVEDELETEDDELTA     12056       //  Number of potential delta variables deleted by XSLPpresolve
#define XSLP_PRESOLVETIGHTENED        12057       //  Number of bounds tightened by XSLPpresolve
#define XSLP_NONCONSTANTCOEFFS        12058       //  Number of coefficients in the augmented problem that might change between SLP iterations 
#define XSLP_SOLVERSELECTED           12075       //  The selected solver for the solve, mapped from solver
#define XSLP_MODELROWS                12079       //  Number of rows in the problem as loaded by the user
#define XSLP_MODELCOLS                12080       //  Number of columns in the problem as loaded by the user
#define XSLP_JOBID                    12081       //  The identifier for the current job (e.g. used in multistart)
#define XSLP_MSSTATUS                 12085       //  The status of the multi-start search
#define XSLP_PRESOLVESTATE            12087       //  If the problem currently in a presolved - transformed form
#define XSLP_MIPSOLS                  12088       //  The number of MIP solutions found so far
#define XSLP_STOPSTATUS               12089       //  Number of eliminations in XSLPpresolve
#define XSLP_PRESOLVEELIMINATIONS     12090       //  Status of the optimization process



/***************************************************************************\
  * Double control variables                                               *
\**************************************************************************/
#define XSLP_DAMP                        12103       //  Damping factor for updating values of variables
#define XSLP_DAMPEXPAND                  12104       //  Multiplier to increase damping factor during dynamic damping
#define XSLP_DAMPSHRINK                  12105       //  Multiplier to decrease damping factor during dynamic damping
#define XSLP_DELTA_A                     12106       //  Absolute perturbation of values for calculating numerical derivatives
#define XSLP_DELTA_R                     12107       //  Relative perturbation of values for calculating numerical derivatives
#define XSLP_DELTA_Z                     12108       //  Zero tolerance used when calculating numerical derivatives
#define XSLP_DELTACOST                   12109       //  Initial penalty cost multiplier for penalty delta vectors
#define XSLP_DELTAMAXCOST                12110       //  Maximum penalty cost multiplier for penalty delta vectors
#define XSLP_DJTOL                       12112       //  Tolerance on DJ value for determining if a variable is at its step bound
#define XSLP_ERRORCOST                   12113       //  Initial penalty cost multiplier for penalty error vectors
#define XSLP_ERRORMAXCOST                12114       //  Maximum penalty cost multiplier for penalty error vectors
#define XSLP_ERRORTOL_A                  12116       //  Absolute tolerance for error vectors
#define XSLP_EXPAND                      12118       //  Multiplier to increase a step bound
#define XSLP_INFINITY                    12119       //  Value returned by a divide-by-zero in a formula
#define XSLP_MAXWEIGHT                   12120       //  Maximum penalty weight for delta or error vectors
#define XSLP_MINWEIGHT                   12121       //  Minimum penalty weight for delta or error vectors
#define XSLP_SHRINK                      12122       //  Multiplier to reduce a step bound
#define XSLP_ZERO                        12123       //  Absolute zero tolerance
#define XSLP_CTOL                        12124       //  Closure convergence tolerance
#define XSLP_ATOL_A                      12125       //  Absolute delta convergence tolerance
#define XSLP_ATOL_R                      12126       //  Relative delta convergence tolerance
#define XSLP_MTOL_A                      12127       //  Absolute effective matrix element convergence tolerance
#define XSLP_MTOL_R                      12128       //  Relative effective matrix element convergence tolerance
#define XSLP_ITOL_A                      12129       //  Absolute impact convergence tolerance
#define XSLP_ITOL_R                      12130       //  Relative impact convergence tolerance
#define XSLP_STOL_A                      12131       //  Absolute slack convergence tolerance
#define XSLP_STOL_R                      12132       //  Relative slack convergence tolerance
#define XSLP_MVTOL                       12133       //  Marginal value tolerance for determining if a constraint is slack
#define XSLP_XTOL_A                      12134       //  Absolute static objective function (1) tolerance
#define XSLP_XTOL_R                      12135       //  Relative static objective function (1) tolerance
#define XSLP_DEFAULTSTEPBOUND            12136       //  Minimum IV for the step bound of an SLP variable if none is explicitly given
#define XSLP_DAMPMAX                     12137       //  Maximum value for the damping factor of a variable during dynamic damping
#define XSLP_DAMPMIN                     12138       //  Minimum value for the damping factor of a variable during dynamic damping
#define XSLP_DELTACOSTFACTOR             12139       //  Factor for increasing cost multiplier on total penalty delta vectors
#define XSLP_ERRORCOSTFACTOR             12140       //  Factor for increasing cost multiplier on total penalty error vectors
#define XSLP_ERRORTOL_P                  12141       //  Absolute tolerance for printing error vectors
#define XSLP_CASCADETOL_PA               12142       //  Absolute cascading print tolerance
#define XSLP_CASCADETOL_PR               12143       //  Relative cascading print tolerance
#define XSLP_DEFAULTIV                   12145       //  Default initial value for an SLP variable if none is explicitly given
#define XSLP_OBJSENSE                    12146       //  Objective function sense
#define XSLP_OTOL_A                      12150       //  Absolute static objective (2) convergence tolerance
#define XSLP_OTOL_R                      12151       //  Relative static objective (2) convergence tolerance
#define XSLP_DELTA_X                     12152       //  Minimum absolute value of delta coefficients to be retained
#define XSLP_ERRORCOSTS                  12153       //  Total penalty costs in the solution
#define XSLP_EQTOL_A                     12154       //  Absolute tolerance on equality testing in logical functions
#define XSLP_EQTOL_R                     12155       //  Relative tolerance on equality testing in logical functions
#define XSLP_GRANULARITY                 12157       //  Base for calculating penalty costs
#define XSLP_MIPCUTOFF_A                 12158       //  Absolute objective function cutoff for MIP termination
#define XSLP_MIPCUTOFF_R                 12159       //  Absolute objective function cutoff for MIP termination
#define XSLP_MIPOTOL_A                   12160       //  Absolute objective function tolerance for MIP termination
#define XSLP_MIPOTOL_R                   12161       //  Relative objective function tolerance for MIP termination
#define XSLP_MEMORYFACTOR                12164       //  Factor for expanding size of dynamic arrays in memory
#define XSLP_VALIDATIONTOL_A             12165       //  Absolute tolerance for the XSLPvalidate procedure
#define XSLP_VALIDATIONTOL_R             12166       //  Relative tolerance for the XSLPvalidate procedure
#define XSLP_VALIDATIONINDEX_A           12167       //  Absolute validation index
#define XSLP_VALIDATIONINDEX_R           12168       //  Relative validation index
#define XSLP_ESCALATION                  12169       //  Factor for increasing cost multiplier on individual penalty error vectors
#define XSLP_OBJTOPENALTYCOST            12170       //  Factor to estimate initial penalty costs from objective function
#define XSLP_SHRINKBIAS                  12171       //  Shrink side step bound update overwrite for improving iterations
#define XSLP_FEASTOLTARGET               12172       //  Ideal target of feasibility level of the linearizations
#define XSLP_OPTIMALITYTOLTARGET         12173       //  Ideal target of optimality level of the linearizations
#define XSLP_VTOL_A                      12177       //  Absolute static objective (3) convergence tolerance
#define XSLP_VTOL_R                      12178       //  Relative static objective (3) convergence tolerance
#define XSLP_OBJVAL                      12179       //  Objective function value excluding any penalty costs
#define XSLP_ETOL_A                      12180       //  Absolute tolerance on penalty vectors
#define XSLP_ETOL_R                      12181       //  Relative tolerance on penalty vectors
#define XSLP_EVTOL_A                     12182       //  Absolute tolerance on total penalty costs
#define XSLP_EVTOL_R                     12183       //  Relative tolerance on total penalty costs
#define XSLP_DELTA_ZERO                  12184       //  Absolute zero tolerance for derivatives. Smaller values not pushed by DELTA_Z
#define XSLP_MINSBFACTOR                 12185       //  The min step bound size no longer decreased is max(ATOL_A, CTOL*IV)*MINSBFACTOR
#define XSLP_CLAMPVALIDATIONTOL_A        12186       //  Absolute validation violation before clamping is applied
#define XSLP_CLAMPVALIDATIONTOL_R        12187       //  Relative validation violation before clamping is applied
#define XSLP_CLAMPSHRINK                 12188       //  Step bound shrinking ratio applied to step bounds during clamping
#define XSLP_ECFTOL_A                    12189       //  Absolute tolerance on testing feasibility at the point of linearization
#define XSLP_ECFTOL_R                    12190       //  Relative tolerance on testing feasibility at the point of linearization
#define XSLP_WTOL_A                      12191       //  Absolute extended convergence continuation tolerance
#define XSLP_WTOL_R                      12192       //  Relative extended convergence continuation tolerance
#define XSLP_PRESOLVEZERO                12193       //  Minimum absolute value for a variable which is identified as nonzero during SLP presolve 
#define XSLP_MATRIXTOL                   12194       //  Override value for the smallest element that can be loaded into the linearization
#define XSLP_DRFIXRANGE                  12195       //  Relaxation relative radius around previous value for CASCADE_DRCOL_PVRANGE
#define XSLP_DRCOLTOL                    12196       //  Minimum value of the main column multiplied with the variable being determined 
#define XSLP_MERITLAMBDA                 12197       //  Ratio, by which the objective can dominate feasibility in the objective
#define XSLP_MIPERRORTOL_A               12198       //  Absolute penalty error cost tolerance for MIP cut-off
#define XSLP_MIPERRORTOL_R               12199       //  Relative penalty error cost tolerance for MIP cut-off
#define XSLP_CDTOL_A                     12200       //  Absolute tolerance for deducing constant derivatives
#define XSLP_CDTOL_R                     12201       //  Relative tolerance for deducing constant derivatives
#define XSLP_ENFORCEMAXCOST              12202       //  Maximum penalty cost in the objective before enforcing most violating rows
#define XSLP_ENFORCECOSTSHRINK           12203       //  Factor by which to decrease the current penalty multiplier when enforcing rows 
#define XSLP_MSMAXBOUNDRANGE             12204       //  The maximum perturbation to any initial value non ranged variables
#define XSLP_VALIDATIONTOL_K             12205       //  Relative tolerance for the XSLPvalidatekkt procedure
#define XSLP_PRESOLVE_ELIMTOL            12206       //  Presolve nonlinear elimination tolerance
#define XSLP_DRCOLDJTOL                  12208       //  Reduced cost tolerance on the delta variable for the drcol handler



/***************************************************************************\
  * Integer control variables                                              *
\**************************************************************************/
#define XSLP_ALGORITHM                12301       //  Bit map describing the SLP algorithm(s) to be used
#define XSLP_AUGMENTATION             12302       //  Bit map describing the SLP augmentation method(s) to be used
#define XSLP_BARLIMIT                 12303       //  Number of initial SLP iterations using the barrier method
#define XSLP_CASCADE                  12304       //  Bit map describing the cascading to be used
#define XSLP_CASCADENLIMIT            12306       //  Maximum number of iterations for cascading with non-linear determining rows
#define XSLP_CONTROL                  12307       //  Bit map describing which SLP functions also activate the corresponding Optimizer Library function 
#define XSLP_DAMPSTART                12308       //  SLP iteration at which damping is activated
#define XSLP_DCLIMIT                  12309       //  Default iteration delay for delayed constraints
#define XSLP_DELTAZLIMIT              12311       //  Number of SLP iterations during which to apply XSLP_DELTA_Z
#define XSLP_FUNCEVAL                 12312       //  Bit map for determining the method of evaluating user functions and their derivatives 
#define XSLP_INFEASLIMIT              12314       //  The maximum number of consecutive infeasible SLP iterations which can occur before Xpress-SLP terminates 
#define XSLP_ITERLIMIT                12315       //  The maximum number of SLP iterations
#define XSLP_LOG                      12316       //  Level of printing during SLP iterations
#define XSLP_SAMECOUNT                12317       //  Number of steps reaching the step bound in the same direction before step bounds are increased 
#define XSLP_SAMEDAMP                 12319       //  Number of steps in same direction before damping factor is increased
#define XSLP_SBSTART                  12320       //  SLP iteration after which step bounds are first applied
#define XSLP_XCOUNT                   12321       //  Number of SLP iterations over which to measure static objective (1) convergence 
#define XSLP_XLIMIT                   12322       //  Number of SLP iterations up to which static objective (1) convergence testing starts 
#define XSLP_EXTRACVS                 12323       //  Expansion number for character variables
#define XSLP_EXTRAUFS                 12324       //  Expansion number for user functions
#define XSLP_EXTRAXVS                 12325       //  Expansion number for XVs
#define XSLP_EXTRAXVITEMS             12326       //  Expansion number for XV items
#define XSLP_DELAYUPDATEROWS          12329       //  Number of SLP iterations before update rows are fully activated
#define XSLP_AUTOSAVE                 12330       //  Frequency with which to save information about the solution process
#define XSLP_ANALYZE                  12332       //  Extra analytics options not directly affecting the optimization
#define XSLP_OCOUNT                   12333       //  Number of SLP iterations over which to measure objective function variation for static objective (2) convergence criterion 
#define XSLP_EVALUATE                 12334       //  Evaluation strategy for user functions
#define XSLP_EXCELVISIBLE             12335       //  Display of Excel when evaluating user functions written in Excel
#define XSLP_MIPALGORITHM             12336       //  Bitmap describing the MISLP algorithms to be used
#define XSLP_MIPRELAXSTEPBOUNDS       12337       //  Bitmap describing the step-bound relaxation strategy during MISLP
#define XSLP_MIPFIXSTEPBOUNDS         12338       //  Bitmap describing the step-bound fixing strategy during MISLP
#define XSLP_MIPITERLIMIT             12339       //  Maximum number of SLP iterations at each node
#define XSLP_MIPCUTOFFLIMIT           12340       //  Number of SLP iterations to check when considering a node for cutting off
#define XSLP_MIPOCOUNT                12341       //  Number of SLP iterations at each node over which to measure objective function variation 
#define XSLP_MIPDEFAULTALGORITHM      12343       //  Default algorithm to be used during the global search in MISLP
#define XSLP_PRESOLVE                 12344       //  Indicates if NLP presolve is on
#define XSLP_TIMEPRINT                12345       //  Print additional timings during SLP optimization
#define XSLP_SLPLOG                   12346       //  Frequency with which SLP log is printed
#define XSLP_MIPLOG                   12347       //  Frequency with which MIP log is printed
#define XSLP_DELTAOFFSET              12348       //  Position of first char of SLP variable name used to create name of delta vector 
#define XSLP_UPDATEOFFSET             12349       //  Position of first char of SLP variable name used to create name of SLP update row 
#define XSLP_ERROROFFSET              12350       //  Position of first char of constraint name used to create name of penalty error vectors 
#define XSLP_SBROWOFFSET              12351       //  Position of first char of SLP variable name used to create name of SLP lower and upper step bound rows 
#define XSLP_SOLVER                   12352       //  Solver library selection. -1: auto, 0 XSLP, 1 Knitro
#define XSLP_STOPOUTOFRANGE           12354       //  Stop optimization and return error code if internal function argument is out of range 
#define XSLP_DISABLEAPICHECKS         12355       //  If the API should check for consistency and calling context
#define XSLP_VCOUNT                   12356       //  Number of SLP iterations over which to measure static objective (3) convergence
#define XSLP_VLIMIT                   12357       //  Number of SLP iterations after which static objective (3) convergence testing starts 
#define XSLP_THREADSAFEUSERFUNC       12359       //  If concurrent calls to user functions are allowed
#define XSLP_JACOBIAN                 12360       //  Determines how the Jacobian is calculated
#define XSLP_HESSIAN                  12361       //  Determines how the Hessian is calculated
#define XSLP_MULTISTART               12362       //  If the multi-start algorithm should be executed
#define XSLP_MULTISTART_THREADS       12363       //  Number of threads to be used in a multistart run
#define XSLP_MULTISTART_MAXSOLVES     12364       //  Maximum number of solves to execute in the multi-start search
#define XSLP_MULTISTART_MAXTIME       12365       //  Maximum total time to be spent in the multi-start search (+:only after feasible)
#define XSLP_MAXTIME                  12366       //  The maximum time in seconds that the optimization will run before it terminates 
#define XSLP_SCALE                    12367       //  When to re-scale the SLP problem
#define XSLP_SCALECOUNT               12368       //  Iteration limit used in determining when to re-scale the SLP matrix
#define XSLP_ECFCHECK                 12369       //  Check feasibility at the point of linearization for extended convergence criteria 
#define XSLP_MIPCUTOFFCOUNT           12370       //  Number of SLP iterations to check when considering a node for cutting off
#define XSLP_NLCONNECTOR              12372       //  If the Knitro solve path is allowed to call out to xknitronl
#define XSLP_DERIVATIVES              12373       //  Method of calculating derivatives
#define XSLP_WCOUNT                   12374       //  Number of SLP iterations over which to measure the objective for the extended convergence continuation criterion 
#define XSLP_PRESOLVEPASSLIMIT        12375       //  Maximum number of passes through the problem to improve SLP bounds
#define XSLP_UNFINISHEDLIMIT          12376       //  Number of times within one SLP iteration that an unfinished LP optimization will be continued 
#define XSLP_CONVERGENCEOPS           12377       //  Bitmap control determining the convergence tests applied
#define XSLP_ZEROCRITERION            12378       //  Bitmap determining the behavior of the placeholder deletion procedure
#define XSLP_ZEROCRITERIONSTART       12379       //  SLP iteration at which criteria for deletion of placeholder entries are first activated 
#define XSLP_ZEROCRITERIONCOUNT       12380       //  Number of consecutive times a placeholder entry is zero before being considered for deletion 
#define XSLP_LSPATTERNLIMIT           12381       //  Number of exploratory steps to be carried out in the line search
#define XSLP_LSITERLIMIT              12382       //  Maximum number of iterations in the line search
#define XSLP_LSSTART                  12383       //  Iteration from which line search is applied (if other LS options are on)
#define XSLP_PENALTYINFOSTART         12384       //  Iteration from which to record row penalty information
#define XSLP_DCLOG                    12385       //  Amount of logging information for activation of delayed constraints
#define XSLP_DECOMPOSE                12386       //  Bitmap controlling the action of function XSLPdecompose
#define XSLP_FILTER                   12387       //  Bitmap controlling how solution updates should be filtered
#define XSLP_TRACEMASKOPS             12388       //  What information to trace using the TRACEMASK control
#define XSLP_LSZEROLIMIT              12389       //  Maximum number of consecutive zero step sizes accepted by the line search
#define XSLP_DECOMPOSEPASSLIMIT       12390       //  Maximum number of repeats of presolve+decompose
#define XSLP_REFORMULATE              12392       //  If problem classes should be recognised and transformed for efficient solve
#define XSLP_PRESOLVEOPS              12393       //  Bitmap indicating the SLP presolve actions to be taken (default: boundreduce)
#define XSLP_AUTOTUNE                 12394       //  If XSLP is to analyze the problem and attempt to tune the settings
#define XSLP_MULTISTART_LOG           12395       //  Level of logging during the multistart
#define XSLP_MULTISTART_SEED          12396       //  The random number seed used to while generating initial point in the multistart
#define XSLP_MULTISTART_POOLSIZE      12397       //  The maximum number of jobs that are allowed to be pooled up in deterministic ms
#define XSLP_POSTSOLVE                12398       //  Indicates if NLP postsolve is automatically carried out
#define XSLP_DETERMINISTIC            12399       //  Indicates if the parallel features of SLP is expected to be deterministic
#define XSLP_HEURSTRATEGY             12400       //  Level of heuristics MISLP should be performing
#define XSLP_ECHOXPRSMESSAGES         12401       //  If SLP should itself re-echo the optimizer messages
#define XSLP_PRESOLVELEVEL            12402       //  Presolving level (how much the individual features may change the problem)



/***************************************************************************\
  * Character string control variables                                     *
\**************************************************************************/
#define XSLP_CVNAME                   12451       //  MPS file: name of the character variable set to use
#define XSLP_DELTAFORMAT              12452       //  Formatting string for creation of names for SLP delta vectors
#define XSLP_IVNAME                   12453       //  MPS file: name of the initial value set to use
#define XSLP_MINUSDELTAFORMAT         12456       //  Formatting string for creation of names for SLP negative penalty delta vectors 
#define XSLP_MINUSERRORFORMAT         12457       //  Formatting string for creation of names for SLP negative penalty error vectors 
#define XSLP_PLUSDELTAFORMAT          12458       //  Formatting string for creation of names for SLP positive penalty delta vectors 
#define XSLP_PLUSERRORFORMAT          12459       //  Formatting string for creation of names for SLP positive penalty error vectors 
#define XSLP_SBNAME                   12460       //  MPS file: name of the step bound set to use
#define XSLP_TOLNAME                  12461       //  MPS file: name of the tolerance set to use
#define XSLP_UPDATEFORMAT             12462       //  Formatting string for creation of names for SLP update rows
#define XSLP_PENALTYROWFORMAT         12463       //  Formatting string for creation of the names of the SLP penalty rows
#define XSLP_PENALTYCOLFORMAT         12464       //  Formatting string for creation of the names of the SLP penalty transfer vectors
#define XSLP_SBLOROWFORMAT            12467       //  Formatting string for creation of names for SLP lower step bound rows
#define XSLP_SBUPROWFORMAT            12468       //  Formatting string for creation of names for SLP upper step bound rows
#define XSLP_TRACEMASK                12472       //  Mask for rows and columns to be traced for changes through the SLP iterations



/***************************************************************************\
  * Character string attributes                                            *
\**************************************************************************/
#define XSLP_UNIQUEPREFIX             12501       //  Unique prefix for generated names
#define XSLP_VERSIONDATE              12502       //  Date of creation of Xpress-SLP



/***************************************************************************\
  * Pointer attributes                                                     *
\**************************************************************************/
#define XSLP_XPRSPROBLEM              12601       //  The underlying Optimizer problem pointer
#define XSLP_XSLPPROBLEM              12602       //  The Xpress-SLP problem pointer
#define XSLP_GLOBALFUNCOBJECT         12607       //  The user-defined global function object
#define XSLP_USERFUNCOBJECT           12608       //  The user function object for the current function
#define XSLP_INSTANCEFUNCOBJECT       12609       //  The instance function object for the current instance
#define XSLP_MIPPROBLEM               12611       //  The underlying Optimizer MIP problem
#define XSLP_SOLUTIONPOOL             12613       //  Access to the XSLP solution pool (see XSLP_ANALYZE)



/***************************************************************************\
  * Double attributes                                                      *
\**************************************************************************/
#define XSLP_CURRENTDELTACOST         12701       //  Current value of penalty cost multiplier for penalty delta vectors
#define XSLP_CURRENTERRORCOST         12702       //  Current value of penalty cost multiplier for penalty error vectors
#define XSLP_PENALTYERRORTOTAL        12704       //  Total activity of penalty error vectors
#define XSLP_PENALTYERRORVALUE        12705       //  Total penalty cost attributed to penalty error vectors
#define XSLP_PENALTYDELTATOTAL        12706       //  Total activity of penalty delta vectors
#define XSLP_PENALTYDELTAVALUE        12707       //  Total penalty cost attributed to penalty delta vectors
#define XSLP_VSOLINDEX                12708       //  Vertex solution index
#define XSLP_VALIDATIONINDEX_K        12718       //  The level of dual side KKT violation of the solution, set by validatekkt



/***************************************************************************\
  * Knitro parameter mapping (to be used in XSLP parameter functions)      *
\**************************************************************************/
#define XKTR_PARAM_NEWPOINT             101001       //  Extra action to take after every iteration in a solve of a continuous problem
#define XKTR_PARAM_HONORBNDS            101002       //  If enforce satisfaction of simple variable bounds throughout the optimization
#define XKTR_PARAM_ALGORITHM            101003       //  Algorithm selection
#define XKTR_PARAM_BAR_MURULE           101004       //  Which strategy to use for modifying the barrier parameter
#define XKTR_PARAM_BAR_FEASIBLE         101006       //  If special emphasis on getting and staying feasible in the IP algorithms
#define XKTR_PARAM_GRADOPT              101007       //  How to compute the gradients of the objective and constraint functions
#define XKTR_PARAM_HESSOPT              101008       //  How to compute the (approximate) Hessian of the Lagrangian
#define XKTR_PARAM_BAR_INITPT           101009       //  If an initial point strategy is used with barrier algorithms
#define XKTR_PARAM_MAXCGIT              101013       //  Maximum number of inner CG iterations per minor iteration
#define XKTR_PARAM_MAXIT                101014       //  Maximum number of iterations before termination
#define XKTR_PARAM_OUTLEV               101015       //  Level of output produced by KNITRO
#define XKTR_PARAM_SCALE                101017       //  If the problem should be scaled
#define XKTR_PARAM_SOC                  101019       //  Whether or not to try second order corrections
#define XKTR_PARAM_DELTA                101020       //  Initial trust region radius factor to determine the initial trust region size
#define XKTR_PARAM_BAR_FEASMODETOL      101021       //  Tolerance determining whether to force subsequent iterates to remain feasible
#define XKTR_PARAM_FEASTOL              101022       //  Final relative stopping tolerance for the feasibility error
#define XKTR_PARAM_FEASTOLABS           101023       //  Final absolute stopping tolerance for the feasibility error
#define XKTR_PARAM_BAR_INITMU           101025       //  Initial value for the barrier parameter
#define XKTR_PARAM_OBJRANGE             101026       //  Limits of the objective function for purposes of determining unboundedness
#define XKTR_PARAM_OPTTOL               101027       //  Relative stopping tolerance for the KKT (optimality) error
#define XKTR_PARAM_OPTTOLABS            101028       //  Absolute stopping tolerance for the KKT (optimality) error
#define XKTR_PARAM_PIVOT                101029       //  Initial pivot threshold used in factorization routines
#define XKTR_PARAM_XTOL                 101030       //  Relative change tolerance
#define XKTR_PARAM_DEBUG                101031       //  Controls the level of debugging output
#define XKTR_PARAM_MULTISTART           101033       //  If to solve from multiple start points to find a better local minimum
#define XKTR_PARAM_MSMAXSOLVES          101034       //  Number of start points to try in multi-start
#define XKTR_PARAM_MSMAXBNDRANGE        101035       //  Maximum range that unbounded variables can take when determining start points
#define XKTR_PARAM_LMSIZE               101038       //  Number of limited memory pairs stored if using the limited-memory BFGS option
#define XKTR_PARAM_BAR_MAXCROSSIT       101039       //  The maximum number of crossover iterations before termination
#define XKTR_PARAM_BLASOPTION           101042       //  Specifies the BLAS/LAPACK function library to use
#define XKTR_PARAM_BAR_MAXREFACTOR      101043       //  Max number of refactorizations of the KKT per iteration before reverting to CG 
#define XKTR_PARAM_BAR_MAXBACKTRACK     101044       //  Max number of backtracks in linesearch before reverting to a CG step
#define XKTR_PARAM_BAR_PENRULE          101049       //  Penalty parameter strategy for determining if to accept a trial iterate
#define XKTR_PARAM_BAR_PENCONS          101050       //  If a penalty approach is applied to the constraints
#define XKTR_PARAM_MSNUMTOSAVE          101051       //  Number of distinct feasible points to save to KNITRO_mspoints.log
#define XKTR_PARAM_MSSAVETOL            101052       //  Specifies the tolerance for deciding if two feasible points are distinct
#define XKTR_PARAM_MSTERMINATE          101054       //  Condition for terminating multi-start
#define XKTR_PARAM_MSSTARTPTRANGE       101055       //  Maximum range that each variable can take when determining new start points
#define XKTR_PARAM_INFEASTOL            101056       //  Relative tolerance used for declaring infeasibility of a model
#define XKTR_PARAM_LINSOLVER            101057       //  Which linear solver to use to solve linear systems
#define XKTR_PARAM_BAR_DIRECTINTERVAL   101058       //  Controls the maximum number of consecutive conjugate gradient (CG) steps
#define XKTR_PARAM_PRESOLVE             101059       //  If to use the KNITRO presolver
#define XKTR_PARAM_PRESOLVE_TOL         101060       //  Tolerance used by the KNITRO presolver
#define XKTR_PARAM_BAR_SWITCHRULE       101061       //  If the barrier can switch from an optimality phase to a pure feasibility phase 
#define XKTR_PARAM_MA_TERMINATE         101063       //  Termination condition for the multi-algorithm
#define XKTR_PARAM_MSSEED               101066       //  Seed value used to generate random initial points in multi-start
#define XKTR_PARAM_BAR_RELAXCONS        101077       //  Indicates whether a relaxation approach is applied to the constraints.
#define XKTR_PARAM_MIP_METHOD           102001       //  Which MIP method to use
#define XKTR_PARAM_MIP_BRANCHRULE       102002       //  Branching rule to use
#define XKTR_PARAM_MIP_SELECTRULE       102003       //  Node selection rule
#define XKTR_PARAM_MIP_OUTLEVEL         102010       //  How much MIP information to print
#define XKTR_PARAM_MIP_OUTINTERVAL      102011       //  Node printing interval for mip_outlevel
#define XKTR_PARAM_MIP_DEBUG            102013       //  Debugging level for MIP solution
#define XKTR_PARAM_MIP_IMPLICATNS       102014       //  If to add constraints to the MIP derived from logical implications
#define XKTR_PARAM_MIP_GUB_BRANCH       102015       //  If allow branching on generalized upper bounds
#define XKTR_PARAM_MIP_KNAPSACK         102016       //  Rules for adding MIP knapsack cuts
#define XKTR_PARAM_MIP_ROUNDING         102017       //  MIP rounding rule to apply
#define XKTR_PARAM_MIP_ROOTALG          102018       //  Algorithm to use for the root node solve in MIP
#define XKTR_PARAM_MIP_LPALG            102019       //  Algorithm to use for solving LPs in the MIP search
#define XKTR_PARAM_MIP_HEURISTIC        102022       //  Which MIP heuristic to apply for an initial integer feasible point
#define XKTR_PARAM_MIP_HEUR_MAXIT       102023       //  Maximum number of iterations to allow for MIP heuristic
#define XKTR_PARAM_MIP_PSEUDOINIT       102026       //  Method to initialize pseudo-costs for variables not yet branched on
#define XKTR_PARAM_MIP_STRONG_MAXIT     102027       //  Maximum number of iterations to allow for strong branching solves
#define XKTR_PARAM_MIP_STRONG_CANDLIM   102028       //  Maximum number of candidates to explore in strong branching
#define XKTR_PARAM_MIP_STRONG_LEVEL     102029       //  Maximum number of tree levels on which to perform strong branching
#define XKTR_PARAM_PAR_NUMTHREADS       103001       //  Number of threads to use for parallel computing features



/***************************************************************************\
  * Memory size controls                                                   *
\**************************************************************************/
#define XSLP_MEM_STRING               12801       //  Memory allocation for strings of all types
#define XSLP_MEM_CVAR                 12802       //  Memory allocation for character variables
#define XSLP_MEM_XV                   12803       //  Memory allocation for XVs
#define XSLP_MEM_XVITEM               12804       //  Memory allocation for individual XV entries
#define XSLP_MEM_UF                   12805       //  Memory allocation for user functions
#define XSLP_MEM_VAR                  12806       //  Memory allocation for SLP variables
#define XSLP_MEM_COEF                 12807       //  Memory allocation for nonlinear coefficients
#define XSLP_MEM_FORMULA              12808       //  Memory allocation for formulae
#define XSLP_MEM_STACK                12809       //  Memory allocation for parsed formulae, analytic derivatives
#define XSLP_MEM_COL                  12810       //  Memory allocation for additional information on matrix columns
#define XSLP_MEM_ROW                  12811       //  Memory allocation for additional information on matrix rows
#define XSLP_MEM_XROW                 12812       //  Memory allocation for extended row information
#define XSLP_MEM_TOL                  12813       //  Memory allocation for tolerance sets
#define XSLP_MEM_FORMULAVALUE         12814       //  Memory allocation for formula values and derivatives
#define XSLP_MEM_CALCSTACK            12815       //  Memory allocation for formula calculations
#define XSLP_MEM_ITERLOG              12816       //  Memory allocation for SLP iteration summary
#define XSLP_MEM_XF                   12817       //  Memory allocation for complicated functions
#define XSLP_MEM_XFVALUE              12818       //  Memory allocation for complicated function values
#define XSLP_MEM_XFNAMES              12819       //  Memory allocation for complicated function input and return names
#define XSLP_MEM_EXCELDOUBLE          12820       //  Memory allocation for return values from Excel user functions
#define XSLP_MEM_RETURNARRAY          12821       //  Memory allocation for return values from multi-valued user function
#define XSLP_MEM_FORMULAHASH          12823       //  Memory allocation for internal formula array
#define XSLP_MEM_DERIVATIVES          12837       //  Memory allocation for analytic derivatives


#define XSLP_OBJTOL_A XSLP_XTOL_A
#define XSLP_OBJTOL_R XSLP_XTOL_R



/***************************************************************************\
  * Token types                                                            *
\**************************************************************************/
#define XSLP_EOF                          0       //  End of formula
#define XSLP_CON                          1       //  Constant, (double) value
#define XSLP_CV                           2       //  Character, variable, index of character variable
#define XSLP_XV                           3       //  Extended variable array, index of XV
#define XSLP_VAR                          4       //  Variable, index of variable
#define XSLP_STRING                       5       //  Character string, internal index of character string
#define XSLP_CONSTRAINT                   8       //  Constraint, index of constraint
#define XSLP_ROW                          9       //  Row, index of matrix row
#define XSLP_COL                         10       //  Column, index of matrix column
#define XSLP_FUN                         11       //  User function, index of function
#define XSLP_IFUN                        12       //  Internal function, index of function
#define XSLP_MPVAR                       13       //  Mosel variable, index of variable
#define XSLP_GEXP                        14       //  Mosel general expression, index of expression
#define XSLP_GENCTR                      15       //  Mosel general constraint, index of constraint
#define XSLP_XVITEM                      16       //  Extended variable item, index of XV item
#define XSLP_LINCTR                      18       //  Mosel linear constraints, index of constraint
#define XSLP_LINEXP                      19       //  Mosel linear expression, index of expression
#define XSLP_VARREF                      20       //  Reference to variable, index of variable
#define XSLP_LB                          21       //  Left bracket
#define XSLP_RB                          22       //  Right bracket
#define XSLP_DEL                         24       //  Delimiter, XSLP_COMMA (1) = comma (','); XSLP_COLON (2) = colon (':')
#define XSLP_UNKFUN                      25       //
#define XSLP_MOSELREF                    26       //  Mosel variable reference
#define XSLP_MOSELSTR                    27       //  Mosel string reference
#define XSLP_MOSELFRM                    28       //  Mosel function reference
#define XSLP_SUFUN                       29       //  Simple unary internal function. Same as IFUN, but with no matching bracket
#define XSLP_OP                          31       //  Operator, XSLP_UMINUS (1) = unary minus ('-'), XSLP_EXPONENT (2) = exponent ('**' or '^'); XSLP_MULTIPLY (3) = multiplication ('*'); XSLP_DIVIDE (4) = division ('/'); XSLP_PLUS (5) = addition ('+'); XSLP_MINUS (6) = subtraction ('-')
#define XSLP_XVVARTYPE                   71       //  Type of variable in XV, XSLP_VAR or XSLP_XV
#define XSLP_XVVARINDEX                  72       //  Index of an XV item
#define XSLP_XVINTINDEX                  73       //  Index of XV item name, index of name in Xpress-SLP string table
#define XSLP_UFARGTYPE                   86       //  Requirements and types of argument for a user function, bitmap of types
#define XSLP_UFEXETYPE                   87       //  Linkage of a user function, bitmap of linkage information
#define XSLP_UNKNOWN                     99       //  Unidentified token, internal index of character string

/***************************************************************************\
  * Token operand types                                                     *
\**************************************************************************/
#define XSLP_UMINUS                       1       //  Operand, unary minus (-)
#define XSLP_EXPONENT                     2       //  Operand, exponent (**) or (^)
#define XSLP_MULTIPLY                     3       //  Operand, multiplication (*)
#define XSLP_DIVIDE                       4       //  Operand, division (/)
#define XSLP_PLUS                         5       //  Operand, addition (+)
#define XSLP_MINUS                        6       //  Operand, subtraction (-)
#define XSLP_LE                           7       //  Logical operand, less or equal (<=)
#define XSLP_EQ                           8       //  Logical operand, equal (=)
#define XSLP_GE                           9       //  Logical operand, greater or equal (<=)
#define XSLP_COMMA                        1       //  Delimiter comma (1)
#define XSLP_COLON                        2       //  Delimiter colon (2)



/***************************************************************************\
  * Internal functions                                                     *
\**************************************************************************/
#define XSLP_MATH_LOG                    13       //  Log to the base 10 maths function.
#define XSLP_MATH_LOG10                  14       //  Log to the base 10 maths function.
#define XSLP_MATH_LN                     15       //  Natural log maths function.
#define XSLP_MATH_EXP                    16       //  Exponential maths function.
#define XSLP_MATH_ABS                    17       //  Absolute value maths function.
#define XSLP_MATH_SQRT                   18       //  Square root maths function.
#define XSLP_LOGICAL_EQ                  19       //  Equality logical function.
#define XSLP_LOGICAL_LE                  20       //  Less than or equal to logical function.
#define XSLP_LOGICAL_GE                  21       //  Greater than or equal to logical function.
#define XSLP_LOGICAL_NE                  22       //  Not equal to logical function.
#define XSLP_LOGICAL_LT                  23       //  Less than logical function.
#define XSLP_LOGICAL_GT                  24       //  Greater than logical function.
#define XSLP_LOGICAL_IF                  25       //  Conditional if logical function.
#define XSLP_LOGICAL_NOT                 26       //  Not logical function.
#define XSLP_MATH_SIN                    27       //  Sine maths function (radians).
#define XSLP_MATH_COS                    28       //  Cosine maths function (radians).
#define XSLP_MATH_TAN                    29       //  Tangent maths function (radians).
#define XSLP_MATH_ARCSIN                 30       //  Arc-sine maths function (returns radians).
#define XSLP_MATH_ARCCOS                 31       //  Arc-cosine maths function (returns radians).
#define XSLP_MATH_ARCTAN                 32       //  Arc-tangent maths function (returns radians).
#define XSLP_MMX_MIN                     33       //  Minimum function accepting arbitrary numbers of arguments and returning the minimum. 
#define XSLP_MMX_MAX                     34       //  Maximum function accepting arbitrary numbers of arguments and returning the maximum. 
#define XSLP_MATH_ITERRPFUNC             35       //  A single point interpolation routine for 2-dimensional data.
#define XSLP_LINLP_MV                    37       //  The marginal value of a requested row.
#define XSLP_LINLP_ACT                   38       //  The activity (left-hand side) of a row.
#define XSLP_LINLP_SLACK                 39       //  The slack activity of a row.
#define XSLP_LINLP_RHS                   40       //  The right-hand side value of a row.
#define XSLP_LINLP_RANGE                 41       //  The upper to lower limit range of a row.
#define XSLP_LINLP_DJ                    42       //  The reduced cost of a requested column.
#define XSLP_LINLP_LO                    43       //  The lower bound of a requested column.
#define XSLP_LINLP_UP                    44       //  The upper bound of a requested column.
#define XSLP_LINLP_MATRIX                45       //  The current matrix entry for a requested row and column index.



/***************************************************************************\
  * Problem status settings                                                *
\**************************************************************************/
#define XSLP_MADECONSTRUCT             0x01       //  The problem is augmented
#define XSLP_MADECASCADE               0x02       //  Cascaded solution
#define XSLP_NODELTAZ                  0x08       //  DeltaZ is off
#define XSLP_GLOBALINIT                0x80       //  Global has been initialized
#define XSLP_GLOBALOPT                0x100       //  Solving an MI-SLP
#define XSLP_ENDGLOBALOPT             0x200       //  Global solve finished
#define XSLP_BOUND_PRESOLVED         0x1000       //  Problem is bound propagated (the legacy notation of presolved)
#define XSLP_FUNCTIONERROR           0x2000       //  There was a function evaluation error
#define XSLP_ERRORCOSTSET           0x20000       //  Error costs have been updated
#define XSLP_NODE0                  0x80000       //  Root node
#define XSLP_ENFORCEDROWS         0x1000000       //  Some rows have been enforced during the solution
#define XSLP_ENFORCESBOUNDS       0x2000000       //  Step bounds were enforced early (before XSLP_SBSTART)
#define XSLP_STEPBOUNDS           0x4000000       //  Step bounds have been activated



/***************************************************************************\
  * XSLP_STATUS settings                                                   *
\**************************************************************************/
#define XSLP_STATUS_CONVERGEDOBJUCC        0x01       //  Converged on objective with no unconverged values in active constraints
#define XSLP_STATUS_CONVERGEDOBJSBX        0x02       //  Converged on objective with some variables converged on extended criteria only 
#define XSLP_STATUS_LPINFEASIBLE           0x04       //  LP solution is infeasible
#define XSLP_STATUS_LPUNFINISHED           0x08       //  LP solution is unfinished (not optimal or infeasible).
#define XSLP_STATUS_MAXSLPITERATIONS       0x10       //  SLP terminated on maximum SLP iterations
#define XSLP_STATUS_INTEGERINFEASIBLE      0x20       //  SLP is integer infeasible.
#define XSLP_STATUS_RESIDUALPENALTIES      0x40       //  SLP converged with residual penalty errors.
#define XSLP_STATUS_CONVERGEDOBJOBJ        0x80       //  Converged on objective
#define XSLP_STATUS_MAXTIME               0x200       //  SLP terminated on max time
#define XSLP_STATUS_USER                  0x400       //  SLP terminated by user
#define XSLP_STATUS_VARSLINKEDINACTIVE    0x800       //  Some variables are linked to active constraints
#define XSLP_STATUS_NOVARSINACTIVE       0x1000       //  No unconverged values in active constraints
#define XSLP_STATUS_OTOL                 0x2000       //  OTOL is satisfied - range of objective change small, active step bounds
#define XSLP_STATUS_VTOL                 0x4000       //  VTOL is satisfied - range of objective change is small
#define XSLP_STATUS_XTOL                 0x8000       //  XTOL is satisfied - range of objective change small, no unconverged in active
#define XSLP_STATUS_WTOL                0x10000       //  WTOL is satisfied - convergence continuation
#define XSLP_STATUS_ERROTOL             0x20000       //  ERRORTOL satisfied - penalties not increased further
#define XSLP_STATUS_EVTOL               0x40000       //  EVTOL satisfied - penalties not increased further
#define XSLP_STATUS_POLISHED            0x80000       //  There were iterations where the solution had to be polished
#define XSLP_STATUS_POLISH_FALIURE     0x100000       //  There were iterations where the solution polishing failed
#define XSLP_STATUS_ENFORCED           0x200000       //  There were iterations where rows were enforced
#define XSLP_STATUS_CONSECUTIVE_INFEAS 0x400000       //  Terminated due to INFEASLIMIT
#define XSLP_STATUS_KEEPBEST           0x800000       //  The solution with the best merit function value has been saved
#define XSLP_STATUS_CLAMPING          0x1000000       //  Clamping of variables converged only in extended criteria has been carried out 
#define XSLP_NLPSTATUS_UNSTARTED              0       //  The optimization has not yet commenced
#define XSLP_NLPSTATUS_LOCALLY_OPTIMAL        1       //  Local solution is found
#define XSLP_NLPSTATUS_OPTIMAL                2       //  Globally optimal solution has been found
#define XSLP_NLPSTATUS_LOCALLY_INFEASIBLE     3       //  The problem is locally infeasible or unbounded
#define XSLP_NLPSTATUS_INFEASIBLE             4       //  The problem is infeasible
#define XSLP_NLPSTATUS_UNBOUNDED              5       //  The problem is unbounded
#define XSLP_NLPSTATUS_UNFINISHED             6       //  The problem has yet been solved to completion
#define XSLP_STOP_NONE                XPRS_STOP_NONE       //  Value of STOPSTATUS: no interruption
#define XSLP_STOP_TIMELIMIT           XPRS_STOP_TIMELIMIT       //  Value of STOPSTATUS: time limit hit
#define XSLP_STOP_CTRLC               XPRS_STOP_CTRLC       //  Value of STOPSTATUS: control C hit
#define XSLP_STOP_NODELIMIT           XPRS_STOP_NODELIMIT       //  Value of STOPSTATUS: node limit hit
#define XSLP_STOP_ITERLIMIT           XPRS_STOP_ITERLIMIT       //  Value of STOPSTATUS: iteration limit hit
#define XSLP_STOP_MIPGAP              XPRS_STOP_MIPGAP       //  Value of STOPSTATUS: MIP gap is sufficiently small
#define XSLP_STOP_SOLLIMIT            XPRS_STOP_SOLLIMIT       //  Value of STOPSTATUS: solution limit hit
#define XSLP_STOP_USER                XPRS_STOP_USER       //  Value of STOPSTATUS: user interrupt



/***************************************************************************\
  * Tolarence                                                              *
\**************************************************************************/
#define XSLP_TOLSET_TC                    0       //  Closure tolerance
#define XSLP_TOLSET_TA                    1       //  Absolute delta tolerance
#define XSLP_TOLSET_RA                    2       //  Relative delta tolerance
#define XSLP_TOLSET_TM                    3       //  Absolute matrix tolerance
#define XSLP_TOLSET_RM                    4       //  Relative matrix tolerance
#define XSLP_TOLSET_TI                    5       //  Absolute impact tolerance
#define XSLP_TOLSET_RI                    6       //  Relative impact tolerance
#define XSLP_TOLSET_TS                    7       //  Absolute slack impact tolerance
#define XSLP_TOLSET_RS                    8       //  Relative slack impact tolerance
#define XSLP_TOLSETBIT_TC             0x001       //  Closure tolerance
#define XSLP_TOLSETBIT_TA             0x002       //  Absolute delta tolerance
#define XSLP_TOLSETBIT_RA             0x004       //  Relative delta tolerance
#define XSLP_TOLSETBIT_TM             0x008       //  Absolute matrix tolerance
#define XSLP_TOLSETBIT_RM             0x010       //  Relative matrix tolerance
#define XSLP_TOLSETBIT_TI             0x020       //  Absolute impact tolerance
#define XSLP_TOLSETBIT_RI             0x040       //  Relative impact tolerance
#define XSLP_TOLSETBIT_TS             0x080       //  Absolute slack impact tolerance
#define XSLP_TOLSETBIT_RS             0x100       //  Relative slack impact tolerance



/***************************************************************************\
  * Convergence specials                                                   *
\**************************************************************************/
#define XSLP_CONVERGEBIT_CTOL                     0x1       //  Closure tolerance                     (1)
#define XSLP_CONVERGEBIT_ATOL                     0x2       //  Delta tolerance                       (2)
#define XSLP_CONVERGEBIT_MTOL                     0x4       //  Matrix tolerance                      (3)
#define XSLP_CONVERGEBIT_ITOL                     0x8       //  Impact tolerance                      (4)
#define XSLP_CONVERGEBIT_STOL                    0x10       //  Slack impact tolerance                (5)
#define XSLP_CONVERGEBIT_USER                    0x20       //  User convergence test                 (6)
#define XSLP_CONVERGEBIT_VTOL                    0x40       //  Objective range check                 (7)
#define XSLP_CONVERGEBIT_XTOL                    0x80       //  Objective range + constraint activity (8)
#define XSLP_CONVERGEBIT_OTOL                   0x100       //  Objective range + active step bounds  (9)
#define XSLP_CONVERGEBIT_WTOL                   0x200       //  Convergence continuation             (10)
#define XSLP_CONVERGEBIT_EXTENDEDSCALING        0x400       //  Take scaling of individual variables / rows into account



/***************************************************************************\
  * Variable status                                                        *
\**************************************************************************/
#define XSLP_HASNOCOEFS                 0x01       //  The variable has no coefficients
#define XSLP_HASDELTA                   0x02       //  The column has a delta vector
#define XSLP_HASIV                      0x04       //  The column has an initial value
#define XSLP_HASCALCIV                  0x08       //  The column has an initial calculated value
#define XSLP_ISDELTA                  0x0100       //  The variable is a delta variable
#define XSLP_ISPLUSPENALTYDELTA       0x0200       //  The variable is a positive penalty delta
#define XSLP_ISMINUSPENALTYDELTA      0x0400       //  The variable is a negative penalty delta
#define XSLP_ISPENALTYDELTA           0x0600       //  The variable is a penalty delta variable
#define XSLP_ISPLUSERRORVECTOR        0x0800       //  The variable is a positive error vector
#define XSLP_ISMINUSERRORVECTOR       0x1000       //  The variable is a negative error vector
#define XSLP_ISERRORVECTOR            0x1800       //  The variable is an error vector
#define XSLP_ISMISCVECTOR             0x2000       //  The variable is a miscellaneous vector
#define XSLP_ISEQUALSCOLUMN           0x4000       //  The variable is the equals column
#define XSLP_HASCONVERGED            0x10000       //  The variable has converged to a solution
#define XSLP_ACTIVESTEPBOUND         0x20000       //  The variable has active stepbounds
#define XSLP_ACTIVESBROW             0x40000       //  The variable has an active step bound row
#define XSLP_HASDOMAINIV             0x80000       //  The variable has an initial value propagated from function domains
#define XSLP_NOTSLPVAR              0x100000       //  The variable is not an SLP variable
#define XSLP_ISSTRUCTURALCOLUMN     0x200000       //  The variable is one of the structural columns
#define XSLP_ISINCOEFS              0x400000       //  The variable appears in non-linear coefficients. Valid after construct.
#define XSLP_ISINGLOBAL             0x800000       //  The variable is involved in the global items
#define XSLP_HASZEROBOUND          0x1000000       //  The variable has a zero lower bound
#define XSLP_FIXEDVAR              0x2000000       //  The variable is a fixed value variable.
#define XSLP_BOUNDSSET             0x4000000       //  The variable is part of a bounds set
#define XSLP_USEFULDELTA           0x8000000       //  Check/mark as a useful delta column
#define XSLP_NOUSEFULDELTA         0x8000000       //  Check/mark as a non-useful delta column
#define XSLP_ISINTEGER            0x10000000       //  The variable is an integer variable
#define XSLP_CASCADECONTRACTION   0x20000000       //



/***************************************************************************\
  * Row status                                                              *
\**************************************************************************/
#define XSLP_ISUPDATEROW               0x02       //  The row is an update row
#define XSLP_ISPENALTYROW              0x04       //  The row is a penalty row
#define XSLP_ISMISCROW                 0x40       //  The row is a miscellaneous row
#define XSLP_ISSBROW                   0x80       //  The row is a step bound row
#define XSLP_HASPLUSERROR             0x100       //  The row has a positive error vector
#define XSLP_HASMINUSERROR            0x200       //  The row has a negative error vector
#define XSLP_HASERROR                 0x300       //  The row has an error vector
#define XSLP_ISDETERMININGROW         0x400       //  The row is a determining row
#define XSLP_NOERRORVECTORS           0x800       //  The row has no error vectors
#define XSLP_HASNONZEROCOEF          0x1000       //  The row has non-zero coefficients
#define XSLP_REDUNDANTROW            0x2000       //  The row is redundant
#define XSLP_UNCONVERGEDROW          0x4000       //  The row is unconverged
#define XSLP_ACTIVEPENALTY           0x8000       //  The row has active penalty vectors
#define XSLP_HASSLPELEMENT          0x10000       //  The row has SLP elements
#define XSLP_TRANSFERROW            0x40000       //  The row is an objective transfer row
#define XSLP_DEFINEVARIABLEROW     0x100000       //  The row has been eliminated on, i.e. a free row representing the original row 



/***************************************************************************\
  * Augmentation settings                                                  *
\**************************************************************************/
#define XSLP_MINIMUMAUGMENTATION       0x01       //  (   1 ) Perform minimal augmentation only
#define XSLP_EVENHANDEDAUGMENTATION    0x02       //  (   2 ) Perform even-handed augmentation
#define XSLP_EQUALITYERRORVECTORS      0x04       //  (   4 ) Penalty error vectors on all non-linear equality constraints
#define XSLP_ALLERRORVECTORS           0x08       //  (   8 ) Penalty error vectors on all non-linear inequality constraints
#define XSLP_PENALTYDELTAVECTORS       0x10       //  (  16 ) Penalty vectors to exceed step bounds
#define XSLP_AMEANWEIGHT               0x20       //  (  32 ) Use arithmetic means to estimate penalty weights
#define XSLP_SBFROMVALUES              0x40       //  (  64 ) Estimate step bounds from values of row coefficients
#define XSLP_SBFROMABSVALUES           0x80       //  ( 128 ) Estimate step bounds from absolute values of row coefficients
#define XSLP_STEPBOUNDROWS            0x100       //  ( 256 ) Row-based step bounds
#define XSLP_ALLROWERRORVECTORS       0x200       //  ( 512 ) Penalty error vectors on all constraints
#define XSLP_NOUPDATEIFONLYIV         0x400       //  (1024 ) Having an IV itself will not cause the augmentation to include the corresponding delta variable 
#define XSLP_NOFORMULADOMAINIV        0x800       //  (2048 ) Skip formula domain propagating into initial values for default IV values



/***************************************************************************\
  * Algorithm settings                                                      *
\**************************************************************************/
#define XSLP_NOSTEPBOUNDS              0x01       //  (    1 ) Do not apply step bounds
#define XSLP_STEPBOUNDSASREQUIRED      0x02       //  (    2 ) Apply step bounds to SLP delta vectors only when required
#define XSLP_ESTIMATESTEPBOUNDS        0x04       //  (    4 ) Estimate step bounds from early SLP iterations
#define XSLP_DYNAMICDAMPING            0x08       //  (    8 ) Use dynamic damping
#define XSLP_HOLDVALUES                0x10       //  (   16 ) Do not update values which are converged within strict tolerance
#define XSLP_RETAINPREVIOUSVALUE       0x20       //  (   32 ) Retain previous value when cascading if determining row is zero
#define XSLP_RESETDELTAZ               0x40       //  (   64 ) Reset XSLP_DELTA_Z to zero when converged and continue SLP
#define XSLP_QUICKCONVERGENCECHECK     0x80       //  (  128 ) Quick convergence check
#define XSLP_ESCALATEPENALTIES        0x100       //  (  256 ) Escalate penalties
#define XSLP_SWITCHTOPRIMAL           0x200       //  (  512 ) Use the primal simplex algorithm when all error vectors become inactive 
#define XSLP_NONZEROBOUND             0x400       //  ( 1024 ) Not currently used
#define XSLP_MAXCOSTOPTION            0x800       //  ( 2048 ) Continue optimizing after penalty cost reaches maximum
#define XSLP_RESIDUALERRORS          0x1000       //  ( 4096 ) Accept a solution which has converged even if there are still significant active penalty error vectors 
#define XSLP_NOLPPOLISHING           0x2000       //  ( 8192 ) Skip the clean up call if the LP postsolve returns a slightly feasible, but claimed optimal solution 
#define XSLP_CASCADEDBOUNDS          0x4000       //  (16384 ) Step bounds are updated to accommodate cascaded values (otherwise cascaded values are pushed to respect step bounds 
#define XSLP_CLAMPEXTENDEDACTIVESB   0x8000       //  (32768 ) Clamp variables converged on extended criteria only with active step bounds
#define XSLP_CLAMPEXTENDEDALL       0x10000       //  (65536 ) Clamp all variables converged on extended criteria only



/***************************************************************************\
  * MIP-Algorithm settings                                                  *
\**************************************************************************/
#define XSLP_MIPINITIALSLP             0x01       //  (    1 ) Solve the initial SLP to convergence
#define XSLP_MIPFINALSLP               0x02       //  (    2 ) Re-solve final SLP to convergence
#define XSLP_MIPINITIALRELAXSLP        0x04       //  (    4 ) Relax step-bounds according to XSLP_MIPRELAXSTEPBOUNDS after initial node 
#define XSLP_MIPINITIALFIXSLP          0x08       //  (    8 ) Fix step-bounds according to XLSP_MIPFIXSTEPBOUNDS after initial node 
#define XSLP_MIPNODERELAXSLP           0x10       //  (   16 ) Relax step-bounds according to XSLP_MIPRELAXSTEPBOUNDS at each node
#define XSLP_MIPNODEFIXSLP             0x20       //  (   32 ) Fix step-bounds according to XSLP_MIPFIXSTEPBOUNDS at each node
#define XSLP_MIPNODELIMITSLP           0x40       //  (   64 ) Limit iterations at each node to XSLP_MIPITERLIMIT
#define XSLP_MIPFINALRELAXSLP          0x80       //  (  128 ) Relax step-bounds according to XSLP_MIPRELAXSTEPBOUNDS after MIP solution is found 
#define XSLP_MIPFINALFIXSLP           0x100       //  (  256 ) Fix step-bounds according to XSLP_MIPFIXSTEPBOUNDS after MIP solution is found 
#define XSLP_MIPWITHINSLP             0x200       //  (  512 ) Use MIP at each SLP iteration instead of SLP at each node
#define XSLP_SLPTHENMIP               0x400       //  ( 1024 ) Use MIP on converged SLP solution and then SLP on the resulting MIP solution 



/***************************************************************************\
  * Formula and coefficient                                                *
\**************************************************************************/



/***************************************************************************\
  * Function types                                                         *
\**************************************************************************/
#define XSLP_USERFUNCTION1                1       //  A user function in a user dll taking a single argument
#define XSLP_MOSELFUNCTION1               2       //  A Mosel user function taking a single argument
#define XSLP_USERFUNCTIONA                3       //  A user function in a user dll taking multiple arguments
#define XSLP_EXCELFUNCTIONA               4       //  A user function in Excel taking multiple arguments
#define XSLP_EXCELMACROA                  5       //  A user function from an Excel macro taking multiple arguments
#define XSLP_EXCELFUNCTION1               6       //  A user function in Excel taking a single argument
#define XSLP_EXCELMACRO1                  7       //  A user function from an Excel macro taking a single argument
#define XSLP_VBFUNCTION1                  8       //  A Visual Basic user function taking a single argument
#define XSLP_VBFUNCTIONA                  9       //  A Visual Basic user function taking multiple arguments
#define XSLP_COMFUNCTION1                10       //  A COM user function taking a single argument
#define XSLP_COMFUNCTIONA                11       //  A COM user function taking multiple arguments
#define XSLP_NETFUNCTION1                47       //  A .Net user function taking a single argument
#define XSLP_NETFUNCTIONA                48       //  A .Net user function taking multiple arguments
#define XSLP_EXCELINTERFACE               0       //  Used to set the interface for a UF as being an Excel interface
#define XSLP_COMINTERFACE                 1       //  Used to set the interface for a UF as being a COM interface (often within the code Excel and COM are grouped together) 
#define XSLP_VBINTERFACE                  2       //  Used to set the interface for a UF as being a Visual Basic interface
#define XSLP_INTERNAL_EXETYPE             0       //  Define the UF executable type as being as internal, function pointer will be updated by user code
#define XSLP_DLL_EXETYPE                  1       //  Define the UF executable type as being a dll
#define XSLP_XLS_EXETYPE                  2       //  Define the UF executable type as being an Excel function
#define XSLP_XLF_EXETYPE                  3       //  Define the UF executable type as being an Excel macro
#define XSLP_MOSEL_EXETYPE                5       //  Define the UF executable type as being a compiled Mosel function
#define XSLP_VB_EXETYPE                   6       //  Define the UF executable type as Visual Basic
#define XSLP_COM_EXETYPE                  7       //  Define the UF executable type as being COM



/***************************************************************************\
  * Calculation status                                                     *
\**************************************************************************/



/***************************************************************************\
  * Info tokens                                                            *
\**************************************************************************/
#define XSLPINFO_CALLERFLAG               1       //  Position in 'arginfo' for: caller flag
#define XSLPINFO_NINPUT                   2       //  Position in 'arginfo' for: number of inout arguments to the function
#define XSLPINFO_NOUTPUT                  3       //  Position in 'arginfo' for: number of expected output arguments
#define XSLPINFO_NDELTA                   4       //  Position in 'arginfo' for: number of expected deltas returned
#define XSLPINFO_NINSTRING                5       //  Position in 'arginfo' for: number of strings for input
#define XSLPINFO_NOUTSTRING               6       //  Position in 'arginfo' for: number of string for output
#define XSLPINFO_FUNCNUM                  7       //  Position in 'arginfo' for: functions number
#define XSLPINFO_INSTANCE                 8       //  Position in 'arginfo' for: function instance number



/***************************************************************************\
  * Misc settigns                                                          *
\**************************************************************************/
#define XSLP_RECALC                           0x08       //  Setting of FUNCEVAL: re-evaluate user functions at each iteration
#define XSLP_TOLCALC                          0x10       //  Setting of FUNCEVAL: re evaluate UFs if independent variables change outside tolerance 
#define XSLP_ALLCALCS                         0x20       //  Setting of FUNCEVAL: overwrite user function specific re-evaluation settings
#define XSLP_2DERIVATIVE                      0x40       //  Setting of FUNCEVAL: use tangential derivatives
#define XSLP_1DERIVATIVE                      0x80       //  Setting of FUNCEVAL: use forward derivatives
#define XSLP_ALLDERIVATIVES                  0x100       //  Setting of FUNCEVAL: overwrite user function specific derivative settings
#define XSLP_CDECL                           0x100       //  Setting of UFEXETYPE: CDECL call (Windows only)
#define XSLP_STDCALL                         0x000       //  Setting of UFEXETYPE: standard call
#define XSLP_INSTANCEFUNCTION                0x200       //  Setting of UFEXETYPE: instantiate function
#define XSLP_NETCALL                         0x400       //
#define XSLP_DEDUCECONSTDERIVS               0x800       //  Setting of UFEXETYPE: assume derivatives which do not change outside tolerance are constant 
#define XSLP_SOMECONSTDERIVS                0x1000       //  Setting of UFEXETYPE: interrogate function for constant derivatives
#define XSLP_MULTIPURPOSE                   0x2000       //  Setting of UFEXETYPE: multi-valued function with dependency matrix
#define XSLP_MULTIVALUED                 0x1000000       //  Setting of UFEXETYPE: function is multivalued
#define XSLP_NODERIVATIVES              0x10000000       //  Setting of UFEXETYPE: function is non differentiable
#define XSLP_PRESOLVEOPS_GENERAL              0x01       //  PRESOLVEOPS: general (simple reductions) presolve
#define XSLP_PRESOLVEFIXZERO                  0x02       //  PRESOLVEOPS: explicitly fix columns identified as fixed to zero
#define XSLP_PRESOLVEFIXALL                   0x04       //  PRESOLVEOPS: explicitly fix all columns identified as fixed
#define XSLP_PRESOLVESETBOUNDS                0x08       //  PRESOLVEOPS: SLP bound tightening
#define XSLP_PRESOLVEINTBOUNDS                0x10       //  PRESOLVEOPS: MISLP bound tightening
#define XSLP_PRESOLVEDOMAIN                   0x20       //  PRESOLVEOPS: Perform reduction based on function domains (e.g. log, sqrt)
#define XSLP_NOPRESOLVECOEFFICIENTS          0x100       //  PRESOLVEOPS: do not presolve coefficients
#define XSLP_NOPRESOLVEDELTAS                0x200       //  PRESOLVEOPS: do not remove delta variables
#define XSLP_PRESOLVEOPS_NO_DUAL_SIDE        0x400       //  PRESOLVEOPS: avoid reductions that can't be dual postsolved
#define XSLP_PRESOLVEOPS_ELIMINATIONS        0x800       //  PRESOLVEOPS: apply nlp eliminations, i.e. look for defined variables
#define XSLP_PRESOLVELEVEL_LOCALIZED             1       //  Individual rows only presolve, no nonlinear transformations
#define XSLP_PRESOLVELEVEL_BASIC                 2       //  Individual rows and bounds only presolve, no nonlinear transformations
#define XSLP_PRESOLVELEVEL_LINEAR                3       //  Presolve allowing changing problem dimension, no nonlinear transformations
#define XSLP_PRESOLVELEVEL_FULL                  4       //  Full presolve
#define XSLP_FUNCINFOSIZE                       12       //  Number of integers the FunctionInfo structure consists of
#define XSLP_GETUFNAME                          31       //  XSLPuserfuncinfo: Retrieve the external name of the user function
#define XSLP_GETUFPARAM1                        32       //  XSLPuserfuncinfo: Retrieve the first string parameter
#define XSLP_GETUFPARAM2                        33       //  XSLPuserfuncinfo: Retrieve the second string parameter
#define XSLP_GETUFPARAM3                        34       //  XSLPuserfuncinfo: Retrieve the third string parameter
#define XSLP_GETUFARGTYPE                       35       //  XSLPuserfuncinfo: Retrieve the argument types
#define XSLP_GETUFEXETYPE                       36       //  XSLPuserfuncinfo: Retrieve the linkage type
#define XSLP_SETUFNAME                          41       //  XSLPuserfuncinfo: Set the external name of the user function
#define XSLP_SETUFPARAM1                        42       //  XSLPuserfuncinfo: Set the first string parameter
#define XSLP_SETUFPARAM2                        43       //  XSLPuserfuncinfo: Set the second string parameter
#define XSLP_SETUFPARAM3                        44       //  XSLPuserfuncinfo: Set the third string parameter
#define XSLP_SETUFARGTYPE                       45       //  XSLPuserfuncinfo: Set the argument types
#define XSLP_SETUFEXETYPE                       46       //  XSLPuserfuncinfo: Set the linkage type
#define XSLP_GETROWNUMPENALTYERRORS            201       //  Setting of XSLProwinfo: get the number of times the penalty error vector has been active 
#define XSLP_GETROWMAXPENALTYERROR             202       //  Setting of XSLProwinfo: get the maximum size of the penalty error vector activity 
#define XSLP_GETROWTOTALPENALTYERROR           203       //  Setting of XSLProwinfo: get the total of the penalty error vector activities
#define XSLP_GETROWAVERAGEPENALTYERROR         204       //  Setting of XSLProwinfo: get the average size of the penalty error vector activity 
#define XSLP_GETROWCURRENTPENALTYERROR         205       //  Setting of XSLProwinfo: get the size of the penalty error vector activity in the current iteration
#define XSLP_GETROWCURRENTPENALTYFACTOR        206       //  Setting of XSLProwinfo: get the size of the penalty error factor for the current iteration 
#define XSLP_SETROWPENALTYFACTOR               207       //  Setting of XSLProwinfo: set the size of the penalty error factor for the next iteration 
#define XSLP_GETROWPENALTYCOLUMN1              208       //  Setting of XSLProwinfo: get the index of the penalty column for the row (+)
#define XSLP_GETROWPENALTYCOLUMN2              209       //  Setting of XSLProwinfo: get the index of the second penalty column for an equality row (-) 
#define XSLP_CASCADE_ALL                      0x01       //  Setting of CASCADE: cascade all variables with determining rows
#define XSLP_CASCADE_COEF_VAR                 0x02       //  Setting of CASCADE: cascade SLP variables which appear in coefficients, and would change by more than XPRS_FEASTOL 
#define XSLP_CASCADE_ALL_COEF_VAR             0x04       //  Setting of CASCADE: cascade all SLP variables which appear in coefficients
#define XSLP_CASCADE_STRUCT_VAR               0x08       //  Setting of CASCADE: cascade SLP variables which are structural and which would change by more than XPRS_FEASTOL 
#define XSLP_CASCADE_ALL_STRUCT_VAR           0x10       //  Setting of CASCADE: cascade all SLP variables which are structural
#define XSLP_CASCADE_SECONDARY_GROUPS         0x20       //  Setting of CASCADE: create secondary grouping for instantiated UF rows
#define XSLP_CASCADE_DRCOL_PREVOUSVALUE       0x40       //  Setting of CASCADE: DRCOLTOL fixes at previous value, not current
#define XSLP_CASCADE_DRCOL_PVRANGE            0x80       //  Setting of CASCADE: DRCOLTOL fixes defines a range around the previous value
#define XSLP_SOLVER_AUTO                        -1       //  Setting of SOLVER: autoselect / not selected yet
#define XSLP_SOLVER_XSLP                         0       //  Setting of SOLVER: use XSLP
#define XSLP_SOLVER_KNITRO                       1       //  Setting of SOLVER: use KNITRO
#define XSLP_SOLVER_OPTIMIZER                    2       //  Setting of SOLVER: use the optimizer
#define XSLP_MSSET_INITIALVALUES                 0       //  Option for the multi-start: load different initial points
#define XSLP_MSSET_SOLVERS                       1       //  Option for the multi-start: load all solvers to try
#define XSLP_MSSET_SLP_BASIC                     2       //  Option for the multi-start: try the fundamental SLP controls
#define XSLP_MSSET_SLP_EXTENDED                  3       //  Option for the multi-start: try the an extensive set of SLP controls
#define XSLP_MSSET_KNITRO_BASIC                  4       //  Option for the multi-start: try the fundamental Knitro controls
#define XSLP_MSSET_KNITRO_EXTENDED               5       //  Option for the multi-start: try the an extensive set of Knitro controls
#define XSLP_MSSET_INITIALFILTERED               6       //  Option for the multi-start: load different initial points filtered my merit
#define XSLP_MSSET_INITIALDYNAMIC                7       //  Option for the multi-start: load different initial points based on best before 
#define XSLP_CLASS_GENERAL                       0       //  Problem classification: general
#define XSLP_CLASS_LP                            1       //  Problem classification: linear
#define XSLP_CLASS_CONVEX_Q                      2       //  Problem classification: convex quadratic, including convex QCQP
#define XSLP_CLASS_NON_CONVEX_Q                  3       //  Problem classification: non-convex quadratic
#define XSLP_CLASS_SOCP                          4       //  Problem classification: second order cone in the standard form
#define XSLP_CLASS_SIMPLE_BLENDING               5       //  Problem classification: simple blending constraints
#define XSLP_CLASS_GENERAL_PROCESS_STRUCTURE     6       //  Problem classification: general with process structural process information
#define XSLP_KKT_CALCULATION_RECALCULATE_RDJ     0       //  KKT calculations: recalculate the rdj using the current duals
#define XSLP_KKT_CALCULATION_MINIMZE_KKT_ERROR   1       //  KKT calculations: fix solution and minimize dual side violations
#define XSLP_KKT_CALCULATION_MEASURE_BOTH        2       //  KKT calculations: both calculate and minimize error
#define XSLP_KKT_CALCULATION_ACTIVITY_BASED      0       //  KKT calculations: use constraint activity to define active constraints
#define XSLP_KKT_CALCULATION_RESPECT_BASIS       1       //  KKT calculations: use the basis status if any to define active constraints
#define XSLP_KKT_CALCULATION_ACTIVITY_BOTH       2       //  KKT calculations: use the basis status & activity to define active constraints
#define XSLP_KKT_JUST_CALCULATE                  0       //  KKT calculations: only do the calculations but do not modify the dual solution
#define XSLP_KKT_UPDATE_MULTIPLIERS              1       //  KKT calculations: update the dual side solution
#define XSLP_PARMTYP_INT                       001       //  Integer parameter
#define XSLP_PARMTYP_DBL                       002       //  Double parameter
#define XSLP_PARMTYP_STR                       004       //  String parameter
#define XSLP_PARMTYP_READ                      010       //  Parameter is readable
#define XSLP_PARMTYP_WRITE                     020       //  Parameter is writeable
#define XSLP_UFARGTYPE_OMITTED                   0       //  Argument is omitted
#define XSLP_UFARGTYPE_NULL                      1       //  Argument is NULL
#define XSLP_UFARGTYPE_INTEGER                   2       //  Argument type is integer
#define XSLP_UFARGTYPE_DOUBLE                    3       //  Argument type is double
#define XSLP_UFARGTYPE_VARIANT                   4       //  Argument type is variant
#define XSLP_UFARGTYPE_CHAR                      6       //  Argument type is string
#define XSLP_ZEROCRITERION_NBSLPVAR              1       //  Setting of ZEROCRITERION: remove placeholders in nonbasic SLP variables
#define XSLP_ZEROCRITERION_NBDELTA               2       //  Setting of ZEROCRITERION: remove placeholders in nonbasic delta variables
#define XSLP_ZEROCRITERION_SLPVARNBUPDATEROW     4       //  Setting of ZEROCRITERION: remove placeholders in a basic SLP variable if its update row is nonbasic 
#define XSLP_ZEROCRITERION_DELTANBUPSATEROW      8       //  Setting of ZEROCRITERION: remove placeholders in a basic delta variable if its update row is nonbasic and the corresponding SLP variable is nonbasic 
#define XSLP_ZEROCRITERION_DELTANBDRROW         16       //  Setting of ZEROCRITERION: remove placeholders in a basic delta variable if the determining row for the corresponding SLP variable is nonbasic 
#define XSLP_ZEROCRITERION_PRINT                32       //  Setting of ZEROCRITERION: print information about zero placeholders
#define XSLP_TRACEMASK_GENERALFIT              0x1       //  Setting of TRACEMASKOPS: the variable name is used a mask, not an exact fit
#define XSLP_TRACEMASK_ROWS                    0x2       //  Setting of TRACEMASKOPS: use mask to trace rows
#define XSLP_TRACEMASK_COLS                    0x4       //  Setting of TRACEMASKOPS: use mask to trace columns
#define XSLP_TRACEMASK_CASCADE                 0x8       //  Setting of TRACEMASKOPS: use mask to trace cascaded columns
#define XSLP_TRACEMASK_TYPE                   0x10       //  Setting of TRACEMASKOPS: show row - column category
#define XSLP_TRACEMASK_SLACK                  0x20       //  Setting of TRACEMASKOPS: trace slack values
#define XSLP_TRACEMASK_DUAL                   0x40       //  Setting of TRACEMASKOPS: trace dual values
#define XSLP_TRACEMASK_WEIGHT                 0x80       //  Setting of TRACEMASKOPS: trace row penalty multipliers
#define XSLP_TRACEMASK_SOLUTION              0x100       //  Setting of TRACEMASKOPS: trace variable values
#define XSLP_TRACEMASK_REDUCEDCOST           0x200       //  Setting of TRACEMASKOPS: trace reduced costs
#define XSLP_TRACEMASK_SLPVALUE              0x400       //  Setting of TRACEMASKOPS: trace slp value (value used in linearization-cascaded)
#define XSLP_TRACEMASK_STEPBOUND             0x800       //  Setting of TRACEMASKOPS: trace step bounds
#define XSLP_TRACEMASK_CONVERGE             0x1000       //  Setting of TRACEMASKOPS: trace convergence status
#define XSLP_TRACEMASK_LINESEARCH           0x2000       //  Setting of TRACEMASKOPS: use mask to trace line search
#define XSLP_FILTER_KEEPBEST                   0x1       //  Setting of FILTER: retrain solution best according to the merit function
#define XSLP_FILTER_CASCADE                    0x2       //  Setting of FILTER: check cascaded solutions for improvements in merit function 
#define XSLP_FILTER_ZEROLINESEARCH             0x4       //  Setting of FILTER: force minimum step sizes in line search
#define XSLP_FILTER_ZEROLINESEARCHTR           0x8       //  Setting of FILTER: accept the trust region step is the line search fails
#define XSLP_ANALYZE_RECORDLINEARIZATION       0x1       //  Setting of ANALYZE: add solutions of the linearizations to the solution pool
#define XSLP_ANALYZE_RECORDCASCADE             0x2       //  Setting of ANALYZE: add cascaded solutions to the solution pool
#define XSLP_ANALYZE_RECORDLINESEARCH          0x4       //  Setting of ANALYZE: add line search solutions to the solution pool
#define XSLP_ANALYZE_EXTENDEDFINALSUMMARY      0x8       //  Setting of ANALYZE: included an extended iteration summary
#define XSLP_ANALYZE_INFEASIBLE_ITERATION     0x10       //  Setting of ANALYZE: run infeasibility analysis on infeasible iterations
#define XSLP_ANALYZE_AUTOSAVEPOOL             0x20       //  Setting of ANALYZE: automatically save the solution pool to file
#define XSLP_ANALYZE_SAVELINEARIZATIONS       0x40       //  Setting of ANALYZE: automatically save the linearizations
#define XSLP_ANALYZE_SAVEITERBASIS            0x80       //  Setting of ANALYZE: automatically save the initial basis of the linearizations 
#define XSLP_ANALYZE_SAVEFILE                0x100       //  Setting of ANALYZE: create a save file at the beginning of an iteration
#define XSLP_REFORMULATE_SLP2QP                0x1       //  Setting of REFORMULATE: solve convex QP objectives using the XPRS library
#define XSLP_REFORMULATE_QP2SLP                0x2       //  Setting of REFORMULATE: convert non-convex QP objectives to SLP constructs
#define XSLP_REFORMULATE_SLP2QCQP              0x4       //  Setting of REFORMULATE: solve convex QCQP constraints using the XPRS library
#define XSLP_REFORMULATE_QCQP2SLP              0x8       //  Setting of REFORMULATE: convert non-convex QCQP constraints to SLP constructs
#define XSLP_REFORMULATE_SOCP2SLP             0x10       //  Setting of REFORMULATE: convert SOCP constraints to XPRS - SLP hybrid
#define XSLP_SIGN_ARGLIST_vv                   044       //  Function signature: taking two subsequent variants
#define XSLP_SIGN_ARGLIST_ArAi                 023       //  Function signature: taking array of double, array of int
#define XSLP_SIGN_RET_r                          1       //  Function signature: returning a single real
#define XSLP_SIGN_RET_Ai_r                       2       //  Function signature: returning an array of reals indexed by integers



/***************************************************************************\
  * AddNames values                                                        *
\**************************************************************************/
#define XSLP_CVNAMES                      3       //  string names
#define XSLP_XVNAMES                      4       //  extended variable names
#define XSLP_USERFUNCNAMES                5       //  user function names
#define XSLP_INTERNALFUNCNAMES            6       //  internal function names
#define XSLP_USERFUNCNAMESNOCASE          7       //  case insensitive lookup of user functions
#define XSLP_INTERNALFUNCNAMESNOCASE      8       //  case insensitive lookup of internal functions



/****************************************************************************\
 * function declarations                                                    *
\****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

double XSLP_CC XSLPcalluserfunc( XSLPprob Prob, int FuncNumber, void * Arg1, void * Arg2, void * Arg3, void * Arg4, void * Arg5, void * Arg6_); 
int XSLP_CC XSLPaddcoefs( XSLPprob Prob, int nSLPCoef, const int * RowIndex, const int * ColIndex, const double * Factor, const int * FormulaStart, int Parsed, const int * Type, const double * Value); 
int XSLP_CC XSLPaddcvars( XSLPprob Prob, int nSLPCVar, const char * cValue); 
int XSLP_CC XSLPadddcs( XSLPprob Prob, int nSLPDC, const int * RowIndex, const int * Delay, const int * DCStart, int Parsed, const int * Type, const double * Value); 
int XSLP_CC XSLPadddfs( XSLPprob Prob, int nDF, const int * ColIndex, const int * RowIndex, const double * DFValue); 
int XSLP_CC XSLPaddivfs( XSLPprob Prob, int nIVF, const int * ColIndex, const int * IVFStart, int Parsed, const int * Type, const double * Value); 
int XSLP_CC XSLPaddnames( XSLPprob Prob, int type, const char * cNames, int First, int Last); 
int XSLP_CC XSLPaddtolsets( XSLPprob Prob, int nSLPTol, const double * SLPTol); 
int XSLP_CC XSLPadduserfuncs( XSLPprob Prob, int nSLPUserFunc, const int * Type, const double * Value); 
int XSLP_CC XSLPaddvars( XSLPprob Prob, int nSLPVar, const int * ColIndex, const int * VarType, const int * DetRow, const int * SeqNum, const int * TolIndex, const double * InitValue, const double * StepBound); 
int XSLP_CC XSLPaddxvs( XSLPprob Prob, int nSLPXV, const int * XVStart, int Parsed, const int * Type, const double * Value); 
int XSLP_CC XSLPcalcslacks( XSLPprob Prob, const double * dsol, double * slacks); 
int XSLP_CC XSLPcascade( XSLPprob Prob); 
int XSLP_CC XSLPcascadeorder( XSLPprob Prob); 
int XSLP_CC XSLPchgccoef( XSLPprob Prob, int RowIndex, int ColIndex, const double * Factor, const char * Formula); 
int XSLP_CC XSLPchgcoef( XSLPprob Prob, int RowIndex, int ColIndex, const double * Factor, int Parsed, const int * Type, const double * Value); 
int XSLP_CC XSLPchgcvar( XSLPprob Prob, int nSLPCVar, const char * cValue); 
int XSLP_CC XSLPchgdc( XSLPprob Prob, int RowIndex, const char * RowType, const int * Delay, const int * IterCount, int Parsed, const int * Type, const double * Value); 
int XSLP_CC XSLPchgdf( XSLPprob Prob, int ColIndex, int RowIndex, const double * DFValue); 
int XSLP_CC XSLPchgfuncobject( int * ArgInfo, int ObjType, void ** Object); 
int XSLP_CC XSLPchgivf( XSLPprob Prob, int ColIndex, int Parsed, const int * Type, const double * Value); 
int XSLP_CC XSLPchgrow( XSLPprob Prob, int RowIndex, int * Status); 
int XSLP_CC XSLPchgrowstatus( XSLPprob Prob, int RowIndex, int * Status); 
int XSLP_CC XSLPchgrowwt( XSLPprob Prob, int RowIndex, const double * Value); 
int XSLP_CC XSLPchgtolset( XSLPprob Prob, int nSLPTol, const int * Status, const double * Tols); 
int XSLP_CC XSLPchguserfunc( XSLPprob Prob, int nSLPUserFunc, const char * xName, const int * ArgType, const int * ExeType, const char * Param1, const char * Param2, const char * Param3); 
int XSLP_CC XSLPchguserfuncaddress( XSLPprob Prob, int FuncNumber, void ** Address); 
int XSLP_CC XSLPchguserfuncobject( XSLPprob Prob, int Entity, void ** Object); 
int XSLP_CC XSLPchgvar( XSLPprob Prob, int ColIndex, const int * DetRow, const double * InitStepBound, const double * StepBound, const double * Penalty, const double * Damp, const double * InitValue, const double * Value, const int * TolSet, const int * History, const int * Converged, const int * VarType); 
int XSLP_CC XSLPchgxv( XSLPprob Prob, int nXSLPXV, const int * nXVitems); 
int XSLP_CC XSLPchgxvitem( XSLPprob Prob, int nSLPXV, int nXVitem, int Parsed, const int * VarType, const int * VarIndex, const int * IntIndex, const double * Delta1, const double * Delta2, const int * Status, const int * Type, const double * Value); 
int XSLP_CC XSLPconstruct( XSLPprob Prob); 
int XSLP_CC XSLPcopycallbacks( XSLPprob Prob, const XSLPprob OldProb); 
int XSLP_CC XSLPcopycontrols( XSLPprob NewProb, const XSLPprob OldProb); 
int XSLP_CC XSLPcopyprob( XSLPprob NewProb, XSLPprob OldProb, const char * probname); 
int XSLP_CC XSLPcreateprob( XSLPprob * Prob, XPRSprob * XprsProb); 
int XSLP_CC XSLPdecompose( XSLPprob Prob, int Count, const int * Index); 
int XSLP_CC XSLPdelcoefs( XSLPprob Prob, int nSLPCoef, int * RowIndex, int * ColIndex); 
int XSLP_CC XSLPdelcvars( XSLPprob Prob, int nCV, int * CVIndex); 
int XSLP_CC XSLPdeldcs( XSLPprob Prob, int nRow, int * RowIndex); 
int XSLP_CC XSLPdelivfs( XSLPprob Prob, int nCol, int * ColIndex); 
int XSLP_CC XSLPdeltolsets( XSLPprob Prob, int nTolSet, int * TolSetIndex); 
int XSLP_CC XSLPdeluserfuncs( XSLPprob Prob, int nUserFunction, int * UserFunctionIndex); 
int XSLP_CC XSLPdelvars( XSLPprob Prob, int nCol, int * ColIndex); 
int XSLP_CC XSLPdelxvs( XSLPprob Prob, int nXV, int * XVIndex); 
int XSLP_CC XSLPdestroyprob( XSLPprob Prob); 
int XSLP_CC XSLPdumpattributes( XSLPprob Prob); 
int XSLP_CC XSLPdumpcontrols( XSLPprob Prob); 
int XSLP_CC XSLPevaluatecoef( XSLPprob Prob, int RowIndex, int ColIndex, double * dValue); 
int XSLP_CC XSLPevaluateformula( XSLPprob Prob, int Parsed, const int * Type, const double * Value, double * dValue); 
int XSLP_CC XSLPfilesol( XSLPprob Prob, char * sFileName); 
int XSLP_CC XSLPwriteslxsol( XSLPprob Prob, char * sFileName, char * sFlags); 
int XSLP_CC XSLPwriteamplsol( XSLPprob Prob, char * sFileName, char * sFlags); 
int XSLP_CC XSLPreadamplsol( XSLPprob Prob, char * sFileName, char * sFlags); 
int XSLP_CC XSLPfixpenalties( XSLPprob Prob, int * RetrunStatus); 
int XSLP_CC XSLPformatvalue( double dValue, char * Buffer); 
int XSLP_CC XSLPfree(); 
int XSLP_CC XSLPgetbanner( char * Banner); 
int XSLP_CC XSLPgetccoef( XSLPprob Prob, int RowIndex, int ColIndex, double * Factor, char * Formula, int fLen); 
int XSLP_CC XSLPgetcoef( XSLPprob Prob, int RowIndex, int ColIndex, double * Factor, int Parsed, int * Type, double * Value); 
int XSLP_CC XSLPgetcoefformula( XSLPprob Prob, int RowIndex, int ColIndex, double * Factor, int Parsed, int BufferSize, int * TokenCount, int * Type, double * Value); 
int XSLP_CC XSLPgetcoefs( XSLPprob Prob, int * nCoef, int * RowIndices, int * ColIndices); 
int XSLP_CC XSLPgetcvar( XSLPprob Prob, int nSLPCVar, char * cValue); 
int XSLP_CC XSLPgetdblattrib( XSLPprob Prob, int param, double * value); 
int XSLP_CC XSLPgetdblcontrol( XSLPprob Prob, int Param, double * dValue); 
int XSLP_CC XSLPgetdc( XSLPprob Prob, int RowIndex, char * RowType, int * Delay, int * IterCount, int Parsed, int * Type, double * Value); 
int XSLP_CC XSLPgetdcformula( XSLPprob Prob, int RowIndex, char * RowType, int * Delay, int * IterCount, int Parsed, int BufferSize, int * TokenCount, int * Type, double * Value); 
int XSLP_CC XSLPgetdf( XSLPprob Prob, int ColIndex, int RowIndex, double * DFValue); 
int XSLP_CC XSLPgetdtime( XSLPprob Prob, double * dSeconds); 
int XSLP_CC XSLPgetfuncinfo( int * ArgInfo, int * CallerFlag, int * nInput, int * nReturn, int * nDelta, int * nInString, int * nOutString, int * nSLPUserFunc, int * nInstance); 
int XSLP_CC XSLPgetfuncinfoV( void * ArgInfo, int * CallerFlags, int * nInput, int * nReturn, int * nDelta, int * nInString, int * nOutString, int * nSLPUserFunc, int * nInstance); 
int XSLP_CC XSLPgetfuncobject( int * ArgCount, int ObjType, void ** Object); 
int XSLP_CC XSLPgetfuncobjectV( void * ArgInfo, int ObjType, void ** Object); 
int XSLP_CC XSLPgetindex( XSLPprob Prob, int Type, const char * Name, int * SeqNo); 
int XSLP_CC XSLPgetintattrib( XSLPprob Prob, int Param, int * iValue); 
int XSLP_CC XSLPgetintcontrol( XSLPprob Prob, int Param, int * iValue); 
int XSLP_CC XSLPgetivf( XSLPprob Prob, int ColIndex, int Parsed, int * Type, double * Value); 
int XSLP_CC XSLPgetivformula( XSLPprob Prob, int ColIndex, int Parsed, int BufferSize, int * TokenCount, int * Type, double * Value); 
int XSLP_CC XSLPgetlasterror( XSLPprob Prob, int * Code, char * Buffer); 
int XSLP_CC XSLPgetmessagetype( int Code, int * Type); 
int XSLP_CC XSLPgetnames( XSLPprob Prob, int type, char * cNames, int First, int Last); 
int XSLP_CC XSLPgetparam( XSLPprob Prob, const char * Name, char * Value); 
int XSLP_CC XSLPgetptrattrib( XSLPprob Prob, int param, void ** value); 
int XSLP_CC XSLPgetrow( XSLPprob Prob, int RowIndex, int * Status); 
int XSLP_CC XSLPgetrowstatus( XSLPprob Prob, int RowIndex, int * Status); 
int XSLP_CC XSLPgetrowwt( XSLPprob Prob, int RowIndex, double * Value); 
int XSLP_CC XSLPgetslpsol( XSLPprob Prob, double * dx, double * dslack, double * dual, double * dj); 
int XSLP_CC XSLPgetstrattrib( XSLPprob Prob, int Param, char * cValue); 
int XSLP_CC XSLPgetstrcontrol( XSLPprob Prob, int Param, char * cValue); 
int XSLP_CC XSLPgetstring( XSLPprob Prob, int Param, char * cValue); 
int XSLP_CC XSLPgettime( XSLPprob Prob, int * Seconds, int * MilliSeconds); 
int XSLP_CC XSLPgettolset( XSLPprob Prob, int nSLPTol, int * Status, double * Tols); 
int XSLP_CC XSLPgetuserfunc( XSLPprob Prob, int nSLPUserFunc, char * xName, int * ArgType, int * ExeType, char * Param1, char * Param2, char * Param3); 
int XSLP_CC XSLPgetuserfuncaddress( XSLPprob Prob, int FuncNumber, void ** Address); 
int XSLP_CC XSLPgetuserfuncobject( XSLPprob Prob, int Entity, void ** Object); 
int XSLP_CC XSLPgetvar( XSLPprob Prob, int ColIndex, int * DetRow, double * InitStepBound, double * StepBound, double * Penalty, double * Damp, double * InitValue, double * Value, int * TolSet, int * History, int * Converged, int * VarType, int * Delta, int * PenaltyDelta, int * UpdateRow, double * OldValue); 
int XSLP_CC XSLPgetversion( int * version, int * minorversion); 
int XSLP_CC XSLPgetxv( XSLPprob Prob, int nXSLPXV, int * nXVitems); 
int XSLP_CC XSLPgetxvitem( XSLPprob Prob, int nSLPXV, int nXVitem, int Parsed, int * VarType, int * VarIndex, int * IntIndex, double * Delta1, double * Delta2, int * Status, int * Type, double * Value); 
int XSLP_CC XSLPgetxvitemformula( XSLPprob Prob, int nSLPXV, int nXVitem, int Parsed, int * VarType, int * VarIndex, int * IntIndex, double * Delta1, double * Delta2, int * Status, int BufferSize, int * TokenCount, int * Type, double * Value); 
int XSLP_CC XSLPglobal( XSLPprob Prob); 
int XSLP_CC XSLPinit(); 
int XSLP_CC XSLPinterrupt( XSLPprob Prob, int Reason); 
int XSLP_CC XSLPitemname( XSLPprob Prob, int Type, double Value, char * Buffer); 
int XSLP_CC XSLPloadcoefs( XSLPprob Prob, int nSLPCoef, const int * RowIndex, const int * ColIndex, const double * Factor, const int * FormulaStart, int Parsed, const int * Type, const double * Value); 
int XSLP_CC XSLPloadcvars( XSLPprob Prob, int nSLPCVar, const char * cValue); 
int XSLP_CC XSLPloaddcs( XSLPprob Prob, int nSLPDC, const int * RowIndex, const int * Delay, const int * DCStart, int Parsed, const int * Type, const double * Value); 
int XSLP_CC XSLPloaddfs( XSLPprob Prob, int nDF, const int * ColIndex, const int * RowIndex, const double * DFValue); 
int XSLP_CC XSLPloadivfs( XSLPprob Prob, int nIVF, const int * ColIndex, const int * IVFStart, int Parsed, const int * Type, const double * Value); 
int XSLP_CC XSLPloadtolsets( XSLPprob Prob, int nSLPTol, const double * SLPTol); 
int XSLP_CC XSLPloaduserfuncs( XSLPprob Prob, int nSLPUserFunc, const int * Type, const double * Value); 
int XSLP_CC XSLPloadvars( XSLPprob Prob, int nSLPVar, const int * ColIndex, const int * VarType, const int * DetRow, const int * SeqNum, const int * TolIndex, const double * InitValue, const double * StepBound); 
int XSLP_CC XSLPloadxvs( XSLPprob Prob, int nSLPXV, const int * XVStart, int Parsed, const int * Type, const double * Value); 
int XSLP_CC XSLPmaxim( XSLPprob Prob, const char * Flags); 
int XSLP_CC XSLPminim( XSLPprob Prob, const char * Flags); 
int XSLP_CC XSLPopt( XSLPprob Prob, const char * Flags); 
int XSLP_CC XSLPparsecformula( XSLPprob Prob, const char * Formula, int * nToken, int * Type, double * Value); 
int XSLP_CC XSLPparseformula( XSLPprob Prob, const int * inType, const double * inValue, int * nToken, int * Type, double * Value); 
int XSLP_CC XSLPpostsolve( XSLPprob Prob); 
int XSLP_CC XSLPpreparseformula( XSLPprob Prob, const char * Formula, int * nToken, int * Type, double * Value, char * StringTable, int * SizeTable); 
int XSLP_CC XSLPpresolve( XSLPprob Prob); 
int XSLP_CC XSLPprintmsg( XSLPprob Prob, int Type, const char * format); 
int XSLP_CC XSLPprintmemory( XSLPprob Prob); 
int XSLP_CC XSLPprintevalinfo( XSLPprob Prob); 
int XSLP_CC XSLPqparse( const char * Record, char ** Token, int NumFields); 
int XSLP_CC XSLPreadprob( XSLPprob Prob, const char * probname, const char * flags); 
int XSLP_CC XSLPreinitialize( XSLPprob Prob); 
int XSLP_CC XSLPremaxim( XSLPprob Prob, const char * Flags); 
int XSLP_CC XSLPreminim( XSLPprob Prob, const char * Flags); 
int XSLP_CC XSLPrestore( XSLPprob Prob, const char * ProbName); 
int XSLP_CC XSLPrevise( XSLPprob Prob, const char * probname); 
int XSLP_CC XSLProwinfo( XSLPprob Prob, int RowIndex, int InfoType, void * Info); 
int XSLP_CC XSLPsave( XSLPprob Prob); 
int XSLP_CC XSLPsaveas( XSLPprob Prob, const char * File); 
int XSLP_CC XSLPscaling( XSLPprob Prob); 
int XSLP_CC XSLPsetcurrentiv( XSLPprob Prob); 
int XSLP_CC XSLPsetdblcontrol( XSLPprob Prob, int Param, double dValue); 
int XSLP_CC XSLPsetdefaultcontrol( XSLPprob Prob, int Param); 
int XSLP_CC XSLPsetdefaults( XSLPprob Prob); 
int XSLP_CC XSLPsetfuncobject( int * ArgCount, int ObjType, void * Object); 
int XSLP_CC XSLPsetfunctionerror( XSLPprob Prob); 
int XSLP_CC XSLPsetintcontrol( XSLPprob Prob, int Param, int iValue); 
int XSLP_CC XSLPsetlogfile( XSLPprob Prob, const char * filename, int option); 
int XSLP_CC XSLPsetparam( XSLPprob Prob, const char * Name, const char * Value); 
int XSLP_CC XSLPsetstrcontrol( XSLPprob Prob, int Param, const char * cValue); 
int XSLP_CC XSLPsetstring( XSLPprob Prob, int * Param, const char * cValue); 
int XSLP_CC XSLPsetuniqueprefix( XSLPprob Prob); 
int XSLP_CC XSLPsetuserfuncaddress( XSLPprob Prob, int FuncNumber, void * Address); 
int XSLP_CC XSLPsetuserfuncinfo( XSLPprob Prob, int * ArgInfo, int CallerFlag, int nInput, int nReturn, int nDelta, int nInString, int nOutString); 
int XSLP_CC XSLPsetuserfuncobject( XSLPprob Prob, int Entity, void * Object); 
int XSLP_CC XSLPtime( XSLPprob Prob); 
int XSLP_CC XSLPtokencount( const char * Record); 
int XSLP_CC XSLPunconstruct( XSLPprob Prob); 
int XSLP_CC XSLPuprintmemory( XSLPprob Prob); 
int XSLP_CC XSLPuserfuncinfo( XSLPprob Prob, int iFunc, int InfoType, void * Info); 
int XSLP_CC XSLPupdatelinearization( XSLPprob Prob); 
int XSLP_CC XSLPvalidate( XSLPprob Prob); 
int XSLP_CC XSLPvalidatevector( XSLPprob Prob, double * Vector, double * SumInf, double * SumScaledInf, double * Objective); 
int XSLP_CC XSLPvalidaterow( XSLPprob Prob, int Row); 
int XSLP_CC XSLPvalidformula( const int * inType, const double * inValue, int * nToken, char * Name, char * StringTable); 
int XSLP_CC XSLPvalidatekkt( XSLPprob Prob, int iCalculationMode, int iRespectBasisStatus, int iUpdateMultipliers, double dKKTViolationTarget); 
int XSLP_CC XSLPwriteprob( XSLPprob Prob, const char * filename, const char * flags); 
int XSLP_CC XSLPchgcascadenlimit( XSLPprob Prob, int iCol, int CascadeNLimit); 
int XSLP_CC XSLPgetcascadenlimit( XSLPprob Prob, int iCol, int * CascadeNLimit); 
int XSLP_CC XSLPgetfunctioninstance( XSLPprob Prob, int Instance, int * nSLPUF, int BufferSize, int * TokenCount, int * Type, double * Value); 
int XSLP_CC XSLPloadivs( XSLPprob Prob, char * Filename, int NameField, int ValueField, int Option); 
int XSLP_CC XSLPsetcbcascadeend( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*), void * Object); 
int XSLP_CC XSLPsetcbcascadestart( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*), void * Object); 
int XSLP_CC XSLPsetcbcascadevar( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*,int), void * Object); 
int XSLP_CC XSLPsetcbcascadevarF( XSLPprob Prob, int (*UserFunc)(XSLPprob,void*,int*), void * Object); 
int XSLP_CC XSLPsetcbcascadevarfail( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*,int), void * Object); 
int XSLP_CC XSLPsetcbconstruct( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*), void * Object); 
int XSLP_CC XSLPsetcbdestroy( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*), void * Object); 
int XSLP_CC XSLPsetcbformula( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*,double,double*), void * Object); 
int XSLP_CC XSLPsetcbintsol( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*), void * Object); 
int XSLP_CC XSLPsetcbiterend( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*), void * Object); 
int XSLP_CC XSLPsetcbiterstart( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*), void * Object); 
int XSLP_CC XSLPsetcbitervar( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*,int), void * Object); 
int XSLP_CC XSLPsetcbitervarF( XSLPprob Prob, int (*UserFunc)(XSLPprob,void*,int*), void * Object); 
int XSLP_CC XSLPsetcbmessage( XSLPprob Prob, void (XSLP_CC *UserFunc)(XSLPprob,void*,char*,int,int), void * Object); 
int XSLP_CC XSLPsetcbmessageF( XSLPprob Prob, void (*UserFunc)(XSLPprob,void*,char*,int*,int*), void * Object); 
int XSLP_CC XSLPsetcboptnode( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*,int*), void * Object); 
int XSLP_CC XSLPsetcbprenode( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*,int*), void * Object); 
int XSLP_CC XSLPsetcbslpend( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*), void * Object); 
int XSLP_CC XSLPsetcbslpnode( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*,int*), void * Object); 
int XSLP_CC XSLPsetcbslpstart( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*), void * Object); 
int XSLP_CC XSLPsetcbdrcol( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*, int, int, double,double *, double,double), void * Object); 
int XSLP_CC XSLPsetcbinterrupt( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*), void * Object); 
int XSLP_CC XSLPsetcbitersol( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*), void * Object); 
int XSLP_CC XSLPsetcbmsjobstart( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*,void*,const char*,int*), void * Object); 
int XSLP_CC XSLPsetcbmsjobend( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*,void*,const char*,int*), void * Object); 
int XSLP_CC XSLPsetcbmswinner( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*,void*,const char*), void * Object); 
int XSLP_CC XSLPsetcbcoefevalerror( XSLPprob Prob, int (XSLP_CC *UserFunc)(XSLPprob,void*,int,int), void * Object); 
int XSLP_CC XSLPmsaddjob( XSLPprob Prob, const char * sDescription, const int nIVs, const int * ivCols, const double * ivValues, const int nIntControls, const int * IntControlIndices, const int * IntControlValues, const int nDblControls, const int * DblControlIndices, const double * DblControlValues, void * pJobObject); 
int XSLP_CC XSLPmsaddpreset( XSLPprob Prob, const char * sDescription, const int Preset, const int Count, void * pJobObject); 
int XSLP_CC XSLPmsaddcustompreset( XSLPprob Prob, const char * sDescription, const int Preset, const int Count, const int nIVs, const int * ivCols, const double * ivValues, const int nIntControls, const int * IntControlIndices, const int * IntControlValues, const int nDblControls, const int * DblControlIndices, const double * DblControlValues, void * pJobObject); 
int XSLP_CC XSLPmsclear( XSLPprob Prob); 
int XSLP_CC XSLPboundtighten( XSLPprob Prob, int * nTightened); 

#ifdef __cplusplus
}
#endif

#endif

