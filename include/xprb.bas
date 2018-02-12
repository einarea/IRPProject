Attribute VB_Name = "XPRB"
' BCL :  Xpress Builder Component Library
' File xprb.bas
' Visual Basic Module
' (c) Copyright Fair Isaac Corporation 2002-2014 All rights reserved.

Option Explicit
Option Base 0
Option Compare Text

#If (VBA7 <> 0) And (Win64 <> 0) Then
Global Const XPRB_NULL As LongPtr = 0
Global Const XPRB_PTR_VARTYPE = vbLongLong
#Else
Global Const XPRB_NULL As Long = 0
Global Const XPRB_PTR_VARTYPE = vbLong
#End If

Global Const XPRB_INFINITY = 1E+20

' BCL version
Global Const XPRB_VERSION = "4.8.4"

' Error types
Global Const XPRB_ERR = 1
Global Const XPRB_WAR = 0

' Variable types
Global Const XPRB_PL = 0
Global Const XPRB_BV = 1
Global Const XPRB_UI = 2
Global Const XPRB_PI = 3
Global Const XPRB_SC = 4
Global Const XPRB_SI = 5

' Constraint types
Global Const XPRB_N = 0
Global Const XPRB_L = 1
Global Const XPRB_G = 2
Global Const XPRB_E = 3
Global Const XPRB_R = 4

' Sense of the objective function
Global Const XPRB_MAXIM = 1
Global Const XPRB_MINIM = 0

' SOS types
Global Const XPRB_S1 = 0
Global Const XPRB_S2 = 1

' Directive types
Global Const XPRB_PR = 1
Global Const XPRB_UP = 2
Global Const XPRB_DN = 3
Global Const XPRB_PU = 4
Global Const XPRB_PD = 5

' Range types
Global Const XPRB_UPACT = 0
Global Const XPRB_LOACT = 1
Global Const XPRB_UUP = 2
Global Const XPRB_UDN = 3
Global Const XPRB_UCOST = 4
Global Const XPRB_LCOST = 5

' Dictionary types
Global Const XPRB_VAR = 1
Global Const XPRB_ARR = 2
Global Const XPRB_CTR = 3
Global Const XPRB_SOS = 4
Global Const XPRB_IDX = 5

' Dictionaries
Global Const XPRB_DICT_NAMES = 0
Global Const XPRB_DICT_IDX = 1

' File formats
Global Const XPRB_LP = 1
Global Const XPRB_MPS = 2

' Synchronization
Global Const XPRB_XPRS_SOL = 1
Global Const XPRB_XPRS_PROB = 2
Global Const XPRB_XPRS_SOLMIP = 3

' BCL problem status
Global Const XPRB_GEN = 1       ' Matrix has been generated
Global Const XPRB_DIR = 2       ' Directive added
Global Const XPRB_MOD = 4       ' Row/Col modification
Global Const XPRB_SOL = 8       ' Solution available

' LP status
Global Const XPRB_LP_OPTIMAL = 1
Global Const XPRB_LP_INFEAS = 2
Global Const XPRB_LP_CUTOFF = 3
Global Const XPRB_LP_UNFINISHED = 4
Global Const XPRB_LP_UNBOUNDED = 5
Global Const XPRB_LP_CUTOFF_IN_DUAL = 6
Global Const XPRB_LP_UNSOLVED = 7
Global Const XPRB_LP_NONCONVEX = 8

' MIP status
Global Const XPRB_MIP_NOT_LOADED = 0
Global Const XPRB_MIP_LP_NOT_OPTIMAL = 1
Global Const XPRB_MIP_LP_OPTIMAL = 2
Global Const XPRB_MIP_NO_SOL_FOUND = 3
Global Const XPRB_MIP_SOLUTION = 4
Global Const XPRB_MIP_INFEAS = 5
Global Const XPRB_MIP_OPTIMAL = 6
Global Const XPRB_MIP_UNBOUNDED = 7

' To set constant term in constraints
Global Const XPRB_RHS = 0

' Stream codes for SetStream
Global Const XPRBIO_IN = 0
Global Const XPRBIO_OUT = 1
Global Const XPRBIO_ERR = 2

Private Enum CB_Contexts
 CBCON_MSG = 0
 CBCON_err
 '...
 CBCON_COUNT
End Enum

Private cbcontexts(0 To CBCON_COUNT - 1) As New Collection
Private initCount As Long

#If (VBA7 <> 0) And (Win64 <> 0) Then

'*** Redirection of IO ***
Declare PtrSafe Function XPRBsetStream Lib "xprvb" Alias "xvbsetstream" (ByVal what As Long, ByVal fname As String) As Long
Private Declare PtrSafe Function xvboutput Lib "xprvb" (ByVal what As Long, ByVal text As String) As Long
Declare PtrSafe Function XPRBflush Lib "xprvb" Alias "xvbflush" (ByVal what As Long) As Long
Declare PtrSafe Function XPRBclose Lib "xprvb" Alias "xvbclose" (ByVal what As Long) As Long

'*** Init/Finish ***
Private Declare PtrSafe Function xbslinit Lib "xprb" () As Long
Private Declare PtrSafe Function xbgetversion Lib "xprb" () As LongPtr
Private Declare PtrSafe Function xbslfinish Lib "xprb" () As Long
Declare PtrSafe Function XPRBgettime Lib "xprb" Alias "xbgettime" () As Long

'*** Error handling ***
Declare PtrSafe Function XPRBseterrctrl Lib "xprb" Alias "xbseterrctrl" (ByVal flag As Long) As Long
Private Declare PtrSafe Function xbdefcberr Lib "xprb" Alias "xbdefcberr_prob" (ByVal prob As LongPtr, ByVal Address As LongPtr, ByVal vp As LongPtr) As Long
Private Declare PtrSafe Sub xvbcallcberr Lib "xprvb" (ByVal fn As LongPtr, ByRef prob As Variant, ByRef context As Variant, ByRef errnum As Variant, ByRef errtype As Variant, ByRef msg As Variant)
Private Declare PtrSafe Function xvbgeterrlvltext Lib "xprvb" (ByVal lvl As Long, ByVal folder As String) As LongPtr

'*** Problem definition ***
Private Declare PtrSafe Function xbnewprob Lib "xprb" (ByVal nam As String) As LongPtr
Declare PtrSafe Function XPRBgetprob Lib "xprb" Alias "xbgetprob" () As LongPtr
Declare PtrSafe Function XPRBchgprob Lib "xprb" Alias "xbchgprob" (ByVal prob As LongPtr) As Long
Private Declare PtrSafe Function xbdelprob Lib "xprb" Alias "xbdelprob_prob" (ByVal prob As LongPtr) As Long
Private Declare PtrSafe Function xbresetprob Lib "xprb" Alias "xbresetprob_prob" (ByVal prob As LongPtr) As Long
Private Declare PtrSafe Function xbgetXPRSprob Lib "xprb" Alias "xbgetsprob_prob" (ByVal prob As LongPtr) As LongPtr
Private Declare PtrSafe Function xbsetprobname Lib "xprb" Alias "xbsetprobname_prob" (ByVal prob As LongPtr, ByVal name As String) As Long

'*** Output and printing ***
Private Declare PtrSafe Function xbdefcbmsg Lib "xprb" Alias "xbdefcbmsg_prob" (ByVal prob As LongPtr, ByVal fn As LongPtr, ByVal context As LongPtr) As Long
Private Declare PtrSafe Sub xvbcallcbmsg Lib "xprvb" (ByVal fn As LongPtr, ByRef prob As Variant, ByRef context As Variant, ByRef msg As Variant)
Private Declare PtrSafe Function xbsetmsglevel Lib "xprb" Alias "xbsetmsglevel_prob" (ByVal prob As LongPtr, ByVal level As Long) As Long   ' default: 2
Private Declare PtrSafe Function xbsetrealfmt Lib "xprb" Alias "xbsetrealfmt_prob" (ByVal prob As LongPtr, ByVal fmt As String) As Long
Private Declare PtrSafe Function xbsetcolorder Lib "xprb" Alias "xbsetcolorder_prob" (ByVal prob As LongPtr, ByVal num As Long) As Long   ' default: 0
Private Declare PtrSafe Function xbfixglobals Lib "xprb" Alias "xbfixglobals_prob" (ByVal prob As LongPtr, ByVal ifFound As Long) As Long

'*** Variable definition ***
Private Declare PtrSafe Function xbnewvar Lib "xprb" Alias "xbnewvar_prob" (ByVal prob As LongPtr, ByVal typ As Long, ByVal nam As String, ByVal lob As Double, ByVal upb As Double) As LongPtr
Declare PtrSafe Function XPRBsetvartype Lib "xprb" Alias "xbsetvartype" (ByVal v As LongPtr, ByVal typ As Long) As Long

'*** Constraint definition ***
Private Declare PtrSafe Function xbnewctr Lib "xprb" Alias "xbnewctr_prob" (ByVal prob As LongPtr, ByVal nam As String, ByVal qrtype As Long) As LongPtr
Declare PtrSafe Function XPRBdelctr Lib "xprb" Alias "xbdelctr" (ByVal lct As Long) As Long
Private Declare PtrSafe Function xbaddexpr_term Lib "xprb" (ByVal lct As LongPtr, ByVal v As LongPtr, ByVal v2 As LongPtr, ByVal coeff As Double) As Long
Declare PtrSafe Function XPRBdelterm Lib "xprb" Alias "xbdelterm" (ByVal lct As LongPtr, ByVal v As LongPtr) As Long
Declare PtrSafe Function XPRBsetterm Lib "xprb" Alias "xbsetterm" (ByVal lct As LongPtr, ByVal v As LongPtr, ByVal coeff As Double) As Long
Declare PtrSafe Function XPRBnewprec Lib "xprb" Alias "xbnewprec_prob" (ByVal prob As LongPtr, ByVal nam As String, ByVal v1 As LongPtr, ByVal coeff As Double, ByVal v2 As LongPtr) As LongPtr
Declare PtrSafe Function XPRBsetctrtype Lib "xprb" Alias "xbsetctrtype" (ByVal lct As LongPtr, ByVal qrtype As Long) As Long
Declare PtrSafe Function XPRBsetrange Lib "xprb" Alias "xbsetrange" (ByVal lct As LongPtr, ByVal low As Double, ByVal up As Double) As Long
Declare PtrSafe Function XPRBsetmodcut Lib "xprb" Alias "xbsetmodcut" (ByVal lct As LongPtr, ByVal mcstat As Long) As Long
Private Declare PtrSafe Function xbaddqterm Lib "xprb" (ByVal lct As LongPtr, ByVal v1 As LongPtr, ByVal v2 As LongPtr, ByVal coeff As Double) As Long
Private Declare PtrSafe Function xbsetqterm Lib "xprb" (ByVal lct As LongPtr, ByVal v1 As LongPtr, ByVal v2 As LongPtr, ByVal coeff As Double) As Long
Private Declare PtrSafe Function xbaddterm Lib "xprb" (ByVal lct As LongPtr, ByVal v1 As LongPtr, ByVal coeff As Double) As Long
Private Declare PtrSafe Function xbsetterm Lib "xprb" (ByVal lct As LongPtr, ByVal v1 As LongPtr, ByVal coeff As Double) As Long
Private Declare PtrSafe Function xbsetexpr_term Lib "xprb" (ByVal lct As LongPtr, ByVal v1 As LongPtr, ByVal v2 As LongPtr, ByVal coeff As Double) As Long

'*** SOS definition ***
Private Declare PtrSafe Function xbnewsos Lib "xprb" Alias "xbnewsos_prob" (ByVal prob As LongPtr, ByVal nam As String, ByVal typ As Long) As LongPtr
Declare PtrSafe Function XPRBdelsos Lib "xprb" Alias "xbdelsos" (ByVal sos As LongPtr) As Long
Declare PtrSafe Function XPRBaddsosel Lib "xprb" Alias "xbaddsosel" (ByVal sos As LongPtr, ByVal v As LongPtr, ByVal ref As Double) As Long
Declare PtrSafe Function XPRBdelsosel Lib "xprb" Alias "xbdelsosel" (ByVal sos As LongPtr, ByVal v As LongPtr) As Long

'*** Objective function ***
Private Declare PtrSafe Function xbsetsense Lib "xprb" Alias "xbsetsense_prob" (ByVal prob As LongPtr, ByVal d As Long) As Long    ' default: minimise
Private Declare PtrSafe Function xbgetsense Lib "xprb" Alias "xbgetsense_prob" (ByVal prob As LongPtr) As Long
Private Declare PtrSafe Function xbsetobj Lib "xprb" Alias "xbsetobj_prob" (ByVal prob As LongPtr, ByVal lct As LongPtr) As Long

'*** Variable bounds ***
Declare PtrSafe Function XPRBsetub Lib "xprb" Alias "xbsetub" (ByVal v As LongPtr, ByVal c As Double) As Long
Declare PtrSafe Function XPRBsetlb Lib "xprb" Alias "xbsetlb" (ByVal v As LongPtr, ByVal c As Double) As Long
Declare PtrSafe Function XPRBfixvar Lib "xprb" Alias "xbfixvar" (ByVal v As LongPtr, ByVal c As Double) As Long
Declare PtrSafe Function XPRBsetlim Lib "xprb" Alias "xbsetlim" (ByVal v As LongPtr, ByVal c As Double) As Long

'*** Names ***
Private Declare PtrSafe Function xbgetbyname Lib "xprb" Alias "xbgetbyname_prob" (ByVal prob As LongPtr, ByVal nam As String, ByVal typ As Long) As LongPtr
Private Declare PtrSafe Function xbsetdictionarysize Lib "xprb" Alias "xbsetdictsize_prob" (ByVal prob As LongPtr, ByVal dico As Long, ByVal siz As Long) As Long
Private Declare PtrSafe Function xbgetidxelname Lib "xprb" (ByVal idx As LongPtr, ByVal i As Long) As LongPtr
Private Declare PtrSafe Function xbgetprobname Lib "xprb" Alias "xbgetprobname_prob" (ByVal prob As LongPtr) As LongPtr
Private Declare PtrSafe Function xbgetvarname Lib "xprb" (ByVal v As LongPtr) As LongPtr
Private Declare PtrSafe Function xbgetctrname Lib "xprb" (ByVal lct As LongPtr) As LongPtr
Private Declare PtrSafe Function xbgetsosname Lib "xprb" (ByVal sos As LongPtr) As LongPtr
Private Declare PtrSafe Function xbgetidxsetname Lib "xprb" (ByVal idx As LongPtr) As LongPtr

'*** Directives ***
Private Declare PtrSafe Function xbsetvardir Lib "xprb" (ByVal v As LongPtr, ByVal typ As Long, ByVal cost As Double) As Long
Private Declare PtrSafe Function xbsetsosdir Lib "xprb" (ByVal ls As LongPtr, ByVal typ As Long, ByVal vl As Double) As Long
Private Declare PtrSafe Function xbcleardir Lib "xprb" Alias "xbcleardir_prob" (ByVal prob As LongPtr) As Long

'*** Index sets ***
Private Declare PtrSafe Function xbnewidxset Lib "xprb" Alias "xbnewidxset_prob" (ByVal prob As LongPtr, ByVal nam As String, ByVal maxsize As Long) As LongPtr
Private Declare PtrSafe Function xbaddidxel Lib "xprb" (ByVal idx As LongPtr, ByVal nam As String) As Long
Private Declare PtrSafe Function xbgetidxel Lib "xprb" (ByVal idx As LongPtr, ByVal nam As String) As Long
Declare PtrSafe Function XPRBgetidxsetsize Lib "xprb" Alias "xbgetidxsetsize" (ByVal idx As LongPtr) As Long

'*** Output and printing ***
Private Declare PtrSafe Function xbprintprob Lib "xprb" Alias "xbprintprob_prob" (ByVal prob As LongPtr) As Long
Private Declare PtrSafe Function xbprintobj Lib "xprb" Alias "xbprintobj_prob" (ByVal prob As LongPtr) As Long
Private Declare PtrSafe Function xbexportprob Lib "xprb" Alias "xbexportprob_prob" (ByVal prob As LongPtr, ByVal typ As Long, ByVal filename As String) As Long

Declare PtrSafe Function XPRBprintvar Lib "xprb" Alias "xbprintvar" (ByVal v As LongPtr) As Long
Declare PtrSafe Function XPRBprintctr Lib "xprb" Alias "xbprintctr" (ByVal lct As LongPtr) As Long
Declare PtrSafe Function XPRBprintsos Lib "xprb" Alias "xbprintsos" (ByVal ls As LongPtr) As Long
Declare PtrSafe Function XPRBprintidxset Lib "xprb" Alias "xbprintidxset" (ByVal idx As LongPtr) As Long
Declare PtrSafe Function XPRBprintexpr Lib "xprb" Alias "xbprintexpr" (ByVal prob As LongPtr, ByVal tls As LongPtr, ByVal nl As Long, ByVal dbl As Double) As Long

'*** Information on variables and constraints ***
Declare PtrSafe Function XPRBgetcolnum Lib "xprb" Alias "xbgetcolnum" (ByVal v As LongPtr) As Long
Declare PtrSafe Function XPRBgetrownum Lib "xprb" Alias "xbgetrownum" (ByVal lct As LongPtr) As Long
Declare PtrSafe Function XPRBgetvartype Lib "xprb" Alias "xbgetvartype" (ByVal v As LongPtr) As Long
Declare PtrSafe Function XPRBgetctrtype Lib "xprb" Alias "xbgetctrtype" (ByVal lct As LongPtr) As Long
Declare PtrSafe Function XPRBgetctrsize Lib "xprb" Alias "xbgetctrsize" (ByVal lct As LongPtr) As Long
Declare PtrSafe Function XPRBgetsostype Lib "xprb" Alias "xbgetsostype" (ByVal sos As LongPtr) As Long
Declare PtrSafe Function XPRBgetrhs Lib "xprb" Alias "xbgetrhs" (ByVal lct As LongPtr) As Double
Declare PtrSafe Function XPRBgetmodcut Lib "xprb" Alias "xbgetmodcut" (ByVal lct As LongPtr) As Long
Declare PtrSafe Function XPRBgetbounds Lib "xprb" Alias "xbgetbounds" (ByVal v As Long, ByRef lbd As Double, ByRef ubd As Double) As Long
Declare PtrSafe Function XPRBgetlim Lib "xprb" Alias "xbgetlim" (ByVal v As LongPtr, lim As Double) As Long
Declare PtrSafe Function XPRBgetrange Lib "xprb" Alias "xbgetrange" (ByVal lct As LongPtr, ByRef low As Double, ByRef up As Double) As Long

'*** Basis management ***
Private Declare PtrSafe Function xbsavebasis Lib "xprb" Alias "xbsavebasis_prob" (ByVal prob As LongPtr) As LongPtr
Declare PtrSafe Function XPRBloadbasis Lib "xprb" Alias "xbloadbasis" (ByVal basis As LongPtr) As Long
Declare PtrSafe Sub XPRBdelbasis Lib "xprb" Alias "xbdelbasis" (ByVal basis As LongPtr)

'*** Problem solving and solution information ***
Private Declare PtrSafe Function xbloadmat Lib "xprb" Alias "xbloadmat_prob" (ByVal prob As LongPtr) As Long
Private Declare PtrSafe Function xbsolve Lib "xprb" Alias "xbsolve_prob" (ByVal prob As LongPtr, ByVal alg As String) As Long
Private Declare PtrSafe Function xbminim Lib "xprb" Alias "xbminim_prob" (ByVal prob As LongPtr, ByVal alg As String) As Long
Private Declare PtrSafe Function xbmaxim Lib "xprb" Alias "xbmaxim_prob" (ByVal prob As LongPtr, ByVal alg As String) As Long
Private Declare PtrSafe Function xblpoptimize Lib "xprb" Alias "xblpoptimize_prob" (ByVal prob As LongPtr, ByVal alg As String) As Long
Private Declare PtrSafe Function xbmipoptimize Lib "xprb" Alias "xbmipoptimize_prob" (ByVal prob As LongPtr, ByVal alg As String) As Long
Private Declare PtrSafe Function xbgetprobstat Lib "xprb" Alias "xbgetprobstat_prob" (ByVal prob As LongPtr) As Long
Private Declare PtrSafe Function xbgetlpstat Lib "xprb" Alias "xbgetlpstat_prob" (ByVal prob As LongPtr) As Long
Private Declare PtrSafe Function xbgetmipstat Lib "xprb" Alias "xbgetmipstat_prob" (ByVal prob As LongPtr) As Long
Private Declare PtrSafe Function xbsync Lib "xprb" Alias "xbsync_prob" (ByVal prob As LongPtr, ByVal synctype As Long) As Long
Private Declare PtrSafe Function xbbegincb Lib "xprb" Alias "xbbegincb_prob" (ByVal prob As LongPtr, ByVal optprob As LongPtr) As Long
Private Declare PtrSafe Function xbendcb Lib "xprb" Alias "xbendcb_prob" (ByVal prob As LongPtr) As Long
Private Declare PtrSafe Function xbgetobjval Lib "xprb" Alias "xbgetobjval_prob" (ByVal prob As LongPtr) As Double
Declare PtrSafe Function XPRBgetsol Lib "xprb" Alias "xbgetsol" (ByVal v As LongPtr) As Double
Declare PtrSafe Function XPRBgetrcost Lib "xprb" Alias "xbgetrcost" (ByVal v As LongPtr) As Double
Declare PtrSafe Function XPRBgetslack Lib "xprb" Alias "xbgetslack" (ByVal lct As LongPtr) As Double
Declare PtrSafe Function XPRBgetact Lib "xprb" Alias "xbgetact" (ByVal lct As LongPtr) As Double
Declare PtrSafe Function XPRBgetdual Lib "xprb" Alias "xbgetdual" (ByVal lct As LongPtr) As Double
Declare PtrSafe Function XPRBgetctrrng Lib "xprb" Alias "xbgetctrrng" (ByVal lct As LongPtr, ByVal rngtype As Long) As Double
Declare PtrSafe Function XPRBgetvarrng Lib "xprb" Alias "xbgetvarrng" (ByVal v As LongPtr, ByVal rngtype As Long) As Double

'*** Indicator constraints ***
Declare PtrSafe Function XPRBsetdelayed Lib "xprb" Alias "xbsetdelayed" (ByVal ctrRef As LongPtr, ByVal delayed As Long) As Long
Declare PtrSafe Function XPRBgetdelayed Lib "xprb" Alias "xbgetdelayed" (ByVal ctrRef As LongPtr) As Long
Declare PtrSafe Function XPRBsetindicator Lib "xprb" Alias "xbsetindicator" (ByVal ctrRef As LongPtr, ByVal dir As Long, ByVal varRef As LongPtr)
Declare PtrSafe Function XPRBgetindicator Lib "xprb" Alias "xbgetindicator" (ByVal ctrRef As LongPtr) As Long
Declare PtrSafe Function XPRBgetindvar Lib "xprb" Alias "xbgetindvar" (ByVal ctrRef As LongPtr) As LongPtr
Declare PtrSafe Function XPRBgetincvars Lib "xprb" Alias "xbgetincvars" (ByVal ctrRef As LongPtr) As Long
Declare PtrSafe Function XPRBsetincvars Lib "xprb" Alias "xbsetincvars" (ByVal ctrRef As LongPtr, ByVal ivstat As Long) As Long

'*** MIP solutions ***
Declare PtrSafe Function XPRBloadmipsol Lib "xprb" Alias "xbloadmipsol_prob" (ByVal probRef As LongPtr, ByRef sol As Double, ByVal ncol As Long, ByVal ifopt As Long) As Long
Declare PtrSafe Function XPRBaddmipsol Lib "xprb" Alias "xbaddmipsol_prob" (ByVal probRef As LongPtr, ByVal solRef As LongPtr, ByVal name As String) As Long

'*** Defining solutions
Private Declare PtrSafe Function xbnewsol Lib "xprb" Alias "xbnewsol_prob" (ByVal probRef As LongPtr) As LongPtr
Declare PtrSafe Function XPRBdelsol Lib "xprb" Alias "xbdelsol" (ByVal solRef As LongPtr) As Long
Declare PtrSafe Function XPRBsetsolvar Lib "xprb" Alias "xbsetsolvar" (ByVal solRef As LongPtr, ByVal varRef As LongPtr, ByVal val As Double) As Long
Declare PtrSafe Function XPRBdelsolvar Lib "xprb" Alias "xbdelsolvar" (ByVal solRef As LongPtr, ByVal varRef As LongPtr) As Long
Declare PtrSafe Function xbgetsolvar Lib "xprb" (ByVal solRef As LongPtr, ByVal varRef As LongPtr, ByRef val As Double) As Long
Declare PtrSafe Function XPRBgetsolsize Lib "xprb" Alias "xbgetsolsize" (ByVal solRef As LongPtr) As Long
Declare PtrSafe Function XPRBprintsol Lib "xprb" Alias "xbprintsol" (ByVal solRef As LongPtr) As Long

'*** Managing solutions
Private Declare PtrSafe Function xbwritesol Lib "xprb" Alias "xbwritesol_prob" (ByVal probRef As LongPtr, ByVal filename As String, ByVal flags As String) As Long
Private Declare PtrSafe Function xbwritebinsol Lib "xprb" Alias "xbwritebinsol_prob" (ByVal probRef As LongPtr, ByVal filename As String, ByVal flags As String) As Long
Private Declare PtrSafe Function xbwriteslxsol Lib "xprb" Alias "xbwriteslxsol_prob" (ByVal probRef As LongPtr, ByVal filename As String, ByVal flags As String) As Long
Private Declare PtrSafe Function xbwriteprtsol Lib "xprb" Alias "xbwriteprtsol_prob" (ByVal probRef As LongPtr, ByVal filename As String, ByVal flags As String) As Long
Private Declare PtrSafe Function xbreadbinsol Lib "xprb" Alias "xbreadbinsol_prob" (ByVal probRef As LongPtr, ByVal filename As String, ByVal flags As String) As Long
Private Declare PtrSafe Function xbreadslxsol Lib "xprb" Alias "xbreadslxsol_prob" (ByVal probRef As LongPtr, ByVal filename As String, ByVal flags As String) As Long

'*** String conversion ***
Private Declare PtrSafe Sub XPRadr2str Lib "xprvb" Alias "xvbadr2str" (ByVal adrstr As LongPtr, ByVal rts As String, ByVal rtslen As Long)
Private Declare PtrSafe Function XPRadrstrlen Lib "xprvb" Alias "xvbadrstrlen" (ByVal adrstr As LongPtr) As Long

'*****************************************************************

Function XPRBgetString(ByRef adrstr As LongPtr) As String
 Dim rts As String
 
 If (adrstr = 0) Then
  XPRBgetString = ""
 Else
  Dim slen As Long
  slen = XPRadrstrlen(adrstr)
  rts = String$(slen + 1, vbNullChar)
  XPRadr2str adrstr, rts, Len(rts)
  XPRBgetString = Left(rts, InStr(1, rts, vbNullChar) - 1)
 End If
End Function

'*** Init/Finish ***
Function XPRBinit(Optional ByRef d As String = vbNullString) As Long
 If initCount <= 0 Then
  XPRBseterrctrl 0
  If Not xvbhascbcontext(CBCON_MSG, XPRB_NULL) Then XPRBdefcbmsg XPRB_NULL, AddressOf XPRBcbmsgdefault
  If Not xvbhascbcontext(CBCON_err, XPRB_NULL) Then XPRBdefcberr XPRB_NULL, AddressOf XPRBcberrdefault
 End If
 
 Dim result As Long
 result = xbslinit()
 If result = 0 Then
   If initCount < 0 Then initCount = 0
   initCount = initCount + 1
 End If
 XPRBinit = result
End Function

Function XPRBfinish() As Long
 XPRBfinish = xbslfinish()
 
 If initCount <= 1 Then
  On Error Resume Next
  initCount = initCount - 1
  If initCount < 0 Then initCount = 0
  Dim context As Variant
  For Each context In cbcontexts
    context.Clear
  Next
 End If
End Function

Function XPRBfree() As Long
 XPRBfree = XPRBfinish()
End Function

Function XPRBgetversion() As String
 XPRBgetversion = XPRBgetString(xbgetversion())
End Function


'*** Problem creation ***
Function XPRBnewprob(Optional ByVal nam As String = vbNullString) As LongPtr
 If initCount <= 0 Then
  If XPRBinit Then
   XPRBnewprob = XPRB_NULL
   Exit Function
  End If
 End If
 XPRBnewprob = xbnewprob(nam)
End Function

'*** Problem definition ***
Function XPRBdelprob(Optional ByVal prob As LongPtr = 0) As Long
 If prob = 0 Then
  XPRBdelprob = xbdelprob(XPRBgetprob())
 Else
  XPRBdelprob = xbdelprob(prob)
 End If
End Function

Function XPRBsetprobname(Optional ByVal prob As LongPtr = 0, Optional ByVal name As String = vbNullString) As Long
  If prob = 0 Then
    XPRBsetprobname = xbsetprobname(XPRBgetprob(), name)
  Else
    XPRBsetprobname = xbsetprobname(prob, name)
  End If
End Function

Function XPRBgetXPRSprob(Optional ByVal prob As LongPtr = 0) As LongPtr
 If prob = 0 Then
  XPRBgetXPRSprob = xbgetXPRSprob(XPRBgetprob())
 Else
  XPRBgetXPRSprob = xbgetXPRSprob(prob)
 End If
End Function

Function XPRBsetmsglevel(Optional ByVal prob As LongPtr = 0, Optional ByVal level As Long = 2) As Long
 If prob = 0 Then
  XPRBsetmsglevel = xbsetmsglevel(XPRBgetprob(), level)
 Else
  XPRBsetmsglevel = xbsetmsglevel(prob, level)
 End If
End Function

Function XPRBsetrealfmt(Optional ByVal prob As LongPtr = 0, Optional ByVal fmt As String) As Long
 If prob = 0 Then
  XPRBsetrealfmt = xbsetrealfmt(XPRBgetprob(), fmt)
 Else
  XPRBsetrealfmt = xbsetrealfmt(prob, fmt)
 End If
End Function

Function XPRBsetcolorder(Optional ByVal prob As LongPtr = 0, Optional ByVal num As Long = 0) As Long
 If prob = 0 Then
  XPRBsetcolorder = xbsetcolorder(XPRBgetprob(), num)
 Else
  XPRBsetcolorder = xbsetcolorder(prob, num)
 End If
End Function

Function XPRBfixglobals(Optional ByVal prob As LongPtr = 0, Optional ByVal ifRound As Long = 0) As Long
 If prob = 0 Then
  XPRBfixglobals = xbfixglobals(XPRBgetprob(), ifRound)
 Else
  XPRBfixglobals = xbfixglobals(prob, ifRound)
 End If
End Function



'*** Variable definition ***
Function XPRBnewvar(Optional ByVal prob As LongPtr = 0, _
        Optional ByVal typ As Long = XPRB_PL, Optional ByVal nam As String = "", _
        Optional ByVal lob As Double = 0, _
        Optional ByVal upb As Double = XPRB_INFINITY) As LongPtr
 If prob = 0 Then
  XPRBnewvar = xbnewvar(XPRBgetprob(), typ, nam, lob, upb)
 Else
  XPRBnewvar = xbnewvar(prob, typ, nam, lob, upb)
 End If
End Function

'*** Constraint definition ***
Function XPRBnewctr(Optional ByVal prob As LongPtr = 0, Optional ByVal nam As String = "", Optional ByVal typ As Long = XPRB_N) As LongPtr
 If prob = 0 Then
  XPRBnewctr = xbnewctr(XPRBgetprob(), nam, typ)
 Else
  XPRBnewctr = xbnewctr(prob, nam, typ)
 End If
End Function

Function XPRBaddterm(ByVal lct As LongPtr, ByVal v As LongPtr, Optional ByVal coeff As Double = 1) As Long
 XPRBaddterm = xbaddterm(lct, v, coeff)
End Function

'*** SOS definition ***
Function XPRBnewsos(Optional ByVal prob As LongPtr = 0, Optional ByVal nam As String = "", Optional ByVal typ As Long = XPRB_S1) As LongPtr
 If prob = 0 Then
  XPRBnewsos = xbnewsos(XPRBgetprob(), nam, typ)
 Else
  XPRBnewsos = xbnewsos(prob, nam, typ)
 End If
End Function

'*** Objective function ***
Function XPRBsetsense(Optional ByVal prob As LongPtr = 0, Optional ByVal d As Long = XPRB_MINIM) As Long
 If prob = 0 Then
  XPRBsetsense = xbsetsense(XPRBgetprob(), d)
 Else
  XPRBsetsense = xbsetsense(prob, d)
 End If
End Function

Function XPRBgetsense(Optional ByVal prob As LongPtr = 0) As Long
 If prob = 0 Then
  XPRBgetsense = xbgetsense(XPRBgetprob())
 Else
  XPRBgetsense = xbgetsense(prob)
 End If
End Function

Function XPRBsetobj(Optional ByVal prob As LongPtr = 0, Optional ByVal obj As LongPtr = 0) As Long
 If prob = 0 Then
  XPRBsetobj = xbsetobj(XPRBgetprob(), obj)
 Else
  XPRBsetobj = xbsetobj(prob, obj)
 End If
End Function

Function XPRBaddqterm(ByVal ctr As LongPtr, Optional ByVal v1 As LongPtr = 0, Optional ByVal v2 As LongPtr = 0, Optional ByVal coeff As Double = 1) As Long
        XPRBaddqterm = xbaddqterm(ctr, v1, v2, coeff)
End Function

Function XPRBsetqterm(ByVal ctr As LongPtr, Optional ByVal v1 As LongPtr = 0, Optional ByVal v2 As LongPtr = 0, Optional ByVal coeff As Double = 1) As Long
        XPRBsetqterm = xbsetqterm(ctr, v1, v2, coeff)
End Function


'*** Names ***
Function XPRBgetbyname(Optional ByVal prob As LongPtr = 0, Optional ByVal nam As String = "", Optional ByVal typ As Long = XPRB_VAR) As Long
 If prob = 0 Then
  XPRBgetbyname = xbgetbyname(XPRBgetprob(), nam, typ)
 Else
  XPRBgetbyname = xbgetbyname(prob, nam, typ)
 End If
End Function

Function XPRBsetdictionarysize(Optional ByVal prob As LongPtr = 0, Optional ByVal dico As Long = XPRB_DICT_NAMES, Optional ByVal siz As Long = 0) As Long
 If prob = 0 Then
  XPRBsetdictionarysize = xbsetdictionarysize(XPRBgetprob(), dico, siz)
 Else
  XPRBsetdictionarysize = xbsetdictionarysize(prob, dico, siz)
 End If
End Function

Function XPRBgetidxelname(ByVal idx As LongPtr, ByVal i As Long) As String
 XPRBgetidxelname = XPRBgetString(xbgetidxelname(idx, i - 1))
End Function

Function XPRBgetprobname(Optional ByVal prob As LongPtr = 0) As String
 If prob = 0 Then
  XPRBgetprobname = XPRBgetString(xbgetprobname(XPRBgetprob()))
 Else
  XPRBgetprobname = XPRBgetString(xbgetprobname(prob))
 End If
End Function

Function XPRBgetvarname(ByVal v As LongPtr) As String
 XPRBgetvarname = XPRBgetString(xbgetvarname(v))
End Function

Function XPRBgetctrname(ByVal lct As LongPtr) As String
 XPRBgetctrname = XPRBgetString(xbgetctrname(lct))
End Function

Function XPRBgetsosname(ByVal sos As LongPtr) As String
 XPRBgetsosname = XPRBgetString(xbgetsosname(sos))
End Function

Function XPRBgetidxsetname(ByVal idx As LongPtr) As String
 XPRBgetidxsetname = XPRBgetString(xbgetidxsetname(idx))
End Function

'*** Directives ***
Function XPRBsetvardir(ByVal v As LongPtr, ByVal typ As Long, Optional ByVal cost As Double = 0) As Long
 XPRBsetvardir = xbsetvardir(v, typ, cost)
End Function

Function XPRBsetsosdir(ByVal ls As LongPtr, ByVal typ As Long, Optional ByVal vl As Double = 0) As Long
 XPRBsetsosdir = xbsetsosdir(ls, typ, vl)
End Function

Function XPRBcleardir(Optional ByVal prob As LongPtr = 0) As Long
 If prob = 0 Then
  XPRBcleardir = xbcleardir(XPRBgetprob())
 Else
  XPRBcleardir = xbcleardir(prob)
 End If
End Function

'*** Index sets ***
Function XPRBnewidxset(Optional ByVal prob As LongPtr = 0, Optional ByVal nam As String = "", Optional ByVal maxsize As Long = 0) As LongPtr
 If prob = 0 Then
  XPRBnewidxset = xbnewidxset(XPRBgetprob(), nam, maxsize)
 Else
  XPRBnewidxset = xbnewidxset(prob, nam, maxsize)
 End If
End Function

Function XPRBaddidxel(ByVal idx As LongPtr, ByVal nam As String) As Long
 XPRBaddidxel = xbaddidxel(idx, nam) + 1
End Function

Function XPRBgetidxel(ByVal idx As LongPtr, ByVal nam As String) As Long
 XPRBgetidxel = xbgetidxel(idx, nam) + 1
End Function

'*** Output and printing ***
Function XPRBprintprob(Optional ByVal prob As LongPtr = 0) As Long
 If prob = 0 Then
  XPRBprintprob = xbprintprob(XPRBgetprob())
 Else
  XPRBprintprob = xbprintprob(prob)
 End If
End Function

Function XPRBprintobj(Optional ByVal prob As LongPtr = 0) As Long
 If prob = 0 Then
  XPRBprintobj = xbprintobj(XPRBgetprob())
 Else
  XPRBprintobj = xbprintobj(prob)
 End If
End Function

Function XPRBexportprob(Optional ByVal prob As LongPtr = 0, Optional ByVal typ As Long = XPRB_LP, Optional ByVal filename As String = "") As Long
 If prob = 0 Then
  XPRBexportprob = xbexportprob(XPRBgetprob(), typ, filename)
 Else
  XPRBexportprob = xbexportprob(prob, typ, filename)
 End If
End Function

'*** Basis management ***
Function XPRBsavebasis(Optional ByVal prob As LongPtr = 0) As LongPtr
 If prob = 0 Then
  XPRBsavebasis = xbsavebasis(XPRBgetprob())
 Else
  XPRBsavebasis = xbsavebasis(prob)
 End If
End Function

'*** Problem solving and solution information ***
Function XPRBloadmat(Optional ByVal prob As LongPtr = 0) As Long
 If prob = 0 Then
  XPRBloadmat = xbloadmat(XPRBgetprob())
 Else
  XPRBloadmat = xbloadmat(prob)
 End If
End Function

Function XPRBsolve(Optional ByVal prob As LongPtr = 0, Optional ByVal alg As String = "") As Long
 If prob = 0 Then
  XPRBsolve = xbsolve(XPRBgetprob(), alg)
 Else
  XPRBsolve = xbsolve(prob, alg)
 End If
End Function

Function XPRBminim(Optional ByVal prob As LongPtr = 0, Optional ByVal alg As String = "") As Long
 If prob = 0 Then
  XPRBminim = xbminim(XPRBgetprob(), alg)
 Else
  XPRBminim = xbminim(prob, alg)
 End If
End Function

Function XPRBmaxim(Optional ByVal prob As LongPtr = 0, Optional ByVal alg As String = "") As Long
 If prob = 0 Then
  XPRBmaxim = xbmaxim(XPRBgetprob(), alg)
 Else
  XPRBmaxim = xbmaxim(prob, alg)
 End If
End Function
Function XPRBlpoptimize(Optional ByVal prob As LongPtr = 0, Optional ByVal alg As String = "") As Long
 If prob = 0 Then
  XPRBlpoptimize = xblpoptimize(XPRBgetprob(), alg)
 Else
  XPRBlpoptimize = xblpoptimize(prob, alg)
 End If
End Function

Function XPRBmipoptimize(Optional ByVal prob As LongPtr = 0, Optional ByVal alg As String = "") As Long
 If prob = 0 Then
  XPRBmipoptimize = xbmipoptimize(XPRBgetprob(), alg)
 Else
  XPRBmipoptimize = xbmipoptimize(prob, alg)
 End If
End Function

Function XPRBgetprobstat(Optional ByVal prob As LongPtr = 0) As Long
 If prob = 0 Then
  XPRBgetprobstat = xbgetprobstat(XPRBgetprob())
 Else
  XPRBgetprobstat = xbgetprobstat(prob)
 End If
End Function

Function XPRBgetlpstat(Optional ByVal prob As LongPtr = 0) As Long
 If prob = 0 Then
  XPRBgetlpstat = xbgetlpstat(XPRBgetprob())
 Else
  XPRBgetlpstat = xbgetlpstat(prob)
 End If
End Function

Function XPRBgetmipstat(Optional ByVal prob As LongPtr = 0) As Long
 If prob = 0 Then
  XPRBgetmipstat = xbgetmipstat(XPRBgetprob())
 Else
  XPRBgetmipstat = xbgetmipstat(prob)
 End If
End Function

Function XPRBsync(Optional ByVal prob As LongPtr = 0, Optional ByVal synctype As Long = 1) As Long
 If prob = 0 Then
  XPRBsync = xbsync(XPRBgetprob(), synctype)
 Else
  XPRBsync = xbsync(prob, synctype)
 End If
End Function

Function XPRBbegincb(ByVal prob As LongPtr, ByVal optprob As LongPtr) As Long
 If prob = XPRB_NULL Then prob = XPRBgetprob()
 XPRBbegincb = xbbegincb(prob, optprob)
End Function

Function XPRBendcb(Optional ByVal prob As LongPtr = XPRB_NULL) As Long
 If prob = XPRB_NULL Then prob = XPRBgetprob()
 XPRBendcb = xbendcb(prob)
End Function

Function XPRBgetobjval(Optional ByVal prob As LongPtr = 0) As Double
 If prob = 0 Then
  XPRBgetobjval = xbgetobjval(XPRBgetprob())
 Else
  XPRBgetobjval = xbgetobjval(prob)
 End If
End Function

Function XPRBdefcbmsg(Optional ByVal prob As LongPtr = XPRB_NULL, Optional ByVal fn As LongPtr = XPRB_NULL, Optional ByVal context As Variant = Empty) As Long
 If prob = XPRB_NULL Then prob = XPRBgetprob()
 If fn Then
  xvbsetcbcontext CBCON_MSG, prob, context
  XPRBdefcbmsg = xbdefcbmsg(prob, AddressOf xcb_msg, fn)
 Else
  xvbdelcbcontext CBCON_MSG, prob
  XPRBdefcbmsg = xbdefcbmsg(prob, XPRB_NULL, XPRB_NULL)
 End If
End Function

Private Sub xcb_msg(ByVal prob As LongPtr, ByVal fn As LongPtr, ByVal msgptr As LongPtr)
 xcb_msg_v prob, fn, msgptr
End Sub

Function XPRBdefcberr(Optional ByVal prob As LongPtr = XPRB_NULL, Optional ByVal fn As LongPtr = XPRB_NULL, Optional ByVal context As Variant = Empty) As Long
 If prob = XPRB_NULL Then prob = XPRBgetprob()
 If fn Then
  xvbsetcbcontext CBCON_err, prob, context
  XPRBdefcberr = xbdefcberr(prob, AddressOf xcb_err, fn)
 Else
  xvbdelcbcontext CBCON_err, prob
  XPRBdefcberr = xbdefcberr(prob, XPRB_NULL, XPRB_NULL)
 End If
End Function

Private Sub xcb_err(ByVal prob As Long, ByVal fn As Long, ByVal errnum As Long, ByVal errtype As Long, ByVal msgptr As Long)
 xcb_err_v prob, fn, errnum, errtype, msgptr
End Sub

Function XPRBnewsol(Optional ByVal prob As LongPtr = 0) As LongPtr
 If prob = 0 Then
  XPRBnewsol = xbnewsol(XPRBgetprob())
 Else
  XPRBnewsol = xbnewsol(prob)
 End If
End Function

Function XPRBgetsolvar(ByVal solRef As LongPtr, ByVal varRef As LongPtr) As Double
 Dim val As Double
 xbgetsolvar solRef, varRef, val
 XPRBgetsolvar = val
End Function

Function XPRBwritesol(Optional ByVal prob As LongPtr = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBwritesol = xbwritesol(prob, filename, flags)
End Function

Function XPRBwritebinsol(Optional ByVal prob As LongPtr = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBwritebinsol = xbwritebinsol(prob, filename, flags)
End Function

Function XPRBwriteslxsol(Optional ByVal prob As LongPtr = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBwriteslxsol = xbwriteslxsol(prob, filename, flags)
End Function

Function XPRBwriteprtsol(Optional ByVal prob As LongPtr = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBwriteprtsol = xbwriteprtsol(prob, filename, flags)
End Function

Function XPRBreadbinsol(Optional ByVal prob As LongPtr = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBreadbinsol = xbreadbinsol(prob, filename, flags)
End Function

Function XPRBreadslxsol(Optional ByVal prob As LongPtr = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBreadslxsol = xbreadslxsol(prob, filename, flags)
End Function



#Else

'*** Redirection of IO ***
Declare Function XPRBsetStream Lib "xprvb" Alias "_xvbsetstream@8" (ByVal what As Long, ByVal fname As String) As Long
Private Declare Function xvboutput Lib "xprvb" Alias "_xvboutput@8" (ByVal what As Long, ByVal text As String) As Long
Declare Function XPRBflush Lib "xprvb" Alias "_xvbflush@4" (ByVal what As Long) As Long
Declare Function XPRBclose Lib "xprvb" Alias "_xvbclose@4" (ByVal what As Long) As Long

'*** Init/Finish ***
Private Declare Function xbslinit Lib "xprb" Alias "_xbslinit@0" () As Long
Private Declare Function xbgetversion Lib "xprb" Alias "_xbgetversion@0" () As Long
Private Declare Function xbslfinish Lib "xprb" Alias "_xbslfinish@0" () As Long
Declare Function XPRBgettime Lib "xprb" Alias "_xbgettime@0" () As Long

'*** Error handling ***
Declare Function XPRBseterrctrl Lib "xprb" Alias "_xbseterrctrl@4" (ByVal flag As Long) As Long
Private Declare Function xbdefcberr Lib "xprb" Alias "_xbdefcberr_prob@12" (ByVal prob As Long, ByVal Address As Long, ByVal vp As Long) As Long
Private Declare Sub xvbcallcberr Lib "xprvb" Alias "_xvbcallcberr@24" (ByVal fn As Long, ByRef prob As Variant, ByRef context As Variant, ByRef errnum As Variant, ByRef errtype As Variant, ByRef msg As Variant)
Private Declare Function xvbgeterrlvltext Lib "xprvb" Alias "_xvbgeterrlvltext@8" (ByVal lvl As Long, ByVal folder As String) As Long

'*** Problem definition ***
Private Declare Function xbnewprob Lib "xprb" Alias "_xbnewprob@4" (ByVal nam As String) As Long
Declare Function XPRBgetprob Lib "xprb" Alias "_xbgetprob@0" () As Long
Declare Function XPRBchgprob Lib "xprb" Alias "_xbchgprob@4" (ByVal prob As Long) As Long
Private Declare Function xbdelprob Lib "xprb" Alias "_xbdelprob_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbresetprob Lib "xprb" Alias "_xbresetprob_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbgetXPRSprob Lib "xprb" Alias "_xbgetsprob_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbsetprobname Lib "xprb" Alias "_xbsetprobname_prob@8" (ByVal prob As Long, ByVal name As String) As Long

'*** Output and printing ***
Private Declare Function xbdefcbmsg Lib "xprb" Alias "_xbdefcbmsg_prob@12" (ByVal prob As Long, ByVal fn As Long, ByVal context As Long) As Long
Private Declare Sub xvbcallcbmsg Lib "xprvb" Alias "_xvbcallcbmsg@16" (ByVal fn As Long, ByRef prob As Variant, ByRef context As Variant, ByRef msg As Variant)
Private Declare Function xbsetmsglevel Lib "xprb" Alias "_xbsetmsglevel_prob@8" (ByVal prob As Long, ByVal level As Long) As Long   ' default: 2
Private Declare Function xbsetrealfmt Lib "xprb" Alias "_xbsetrealfmt_prob@8" (ByVal prob As Long, ByVal fmt As String) As Long
Private Declare Function xbsetcolorder Lib "xprb" Alias "_xbsetcolorder_prob@8" (ByVal prob As Long, ByVal num As Long) As Long   ' default: 0
Private Declare Function xbfixglobals Lib "xprb" Alias "_xbfixglobals_prob@8" (ByVal prob As Long, ByVal ifFound As Long) As Long

'*** Variable definition ***
Private Declare Function xbnewvar Lib "xprb" Alias "_xbnewvar_prob@28" (ByVal prob As Long, ByVal typ As Long, ByVal nam As String, ByVal lob As Double, ByVal upb As Double) As Long
Declare Function XPRBsetvartype Lib "xprb" Alias "_xbsetvartype@8" (ByVal v As Long, ByVal typ As Long) As Long

'*** Constraint definition ***
Private Declare Function xbnewctr Lib "xprb" Alias "_xbnewctr_prob@12" (ByVal prob As Long, ByVal nam As String, ByVal qrtype As Long) As Long
Declare Function XPRBdelctr Lib "xprb" Alias "_xbdelctr@4" (ByVal lct As Long) As Long
Private Declare Function xbaddexpr_term Lib "xprb" Alias "_xbaddexpr_term@20" (ByVal lct As Long, ByVal v As Long, ByVal v2 As Long, ByVal coeff As Double) As Long
Declare Function XPRBdelterm Lib "xprb" Alias "_xbdelterm@8" (ByVal lct As Long, ByVal v As Long) As Long
Declare Function XPRBsetterm Lib "xprb" Alias "_xbsetterm@16" (ByVal lct As Long, ByVal v As Long, ByVal coeff As Double) As Long
Declare Function XPRBnewprec Lib "xprb" Alias "_xbnewprec_prob@24" (ByVal prob As Long, ByVal nam As String, ByVal v1 As Long, ByVal coeff As Double, ByVal v2 As Long) As Long
Declare Function XPRBsetctrtype Lib "xprb" Alias "_xbsetctrtype@8" (ByVal lct As Long, ByVal qrtype As Long) As Long
Declare Function XPRBsetrange Lib "xprb" Alias "_xbsetrange@20" (ByVal lct As Long, ByVal low As Double, ByVal up As Double) As Long
Declare Function XPRBsetmodcut Lib "xprb" Alias "_xbsetmodcut@8" (ByVal lct As Long, ByVal mcstat As Long) As Long
Private Declare Function xbaddqterm Lib "xprb" Alias "_xbaddqterm@20" (ByVal lct As Long, ByVal v1 As Long, ByVal v2 As Long, ByVal coeff As Double) As Long
Private Declare Function xbsetqterm Lib "xprb" Alias "_xbsetqterm@20" (ByVal lct As Long, ByVal v1 As Long, ByVal v2 As Long, ByVal coeff As Double) As Long
Private Declare Function xbaddterm Lib "xprb" Alias "_xbaddterm@16" (ByVal lct As Long, ByVal v1 As Long, ByVal coeff As Double) As Long
Private Declare Function xbsetterm Lib "xprb" Alias "_xbsetterm@16" (ByVal lct As Long, ByVal v1 As Long, ByVal coeff As Double) As Long
Private Declare Function xbsetexpr_term Lib "xprb" Alias "_xbsetexpr_term@20" (ByVal lct As Long, ByVal v1 As Long, ByVal v2 As Long, ByVal coeff As Double) As Long

'*** SOS definition ***
Private Declare Function xbnewsos Lib "xprb" Alias "_xbnewsos_prob@12" (ByVal prob As Long, ByVal nam As String, ByVal typ As Long) As Long
Declare Function XPRBdelsos Lib "xprb" Alias "_xbdelsos@4" (ByVal sos As Long) As Long
Declare Function XPRBaddsosel Lib "xprb" Alias "_xbaddsosel@16" (ByVal sos As Long, ByVal v As Long, ByVal ref As Double) As Long
Declare Function XPRBdelsosel Lib "xprb" Alias "_xbdelsosel@8" (ByVal sos As Long, ByVal v As Long) As Long

'*** Objective function ***
Private Declare Function xbsetsense Lib "xprb" Alias "_xbsetsense_prob@8" (ByVal prob As Long, ByVal d As Long) As Long    ' default: minimise
Private Declare Function xbgetsense Lib "xprb" Alias "_xbgetsense_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbsetobj Lib "xprb" Alias "_xbsetobj_prob@8" (ByVal prob As Long, ByVal lct As Long) As Long

'*** Variable bounds ***
Declare Function XPRBsetub Lib "xprb" Alias "_xbsetub@12" (ByVal v As Long, ByVal c As Double) As Long
Declare Function XPRBsetlb Lib "xprb" Alias "_xbsetlb@12" (ByVal v As Long, ByVal c As Double) As Long
Declare Function XPRBfixvar Lib "xprb" Alias "_xbfixvar@12" (ByVal v As Long, ByVal c As Double) As Long
Declare Function XPRBsetlim Lib "xprb" Alias "_xbsetlim@12" (ByVal v As Long, ByVal c As Double) As Long

'*** Names ***
Private Declare Function xbgetbyname Lib "xprb" Alias "_xbgetbyname_prob@12" (ByVal prob As Long, ByVal nam As String, ByVal typ As Long) As Long
Private Declare Function xbsetdictionarysize Lib "xprb" Alias "_xbsetdictsize_prob@12" (ByVal prob As Long, ByVal dico As Long, ByVal siz As Long) As Long
Private Declare Function xbgetidxelname Lib "xprb" Alias "_xbgetidxelname@8" (ByVal idx As Long, ByVal i As Long) As Long
Private Declare Function xbgetprobname Lib "xprb" Alias "_xbgetprobname_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbgetvarname Lib "xprb" Alias "_xbgetvarname@4" (ByVal v As Long) As Long
Private Declare Function xbgetctrname Lib "xprb" Alias "_xbgetctrname@4" (ByVal lct As Long) As Long
Private Declare Function xbgetsosname Lib "xprb" Alias "_xbgetsosname@4" (ByVal sos As Long) As Long
Private Declare Function xbgetidxsetname Lib "xprb" Alias "_xbgetidxsetname@4" (ByVal idx As Long) As Long

'*** Directives ***
Private Declare Function xbsetvardir Lib "xprb" Alias "_xbsetvardir@16" (ByVal v As Long, ByVal typ As Long, ByVal cost As Double) As Long
Private Declare Function xbsetsosdir Lib "xprb" Alias "_xbsetsosdir@16" (ByVal ls As Long, ByVal typ As Long, ByVal vl As Double) As Long
Private Declare Function xbcleardir Lib "xprb" Alias "_xbcleardir_prob@4" (ByVal prob As Long) As Long

'*** Index sets ***
Private Declare Function xbnewidxset Lib "xprb" Alias "_xbnewidxset_prob@12" (ByVal prob As Long, ByVal nam As String, ByVal maxsize As Long) As Long
Private Declare Function xbaddidxel Lib "xprb" Alias "_xbaddidxel@8" (ByVal idx As Long, ByVal nam As String) As Long
Private Declare Function xbgetidxel Lib "xprb" Alias "_xbgetidxel@8" (ByVal idx As Long, ByVal nam As String) As Long
Declare Function XPRBgetidxsetsize Lib "xprb" Alias "_xbgetidxsetsize@4" (ByVal idx As Long) As Long

'*** Output and printing ***
Private Declare Function xbprintprob Lib "xprb" Alias "_xbprintprob_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbprintobj Lib "xprb" Alias "_xbprintobj_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbexportprob Lib "xprb" Alias "_xbexportprob_prob@12" (ByVal prob As Long, ByVal typ As Long, ByVal filename As String) As Long

Declare Function XPRBprintvar Lib "xprb" Alias "_xbprintvar@4" (ByVal v As Long) As Long
Declare Function XPRBprintctr Lib "xprb" Alias "_xbprintctr@4" (ByVal lct As Long) As Long
Declare Function XPRBprintsos Lib "xprb" Alias "_xbprintsos@4" (ByVal ls As Long) As Long
Declare Function XPRBprintidxset Lib "xprb" Alias "_xbprintidxset@4" (ByVal idx As Long) As Long
Declare Function XPRBprintexpr Lib "xprb" Alias "_xbprintexpr@20" (ByVal prob As Long, ByVal tls As Long, ByVal nl As Long, ByVal dbl As Double) As Long

'*** Information on variables and constraints ***
Declare Function XPRBgetcolnum Lib "xprb" Alias "_xbgetcolnum@4" (ByVal v As Long) As Long
Declare Function XPRBgetrownum Lib "xprb" Alias "_xbgetrownum@4" (ByVal lct As Long) As Long
Declare Function XPRBgetvartype Lib "xprb" Alias "_xbgetvartype@4" (ByVal v As Long) As Long
Declare Function XPRBgetctrtype Lib "xprb" Alias "_xbgetctrtype@4" (ByVal lct As Long) As Long
Declare Function XPRBgetctrsize Lib "xprb" Alias "_xbgetctrsize@4" (ByVal lct As Long) As Long
Declare Function XPRBgetsostype Lib "xprb" Alias "_xbgetsostype@4" (ByVal sos As Long) As Long
Declare Function XPRBgetrhs Lib "xprb" Alias "_xbgetrhs@4" (ByVal lct As Long) As Double
Declare Function XPRBgetmodcut Lib "xprb" Alias "_xbgetmodcut@4" (ByVal lct As Long) As Long
Declare Function XPRBgetbounds Lib "xprb" Alias "_xbgetbounds@12" (ByVal v As Long, ByRef lbd As Double, ByRef ubd As Double) As Long
Declare Function XPRBgetlim Lib "xprb" Alias "_xbgetlim@8" (ByVal v As Long, lim As Double) As Long
Declare Function XPRBgetrange Lib "xprb" Alias "_xbgetrange@12" (ByVal lct As Long, ByRef low As Double, ByRef up As Double) As Long

'*** Basis management ***
Private Declare Function xbsavebasis Lib "xprb" Alias "_xbsavebasis_prob@4" (ByVal prob As Long) As Long
Declare Function XPRBloadbasis Lib "xprb" Alias "_xbloadbasis@4" (ByVal basis As Long) As Long
Declare Sub XPRBdelbasis Lib "xprb" Alias "_xbdelbasis@4" (ByVal basis As Long)

'*** Problem solving and solution information ***
Private Declare Function xbloadmat Lib "xprb" Alias "_xbloadmat_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbsolve Lib "xprb" Alias "_xbsolve_prob@8" (ByVal prob As Long, ByVal alg As String) As Long
Private Declare Function xbminim Lib "xprb" Alias "_xbminim_prob@8" (ByVal prob As Long, ByVal alg As String) As Long
Private Declare Function xbmaxim Lib "xprb" Alias "_xbmaxim_prob@8" (ByVal prob As Long, ByVal alg As String) As Long
Private Declare Function xblpoptimize Lib "xprb" Alias "_xblpoptimize_prob@8" (ByVal prob As Long, ByVal alg As String) As Long
Private Declare Function xbmipoptimize Lib "xprb" Alias "_xbmipoptimize_prob@8" (ByVal prob As Long, ByVal alg As String) As Long
Private Declare Function xbgetprobstat Lib "xprb" Alias "_xbgetprobstat_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbgetlpstat Lib "xprb" Alias "_xbgetlpstat_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbgetmipstat Lib "xprb" Alias "_xbgetmipstat_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbsync Lib "xprb" Alias "_xbsync_prob@8" (ByVal prob As Long, ByVal synctype As Long) As Long
Private Declare Function xbbegincb Lib "xprb" Alias "_xbbegincb_prob@8" (ByVal prob As Long, ByVal optprob As Long) As Long
Private Declare Function xbendcb Lib "xprb" Alias "_xbendcb_prob@4" (ByVal prob As Long) As Long
Private Declare Function xbgetobjval Lib "xprb" Alias "_xbgetobjval_prob@4" (ByVal prob As Long) As Double
Declare Function XPRBgetsol Lib "xprb" Alias "_xbgetsol@4" (ByVal v As Long) As Double
Declare Function XPRBgetrcost Lib "xprb" Alias "_xbgetrcost@4" (ByVal v As Long) As Double
Declare Function XPRBgetslack Lib "xprb" Alias "_xbgetslack@4" (ByVal lct As Long) As Double
Declare Function XPRBgetact Lib "xprb" Alias "_xbgetact@4" (ByVal lct As Long) As Double
Declare Function XPRBgetdual Lib "xprb" Alias "_xbgetdual@4" (ByVal lct As Long) As Double
Declare Function XPRBgetctrrng Lib "xprb" Alias "_xbgetctrrng@8" (ByVal lct As Long, ByVal rngtype As Long) As Double
Declare Function XPRBgetvarrng Lib "xprb" Alias "_xbgetvarrng@8" (ByVal v As Long, ByVal rngtype As Long) As Double

'*** Indicator constraints ***
Declare Function XPRBsetdelayed Lib "xprb" Alias "_xbsetdelayed@8" (ByVal ctrRef As Long, ByVal delayed As Long) As Long
Declare Function XPRBgetdelayed Lib "xprb" Alias "_xbgetdelayed@4" (ByVal ctrRef As Long) As Long
Declare Function XPRBsetindicator Lib "xprb" Alias "_xbsetindicator@12" (ByVal ctrRef As Long, ByVal dir As Long, ByVal varRef As Long)
Declare Function XPRBgetindicator Lib "xprb" Alias "_xbgetindicator@4" (ByVal ctrRef As Long) As Long
Declare Function XPRBgetindvar Lib "xprb" Alias "_xbgetindvar@4" (ByVal ctrRef As Long) As Long
Declare Function XPRBgetincvars Lib "xprb" Alias "_xbgetincvars@4" (ByVal ctrRef As Long) As Long
Declare Function XPRBsetincvars Lib "xprb" Alias "_xbsetincvars@8" (ByVal ctrRef As Long, ByVal ivstat As Long) As Long

'*** MIP solutions ***
Declare Function XPRBloadmipsol Lib "xprb" Alias "_xbloadmipsol_prob@16" (ByVal probRef As Long, ByRef sol As Double, ByVal ncol As Long, ByVal ifopt As Long) As Long
Declare Function XPRBaddmipsol Lib "xprb" Alias "_xbaddmipsol_prob@12" (ByVal probRef As Long, ByVal solRef As Long, ByVal name As String) As Long

'*** Defining solutions
Private Declare Function xbnewsol Lib "xprb" Alias "_xbnewsol_prob@4" (ByVal probRef As Long) As Long
Declare Function XPRBdelsol Lib "xprb" Alias "_xbdelsol@4" (ByVal solRef As Long) As Long
Declare Function XPRBsetsolvar Lib "xprb" Alias "_xbsetsolvar@16" (ByVal solRef As Long, ByVal varRef As Long, ByVal val As Double) As Long
Declare Function XPRBdelsolvar Lib "xprb" Alias "_xbdelsolvar@8" (ByVal solRef As Long, ByVal varRef As Long) As Long
Declare Function xbgetsolvar Lib "xprb" Alias "_xbgetsolvar@12" (ByVal solRef As Long, ByVal varRef As Long, ByRef val As Double) As Long
Declare Function XPRBgetsolsize Lib "xprb" Alias "_xbgetsolsize@4" (ByVal solRef As Long) As Long
Declare Function XPRBprintsol Lib "xprb" Alias "_xbprintsol@4" (ByVal solRef As Long) As Long

'*** Managing solutions
Private Declare Function xbwritesol Lib "xprb" Alias "_xbwritesol_prob@12" (ByVal probRef As Long, ByVal filename As String, ByVal flags As String) As Long
Private Declare Function xbwritebinsol Lib "xprb" Alias "_xbwritebinsol_prob@12" (ByVal probRef As Long, ByVal filename As String, ByVal flags As String) As Long
Private Declare Function xbwriteslxsol Lib "xprb" Alias "_xbwriteslxsol_prob@12" (ByVal probRef As Long, ByVal filename As String, ByVal flags As String) As Long
Private Declare Function xbwriteprtsol Lib "xprb" Alias "_xbwriteprtsol_prob@12" (ByVal probRef As Long, ByVal filename As String, ByVal flags As String) As Long
Private Declare Function xbreadbinsol Lib "xprb" Alias "_xbreadbinsol_prob@12" (ByVal probRef As Long, ByVal filename As String, ByVal flags As String) As Long
Private Declare Function xbreadslxsol Lib "xprb" Alias "_xbreadslxsol_prob@12" (ByVal probRef As Long, ByVal filename As String, ByVal flags As String) As Long

'*** String conversion ***
Private Declare Sub XPRadr2str Lib "xprvb" Alias "_xvbadr2str@12" (ByVal adrstr As Long, ByVal rts As String, ByVal rtslen As Long)
Private Declare Function XPRadrstrlen Lib "xprvb" Alias "_xvbadrstrlen@4" (ByVal adrstr As Long) As Long

'*****************************************************************

Function XPRBgetString(ByRef adrstr As Long) As String
 Dim rts As String
 
 If (adrstr = 0) Then
  XPRBgetString = ""
 Else
  Dim slen As Long
  slen = XPRadrstrlen(adrstr)
  rts = String$(slen + 1, vbNullChar)
  XPRadr2str adrstr, rts, Len(rts)
  XPRBgetString = Left(rts, InStr(1, rts, vbNullChar) - 1)
 End If
End Function

'*** Init/Finish ***
Function XPRBinit(Optional ByRef d As String = vbNullString) As Long
 If initCount <= 0 Then
  XPRBseterrctrl 0
  If Not xvbhascbcontext(CBCON_MSG, XPRB_NULL) Then XPRBdefcbmsg XPRB_NULL, AddressOf XPRBcbmsgdefault
  If Not xvbhascbcontext(CBCON_err, XPRB_NULL) Then XPRBdefcberr XPRB_NULL, AddressOf XPRBcberrdefault
 End If
 
 Dim result As Long
 result = xbslinit()
 If result = 0 Then
   If initCount < 0 Then initCount = 0
   initCount = initCount + 1
 End If
 XPRBinit = result
End Function

Function XPRBfinish() As Long
 XPRBfinish = xbslfinish()
 
 If initCount <= 1 Then
  On Error Resume Next
  initCount = initCount - 1
  If initCount < 0 Then initCount = 0
  Dim context As Variant
  For Each context In cbcontexts
    context.Clear
  Next
 End If
End Function

Function XPRBfree() As Long
 XPRBfree = XPRBfinish()
End Function

Function XPRBgetversion() As String
 XPRBgetversion = XPRBgetString(xbgetversion())
End Function

'*** Problem creation ***
Function XPRBnewprob(Optional ByVal nam As String = vbNullString) As Long
 If initCount <= 0 Then
  If XPRBinit Then
   XPRBnewprob = XPRB_NULL
   Exit Function
  End If
 End If
 XPRBnewprob = xbnewprob(nam)
End Function

'*** Problem deletion***
Function XPRBdelprob(Optional ByVal prob As Long = XPRB_NULL) As Long
 If prob = XPRB_NULL Then prob = XPRBgetprob()
 XPRBdelprob = xbdelprob(prob)
 
 On Error Resume Next
 Dim coll As Variant
 For Each coll In cbcontexts
  coll.Remove Hex(prob)
 Next
 On Error GoTo 0
End Function

Function XPRBsetprobname(Optional ByVal prob As Long = 0, Optional ByVal name As String = vbNullString) As Long
  If prob = 0 Then
    XPRBsetprobname = xbsetprobname(XPRBgetprob(), name)
  Else
    XPRBsetprobname = xbsetprobname(prob, name)
  End If
End Function

Function XPRBgetXPRSprob(Optional ByVal prob As Long = 0) As Long
 If prob = 0 Then
  XPRBgetXPRSprob = xbgetXPRSprob(XPRBgetprob())
 Else
  XPRBgetXPRSprob = xbgetXPRSprob(prob)
 End If
End Function

Function XPRBsetmsglevel(Optional ByVal prob As Long = 0, Optional ByVal level As Long = 2) As Long
 If prob = 0 Then
  XPRBsetmsglevel = xbsetmsglevel(XPRBgetprob(), level)
 Else
  XPRBsetmsglevel = xbsetmsglevel(prob, level)
 End If
End Function

Function XPRBsetrealfmt(Optional ByVal prob As Long = 0, Optional ByVal fmt As String) As Long
 If prob = 0 Then
  XPRBsetrealfmt = xbsetrealfmt(XPRBgetprob(), fmt)
 Else
  XPRBsetrealfmt = xbsetrealfmt(prob, fmt)
 End If
End Function

Function XPRBsetcolorder(Optional ByVal prob As Long = 0, Optional ByVal num As Long = 0) As Long
 If prob = 0 Then
  XPRBsetcolorder = xbsetcolorder(XPRBgetprob(), num)
 Else
  XPRBsetcolorder = xbsetcolorder(prob, num)
 End If
End Function

Function XPRBfixglobals(Optional ByVal prob As Long = 0, Optional ByVal ifRound As Long = 0) As Long
 If prob = 0 Then
  XPRBfixglobals = xbfixglobals(XPRBgetprob(), ifRound)
 Else
  XPRBfixglobals = xbfixglobals(prob, ifRound)
 End If
End Function

'*** Variable definition ***
Function XPRBnewvar(Optional ByVal prob As Long = 0, _
        Optional ByVal typ As Long = XPRB_PL, Optional ByVal nam As String = "", _
        Optional ByVal lob As Double = 0, _
        Optional ByVal upb As Double = XPRB_INFINITY) As Long
 If prob = 0 Then
  XPRBnewvar = xbnewvar(XPRBgetprob(), typ, nam, lob, upb)
 Else
  XPRBnewvar = xbnewvar(prob, typ, nam, lob, upb)
 End If
End Function

'*** Constraint definition ***
Function XPRBnewctr(Optional ByVal prob As Long = 0, Optional ByVal nam As String = "", Optional ByVal typ As Long = XPRB_N) As Long
 If prob = 0 Then
  XPRBnewctr = xbnewctr(XPRBgetprob(), nam, typ)
 Else
  XPRBnewctr = xbnewctr(prob, nam, typ)
 End If
End Function

Function XPRBaddterm(ByVal lct As Long, ByVal v As Long, Optional ByVal coeff As Double = 1) As Long
 XPRBaddterm = xbaddterm(lct, v, coeff)
End Function

'*** SOS definition ***
Function XPRBnewsos(Optional ByVal prob As Long = 0, Optional ByVal nam As String = "", Optional ByVal typ As Long = XPRB_S1) As Long
 If prob = 0 Then
  XPRBnewsos = xbnewsos(XPRBgetprob(), nam, typ)
 Else
  XPRBnewsos = xbnewsos(prob, nam, typ)
 End If
End Function

'*** Objective function ***
Function XPRBsetsense(Optional ByVal prob As Long = 0, Optional ByVal d As Long = XPRB_MINIM) As Long
 If prob = 0 Then
  XPRBsetsense = xbsetsense(XPRBgetprob(), d)
 Else
  XPRBsetsense = xbsetsense(prob, d)
 End If
End Function

Function XPRBgetsense(Optional ByVal prob As Long = 0) As Long
 If prob = 0 Then
  XPRBgetsense = xbgetsense(XPRBgetprob())
 Else
  XPRBgetsense = xbgetsense(prob)
 End If
End Function

Function XPRBsetobj(Optional ByVal prob As Long = 0, Optional ByVal obj As Long = 0) As Long
 If prob = 0 Then
  XPRBsetobj = xbsetobj(XPRBgetprob(), obj)
 Else
  XPRBsetobj = xbsetobj(prob, obj)
 End If
End Function

Function XPRBaddqterm(ByVal ctr As Long, Optional ByVal v1 As Long = 0, Optional ByVal v2 As Long = 0, Optional ByVal coeff As Double = 1) As Long
        XPRBaddqterm = xbaddqterm(ctr, v1, v2, coeff)
End Function

Function XPRBsetqterm(ByVal ctr As Long, Optional ByVal v1 As Long = 0, Optional ByVal v2 As Long = 0, Optional ByVal coeff As Double = 1) As Long
        XPRBsetqterm = xbsetqterm(ctr, v1, v2, coeff)
End Function


'*** Names ***
Function XPRBgetbyname(Optional ByVal prob As Long = 0, Optional ByVal nam As String = "", Optional ByVal typ As Long = XPRB_VAR) As Long
 If prob = 0 Then
  XPRBgetbyname = xbgetbyname(XPRBgetprob(), nam, typ)
 Else
  XPRBgetbyname = xbgetbyname(prob, nam, typ)
 End If
End Function

Function XPRBsetdictionarysize(Optional ByVal prob As Long = 0, Optional ByVal dico As Long = XPRB_DICT_NAMES, Optional ByVal siz As Long = 0) As Long
 If prob = 0 Then
  XPRBsetdictionarysize = xbsetdictionarysize(XPRBgetprob(), dico, siz)
 Else
  XPRBsetdictionarysize = xbsetdictionarysize(prob, dico, siz)
 End If
End Function

Function XPRBgetidxelname(ByVal idx As Long, ByVal i As Long) As String
 XPRBgetidxelname = XPRBgetString(xbgetidxelname(idx, i - 1))
End Function

Function XPRBgetprobname(Optional ByVal prob As Long = 0) As String
 If prob = 0 Then
  XPRBgetprobname = XPRBgetString(xbgetprobname(XPRBgetprob()))
 Else
  XPRBgetprobname = XPRBgetString(xbgetprobname(prob))
 End If
End Function

Function XPRBgetvarname(ByVal v As Long) As String
 XPRBgetvarname = XPRBgetString(xbgetvarname(v))
End Function

Function XPRBgetctrname(ByVal lct As Long) As String
 XPRBgetctrname = XPRBgetString(xbgetctrname(lct))
End Function

Function XPRBgetsosname(ByVal sos As Long) As String
 XPRBgetsosname = XPRBgetString(xbgetsosname(sos))
End Function

Function XPRBgetidxsetname(ByVal idx As Long) As String
 XPRBgetidxsetname = XPRBgetString(xbgetidxsetname(idx))
End Function

'*** Directives ***
Function XPRBsetvardir(ByVal v As Long, ByVal typ As Long, Optional ByVal cost As Double = 0) As Long
 XPRBsetvardir = xbsetvardir(v, typ, cost)
End Function

Function XPRBsetsosdir(ByVal ls As Long, ByVal typ As Long, Optional ByVal vl As Double = 0) As Long
 XPRBsetsosdir = xbsetsosdir(ls, typ, vl)
End Function

Function XPRBcleardir(Optional ByVal prob As Long = 0) As Long
 If prob = 0 Then
  XPRBcleardir = xbcleardir(XPRBgetprob())
 Else
  XPRBcleardir = xbcleardir(prob)
 End If
End Function

'*** Index sets ***
Function XPRBnewidxset(Optional ByVal prob As Long = 0, Optional ByVal nam As String = "", Optional ByVal maxsize As Long = 0) As Long
 If prob = 0 Then
  XPRBnewidxset = xbnewidxset(XPRBgetprob(), nam, maxsize)
 Else
  XPRBnewidxset = xbnewidxset(prob, nam, maxsize)
 End If
End Function

Function XPRBaddidxel(ByVal idx As Long, ByVal nam As String) As Long
 XPRBaddidxel = xbaddidxel(idx, nam) + 1
End Function

Function XPRBgetidxel(ByVal idx As Long, ByVal nam As String) As Long
 XPRBgetidxel = xbgetidxel(idx, nam) + 1
End Function

'*** Output and printing ***
Function XPRBprintprob(Optional ByVal prob As Long = 0) As Long
 If prob = 0 Then
  XPRBprintprob = xbprintprob(XPRBgetprob())
 Else
  XPRBprintprob = xbprintprob(prob)
 End If
End Function

Function XPRBprintobj(Optional ByVal prob As Long = 0) As Long
 If prob = 0 Then
  XPRBprintobj = xbprintobj(XPRBgetprob())
 Else
  XPRBprintobj = xbprintobj(prob)
 End If
End Function

Function XPRBexportprob(Optional ByVal prob As Long = 0, Optional ByVal typ As Long = XPRB_LP, Optional ByVal filename As String = "") As Long
 If prob = 0 Then
  XPRBexportprob = xbexportprob(XPRBgetprob(), typ, filename)
 Else
  XPRBexportprob = xbexportprob(prob, typ, filename)
 End If
End Function

'*** Basis management ***
Function XPRBsavebasis(Optional ByVal prob As Long = 0) As Long
 If prob = 0 Then
  XPRBsavebasis = xbsavebasis(XPRBgetprob())
 Else
  XPRBsavebasis = xbsavebasis(prob)
 End If
End Function

'*** Problem solving and solution information ***
Function XPRBloadmat(Optional ByVal prob As Long = 0) As Long
 If prob = 0 Then
  XPRBloadmat = xbloadmat(XPRBgetprob())
 Else
  XPRBloadmat = xbloadmat(prob)
 End If
End Function

Function XPRBsolve(Optional ByVal prob As Long = 0, Optional ByVal alg As String = "") As Long
 If prob = 0 Then
  XPRBsolve = xbsolve(XPRBgetprob(), alg)
 Else
  XPRBsolve = xbsolve(prob, alg)
 End If
End Function

Function XPRBminim(Optional ByVal prob As Long = 0, Optional ByVal alg As String = "") As Long
 If prob = 0 Then
  XPRBminim = xbminim(XPRBgetprob(), alg)
 Else
  XPRBminim = xbminim(prob, alg)
 End If
End Function

Function XPRBmaxim(Optional ByVal prob As Long = 0, Optional ByVal alg As String = "") As Long
 If prob = 0 Then
  XPRBmaxim = xbmaxim(XPRBgetprob(), alg)
 Else
  XPRBmaxim = xbmaxim(prob, alg)
 End If
End Function

Function XPRBlpoptimize(Optional ByVal prob As Long = 0, Optional ByVal alg As String = "") As Long
 If prob = 0 Then
  XPRBlpoptimize = xblpoptimize(XPRBgetprob(), alg)
 Else
  XPRBlpoptimize = xblpoptimize(prob, alg)
 End If
End Function

Function XPRBmipoptimize(Optional ByVal prob As Long = 0, Optional ByVal alg As String = "") As Long
 If prob = 0 Then
  XPRBmipoptimize = xbmipoptimize(XPRBgetprob(), alg)
 Else
  XPRBmipoptimize = xbmipoptimize(prob, alg)
 End If
End Function

Function XPRBgetprobstat(Optional ByVal prob As Long = 0) As Long
 If prob = 0 Then
  XPRBgetprobstat = xbgetprobstat(XPRBgetprob())
 Else
  XPRBgetprobstat = xbgetprobstat(prob)
 End If
End Function

Function XPRBgetlpstat(Optional ByVal prob As Long = 0) As Long
 If prob = 0 Then
  XPRBgetlpstat = xbgetlpstat(XPRBgetprob())
 Else
  XPRBgetlpstat = xbgetlpstat(prob)
 End If
End Function

Function XPRBgetmipstat(Optional ByVal prob As Long = 0) As Long
 If prob = 0 Then
  XPRBgetmipstat = xbgetmipstat(XPRBgetprob())
 Else
  XPRBgetmipstat = xbgetmipstat(prob)
 End If
End Function

Function XPRBsync(Optional ByVal prob As Long = 0, Optional ByVal synctype As Long = 1) As Long
 If prob = 0 Then
  XPRBsync = xbsync(XPRBgetprob(), synctype)
 Else
  XPRBsync = xbsync(prob, synctype)
 End If
End Function

Function XPRBbegincb(ByVal prob As Long, ByVal optprob As Long) As Long
 If prob = XPRB_NULL Then prob = XPRBgetprob()
 XPRBbegincb = xbbegincb(prob, optprob)
End Function

Function XPRBendcb(Optional ByVal prob As Long = XPRB_NULL) As Long
 If prob = XPRB_NULL Then prob = XPRBgetprob()
 XPRBendcb = xbendcb(prob)
End Function

Function XPRBgetobjval(Optional ByVal prob As Long = 0) As Double
 If prob = 0 Then
  XPRBgetobjval = xbgetobjval(XPRBgetprob())
 Else
  XPRBgetobjval = xbgetobjval(prob)
 End If
End Function

Function XPRBdefcbmsg(Optional ByVal prob As Long = XPRB_NULL, Optional ByVal fn As Long = XPRB_NULL, Optional ByVal context As Variant = Empty) As Long
 If fn Then
  xvbsetcbcontext CBCON_MSG, prob, context
  XPRBdefcbmsg = xbdefcbmsg(prob, AddressOf xcb_msg, fn)
 Else
  xvbdelcbcontext CBCON_MSG, prob
  XPRBdefcbmsg = xbdefcbmsg(prob, XPRB_NULL, XPRB_NULL)
 End If
End Function

Private Sub xcb_msg(ByVal prob As Long, ByVal fn As Long, ByVal msgptr As Long)
 xcb_msg_v prob, fn, msgptr
End Sub

Function XPRBdefcberr(Optional ByVal prob As Long = XPRB_NULL, Optional ByVal fn As Long = XPRB_NULL, Optional ByVal context As Variant = Empty) As Long
 If fn Then
  xvbsetcbcontext CBCON_err, prob, context
  XPRBdefcberr = xbdefcberr(prob, AddressOf xcb_err, fn)
 Else
  xvbdelcbcontext CBCON_err, prob
  XPRBdefcberr = xbdefcberr(prob, XPRB_NULL, XPRB_NULL)
 End If
End Function

Private Sub xcb_err(ByVal prob As Long, ByVal fn As Long, ByVal errnum As Long, ByVal errtype As Long, ByVal msgptr As Long)
 xcb_err_v prob, fn, errnum, errtype, msgptr
End Sub


Function XPRBnewsol(Optional ByVal prob As Long = 0) As Long
 If prob = 0 Then
  XPRBnewsol = xbnewsol(XPRBgetprob())
 Else
  XPRBnewsol = xbnewsol(prob)
 End If
End Function

Function XPRBgetsolvar(ByVal solRef As Long, ByVal varRef As Long) As Double
 Dim val As Double
 xbgetsolvar solRef, varRef, val
 XPRBgetsolvar = val
End Function

Function XPRBwritesol(Optional ByVal prob As Long = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBwritesol = xbwritesol(prob, filename, flags)
End Function

Function XPRBwritebinsol(Optional ByVal prob As Long = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBwritebinsol = xbwritebinsol(prob, filename, flags)
End Function

Function XPRBwriteslxsol(Optional ByVal prob As Long = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBwriteslxsol = xbwriteslxsol(prob, filename, flags)
End Function

Function XPRBwriteprtsol(Optional ByVal prob As Long = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBwriteprtsol = xbwriteprtsol(prob, filename, flags)
End Function

Function XPRBreadbinsol(Optional ByVal prob As Long = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBreadbinsol = xbreadbinsol(prob, filename, flags)
End Function

Function XPRBreadslxsol(Optional ByVal prob As Long = 0, Optional ByVal filename As String = "", Optional ByVal flags As String = "")
 If prob = 0 Then
  prob = XPRBgetprob()
 End If
 XPRBreadslxsol = xbreadslxsol(prob, filename, flags)
End Function


#End If

Private Function xvbgetcbcontext(ByVal nctx As CB_Contexts, ByVal prob As Variant) As Variant
 Dim coll As Collection
 On Error Resume Next
 Set coll = cbcontexts(nctx)
 xvbgetcbcontext = coll(Hex(XPRB_NULL))
 xvbgetcbcontext = coll(Hex(prob))
End Function
Private Function xvbhascbcontext(ByVal nctx As CB_Contexts, ByVal prob As Variant) As Boolean
 On Error GoTo NoContext
 Dim coll As Collection
 Set coll = cbcontexts(nctx)
 Call coll(Hex(prob))
 xvbhascbcontext = True
NoContext: ' just fall through to exit
End Function

Private Sub xvbsetcbcontext(ByVal nctx As CB_Contexts, ByVal prob As Variant, ByVal data As Variant)
 On Error Resume Next
 cbcontexts(nctx).Remove Hex(prob)
 cbcontexts(nctx).Add data, Key:=Hex(prob)
End Sub

Private Sub xvbdelcbcontext(ByVal nctx As CB_Contexts, ByVal prob As Variant)
 On Error Resume Next
 cbcontexts(nctx).Remove Hex(prob)
End Sub
Private Sub xcb_msg_v(ByVal prob As Variant, ByVal fn As Variant, ByVal msgptr As Variant)
 Dim msg As String
 msg = XPRBgetString((msgptr))
 
 If fn = XPRB_NULL Then
  XPRBcbmsgdefault prob, Empty, msg
 Else
  Dim context As Variant
  context = xvbgetcbcontext(CBCON_MSG, prob)
  xvbcallcbmsg (fn), (prob), context, (msg)
  xvbsetcbcontext CBCON_MSG, prob, context
 End If
End Sub
Public Sub XPRBcbmsgdefault(ByRef prob As Variant, ByRef context As Variant, ByRef msg As Variant)
 xvboutput XPRBIO_OUT, msg
End Sub

Private Sub xcb_err_v(ByVal prob As Variant, ByVal fn As Variant, ByVal errnum As Long, ByVal errtype As Long, ByVal msgptr As Variant)
 Dim msg As String
 msg = XPRBgetString((msgptr))
 
 If fn = XPRB_NULL Then
  XPRBcberrdefault prob, Empty, errnum, errtype, msg
 Else
  Dim context As Variant
  context = xvbgetcbcontext(CBCON_MSG, prob)
  xvbcallcberr (fn), (prob), context, (errnum), (errtype), (msg)
  xvbsetcbcontext CBCON_MSG, prob, context
 End If
End Sub
Public Sub XPRBcberrdefault(ByRef prob As Variant, ByRef context As Variant, ByRef errnum As Variant, ByRef errtype As Variant, ByRef msg As Variant)
 Dim fullmsg As String
 fullmsg = XPRBformaterrmsg(errnum, errtype, msg)
 xvboutput XPRBIO_ERR, fullmsg
End Sub
Public Function XPRBgeterrlvltext(ByVal lvl As Long) As String
 Static folder As Variant
 If VarType(folder) = vbEmpty Then
  Dim evar, evars()
  evars = Array("BCL", "XPRESSDIR", "XPRESS")
  For Each evar In evars
   folder = Environ(evar)
   If Len(folder) Then Exit For
  Next
  If Len(folder) Then folder = folder & "\locale"
 End If
 
 XPRBgeterrlvltext = XPRBgetString(xvbgeterrlvltext(lvl, folder))
End Function
Public Function XPRBformaterrmsg(ByVal errnum As Long, ByVal errtype As Long, ByVal msg As String) As String
 XPRBformaterrmsg = "BCL: " & XPRBgeterrlvltext(errtype) & " " & errnum & ": " & msg
End Function





