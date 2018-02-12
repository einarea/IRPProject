Attribute VB_Name = "Xpress_SLP"
' Module XSLP.BAS
' Function and Constant declarations for use with
' Xpress SLP Runtime library
' Release version 1.42 Sept 2008
' (c) Copyright Fair Isaac Corporation 2008


Option Explicit
Option Compare Text


'****************************************************************************
' Redirection of IO
'****************************************************************************
Declare Function XSLPsetStream Lib "xprvb" Alias "_xvbsetstream@8" (ByVal what As Long, ByVal fname As String) As Long
Declare Function XSLPflush Lib "xprvb" Alias "_xvbflush@4" (ByVal what As Long) As Long
Declare Function XSLPclose Lib "xprvb" Alias "_xvbclose@4" (ByVal what As Long) As Long

'****************************************************************************
' Initialization/Finish
'****************************************************************************
Declare Function XSLPinit Lib "xslp" Alias "_XSLPinit@0" () As Long
Declare Function XSLPcreateprob Lib "xslp" Alias "_XSLPcreateprob@8" (ByRef xslp_prob As Long, ByRef Prob As Long) As Long
Declare Function XSLPreadprob Lib "xslp" Alias "_XSLPreadprob@12" (ByVal Prob As Long, ByVal probname As String, ByVal flags As String) As Long
Declare Function XSLPdestroyprob Lib "xslp" Alias "_XSLPdestroyprob@4" (ByVal Prob As Long) As Long
Declare Function XSLPfree Lib "xslp" Alias "_XSLPfree@0" () As Long

'****************************************************************************
'XSLPProb manipulation
'****************************************************************************
Declare Function XSLPwriteprob Lib "xslp" Alias "_XSLPwriteprob@12" (ByVal Prob As Long, ByVal FileName As String, ByVal flags As String) As Long
Declare Function XSLPsave Lib "xslp" Alias "_XSLPsave@4" (ByVal Prob As Long) As Long
Declare Function XSLPsaveas Lib "xslp" Alias "_XSLPsaveas@8" (ByVal Prob As Long, ByVal FileName As String) As Long
Declare Function XSLPrestore Lib "xslp" Alias "_XSLPrestore@8" (ByVal Prob As Long, ByVal probname As String) As Long
Declare Function XSLPcopyprob Lib "xslp" Alias "_XSLPcopyprob@12" (ByVal Prob As Long, ByVal oldprob As Long, ByVal probname As String) As Long
Declare Function XSLPcopycallbacks Lib "xslp" Alias "_XSLPcopycallbacks@8" (ByVal NewProb As Long, ByVal oldprob As Long) As Long
Declare Function XSLPcopycontrols Lib "xslp" Alias "_XSLPcopycontrols@8" (ByVal NewProb As Long, ByVal oldprob As Long) As Long
Declare Function XSLPcascade Lib "xslp" Alias "_XSLPcascade@4" (ByVal Prob As Long) As Long
Declare Function XSLPgetsol Lib "xslp" Alias "_XSLPgetsol@4" (ByVal Prob As Long) As Long
Declare Function XSLPgetslpsol Lib "xslp" Alias "_XSLPgetslpsol@20" (ByVal Prob As Long, ByRef dx As Double, ByRef dslack As Double, ByRef dual As Double, ByRef dj As Double) As Long
Declare Function XSLPconstruct Lib "xslp" Alias "_XSLPconstruct@4" (ByVal Prob As Long) As Long
Declare Function XSLPrevise Lib "xslp" Alias "_XSLPrevise@8" (ByVal Prob As Long, ByVal FileName As String) As Long

'****************************************************************************
' Attributes and control getting/setting
'****************************************************************************
Declare Function XSLPgetdblattrib Lib "xslp" Alias "_XSLPgetdblattrib@12" (ByVal Prob As Long, ByVal Param As Long, ByRef Value As Double) As Long
Declare Function XSLPgetdblcontrol Lib "xslp" Alias "_XSLPgetdblcontrol@12" (ByVal Prob As Long, ByVal Param As Long, ByRef dValue As Double) As Long
Declare Function XSLPgetintattrib Lib "xslp" Alias "_XSLPgetintattrib@12" (ByVal Prob As Long, ByVal Param As Long, ByRef iValue As Long) As Long
Declare Function XSLPgetintcontrol Lib "xslp" Alias "_XSLPgetintcontrol@12" (ByVal Prob As Long, ByVal Param As Long, ByRef iValue As Long) As Long
Declare Function XSLPsetdblcontrol Lib "xslp" Alias "_XSLPsetdblcontrol@16" (ByVal Prob As Long, ByVal Param As Long, ByVal dValue As Double) As Long
Declare Function XSLPsetintcontrol Lib "xslp" Alias "_XSLPsetintcontrol@12" (ByVal Prob As Long, ByVal Param As Long, ByVal iValue As Long) As Long
Declare Function XSLPsetstrcontrol Lib "xslp" Alias "_XSLPsetstrcontrol@12" (ByVal Prob As Long, ByVal Param As Long, ByVal cValue As String) As Long
Declare Function XSLPsetstring Lib "xslp" Alias "_XSLPsetstring@12" (ByVal Prob As Long, ByRef Param As Long, ByVal cValue As String) As Long
Declare Function XSLPgetptrattrib Lib "xslp" Alias "_XSLPgetptrattrib@12" (ByVal Prob As Long, ByVal Param As Long, ByRef pValue As Long) As Long
Declare Function XSLPsetdefaults Lib "xslp" Alias "_XSLPsetdefaults@4" (ByVal Prob As Long) As Long

Declare Function XSLPcontrolcode Lib "xslp" Alias "_XSLPcontrolcode@12" (ByRef Param As Long, ByVal Name As String, ByRef theType As Long) As Long
Declare Function XSLPsetdefaultcontrol Lib "xslp" Alias "_XSLPsetdefaultcontrol@8" (ByVal Prob As Long, ByVal Param As Long) As Long
Declare Function XSLPsetparam Lib "xslp" Alias "_XSLPsetparam@12" (ByVal Prob As Long, ByVal Name As String, ByVal Value As String) As Long

'****************************************************************************
' Solving
'****************************************************************************
Declare Function XSLPpresolve Lib "xslp" Alias "_XSLPpresolve@4" (ByVal Prob As Long) As Long
Declare Function XSLPmaxim Lib "xslp" Alias "_XSLPmaxim@8" (ByVal Prob As Long, ByVal flags As String) As Long
Declare Function XSLPminim Lib "xslp" Alias "_XSLPminim@8" (ByVal Prob As Long, ByVal flags As String) As Long
Declare Function XSLPopt Lib "xslp" Alias "_XSLPopt@8" (ByVal Prob As Long, ByVal flags As String) As Long
Declare Function XSLPremaxim Lib "xslp" Alias "_XSLPremaxim@8" (ByVal Prob As Long, ByVal flags As String) As Long
Declare Function XSLPreminim Lib "xslp" Alias "_XSLPreminim@8" (ByVal Prob As Long, ByVal flags As String) As Long
Declare Function XSLPpostsolve Lib "xslp" Alias "_XSLPpostsolve@4" (ByVal Prob As Long) As Long
Declare Function XSLPsetlogfile Lib "xslp" Alias "_XSLPsetlogfile@12" (ByVal Prob As Long, ByVal FileName As String, ByVal Options As Long) As Long
Declare Function XSLPglobal Lib "xslp" Alias "_XSLPglobal@4" (ByVal Prob As Long) As Long

'****************************************************************************
' callbacks
'****************************************************************************
Declare Function XSLPsetcbmessage Lib "xslp" Alias "_XSLPsetcbmessage@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbiterend Lib "xslp" Alias "_XSLPsetcbiterend@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbiterstart Lib "xslp" Alias "_XSLPsetcbiterstart@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbitervar Lib "xslp" Alias "_XSLPsetcbitervar@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbcascadestart Lib "xslp" Alias "_XSLPsetcbcascadestart@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbcascadevar Lib "xslp" Alias "_XSLPsetcbcascadevar@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbcascadeend Lib "xslp" Alias "_XSLPsetcbcascadeend@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbslpend Lib "xslp" Alias "_XSLPsetcbslpend@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbslpstart Lib "xslp" Alias "_XSLPsetcbslpstart@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbprenode Lib "xslp" Alias "_XSLPsetcbprenode@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbslpnode Lib "xslp" Alias "_XSLPsetcbslpnode@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbcascadevarF Lib "xslp" Alias "_XSLPsetcbcascadevarF@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbitervarF Lib "xslp" Alias "_XSLPsetcbitervarF@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbmessageF Lib "xslp" Alias "_XSLPsetcbmessageF@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbdestroy Lib "xslp" Alias "_XSLPsetcbdestroy@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbintsol Lib "xslp" Alias "_XSLPsetcbintsol@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcboptnode Lib "xslp" Alias "_XSLPsetcboptnode@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbinterrupt Lib "xslp" Alias "_XSLPsetcbinterrupt@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbpvprenode Lib "xslp" Alias "_XSLPsetcbpvprenode@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbpvoptsol Lib "xslp" Alias "_XSLPsetcbpvoptsol@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbpvintsol Lib "xslp" Alias "_XSLPsetcbpvintsol@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbpvpostnode Lib "xslp" Alias "_XSLPsetcbpvpostnode@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbconstruct Lib "xslp" Alias "_XSLPsetcbconstruct@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbformula Lib "xslp" Alias "_XSLPsetcbformula@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long

Declare Function XSLPsetcbthreadstart Lib "xslp" Alias "_XSLPsetcbthreadstart@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcbthreadend Lib "xslp" Alias "_XSLPsetcbthreadend@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcblogicalthreadstart Lib "xslp" Alias "_XSLPsetcblogicalthreadstart@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long
Declare Function XSLPsetcblogicalthreadend Lib "xslp" Alias "_XSLPsetcblogicalthreadend@12" (ByVal Prob As Long, ByVal Address As Long, ByVal object As Long) As Long

'****************************************************************************
' Problem manipulation
'****************************************************************************
Declare Function XSLPaddcvars Lib "xslp" Alias "_XSLPaddcvars@12" (ByVal Prob As Long, ByVal nSLPCVar As Long, ByVal cValue As String) As Long
Declare Function XSLPchgcvar Lib "xslp" Alias "_XSLPchgcvar@12" (ByVal Prob As Long, ByVal nSLPCVar As Long, ByVal cValue As String) As Long
Declare Function XSLPaddnames Lib "xslp" Alias "_XSLPaddnames@20" (ByVal Prob As Long, ByVal theType As Long, ByVal cnames As String, ByVal first As Long, ByVal last As Long) As Long
Declare Function XSLPloadcvars Lib "xslp" Alias "_XSLPloadcvars@12" (ByVal Prob As Long, ByVal nSLPCVar As Long, ByVal cValue As String) As Long
Declare Function XSLPloaduserfuncs Lib "xslp" Alias "_XSLPloaduserfuncs@16" (ByVal Prob As Long, ByVal nSLPUserFunc As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPadduserfuncs Lib "xslp" Alias "_XSLPadduserfuncs@16" (ByVal Prob As Long, ByVal nSLPUserFunc As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPchguserfunc Lib "xslp" Alias "_XSLPchguserfunc@32" (ByVal Prob As Long, ByVal nSLPUserFunc As Long, ByVal xName As String, ByRef argtype As Long, ByRef exetype As Long, ByVal param1 As String, ByVal param2 As String, ByVal param3 As String) As Long
Declare Function XSLPloadxvs Lib "xslp" Alias "_XSLPloadxvs@24" (ByVal Prob As Long, ByVal nSLPXV As Long, ByRef XVStart As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPaddxvs Lib "xslp" Alias "_XSLPaddxvs@24" (ByVal Prob As Long, ByVal nSLPXV As Long, ByRef XVStart As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPconstructxf Lib "xslp" Alias "_XSLPconstructxf@4" (ByVal Prob As Long) As Long
Declare Function XSLPchgdc Lib "xslp" Alias "_XSLPchgdc@32" (ByVal Prob As Long, ByVal RowIndex As Long, ByVal rowtype As String, ByRef Delay As Long, ByRef IterCount As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPgetdc Lib "xslp" Alias "_XSLPgetdc@32" (ByVal Prob As Long, ByVal RowIndex As Long, ByVal rowtype As String, ByRef Delay As Long, ByRef IterCount As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPevaluateformula Lib "xslp" Alias "_XSLPevaluateformula@20" (ByVal Prob As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Long, ByRef dValue As Double) As Long
Declare Function XSLPgetvar Lib "xslp" Alias "_XSLPgetvar@68" (ByVal Prob As Long, ByVal ColIndex As Long, ByRef DetRow As Long, ByRef InitStepBound As Double, ByRef StepBound As Double, ByRef Penalty As Double, ByRef Damp As Double, ByRef InitValue As Double, ByRef Value As Double, ByRef TolSet As Long, ByRef History As Long, ByRef Converged As Long, ByRef VarType As Long, ByRef Delta As Long, ByRef PenaltyDelta As Long, ByRef UpdateRow As Long, ByRef OldValue As Double) As Long
Declare Function XSLPchgvar Lib "xslp" Alias "_XSLPchgvar@52" (ByVal Prob As Long, ByVal ColIndex As Long, ByRef DetRow As Long, ByRef InitStepBound As Double, ByRef StepBound As Double, ByRef Penalty As Double, ByRef Damp As Double, ByRef InitValue As Double, ByRef Value As Double, ByRef TolSet As Long, ByRef History As Long, ByRef Converged As Long, ByRef VarType As Long) As Long
Declare Function XSLPsetuniqueprefix Lib "xslp" Alias "_XSLPsetuniqueprefix@4" (ByVal Prob As Long) As Long
Declare Function XSLPgetuserfuncaddress Lib "xslp" Alias "_XSLPgetuserfuncaddress@12" (ByVal Prob As Long, ByVal FuncNumber As Long, ByRef Address As Long) As Long
Declare Function XSLPchguserfuncaddress Lib "xslp" Alias "_XSLPchguserfuncaddress@12" (ByVal Prob As Long, ByVal FuncNumber As Long, ByRef Address As Long) As Long
Declare Function XSLPsetuserfuncaddress Lib "xslp" Alias "_XSLPsetuserfuncaddress@12" (ByVal Prob As Long, ByVal FuncNumber As Long, ByVal Address As Long) As Long
Declare Function XSLPgetindex Lib "xslp" Alias "_XSLPgetindex@16" (ByVal Prob As Long, ByVal theType As Long, ByVal Name As String, ByRef SeqNo As Long) As Long
Declare Function XSLPgetrow Lib "xslp" Alias "_XSLPgetrow@12" (ByVal Prob As Long, ByVal RowIndex As Long, ByRef Status As Long) As Long
Declare Function XSLPchgrow Lib "xslp" Alias "_XSLPchgrow@12" (ByVal Prob As Long, ByVal RowIndex As Long, ByRef Status As Long) As Long
Declare Function XSLPgetxv Lib "xslp" Alias "_XSLPgetxv@12" (ByVal Prob As Long, ByVal nXSLPXV As Long, ByRef nXVitems As Long) As Long
Declare Function XSLPchgxv Lib "xslp" Alias "_XSLPchgxv@12" (ByVal Prob As Long, ByVal nXSLPXV As Long, ByRef nXVitems As Long) As Long
Declare Function XSLPgetxvitem Lib "xslp" Alias "_XSLPgetxvitem@48" (ByVal Prob As Long, ByVal SLPXV As Long, ByVal nXVitem As Long, ByVal Parsed As Long, ByRef VarType As Long, ByRef VarIndex As Long, ByRef IntIndex As Long, ByRef Delta1 As Double, ByRef Delta2 As Double, ByRef Status As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPchgxvitem Lib "xslp" Alias "_XSLPchgxvitem@48" (ByVal Prob As Long, ByVal nSLPXV As Long, ByVal nXVitem As Long, ByVal Parsed As Long, ByRef VarType As Long, ByRef VarIndex As Long, ByRef IntIndex As Long, ByRef Delta1 As Double, ByRef Delta2 As Double, ByRef Status As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPloadcoefs Lib "xslp" Alias "_XSLPloadcoefs@36" (ByVal Prob As Long, ByVal nSLPCoef As Long, ByRef RowIndex As Long, ByRef ColIndex As Long, ByRef Factor As Double, ByRef FormulaStart As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPaddcoefs Lib "xslp" Alias "_XSLPaddcoefs@36" (ByVal Prob As Long, ByVal nSLPCoef As Long, ByRef RowIndex As Long, ByRef ColIndex As Long, ByRef Factor As Double, ByRef FormulaStart As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPloadvars Lib "xslp" Alias "_XSLPloadvars@36" (ByVal Prob As Long, ByVal nSLPVar As Long, ByRef ColIndex As Long, ByRef VarType As Long, ByRef DetRow As Long, ByRef SeqNum As Long, ByRef TolIndex As Long, ByRef InitValue As Double, ByRef StepBound As Double) As Long
Declare Function XSLPaddvars Lib "xslp" Alias "_XSLPaddvars@36" (ByVal Prob As Long, ByVal nSLPVar As Long, ByRef ColIndex As Long, ByRef VarType As Long, ByRef DetRow As Long, ByRef SeqNum As Long, ByRef TolIndex As Long, ByRef InitValue As Double, ByRef StepBound As Double) As Long
Declare Function XSLPloaddcs Lib "xslp" Alias "_XSLPloaddcs@32" (ByVal Prob As Long, ByVal nSLPDC As Long, ByRef RowIndex As Long, ByRef Delay As Long, ByRef DCStart As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPadddcs Lib "xslp" Alias "_XSLPadddcs@32" (ByVal Prob As Long, ByVal nSLPDC As Long, ByRef RowIndex As Long, ByRef Delay As Long, ByRef DCStart As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPloadtolsets Lib "xslp" Alias "_XSLPloadtolsets@12" (ByVal Prob As Long, ByVal nSLPTol, ByRef SLPTol As Double) As Long
Declare Function XSLPaddtolsets Lib "xslp" Alias "_XSLPaddtolsets@12" (ByVal Prob As Long, ByVal nSLPTol, ByRef SLPTol As Double) As Long
Declare Function XSLPparsecformula Lib "xslp" Alias "_XSLPparsecformula@20" (ByVal Prob As Long, ByVal Formula As String, ByRef nToken As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPparseformula Lib "xslp" Alias "_XSLPparseformula@24" (ByVal Prob As Long, ByRef inType As Long, ByRef inValue As Double, ByRef nToken, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPchgcoef Lib "xslp" Alias "_XSLPchgcoef28@" (ByVal Prob As Long, ByVal RowIndex As Long, ByVal ColIndex As Long, ByRef Factor As Double, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPchgccoef Lib "xslp" Alias "_XSLPchgccoef@20" (ByVal Prob As Long, ByVal RowIndex As Long, ByVal ColIndex As Long, ByRef Factor As Double, ByVal Formula As String) As Long
Declare Function XSLPgetccoef Lib "xslp" Alias "_XSLPgetccoef@24" (ByVal Prob As Long, ByVal RowIndex As Long, ByVal ColIndex As Long, ByRef Factor As Double, ByVal Formula As String, ByVal fLen As Long) As Long
Declare Function XSLPgetcoef Lib "xslp" Alias "_XSLPgetcoef@28" (ByVal Prob As Long, ByVal RowIndex As Long, ByVal ColIndex As Long, ByRef Factor As Double, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPqparse Lib "xslp" Alias "_XSLPqparse@12" (ByVal Record As String, ByRef Token As Byte, ByVal NumFields As Long) As Long
Declare Function XSLPtokencount Lib "xslp" Alias "_XSLPtokencount@4" (ByVal Record As String) As Long
Declare Function XSLPchgtolset Lib "xslp" Alias "_XSLPchgtolset@16" (ByVal Prob As Long, ByVal nSLPTol As Long, ByRef Status As Long, ByRef Tols As Double) As Long
Declare Function XSLPgettolset Lib "xslp" Alias "_XSLPgettolset@16" (ByVal Prob As Long, ByVal nSLPTol As Long, ByRef Status As Long, ByRef Tols As Double) As Long
Declare Function XSLPevaluatecoef Lib "xslp" Alias "_XSLPevaluatecoef@16" (ByVal Prob As Long, ByVal RowIndex As Long, ByVal ColIndex As Long, ByRef dValue As Long) As Long
Declare Function XSLPcascadeorder Lib "xslp" Alias "_XSLPcascadeorder@4" (ByVal Prob As Long) As Long
Declare Function XSLPscaling Lib "xslp" Alias "_XSLPscaling@4" (ByVal Prob As Long) As Long
Declare Function XSLPloadivfs Lib "xslp" Alias "_XSLPloadivfs@28" (ByVal Prob As Long, ByVal nIVF As Long, ByRef ColIndex As Long, ByRef IVFStart As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPaddivfs Lib "xslp" Alias "_XSLPaddivfs@28" (ByVal Prob As Long, ByVal nIVF As Long, ByRef ColIndex As Long, ByRef IVFStart As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPgetivf Lib "xslp" Alias "_XSLPgetivf@20" (ByVal Prob As Long, ByVal ColIndex As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long
Declare Function XSLPchgivf Lib "xslp" Alias "_XSLPchgivf@20" (ByVal Prob As Long, ByVal ColIndex As Long, ByVal Parsed As Long, ByRef theType As Long, ByRef Value As Double) As Long

Declare Function XSLPloadpvs Lib "xslp" Alias "_XSLPloadpvs@20" (ByVal Prob As Long, ByVal nSLPPV As Long, ByRef Index As Long, ByRef Value As Double, ByRef Priority As Double) As Long
Declare Function XSLPaddpvs Lib "xslp" Alias "_XSLPaddpvs@20" (ByVal Prob As Long, ByVal nSLPPV As Long, ByRef Index As Long, ByRef Value As Double, ByRef Priority As Double) As Long
Declare Function XSLPgetmpv Lib "xslp" Alias "_XSLPgetmpv@24" (ByVal Prob As Long, ByVal ColIndex As Long, ByVal Space As Long, ByRef nItem As Long, ByRef Value As Double, ByRef Priority As Double) As Long
Declare Function XSLPchgmpv Lib "xslp" Alias "_XSLPchgmpv@20" (ByVal Prob As Long, ByVal ColIndex As Long, ByVal nItem As Long, ByRef Value As Double, ByRef Priority As Double) As Long
Declare Function XSLPgetintpvinfo  Lib "xslp" Alias "_XSLPgetpvinfo@16" (ByVal Prob As Long, ByVal theType As Long, ByRef Value As Long, ByVal FileName As String)
Declare Function XSLPgetdblpvinfo  Lib "xslp" Alias "_XSLPgetpvinfo@16" (ByVal Prob As Long, ByVal theType As Long, ByRef Value As Double, ByVal FileName As String)
Declare Function XSLPgetpvvalues  Lib "xslp" Alias "_XSLPgetpvvalues@24" (ByVal Prob As Long, ByVal First As Long, ByVal Last As Long, ByRef Values As Double, ByRef Parents As Long, ByVal FileName As String)

Declare Function XSLPgetrowwt Lib "xslp" Alias "_XSLPgetrowwt@12" (ByVal Prob As Long, ByVal Index As Long, ByRef Value As Double) As Long
Declare Function XSLPchgrowwt Lib "xslp" Alias "_XSLPchgrowwt@12" (ByVal Prob As Long, ByVal Index As Long, ByRef Value As Double) As Long

Declare Function XSLPgetintrowinfo Lib "xslp" Alias "_XSLProwinfo@16" (ByVal Prob As Long, ByVal Index As Long,  ByVal theType As Long, ByRef Value As Long) As Long
Declare Function XSLPgetdblrowinfo Lib "xslp" Alias "_XSLProwinfo@16" (ByVal Prob As Long, ByVal Index As Long,  ByVal theType As Long, ByRef Value As Double) As Long
Declare Function XSLPsetdblrowinfo Lib "xslp" Alias "_XSLProwinfo@16" (ByVal Prob As Long, ByVal Index As Long,  ByVal theType As Long, ByRef Value As Double) As Long

Declare Function XSLPreaddfs Lib "xslp" Alias "_XSLPreaddfs@8" (ByVal Prob As Long, ByVal Filename As String) As Long
Declare Function XSLPwritedfs Lib "xslp" Alias "_XSLPwritedfs@8" (ByVal Prob As Long, ByVal Filename As String) As Long
Declare Function XSLPloaddfs Lib "xslp" Alias "_XSLPloaddfs@20" (ByVal Prob As Long, ByVal nDF As Long, ByRef ColIndex As Long, ByRef RowIndex As Long, ByRef DFValue As Double) As Long
Declare Function XSLPadddfs Lib "xslp" Alias "_XSLPadddfs@20" (ByVal Prob As Long, ByVal nDF As Long, ByRef ColIndex As Long, ByRef RowIndex As Long, ByRef DFValue As Double) As Long
Declare Function XSLPgetdf Lib "xslp" Alias "_XSLPgetdf@16" (ByVal Prob As Long, ByVal ColIndex As Long, ByVal RowIndex As Long, ByRef DFValue As Double) As Long
Declare Function XSLPchgdf Lib "xslp" Alias "_XSLPchgdf@16" (ByVal Prob As Long, ByVal ColIndex As Long, ByVal RowIndex As Long, ByRef DFValue As Double) As Long

'****************************************************************************
' User functions
'****************************************************************************
Declare Function XSLPcalluserfunc Lib "xslp" Alias "_XSLPcalluserfunc@32" (ByVal Prob As Long, ByVal FuncNumber As Long, ByRef Arg1 As Double, ByRef Arg2 As Long, ByVal Arg3 As String, ByVal Arg4 As String, ByRef Arg5 As Double, ByRef Arg6 As Double) As Double
Declare Function XSLPchguserfuncobject Lib "xslp" Alias "_XSLPchguserfuncobject@12" (ByVal Prob As Long, ByVal Entity As Long, ByRef obj As Long) As Long
Declare Function XSLPgetfuncinfo Lib "xslp" Alias "_XSLPgetfuncinfo@36" (ByRef ArgInfo As Long, ByRef CallerFlag As Long, ByRef nInput As Long, ByRef nReturn As Long, ByRef nDelta As Long, ByRef nInString As Long, ByRef nOutString As Long, ByRef nSLPUserFunc As Long, ByRef nInstance As Long) As Long
Declare Function XSLPgetfuncinfoV Lib "xslp" Alias "_XSLPgetfuncinfoV@36" (ByRef ArgInfo As Variant, ByRef CallerFlag As Long, ByRef nInput As Long, ByRef nReturn As Long, ByRef nDelta As Long, ByRef nInString As Long, ByRef nOutString As Long, ByRef nSLPUserFunc As Long, ByRef nInstance As Long) As Long
Declare Function XSLPgetfuncobject Lib "xslp" Alias "_XSLPgetfuncobject@12" (ByRef ArgInfo As Long, ByVal ObjType As Long, ByRef obj As Long) As Long
Declare Function XSLPgetfuncobjectV Lib "xslp" Alias "_XSLPgetfuncobjectV@12" (ByRef ArgInfo As Variant, ByVal ObjType As Long, ByRef obj As Long) As Long
Declare Function XSLPgetuserfuncobject Lib "xslp" Alias "_XSLPgetuserfuncobject@12" (ByVal Prob As Long, ByVal Entity As Long, ByRef obj As Long) As Long
Declare Function XSLPsetfuncobject Lib "xslp" Alias "_XSLPsetfuncobject@12" (ByRef ArgInfo As Long, ByVal ObjType As Long, ByVal obj As Long) As Long
Declare Function XSLPsetfunctionerror Lib "xslp" Alias "_XSLPsetfunctionerror@4" (ByVal Prob As Long) As Long
Declare Function XSLPsetuserfuncinfo Lib "xslp" Alias "_XSLPsetuserfuncinfo@32" (ByVal Prob As Long, ByRef ArgInfo As Long, ByVal CallerFlag As Long, ByVal nInput As Long, ByVal nReturn As Long, ByVal nDelta As Long, ByVal nInString As Long, ByVal nOutString As Long) As Long
Declare Function XSLPsetuserfuncobject Lib "xslp" Alias "_XSLPsetuserfuncobject@12" (ByVal Prob As Long, ByVal Entity As Long, ByVal obj As Long) As Long

Declare Function XSLPgetintuserfuncinfo Lib "xslp" Alias "_XSLPuserfuncinfo@16" (ByVal Prob As Long, ByVal Index As Long,  ByVal theType As Long, ByRef Value As Long) As Long
Declare Function XSLPgetdbluserfuncinfo Lib "xslp" Alias "_XSLPuserfuncinfo@16" (ByVal Prob As Long, ByVal Index As Long,  ByVal theType As Long, ByRef Value As Double) As Long
Declare Function XSLPsetintuserfuncinfo Lib "xslp" Alias "_XSLPuserfuncinfo@16" (ByVal Prob As Long, ByVal Index As Long,  ByVal theType As Long, ByRef Value As Long) As Long
Declare Function XSLPgetfuncintattrib Lib "xslp" Alias "_XSLPgetfuncattrib@12" (ByRef ArgInfo As Long, ByVal Param As Long, ByRef iValue As Long) As Long
Declare Function XSLPthreadactive Lib "xslp" Alias "_XSLPthreadactive@4" (ByVal ArgInfo As Long) As Long

'****************************************************************************
' Misc
'****************************************************************************
Declare Function XSLPtime Lib "xslp" Alias "_XSLPtime@4" (ByVal Prob As Long) As Long
Declare Function XSLPgettime Lib "xslp" Alias "_XSLPgettime@12" (ByVal Prob As Long, ByRef Seconds As Long, ByRef MilliSeconds As Long) As Long
Declare Function XSLPgetdtime Lib "xslp" Alias "_XSLPgetdtime@8" (ByVal Prob As Long, ByRef dSeconds As Double) As Long
Declare Function XSLPgetmessagetype Lib "xslp" Alias "_XSLPgetmessagetype@8" (ByVal theCode As Long, ByRef theType As Long) As Long
Declare Function XSLPprintmsg Lib "xslp" Alias "_XSLPprintmsg@12" (ByVal Prob As Long, ByVal theType As Long, ByVal theFormat As String) As Long
Declare Function XSLPuprintmemory Lib "xslp" Alias "_XSLPuprintmemory@4" (ByVal Prob As Long) As Long
Declare Function XSLPgetversion Lib "xslp" Alias "_XSLPgetversion@8" (ByRef theVersion As Long, ByRef theMinorVersion As Long) As Long
Declare Function XSLPinterrupt Lib "xslp" Alias "_XSLPinterrupt@8" (ByVal Prob As Long, ByVal Reason As Long) As Long
Declare Function XSLPcheckinterrupt Lib "xslp" Alias "_XSLPcheckinterrupt@4" (ByVal Prob As Long) As Long
Declare Function XSLPdecompose Lib "xslp" Alias "_XSLPdecompose@12" (ByVal Prob As Long, ByVal Count As Long, ByRef Index As Long) As Long

'****************************************************************************
' buffer sizes for intermediate buffers
'****************************************************************************
' for large problems, these might have to be increased
Global Const BUFFERSIZE = 256
Global Const HUGEBUFFERSIZE = 1024


'****************************************************************************
' Stream codes for SetStream
'****************************************************************************
Global Const XPRBIO_IN = 0
Global Const XPRBIO_OUT = 1
Global Const XPRBIO_ERR = 2


'****************************************************************************
'Integer problem attributes
'****************************************************************************

Global Const XSLP_INTATTRIBSTART = 12000
Global Const XSLP_ITER = 12001
Global Const XSLP_STATUS = 12002
Global Const XSLP_UNCONVERGED = 12003
Global Const XSLP_SBXCONVERGED = 12004
Global Const XSLP_CVS = 12005
Global Const XSLP_XVS = 12006
Global Const XSLP_UFS = 12007
Global Const XSLP_IFS = 12008
Global Const XSLP_PENALTYDELTAROW = 12009
Global Const XSLP_PENALTYDELTACOLUMN = 12010
Global Const XSLP_PENALTYERRORROW = 12011
Global Const XSLP_PENALTYERRORCOLUMN = 12012
Global Const XSLP_EQUALSCOLUMN = 12013
Global Const XSLP_VARIABLES = 12014
Global Const XSLP_IMPLICITVARIABLES = 12015
Global Const XSLP_COEFFICIENTS = 12016
Global Const XSLP_PENALTYDELTAS = 12017
Global Const XSLP_PENALTYERRORS = 12018
Global Const XSLP_PLUSPENALTYERRORS = 12019
Global Const XSLP_MINUSPENALTYERRORS = 12020
Global Const XSLP_UCCONSTRAINEDCOUNT = 12021
Global Const XSLP_MIPNODES = 12022
Global Const XSLP_MIPITER = 12023
Global Const XSLP_VERSION = 12024
Global Const XSLP_MINORVERSION = 12025
Global Const XSLP_NONLINEARCONSTRAINTS = 12026
Global Const XSLP_IPROBLEM = 12027
Global Const XSLP_TOLSETS = 12028
Global Const XSLP_VALIDATIONCOUNT = 12029
Global Const XSLP_INTERNALFUNCCALLS = 12030
Global Const XSLP_USERFUNCCALLS = 12031
Global Const XSLP_NLOBJROW = 12032
Global Const XSLP_NLOBJCOL = 12033
Global Const XSLP_UFINSTANCES = 12034
Global Const XSLP_ECFCOUNT = 12035
Global Const XSLP_CHECKSUM = 12036
Global Const XSLP_USEDERIVATIVES = 12037
Global Const XSLP_PRESOLVEPASSES = 12038
Global Const XSLP_INTERRUPT = 12039
Global Const XSLP_USEDFORMULAHASH = 12040
Global Const XSLP_DELTAS = 12041
Global Const XSLP_ITERSOL = 12043
Global Const XSLP_PVNODES = 12044
Global Const XSLP_PVPARENT = 12045
Global Const XSLP_ZEROESRESET = 12046
Global Const XSLP_ZEROESTOTAL = 12047
Global Const XSLP_ZEROESRETAINED = 12048
Global Const XSLP_THREADSTARTTIME = 12049
Global Const XSLP_THREADOPTWINNER = 12050
Global Const XSLP_PRESOLVEFIXEDSLPVAR = 12051
Global Const XSLP_PRESOLVEFIXEDDR = 12052
Global Const XSLP_PRESOLVEFIXEDCOEF = 12053
Global Const XSLP_PRESOLVEFIXEDZCOL = 12054
Global Const XSLP_PRESOLVEFIXEDNZCOL = 12055
Global Const XSLP_PRESOLVEDELETEDDELTA = 12056
Global Const XSLP_PRESOLVETIGHTENED = 12057
Global Const XSLP_INTATTRIBEND = 12058
    
'****************************************************************************
' Double control variables
'****************************************************************************

Global Const XSLP_DBLCONTROLSTART = 12100
Global Const XSLP_CASCADETOL_A = 12101
Global Const XSLP_CASCADETOL_R = 12102
Global Const XSLP_DAMP = 12103
Global Const XSLP_DAMPEXPAND = 12104
Global Const XSLP_DAMPSHRINK = 12105
Global Const XSLP_DELTA_A = 12106
Global Const XSLP_DELTA_R = 12107
Global Const XSLP_DELTA_Z = 12108
Global Const XSLP_DELTACOST = 12109
Global Const XSLP_DELTAMAXCOST = 12110
Global Const XSLP_DELTAMULT = 12111
Global Const XSLP_DJTOL = 12112
Global Const XSLP_ERRORCOST = 12113
Global Const XSLP_ERRORMAXCOST = 12114
Global Const XSLP_ERRORMULT = 12115
Global Const XSLP_ERRORTOL_A = 12116
Global Const XSLP_ERRORTOL_R = 12117
Global Const XSLP_EXPAND = 12118
Global Const XSLP_INFINITY = 12119
Global Const XSLP_MAXWEIGHT = 12120
Global Const XSLP_MINWEIGHT = 12121
Global Const XSLP_SHRINK = 12122
Global Const XSLP_ZERO = 12123

'****************************************************************************
' Convergence tolerances
'****************************************************************************

Global Const XSLP_TOLCONTROLSTART = 12124
Global Const XSLP_CTOL = 12124
Global Const XSLP_ATOL_A = 12125
Global Const XSLP_ATOL_R = 12126
Global Const XSLP_MTOL_A = 12127
Global Const XSLP_MTOL_R = 12128
Global Const XSLP_ITOL_A = 12129
Global Const XSLP_ITOL_R = 12130
Global Const XSLP_STOL_A = 12131
Global Const XSLP_STOL_R = 12132
Global Const XSLP_MVTOL = 12133
Global Const XSLP_XTOL_A = 12134
Global Const XSLP_XTOL_R = 12135
Global Const XSLP_TOLCONTROLEND = 12136
'****************************************************************************
'  End of convergence tolerances
'****************************************************************************

Global Const XSLP_DEFAULTSTEPBOUND = 12136
Global Const XSLP_DAMPMAX = 12137
Global Const XSLP_DAMPMIN = 12138
Global Const XSLP_DELTACOSTFACTOR = 12139
Global Const XSLP_ERRORCOSTFACTOR = 12140
Global Const XSLP_ERRORTOL_P = 12141
Global Const XSLP_CASCADETOL_PA = 12142
Global Const XSLP_CASCADETOL_PR = 12143
Global Const XSLP_CASCADETOL_Z = 12144
Global Const XSLP_DEFAULTIV = 12145
Global Const XSLP_OBJSENSE = 12146
Global Const XSLP_OPTTIME = 12147
Global Const XSLP_UPDATESCALE = 12148
Global Const XSLP_DELTASCALE = 12149
Global Const XSLP_OTOL_A = 12150
Global Const XSLP_OTOL_R = 12151
Global Const XSLP_DELTA_X = 12152
Global Const XSLP_ERRORCOSTS = 12153
Global Const XSLP_EQTOL_A = 12154
Global Const XSLP_EQTOL_R = 12155
Global Const XSLP_DELTA_ZR = 12156
Global Const XSLP_GRANULARITY = 12157
Global Const XSLP_MIPCUTOFF_A = 12158
Global Const XSLP_MIPCUTOFF_R = 12159
Global Const XSLP_MIPOTOL_A = 12160
Global Const XSLP_MIPOTOL_R = 12161
Global Const XSLP_MINZEROBOUND = 12162
Global Const XSLP_SBFROMMAX = 12163
Global Const XSLP_MEMORYFACTOR = 12164
Global Const XSLP_VALIDATIONTOL_A = 12165
Global Const XSLP_VALIDATIONTOL_R = 12166
Global Const XSLP_VALIDATIONINDEX_A = 12167
Global Const XSLP_VALIDATIONINDEX_R = 12168
Global Const XSLP_ESCALATION = 12169
Global Const XSLP_OBJTOPENALTYCOST = 12170
Global Const XSLP_PDRATIO = 12171
Global Const XSLP_DPRATIO = 12172
Global Const XSLP_UNBSTEPFACTOR = 12176
Global Const XSLP_VTOL_A = 12177
Global Const XSLP_VTOL_R = 12178
Global Const XSLP_OBJVAL = 12179
Global Const XSLP_ETOL_A = 12180
Global Const XSLP_ETOL_R = 12181
Global Const XSLP_EVTOL_A = 12182
Global Const XSLP_EVTOL_R = 12183
Global Const XSLP_GTOL_A = 12184
Global Const XSLP_GDELTA = 12185
Global Const XSLP_NLSMALL = 12186
Global Const XSLP_NLSMALLSLOPE = 12187
Global Const XSLP_NLSMALLPSLOPE = 12188
Global Const XSLP_ECFTOL_A = 12189
Global Const XSLP_ECFTOL_R = 12190
Global Const XSLP_WTOL_A = 12191
Global Const XSLP_WTOL_R = 12192
Global Const XSLP_PRESOLVEZERO = 12193
Global Const XSLP_PVERRORTOL_A = 12194
Global Const XSLP_PVERRORTOL_R = 12195
Global Const XSLP_USERFUNCTIONTHREADWAIT = 12196
Global Const XSLP_USERFUNCTIONTHREADINCREMENT = 12197
Global Const XSLP_MIPERRORTOL_A = 12198
Global Const XSLP_MIPERRORTOL_R = 12199
Global Const XSLP_CDTOL_A = 12200
Global Const XSLP_CDTOL_R = 12201
Global Const XSLP_DBLCONTROLEND = 12202
    
'****************************************************************************
' Integer control variables
'****************************************************************************

Global Const XSLP_INTCONTROLSTART = 12300
Global Const XSLP_ALGORITHM = 12301
Global Const XSLP_AUGMENTATION = 12302
Global Const XSLP_BARLIMIT = 12303
Global Const XSLP_CASCADE = 12304
Global Const XSLP_CASCADEFLIMIT = 12305
Global Const XSLP_CASCADENLIMIT = 12306
Global Const XSLP_CONTROL = 12307
Global Const XSLP_DAMPSTART = 12308
Global Const XSLP_DCLIMIT = 12309
Global Const XSLP_DEBUG = 12310
Global Const XSLP_DELTAZLIMIT = 12311
Global Const XSLP_FUNCEVAL = 12312
Global Const XSLP_INFEASCHECK = 12313
Global Const XSLP_INFEASLIMIT = 12314
Global Const XSLP_ITERLIMIT = 12315
Global Const XSLP_LOG = 12316
Global Const XSLP_SAMECOUNT = 12317
Global Const XSLP_PROFILER = 12318
Global Const XSLP_SAMEDAMP = 12319
Global Const XSLP_SBSTART = 12320
Global Const XSLP_XCOUNT = 12321
Global Const XSLP_XLIMIT = 12322
Global Const XSLP_MEMORYSTART = 12323
Global Const XSLP_EXTRACVS = 12323
Global Const XSLP_EXTRAUFS = 12324
Global Const XSLP_EXTRAXVS = 12325
Global Const XSLP_EXTRAXVITEMS = 12326
Global Const XSLP_MEMORYEND = 12327
Global Const XSLP_CONTINUE = 12328
Global Const XSLP_DELAYUPDATEROWS = 12329
Global Const XSLP_AUTOSAVE = 12330
Global Const XSLP_SCRAMBLENAMES = 12331
Global Const XSLP_PRINTFAILED = 12332
Global Const XSLP_OCOUNT = 12333
Global Const XSLP_EVALUATE = 12334
Global Const XSLP_EXCELVISIBLE = 12335
Global Const XSLP_MIPALGORITHM = 12336
Global Const XSLP_MIPRELAXSTEPBOUNDS = 12337
Global Const XSLP_MIPFIXSTEPBOUNDS = 12338
Global Const XSLP_MIPITERLIMIT = 12339
Global Const XSLP_MIPCUTOFFLIMIT = 12340
Global Const XSLP_MIPOCOUNT = 12341
Global Const XSLP_MIPINFEASLIMIT = 12342
Global Const XSLP_MIPDEFAULTALGORITHM = 12343
Global Const XSLP_PRESOLVE = 12344
Global Const XSLP_TIMEPRINT = 12345
Global Const XSLP_SLPLOG = 12346
Global Const XSLP_MIPLOG = 12347
Global Const XSLP_DELTAOFFSET = 12348
Global Const XSLP_UPDATEOFFSET = 12349
Global Const XSLP_ERROROFFSET = 12350
Global Const XSLP_SBROWOFFSET = 12351
Global Const XSLP_PDSTART = 12352
Global Const XSLP_NLINFO = 12353
Global Const XSLP_STOPOUTOFRANGE = 12354
Global Const XSLP_RESERVED = 12355
Global Const XSLP_VCOUNT = 12356
Global Const XSLP_VLIMIT = 12357
Global Const XSLP_ABORT = 12358
Global Const XSLP_SAVEALL = 12359
Global Const XSLP_NLALGORITHM = 12360
Global Const XSLP_WARMSTART = 12361
Global Const XSLP_NLITERPRINT = 12362
Global Const XSLP_NLITERFREQ = 12363
Global Const XSLP_NLITERLIMIT = 12364
Global Const XSLP_NLSRC = 12365
Global Const XSLP_MAXTIME = 12366
Global Const XSLP_SCALE = 12367
Global Const XSLP_SCALECOUNT = 12368
Global Const XSLP_ECFCHECK = 12369
Global Const XSLP_MIPCUTOFFCOUNT = 12370
Global Const XSLP_FORMULAHASH = 12371
Global Const XSLP_UNQUIETMIP = 12372
Global Const XSLP_DERIVATIVES = 12373
Global Const XSLP_WCOUNT = 12374
Global Const XSLP_PRESOLVEPASSLIMIT = 12375
Global Const XSLP_UNFINISHEDLIMIT = 12376
Global Const XSLP_PVCONTROL = 12377
Global Const XSLP_ZEROCRITERION = 12378
Global Const XSLP_ZEROCRITERIONSTART = 12379
Global Const XSLP_ZEROCRITERIONCOUNT = 12380
Global Const XSLP_USERFUNCTIONTHREADS = 12381
Global Const XSLP_USERFUNCTIONTHREADLOG = 12382
Global Const XSLP_USERFUNCTIONCBMUTEX = 12383
Global Const XSLP_PENALTYINFOSTART = 12384
Global Const XSLP_DCLOG = 12385
Global Const XSLP_DECOMPOSE = 12386
Global Const XSLP_THREADOPT = 12387
Global Const XSLP_THREADOPTLOG = 12388
Global Const XSLP_THREADBARLIMIT = 12389
Global Const XSLP_DECOMPOSEPASSLIMIT = 12390
Global Const XSLP_INTCONTROLEND = 12391
    
'****************************************************************************
' Character string control variables
'****************************************************************************
Global Const XSLP_STRCONTROLSTART = 12400
Global Const XSLP_CVNAME = 12401
Global Const XSLP_DELTAFORMAT = 12402
Global Const XSLP_IVNAME = 12403
Global Const XSLP_MAXFORMAT = 12404
Global Const XSLP_MINFORMAT = 12405
Global Const XSLP_MINUSDELTAFORMAT = 12406
Global Const XSLP_MINUSERRORFORMAT = 12407
Global Const XSLP_PLUSDELTAFORMAT = 12408
Global Const XSLP_PLUSERRORFORMAT = 12409
Global Const XSLP_SBNAME = 12410
Global Const XSLP_TOLNAME = 12411
Global Const XSLP_UPDATEFORMAT = 12412
Global Const XSLP_PENALTYROWFORMAT = 12413
Global Const XSLP_PENALTYCOLFORMAT = 12414
Global Const XSLP_LOWERBOUNDFORMAT = 12415
Global Const XSLP_UPPERBOUNDFORMAT = 12416
Global Const XSLP_SBLOROWFORMAT = 12417
Global Const XSLP_SBUPROWFORMAT = 12418
Global Const XSLP_NLOBJFORMAT = 12419
Global Const XSLP_CASCADEREPORT = 12420
Global Const XSLP_CONVERGEREPORT = 12421
Global Const XSLP_THREADOPTCHOICE = 12422
Global Const XSLP_ALGORITHMCHOICE = 12423
Global Const XSLP_STRCONTROLEND = 12424
    
'****************************************************************************
' Character string attributes
'****************************************************************************
Global Const XSLP_STRATTRIBSTART = 12500
Global Const XSLP_UNIQUEPREFIX = 12501
Global Const XSLP_VERSIONDATE = 12502
Global Const XSLP_THREADOPTALGORITHMS = 12503
Global Const XSLP_STRATTRIBEND = 12504

'****************************************************************************
' Pointer attributes
'****************************************************************************
Global Const XSLP_POINTERATTRIBSTART = 12600
Global Const XSLP_XPRSPROBLEM = 12601
Global Const XSLP_XSLPPROBLEM = 12602
Global Const XSLP_XPRMLIBCTX = 12603
Global Const XSLP_XPRMMOSELIDCALL = 12604
Global Const XSLP_XPRMFUNCTIONCALL = 12605
Global Const XSLP_XPRMFUNCTIONCALLA = 12606
Global Const XSLP_GLOBALFUNCOBJECT = 12607
Global Const XSLP_USERFUNCOBJECT = 12608
Global Const XSLP_INSTANCEFUNCOBJECT = 12609
Global Const XSLP_XPRMFUNCTIONCALLX = 12610
Global Const XSLP_MIPPROBLEM = 12611
Global Const XSLP_XPRMMOSELIDREF = 12612
Global Const XSLP_POINTERATTRIBEND = 12613

'***************************************************************************
' Double attributes
'****************************************************************************
Global Const XSLP_DBLATTRIBSTART = 12700
Global Const XSLP_CURRENTDELTACOST = 12701
Global Const XSLP_CURRENTERRORCOST = 12702
Global Const XSLP_XTIME = 12703
Global Const XSLP_PENALTYERRORTOTAL = 12704
Global Const XSLP_PENALTYERRORVALUE = 12705
Global Const XSLP_PENALTYDELTATOTAL = 12706
Global Const XSLP_PENALTYDELTAVALUE = 12707
Global Const XSLP_VSOLINDEX = 12708
Global Const XSLP_CLOCKSTART = 12709
Global Const XSLP_DBLATTRIBEND = 12710

'**************************************************************************
' Memory size controls                                               
'************************************************************************** 
Global Const XSLP_MEMCONTROLSTART = 12800
Global Const XSLP_MEM_STRING = 12801
Global Const XSLP_MEM_CVAR = 12802
Global Const XSLP_MEM_XV = 12803
Global Const XSLP_MEM_XVITEM = 12804
Global Const XSLP_MEM_UF = 12805
Global Const XSLP_MEM_VAR = 12806
Global Const XSLP_MEM_COEF = 12807
Global Const XSLP_MEM_FORMULA = 12808
Global Const XSLP_MEM_STACK = 12809
Global Const XSLP_MEM_COL = 12810
Global Const XSLP_MEM_ROW = 12811
Global Const XSLP_MEM_XROW = 12812
Global Const XSLP_MEM_TOL = 12813
Global Const XSLP_MEM_FORMULAVALUE = 12814
Global Const XSLP_MEM_CALCSTACK = 12815
Global Const XSLP_MEM_ITERLOG = 12816
Global Const XSLP_MEM_XF = 12817
Global Const XSLP_MEM_XFVALUE = 12818
Global Const XSLP_MEM_XFNAMES = 12819
Global Const XSLP_MEM_EXCELDOUBLE = 12820
Global Const XSLP_MEM_RETURNARRAY = 12821
Global Const XSLP_MEM_VALSTACK = 12822
Global Const XSLP_MEM_FORMULAHASH = 12823
Global Const XSLP_MEM_GETNEXT = 12824
Global Const XSLP_MEM_LU = 12825
Global Const XPRM_MEM_GEXP = 12826
Global Const XPRM_MEM_STACK = 12827
Global Const XPRM_MEM_GENCTR = 12828
Global Const XPRM_MEM_XTRA = 12829
Global Const XPRM_MEM_XSTACK = 12830
Global Const XPRM_MEM_DATA = 12831
Global Const XPRM_MEM_XV = 12832
Global Const XPRM_MEM_ROW = 12833
Global Const XPRM_MEM_COL = 12834
Global Const XPRM_MEM_GXSTACK = 12835
Global Const XPRM_MEM_QSTACK = 12836
Global Const XSLP_MEM_DERIVATIVES = 12837
Global Const XSLP_MEMCONTROLEND = 12838

'***************************************************************************
' End of list                                                              *
'*************************************************************************** 

Global Const XSLP_OBJTOL_A = 12134
Global Const XSLP_OBJTOL_R = 12135

'****************************************************************************
' Token types
'****************************************************************************
Global Const XSLP_EOF = 0
Global Const XSLP_FIRSTOPERAND = 1
Global Const XSLP_CON = 1
Global Const XSLP_CV = 2
Global Const XSLP_XV = 3
Global Const XSLP_VAR = 4
Global Const XSLP_STRING = 5
Global Const XSLP_RESOLVEDTOCONSTANT = 6
Global Const XSLP_FORMULA = 7
Global Const XSLP_CONSTRAINT = 8
Global Const XSLP_ROW = 9
Global Const XSLP_COL = 10
Global Const XSLP_LASTOPERAND = 10
Global Const XSLP_FUN = 11
Global Const XSLP_IFUN = 12
Global Const XSLP_MPVAR = 13
Global Const XSLP_GEXP = 14
Global Const XSLP_GENCTR = 15
Global Const XSLP_XVITEM = 16
Global Const XSLP_MPXV = 17
Global Const XSLP_LINCTR = 18
Global Const XSLP_LINEXP = 19
Global Const XSLP_VARREF = 20
Global Const XSLP_LB = 21
Global Const XSLP_RB = 22
Global Const XSLP_DEL = 24
Global Const XSLP_UNKFUN = 25
Global Const XSLP_MOSELREF = 26
Global Const XSLP_MOSELSTR = 27
Global Const XSLP_MOSELFRM = 28
Global Const XSLP_OP = 31
Global Const XSLP_RESOLVED = 91
Global Const XSLP_DELETED = 92
Global Const XSLP_UNKNOWN = 99
Global Const XSLP_UFUNKNOWN = 98
Global Const XSLP_INFOTOKEN = 81
Global Const XSLP_COEFTOKEN = 82
Global Const XSLP_XVTOKEN = 83
Global Const XSLP_UFTOKEN = 84
Global Const XSLP_DCTOKEN = 85
Global Const XSLP_UFARGTYPE = 86
Global Const XSLP_UFEXETYPE = 87
Global Const XSLP_ECTOKEN = 88
Global Const XSLP_IVFTOKEN = 89
Global Const XSLP_UFLTHREAD = 90
Global Const XSLP_XVVARTYPE = 71
Global Const XSLP_XVVARINDEX = 72
Global Const XSLP_XVINTINDEX = 73
Global Const XSLP_XVDELTA1 = 74
Global Const XSLP_XVDELTA2 = 75
Global Const XSLP_XVINTADDRESS = 76

Global Const XSLP_UMINUS = 1
Global Const XSLP_EXPONENT = 2
Global Const XSLP_MULTIPLY = 3
Global Const XSLP_DIVIDE = 4
Global Const XSLP_PLUS = 5
Global Const XSLP_MINUS = 6
Global Const XSLP_LE = 7
Global Const XSLP_EQ = 8
Global Const XSLP_GE = 9
Global Const XSLP_FUNCTION = 10
Global Const XSLP_NULL = 32

Global Const XSLP_COMMA = 1
Global Const XSLP_COLON = 2
Global Const XSLP_EQUALS = 11

'****************************************************************************
' Problem status settings
'****************************************************************************
Global Const XSLP_MADECONSTRUCT = 1
Global Const XSLP_MADECASCADE = 2
Global Const XSLP_SYNCHRONISED = 4
Global Const XSLP_NODELTAZ = 8
Global Const XSLP_BASECALC = 16
Global Const XSLP_DELTACALC = 32
Global Const XSLP_ONEOFFCALC = 64
Global Const XSLP_GLOBALINIT = 128
Global Const XSLP_GLOBALOPT = 256
Global Const XSLP_ENDGLOBALOPT = 512
Global Const XSLP_GLOBALTOCONVERGE = 1024
Global Const XSLP_FIXEDERRORS = 2048
Global Const XSLP_PRESOLVED = 4096
Global Const XSLP_FUNCTIONERROR = 8192
Global Const XSLP_GLOBALSTART = 16384
Global Const XSLP_BOUNDSRESTORED = 32768
Global Const XSLP_SWITCHEDTOPRIMAL = 65536
Global Const XSLP_ERRORCOSTSET = 131072
Global Const XSLP_ENDMIPWITHINSLP = 262144
Global Const XSLP_NODE0 = 524288
Global Const XSLP_PVRUN = 1048576
Global Const XSLP_REMAKE = 2097152

'****************************************************************************
'  XSLP_STATUS settings
'****************************************************************************
Global Const XSLP_STATUS_CONVERGEDOBJUCC = 1
Global Const XSLP_STATUS_CONVERGEDOBJSBX = 2
Global Const XSLP_STATUS_LPINFEASIBLE = 4
Global Const XSLP_STATUS_LPUNFINISHED = 8
Global Const XSLP_STATUS_MAXSLPITERATIONS = 16
Global Const XSLP_STATUS_INTEGERINFEASIBLE = 32
Global Const XSLP_STATUS_RESIDUALPENALTIES = 64
Global Const XSLP_STATUS_CONVERGEDOBJOBJ = 128
Global Const XSLP_STATUS_CONVERGEDOBJSLOPE = 256
Global Const XSLP_STATUS_MAXTIME = 512
Global Const XSLP_STATUS_USER = 1024

'****************************************************************************
' Misc settings
'****************************************************************************
Global Const XSLP_RECALC = 8
Global Const XSLP_TOLCALC = 16
Global Const XSLP_ALLCALCS = 32
Global Const XSLP_2DERIVATIVE = 64
Global Const XSLP_1DERIVATIVE = 128
Global Const XSLP_ALLDERIVATIVES = 256
Global Const XSLP_CDECL = 256
Global Const XSLP_STDCALL = 0
Global Const XSLP_INSTANCEFUNCTION = 512
Global Const XSLP_NETCALL = 1024
Global Const XSLP_DEDUCECONSTDERIVS = 2048
Global Const XSLP_SOMECONSTDERIVS = 4096
Global Const XSLP_MULTIVALUED = 16777216
Global Const XSLP_MOSELARRAYARGS = 33554432
Global Const XSLP_ROWARG = 67108864
Global Const XSLP_COLARG = 134217728
Global Const XSLP_NODERIVATIVES = 268435456
Global Const XSLP_THREADSAFE = 536870912
Global Const XSLP_LOGICALTHREAD = 1073741824

Global Const XSLP_PRESOLVEFIXZERO = 2
Global Const XSLP_PRESOLVEFIXALL = 4
Global Const XSLP_PRESOLVESETBOUNDS = 8
Global Const XSLP_PRESOLVEINTBOUNDS = 16
Global Const XSLP_NOPRESOLVECOEFFICIENTS = 256
Global Const XSLP_NOPRESOLVEDELTAS = 512

Global Const XSLP_FUNCINFOSIZE = 12
Global Const XSLPINFO_CALLERFLAG = 1
Global Const XSLPINFO_NINPUT = 2
Global Const XSLPINFO_NOUTPUT = 3
Global Const XSLPINFO_NDELTA = 4
Global Const XSLPINFO_NINSTRING = 5
Global Const XSLPINFO_NOUTSTRING = 6
Global Const XSLPINFO_FUNCNUM = 7
Global Const XSLPINFO_INSTANCE = 8
Global Const XSLPINFO_THREAD = 9
Global Const XSLPINFO_LOGICALTHREAD = 10

Global Const XSLP_GETUFNAME = 31
Global Const XSLP_GETUFPARAM1 = 32
Global Const XSLP_GETUFPARAM2 = 33
Global Const XSLP_GETUFPARAM3 = 34
Global Const XSLP_GETUFARGTYPE = 35
Global Const XSLP_GETUFEXETYPE = 36
Global Const XSLP_GETUFLTHREAD = 37

Global Const XSLP_SETUFNAME = 41
Global Const XSLP_SETUFPARAM1 = 42
Global Const XSLP_SETUFPARAM2 = 43
Global Const XSLP_SETUFPARAM3 = 44
Global Const XSLP_SETUFARGTYPE = 45
Global Const XSLP_SETUFEXETYPE = 46
Global Const XSLP_SETUFLTHREAD = 47

Global Const XSLP_GETPVNODES = 101 
Global Const XSLP_GETPVBESTNODE = 102
Global Const XSLP_GETPVBESTVALUE = 103
Global Const XSLP_GETPVSENSE = 104

Global Const XSLP_GETROWNUMPENALTYERRORS = 201
Global Const XSLP_GETROWMAXPENALTYERROR = 202
Global Const XSLP_GETROWTOTALPENALTYERROR = 203
Global Const XSLP_GETROWAVERAGEPENALTYERROR = 204
Global Const XSLP_GETROWCURRENTPENALTYERROR = 205
Global Const XSLP_GETROWCURRENTPENALTYFACTOR = 206
Global Const XSLP_SETROWPENALTYFACTOR = 207
Global Const XSLP_GETROWPENALTYCOLUMN1 = 208
Global Const XSLP_GETROWPENALTYCOLUMN2 = 209

Global Const XSLP_CASCADE_COEF_VAR = 2
Global Const XSLP_CASCADE_ALL_COEF_VAR = 4
Global Const XSLP_CASCADE_STRUCT_VAR = 8
Global Const XSLP_CASCADE_ALL_STRUCT_VAR = 16

Global Const XSLP_PARMTYP_INT = 1
Global Const XSLP_PARMTYP_DBL = 2
Global Const XSLP_PARMTYP_STR = 4
Global Const XSLP_PARMTYP_REF = 64
Global Const XSLP_PARMTYP_MEM = 129

Global Const XSLP_PARMTYP_READ = 8
Global Const XSLP_PARMTYP_WRITE = 16
Global Const XSLP_PARMTYP_CONTROL = 32

'***************************************************************************
' AddNames values                                                          
'*************************************************************************** 

Global Const XSLP_CVNAMES = 3
Global Const XSLP_XVNAMES = 4
Global Const XSLP_USERFUNCNAMES = 5
Global Const XSLP_INTERNALFUNCNAMES = 6
Global Const XSLP_USERFUNCNAMESNOCASE = 7
Global Const XSLP_INTERNALFUNCNAMESNOCASE = 8


'****************************************************************************
' PRIVATE for internal use, dont call
'****************************************************************************
Private Declare Function xslp_getlasterror Lib "xslp" Alias "_XSLPgetlasterror@12" (ByVal Prob As Long, ByRef Code As Long, ByVal Buffer As String) As Long
Private Declare Function xslp_getbanner Lib "xslp" Alias "_XSLPgetbanner@4" (ByVal Buffer As String) As Long
Private Declare Function xslp_getstrattrib Lib "xslp" Alias "_XSLPgetstrattrib@12" (ByVal Prob As Long, ByVal Param As Long, ByVal cValue As String) As Long
Private Declare Function xslp_getstrcontrol Lib "xslp" Alias "_XSLPgetstrcontrol@12" (ByVal Prob As Long, ByVal Param As Long, ByVal cValue As String) As Long
Private Declare Function xslp_getstring Lib "xslp" Alias "_XSLPgetstring@12" (ByVal Prob As Long, ByVal Param As Long, ByVal cValue As String) As Long
Private Declare Function xslp_preparseformula Lib "xslp" Alias "_XSLPpreparseformula@28" (ByVal Prob As Long, ByVal Formula As String, ByRef nToken As Long, ByRef theType As Long, ByRef Value As Double, ByVal StringTable As Long, ByRef sizetable As Long) As Long
Private Declare Function xslp_getnames Lib "xslp" Alias "_XSLPgetnames@20" (ByVal Prob As Long, ByVal theType As Long, ByVal cnames As String, ByVal first As Long, ByVal last As Long) As Long
Private Declare Function xslp_getcvar Lib "xslp" Alias "_XSLPgetcvar@12" (ByVal Prob As Long, ByVal nSLPCVar As Long, ByVal cValue As String) As Long
Private Declare Function xslp_getuserfunc Lib "xslp" Alias "_XSLPgetuserfunc@32" (ByVal Prob As Long, ByVal nSLPUserFunc As Long, ByVal xName As String, ByRef argtype As Long, ByRef exetype As Long, ByVal param1 As String, ByVal param2 As String, ByVal param3 As String) As Long
Private Declare Function xslp_formatvalue Lib "xslp" Alias "_XSLPformatvalue@12" (ByVal dValue As Double, ByVal Buffer As String) As Long
Private Declare Function xslp_itemname Lib "xslp" Alias "_XSLPitemname@20" (ByVal Prob As Long, ByVal theType As Long, ByVal Value As Double, ByVal Buffer As String) As Long
Private Declare Function xslp_validformula Lib "xslp" Alias "_XSLPvalidformula@20" (ByRef inType As Long, ByRef inValue As Double, ByRef nToken As Long, ByVal Name As String, ByVal StringTable As String) As Long
Private Declare Sub XPRadr2str Lib "xprvb" Alias "_xvbadr2str@12" (ByVal adrstr As Long, ByVal rts As String, ByVal rtslen As Long)

Private Declare Function xslp_controlname Lib "xslp" Alias "_XSLPcontrolname@12" (ByVal Param As Long, ByVal Name As String, ByRef theType As Long) As Long
Private Declare Function xslp_getparam Lib "xslp" Alias "_XSLPgetparam@12" (ByVal Prob As Long, ByVal Name As String, ByVal Value As String) As Long
Private Declare Function xslp_nextparam Lib "xslp" Alias "_XSLPnextparam@16" (ByVal Name As String, ByRef Code As Long, ByRef theType As Long, ByRef refx As Long) As Long
' END PRIVATE



'****************************************************************************
' Wrapper function to sweeten the VB<->C transition
'****************************************************************************
Public Function XSLPgetnames(ByVal Prob As Long, ByVal theType As Long, ByRef cnames As String, ByVal first As Long, ByVal last As Long) As Long
  Dim Buffer As String * HUGEBUFFERSIZE
  Dim i As Integer
  
  XSLPgetnames = xslp_getnames(Prob, theType, Buffer, first, last)
  
  ' multiple null terminated strings so gotta search manually for end of string
  ' quick check.. if the string starts with a null then we certainly didnt get anything back
  i = 0
  
  If Not Mid(Buffer, 1, 1) = vbNullChar Then
    i = HUGEBUFFERSIZE
    While (i > 1 And (Mid(Buffer, i, 1) = vbNullChar))
      i = i - 1
    Wend
  End If
  
  If i > 1 Then
      cnames = Left(Buffer, i + 1)
  End If
End Function


Public Function XSLPpreparseformula(ByVal Prob As Long, ByVal Formula As String, ByRef nToken As Long, ByRef theType() As Long, ByRef Value() As Double, ByRef StringTable As Long, ByRef sizetable As Long) As Long
  ' unfortunately we dont have any idea how long the returned arrays will be
  ' so we need to err on the cautious side
  Dim theType_origsize As Integer
  Dim Value_origsize As Integer
  
  theType_origsize = UBound(theType)
  Value_origsize = UBound(Value)
  
  ReDim theType(HUGEBUFFERSIZE)
  ReDim Value(HUGEBUFFERSIZE)
  
  Dim buffer_stringtable As String * HUGEBUFFERSIZE
  
  XSLPpreparseformula = xslp_preparseformula(Prob, Formula, nToken, theType(0), Value(0), buffer_stringtable, sizetable)
  
  If XSLPpreparseformula = 0 And nToken > 0 Then
    ReDim theType(nToken - 1)
    ReDim Value(nToken - 1)
  
    Dim i As Integer
    i = InStr(1, buffer_stringtable, vbNullChar)
    If i > 1 Then
        StringTable = Left(buffer_stringtable, InStr(1, buffer_stringtable, vbNullChar) - 1)
    End If
  Else
    ReDim theType(theType_origsize)
    ReDim Value(Value_origsize)
  End If
  
End Function


Public Function XSLPgetuserfunc(ByVal Prob As Long, ByVal nSLPUserFunc As Long, ByVal xName As String, ByRef argtype As Long, ByRef exetype As Long, ByVal param1 As String, ByVal param2 As String, ByVal param3 As String) As Long
  Dim buffer1 As String * BUFFERSIZE
  Dim buffer2 As String * BUFFERSIZE
  Dim buffer3 As String * BUFFERSIZE
  Dim buffer4 As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPgetuserfunc = xslp_getuserfunc(Prob, nSLPUserFunc, buffer1, argtype, exetype, buffer2, buffer3, buffer4)
  
  i = InStr(1, buffer1, vbNullChar)
  If i > 1 Then
      xName = Left(buffer1, InStr(1, buffer1, vbNullChar) - 1)
  End If
  
  i = InStr(1, buffer2, vbNullChar)
  If i > 1 Then
      param1 = Left(buffer2, InStr(1, buffer2, vbNullChar) - 1)
  End If
  
  i = InStr(1, buffer3, vbNullChar)
  If i > 1 Then
      param2 = Left(buffer3, InStr(1, buffer3, vbNullChar) - 1)
  End If
  
  i = InStr(1, buffer4, vbNullChar)
  If i > 1 Then
      param3 = Left(buffer4, InStr(1, buffer4, vbNullChar) - 1)
  End If

End Function


Public Function XSLPvalidformula(ByRef inType As Long, ByRef inValue As Double, ByRef nToken As Long, ByRef Name As String, ByVal StringTable As String) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPvalidformula = xslp_validformula(inType, inValue, nToken, Buffer, StringTable)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      Name = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function

Public Function XSLPitemname(ByVal Prob As Long, ByVal theType As Long, ByVal Value As Double, ByRef itemname As String) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPitemname = xslp_itemname(Prob, theType, Value, Buffer)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      itemname = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function


Public Function XSLPgetcvar(ByVal Prob As Long, ByVal nSLPCVar As Long, ByRef cValue As String) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPgetcvar = xslp_getcvar(Prob, nSLPCVar, Buffer)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      cValue = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function

Public Function XSLPformatvalue(ByVal dValue As Double, ByRef formatted As String) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPformatvalue = xslp_formatvalue(dValue, Buffer)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      formatted = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function


Public Function XSLPgetbanner(ByRef banner As String) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPgetbanner = xslp_getbanner(Buffer)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      banner = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function

Public Function XSLPgetstrattrib(ByVal Prob As Long, ByVal Param As Long, ByRef cValue As String) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPgetstrattrib = xslp_getstrattrib(Prob, Param, Buffer)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      cValue = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function

Public Function XSLPgetstrcontrol(ByVal Prob As Long, ByVal Param As Long, ByRef cValue As String) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPgetstrcontrol = xslp_getstrcontrol(Prob, Param, Buffer)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      cValue = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function

Public Function XSLPgetstring(ByVal Prob As Long, ByVal Param As Long, ByRef cValue As String) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPgetstring = xslp_getstring(Prob, Param, Buffer)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      cValue = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function

Public Function XSLPgetlasterror(ByVal Prob As Long, ByRef Code As Long, ByRef cValue As String) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPgetlasterror = xslp_getlasterror(Prob, Code, Buffer)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      cValue = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function

Public Function XSLPtoVBString(adrstr As Long) As String
 Dim rts As String
 
 If (adrstr = 0) Then
  XSLPtoVBString = ""
 Else
  rts = String$(255, vbNullChar)
  XPRadr2str adrstr, rts, Len(rts)
  XSLPtoVBString = Left(rts, InStr(1, rts, vbNullChar) - 1)
 End If
End Function

Public Function XSLPcontrolname(ByVal Param As Long, ByRef Name As String, ByRef theType As Long) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPcontrolname = xslp_controlname(Param, Buffer, theType)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      Name = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function

Public Function XSLPgetparam(ByVal Prob As Long, ByVal Name As String, ByRef Value As String) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPgetparam = xslp_getparam(Prob, Name, Buffer)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      Value = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function

Public Function XSLPnextparam (ByRef Name As String, ByRef Code As Long, ByRef theType As Long, ByRef refx As Long) As Long
  Dim Buffer As String * BUFFERSIZE
  Dim i As Integer
  
  XSLPnextparam = xslp_nextparam(Buffer, Code, theType, refx)
  i = InStr(1, Buffer, vbNullChar)
  If i > 1 Then
      Name = Left(Buffer, InStr(1, Buffer, vbNullChar) - 1)
  End If
End Function


