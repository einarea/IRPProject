Attribute VB_Name = "XPRM"


' Function and Constant declarations for use with
' Xpress Mosel Compiler and Runtime libraries
' (c) Copyright Fair Isaac Corporation 2002-2015. All rights reserved.

Option Explicit
Option Compare Text


' ----------------------------------------------------------------------------------
' Constants

Global Const XPRM_VERSION = "4.0.0"

#If (VBA7 <> 0) And (Win64 <> 0) Then
' this is the new 64-bit type
Global Const XPRM_NULL As LongPtr = 0
Global Const XPRM_PTR_VARTYPE = vbLongLong
#Else
Global Const XPRM_NULL As Long = 0
Global Const XPRM_PTR_VARTYPE = vbLong
#End If



' Possible types
Global Const XPRM_TYP_NOT = 0
Global Const XPRM_TYP_INT = 1
Global Const XPRM_TYP_REAL = 2
Global Const XPRM_TYP_STRING = 3
Global Const XPRM_TYP_BOOL = 4
Global Const XPRM_TYP_MPVAR = 5
Global Const XPRM_TYP_LINCTR = 6
Global Const XPRM_TYP_ARR = 7
Global Const XPRM_TYP_SET = 8
Global Const XPRM_TYP_EXTN = &HFFF

' Possible structures
Global Const XPRM_STR_CONST = 0
Global Const XPRM_STR_REF = 4096
Global Const XPRM_STR_ARR = 8192
Global Const XPRM_STR_SET = 12288
Global Const XPRM_STR_LIST = 16384
Global Const XPRM_STR_PROC = 20480
Global Const XPRM_STR_MEM = 28672
Global Const XPRM_STR_UTYP = 32768
Global Const XPRM_STR_NTYP = 36864
Global Const XPRM_STR_REC = 40960
Global Const XPRM_STR_PROB = 45056

' Possible set/array structure
Global Const XPRM_GRP_FIX = 0
Global Const XPRM_GRP_DYN = 131072
Global Const XPRM_GRP_RNG = 0
Global Const XPRM_GRP_GEN = 262144
Global Const XPRM_ARR_DENSE = XPRM_GRP_FIX And XPRM_GRP_RNG
Global Const XPRM_ARR_SPARSE = XPRM_GRP_DYN And XPRM_GRP_GEN

' DSO parameters coding read/write
Global Const XPRM_CPAR_READ = 4096
Global Const XPRM_CPAR_WRITE = 8192

' DSO type properties encoding
Global Const XPRM_MTP_CREAT = 1
Global Const XPRM_MTP_DELET = 2
Global Const XPRM_MTP_TOSTR = 4
Global Const XPRM_MTP_FRSTR = 8
Global Const XPRM_MTP_PRTBL = 16
Global Const XPRM_MTP_RFCNT = 32
Global Const XPRM_MTP_COPY = 64
Global Const XPRM_MTP_APPND = 128
Global Const XPRM_MTP_ORSET = 256
Global Const XPRM_MTP_PROB = 512
Global Const XPRM_MTP_CMP = 1024

' Mask
Global Const XPRM_MSK_TYP = &HFFF
Global Const XPRM_MSK_STR = &H1F000
Global Const XPRM_MSK_GRP = &H60000
Global Const XPRM_MSK_FIN = &H80000
Global Const XPRM_MSK_LOC = &HFFF00000

' Problem status
Global Const XPRM_PBSOL = 1   '  A solution is available
Global Const XPRM_PBOPT = 2   ' Optimal solution found
Global Const XPRM_PBUNF = 4   ' Optimisation unfinished
Global Const XPRM_PBINF = 6   ' Problem infeasible
Global Const XPRM_PBUNB = 8   ' Problem is unbounded
Global Const XPRM_PBOTH = 10  ' Optimisation failed (cutoff)
Global Const XPRM_PBRES = 14
Global Const XPRM_PBCHG = 16  ' modified since last matrix generation
Global Const XPRM_PBOBJ = 32  ' objective modified since last generation

' Returned values from "runmodel"
Global Const XPRM_RT_OK = 0
Global Const XPRM_RT_INSTR = 1     ' Invalid instruction
Global Const XPRM_RT_MATHERR = 3   ' Math error
Global Const XPRM_RT_UNKN_PF = 5   ' Call to an unknown procedure/function
Global Const XPRM_RT_UNKN_SYS = 9  ' Call to an unknown system function
Global Const XPRM_RT_PROB = 10     ' Error when opening/closing a problem
Global Const XPRM_RT_ERROR = 11    ' Runtime Error code
Global Const XPRM_RT_EXIT = 12     ' Termination via exit(code) [callproc only]
Global Const XPRM_RT_IOERR = 13    ' IO Error code
Global Const XPRM_RT_BREAK = 14    ' Stopped on a breakpoint (debugger only)
Global Const XPRM_RT_NIFCT = 15    ' Stopped in native function (debugger only)
Global Const XPRM_RT_NULL = 16     ' NULL reference
Global Const XPRM_RT_LICERR = 17   ' License error
Global Const XPRM_RT_STOP = 128    ' Stopped

' IO codes
Global Const XPRM_F_TEXT = 0
Global Const XPRM_F_BINARY = 1
Global Const XPRM_F_READ = 0
Global Const XPRM_F_INPUT = 0
Global Const XPRM_F_WRITE = 2
Global Const XPRM_F_OUTPUT = 2
Global Const XPRM_F_APPEND = 4
Global Const XPRM_F_ERROR = 8
Global Const XPRM_F_LINBUF = 16
Global Const XPRM_F_INIT = 32
Global Const XPRM_F_SILENT = 64

' Return values for debugger
Global Const XPRM_DBG_STOP = -3
Global Const XPRM_DBG_NEXT = -2
Global Const XPRM_DBG_STEP = -1
Global Const XPRM_DBG_CONT = 0

' Properties for get[mod|dso]prop
Global Const XPRM_PROP_NAME = 0
Global Const XPRM_PROP_ID = 1
Global Const XPRM_PROP_VERSION = 2
Global Const XPRM_PROP_SYSCOM = 3
Global Const XPRM_PROP_USRCOM = 4
Global Const XPRM_PROP_SIZE = 5
Global Const XPRM_PROP_NBREF = 6
Global Const XPRM_PROP_DATE = 7
Global Const XPRM_PROP_PATH = 8
Global Const XPRM_PROP_IMCI = 9
Global Const XPRM_PROP_PRIORITY = 10
Global Const XPRM_PROP_SECSTAT = 11
Global Const XPRM_PROP_SKEYFP = 12

' Model security status decoding
Global Const XPRM_SECSTAT_CRYPTED = 1
Global Const XPRM_SECSTAT_SIGNED = 2
Global Const XPRM_SECSTAT_VERIFIED = 4
Global Const XPRM_SECSTAT_UNVERIFIED = 8

' Properties for gettypeprop
Global Const XPRM_TPROP_NAME = 0
Global Const XPRM_TPROP_FEAT = 1
Global Const XPRM_TPROP_EXP = 2
Global Const XPRM_TPROP_BID = 3

' Control characters for CB init
Global Const XPRM_CBC_SKIP = 0
Global Const XPRM_CBC_OPENLST = 1
Global Const XPRM_CBC_CLOSELST = 2
Global Const XPRM_CBC_OPENNDX = 3
Global Const XPRM_CBC_CLOSENDX = 4



' ----------------------------------------------------------------------------------
' DLL stubs

#If (VBA7 <> 0) And (Win64 <> 0) Then

' Utilities
Declare PtrSafe Function XPRMreadcstring Lib "xprmvb" Alias "vbXPRMreadcstring" (ByVal stringptr As LongPtr) As String

' Model functions
Declare PtrSafe Function XPRMcompmod Lib "xprm_mc" (ByVal options As String, ByVal srcfile As String, ByVal dstfile As String, ByVal userc As String) As Long
Declare PtrSafe Function XPRMcompmodsec Lib "xprm_mc" (ByVal options As String, ByVal srcfile As String, ByVal dstfile As String, ByVal userc As String, ByVal passfile As String, ByVal privkey As String, ByVal kfile As String) As Long
Declare PtrSafe Function XPRMexecmod Lib "xprm_mc" (ByVal options As String, ByVal srcfile As String, ByVal parlist As String, ByRef returned As Long, Optional ByRef rtmod As LongPtr = 0) As Long
Declare PtrSafe Function XPRMfindmod Lib "xprm_rt" (ByVal name As String, ByVal number As Long) As LongPtr
Declare PtrSafe Function XPRMgetnextmod Lib "xprm_rt" (ByVal model As LongPtr) As LongPtr
Declare PtrSafe Function XPRMrunmod Lib "xprm_rt" (ByVal model As LongPtr, ByRef returned As Long, ByVal parlist As String) As Long
Declare PtrSafe Sub XPRMresetmod Lib "xprm_rt" (ByVal model As LongPtr)
Declare PtrSafe Function XPRMisrunmod Lib "xprm_rt" (ByVal model As LongPtr) As Long
Declare PtrSafe Sub XPRMstoprunmod Lib "xprm_rt" (ByVal model As LongPtr)
Declare PtrSafe Function XPRMloadmod Lib "xprm_rt" (ByVal bname As String, ByVal intname As String) As LongPtr
Declare PtrSafe Function XPRMloadmodsec Lib "xprm_rt" (ByVal bname As String, ByVal intname As String, ByVal flags As String, ByVal passfile As String, ByVal privkey As String, ByVal keys As String) As LongPtr
Declare PtrSafe Function XPRMunloadmod Lib "xprm_rt" (ByVal model As LongPtr) As Long
Declare PtrSafe Function XPRMgetmodprop Lib "xprmvb" Alias "vbXPRMgetmodprop" (ByVal model As LongPtr, ByVal what As Long, ByRef result As Variant) As Long
Declare PtrSafe Function XPRMgetmodversion Lib "xprmvb" Alias "vbXPRMgetmodversion" (ByVal model As LongPtr, ByVal what As Long, ByRef result As Variant) As Long


Declare PtrSafe Function XPRMgetdllpath Lib "xprmvb" Alias "vbXPRMgetlibpath" () As String
Declare PtrSafe Function XPRMgetlibpath Lib "xprmvb" Alias "vbXPRMgetlibpath" () As String
Declare PtrSafe Sub XPRMfreelibpath Lib "xprm_rt" ()

' Dictionary access
Declare PtrSafe Function XPRMfindident Lib "xprmvb" Alias "vbXPRMfindident" (ByVal model As LongPtr, ByVal text As String, ByRef value As Variant) As Long
Declare PtrSafe Function XPRMgetnextident Lib "xprmvb" Alias "vbXPRMgetnextident" (ByVal model As LongPtr, ByRef ref As LongPtr) As String
Declare PtrSafe Function XPRMgetnextparam Lib "xprmvb" Alias "vbXPRMgetnextparam" (ByVal model As LongPtr, ByRef ref As LongPtr) As String
Declare PtrSafe Function XPRMgetnextdep Lib "xprmvb" Alias "vbXPRMgetnextdep" (ByVal model As LongPtr, ByVal ref As LongPtr, ByRef name As String, ByRef version As Long, ByRef dso_pkg As Long) As LongPtr
Declare PtrSafe Function XPRMgetnextreq Lib "xprmvb" Alias "vbXPRMgetnextreq" (ByVal model As LongPtr, ByVal ref As LongPtr, ByRef name As String, ByRef typecode As Long, ByRef data As LongPtr) As LongPtr
Declare PtrSafe Function XPRMgetnextpbcomp Lib "xprm_rt" (ByVal model As LongPtr, ByVal ref As LongPtr, ByVal code As Long, ByRef typecode As Long) As LongPtr
Declare PtrSafe Function XPRMselectprob Lib "xprm_rt" (ByVal model As LongPtr, ByVal code As Long, ByVal pb As LongPtr) As Long

' Procedures/functions
Declare PtrSafe Function XPRMgetprocinfo Lib "xprmvb" Alias "vbXPRMgetprocinfo" (ByVal proc As LongPtr, ByRef partyp As String, ByRef nbpar As Long, ByRef ftype As Long) As Long
Declare PtrSafe Function XPRMgetnextproc Lib "xprmvb" Alias "vbXPRMgetnextproc" (ByVal proc As LongPtr) As LongPtr

' Set access
Declare PtrSafe Function XPRMgetsetsize Lib "xprmvb" Alias "vbXPRMgetsetsize" (ByVal mmset As LongPtr) As Long
Declare PtrSafe Function XPRMgetsettype Lib "xprmvb" Alias "vbXPRMgetsettype" (ByVal mmset As LongPtr) As Long
Declare PtrSafe Function XPRMgetfirstsetndx Lib "xprmvb" Alias "vbXPRMgetfirstsetndx" (ByVal mmset As LongPtr) As Long
Declare PtrSafe Function XPRMgetlastsetndx Lib "xprmvb" Alias "vbXPRMgetlastsetndx" (ByVal mmset As LongPtr) As Long
Declare PtrSafe Sub XPRMgetelsetval Lib "xprmvb" Alias "vbXPRMgetelsetval" (ByVal mmset As LongPtr, ByVal ndx As Long, ByRef value As Variant)
Declare PtrSafe Function XPRMgetelsetndx Lib "xprmvb" Alias "vbXPRMgetelsetndx" (ByVal model As LongPtr, ByVal mmset As LongPtr, ByRef value As Variant) As Long

' List access
Declare PtrSafe Function XPRMgetlistsize Lib "xprmvb" Alias "vbXPRMgetlistsize" (ByVal list As LongPtr) As Long
Declare PtrSafe Function XPRMgetlisttype Lib "xprmvb" Alias "vbXPRMgetlisttype" (ByVal list As LongPtr) As Long
Declare PtrSafe Function XPRMgetprevlistelt Lib "xprmvb" Alias "vbXPRMgetprevlistelt" (ByVal list As LongPtr, ByVal ref As LongPtr, ByRef typeref As Long, ByRef value As Variant) As LongPtr
Declare PtrSafe Function XPRMgetnextlistelt Lib "xprmvb" Alias "vbXPRMgetnextlistelt" (ByVal list As LongPtr, ByVal ref As LongPtr, ByRef typeref As Long, ByRef value As Variant) As LongPtr

' Array access
Declare PtrSafe Function XPRMgetarrval Lib "xprmvb" Alias "vbXPRMgetarrval" (ByVal mmarray As LongPtr, ByRef indices() As Long, ByRef adr As Variant) As Long
Declare PtrSafe Sub XPRMgetarrsets Lib "xprmvb" Alias "vbXPRMgetarrsets" (ByVal mm_array As LongPtr, ByRef sets() As LongPtr)
Declare PtrSafe Function XPRMgetarrdim Lib "xprmvb" Alias "vbXPRMgetarrdim" (ByVal mmarray As LongPtr) As Long
Declare PtrSafe Function XPRMgetarrtype Lib "xprmvb" Alias "vbXPRMgetarrtype" (ByVal mmarray As LongPtr) As Long
Declare PtrSafe Function XPRMgetarrsize Lib "xprmvb" Alias "vbXPRMgetarrsize" (ByVal mmarray As LongPtr) As Long
Declare PtrSafe Function XPRMgetfirstarrentry Lib "xprmvb" Alias "vbXPRMgetfirstarrentry" (ByVal mmarray As LongPtr, ByRef indices() As Long) As Long
Declare PtrSafe Function XPRMgetfirstarrtruentry Lib "xprmvb" Alias "vbXPRMgetfirstarrtruentry" (ByVal mmarray As LongPtr, ByRef indices() As Long) As Long
Declare PtrSafe Function XPRMgetlastarrentry Lib "xprmvb" Alias "vbXPRMgetlastarrentry" (ByVal mmarray As LongPtr, ByRef indices() As Long) As Long
Declare PtrSafe Function XPRMgetnextarrentry Lib "xprmvb" Alias "vbXPRMgetnextarrentry" (ByVal mmarray As LongPtr, ByRef indices() As Long) As Long
Declare PtrSafe Function XPRMgetnextarrtruentry Lib "xprmvb" Alias "vbXPRMgetnextarrtruentry" (ByVal mmarray As LongPtr, ByRef indices() As Long) As Long
Declare PtrSafe Function XPRMchkarrind Lib "xprmvb" Alias "vbXPRMchkarrind" (ByVal mmarray As LongPtr, ByRef indices() As Long) As Long
Declare PtrSafe Function XPRMcmpindices Lib "xprmvb" Alias "vbXPRMcmpindices" (ByVal nbdim As Long, ByRef ind1() As Long, ByRef ind2() As Long) As Long

'Record access
Declare PtrSafe Function XPRMgetnextfield Lib "xprmvb" Alias "vbXPRMgetnextfield" (ByVal model As LongPtr, ByVal ref As LongPtr, ByVal code As Long, ByRef name As String, ByRef typecode As Long, ByRef number As Long) As LongPtr
' The VB version of XPRMgetfieldval takes an extra parameter (compared to the C version) telling it the type of the data in the field
Declare PtrSafe Sub XPRMgetfieldval Lib "xprmvb" Alias "vbXPRMgetfieldval" (ByVal model As LongPtr, ByVal recordtype As Long, ByVal recordptr As LongPtr, ByVal fieldnum As Long, ByVal fieldtype As Long, ByRef value As Variant)

' Solution
Declare PtrSafe Function XPRMgetvarnum Lib "xprmvb" Alias "vbXPRMgetvarnum" (ByVal model As LongPtr, ByVal var As LongPtr) As Long
Declare PtrSafe Function XPRMgetctrnum Lib "xprmvb" Alias "vbXPRMgetctrnum" (ByVal model As LongPtr, ByVal ctr As LongPtr) As Long
Declare PtrSafe Function XPRMgetprobstat Lib "xprmvb" Alias "vbXPRMgetprobstat" (ByVal model As LongPtr) As Long
Declare PtrSafe Function XPRMgetobjval Lib "xprmvb" Alias "vbXPRMgetobjval" (ByVal model As LongPtr) As Double
Declare PtrSafe Function XPRMgetvsol Lib "xprmvb" Alias "vbXPRMgetvsol" (ByVal model As LongPtr, ByVal var As LongPtr) As Double
Declare PtrSafe Function XPRMgetcsol Lib "xprmvb" Alias "vbXPRMgetcsol" (ByVal model As LongPtr, ByVal ctr As LongPtr) As Double
Declare PtrSafe Function XPRMgetrcost Lib "xprmvb" Alias "vbXPRMgetrcost" (ByVal model As LongPtr, ByVal var As LongPtr) As Double
Declare PtrSafe Function XPRMgetdual Lib "xprmvb" Alias "vbXPRMgetdual" (ByVal model As LongPtr, ByVal crt As LongPtr) As Double
Declare PtrSafe Function XPRMgetslack Lib "xprmvb" Alias "vbXPRMgetslack" (ByVal model As LongPtr, ByVal ctr As LongPtr) As Double
Declare PtrSafe Function XPRMgetact Lib "xprmvb" Alias "vbXPRMgetact" (ByVal model As LongPtr, ByVal ctr As LongPtr) As Double

' Miscellaneous
Declare PtrSafe Function XPRMinit Lib "xprm_rt" () As Long
Declare PtrSafe Function XPRMfinish Lib "xprmvb" Alias "vbXPRMfinish" () As Long
Declare PtrSafe Function XPRMfree Lib "xprmvb" Alias "vbXPRMfinish" () As Long
Declare PtrSafe Function XPRMgetlicerrmsg Lib "xprmvb" Alias "vbXPRMgetlicerrmsg" (ByRef msg As String) As Long
Declare PtrSafe Function XPRMlicense Lib "xprmvb" Alias "vbXPRMlicense" (ByRef oemnum As Long, ByRef oemmsg As String) As Long
Declare PtrSafe Function XPRMgetlicinfo Lib "xprmvb" Alias "vbXPRMgetlicinfo" () As String
Declare PtrSafe Sub XPRMsetlocaledir Lib "xprm_rt" (ByVal path As String)
Declare PtrSafe Function XPRMgetlocaledir Lib "xprmvb" Alias "vbXPRMgetlocaledir" () As String
Declare PtrSafe Function XPRMgetversion Lib "xprmvb" Alias "vbXPRMgetversion" () As String
Declare PtrSafe Function XPRMgetversions Lib "xprm_rt" (ByVal whichone As Long) As Long
Declare PtrSafe Function XPRMexportprob Lib "xprmvb" Alias "vbXPRMexportprob" (ByVal model As LongPtr, ByVal options As String, ByVal fname As String, ByVal obj As LongPtr) As Long

Declare PtrSafe Sub XPRMskipalldep Lib "xprmvb" Alias "vbXPRMskipalldep" (ByVal model As LongPtr, ByVal ref As LongPtr)

Declare PtrSafe Function XPRMgettypeprop Lib "xprmvb" Alias "vbXPRMgettypeprop" (ByVal model As LongPtr, ByVal ntyp As Long, ByVal what As Long, ByRef value As Variant) As Long
Declare PtrSafe Function XPRMfindtypecode Lib "xprm_rt" (ByVal model As LongPtr, ByVal typenme As String) As Long

' DSO module
Declare PtrSafe Function XPRMregstatdso Lib "xprm_rt" (ByVal name As String, ByVal dsoinit As LongPtr) As Long
Declare PtrSafe Function XPRMpreloaddso Lib "xprm_rt" (ByVal name As String) As LongPtr
Declare PtrSafe Function XPRMgetnextdsoproc Lib "xprmvb" Alias "vbXPRMgetnextdsoproc" (ByVal dso As LongPtr, ByVal ref As LongPtr, ByRef name As String, ByRef partyp As String, ByRef nbpar As Long, ByRef ftype As Long) As LongPtr
Declare PtrSafe Function XPRMgetnextdsoconst Lib "xprmvb" Alias "vbXPRMgetnextdsoconst" (ByVal dso As LongPtr, ByVal ref As LongPtr, ByRef name As String, ByRef ctype As Long, ByRef value As Variant) As LongPtr
Declare PtrSafe Function XPRMgetnextdsotype Lib "xprmvb" Alias "vbXPRMgetnextdsotype" (ByVal dso As LongPtr, ByVal ref As LongPtr, ByRef name As String, ByRef flag As Long) As LongPtr
Declare PtrSafe Function XPRMgetnextdsoparam Lib "xprmvb" Alias "vbXPRMgetnextdsoparam" (ByVal dso As LongPtr, ByVal ref As LongPtr, ByRef name As String, ByRef desc As String, ByRef ptype As Long) As LongPtr
Declare PtrSafe Function XPRMgetdsoparam Lib "xprmvb" Alias "vbXPRMgetdsoparam" (ByVal model As LongPtr, ByVal dso As LongPtr, ByVal name As String, ByRef ptype As Long, ByRef value As Variant) As Long
Declare PtrSafe Sub XPRMautounloaddso Lib "xprm_rt" (ByVal yesno As Long)
Declare PtrSafe Sub XPRMflushdso Lib "xprm_rt" ()
Declare PtrSafe Function XPRMgetnextdso Lib "xprm_rt" (ByVal dso As LongPtr) As LongPtr
Declare PtrSafe Function XPRMfinddso Lib "xprm_rt" (ByVal name As String) As LongPtr
Declare PtrSafe Sub XPRMsetdsopath Lib "xprm_rt" (ByVal path As String)
Declare PtrSafe Function XPRMgetdsopath Lib "xprmvb" Alias "vbXPRMgetdsopath" (ByRef path As String) As Long
Declare PtrSafe Function XPRMdsotyptostr Lib "xprmvb" Alias "vbXPRMdsotyptostr" (ByVal model As LongPtr, ByVal code As Long, ByVal value As LongPtr, ByRef str As String) As Long
Declare PtrSafe Function XPRMgetdsoprop Lib "xprmvb" Alias "vbXPRMgetdsoprop" (ByVal dso As LongPtr, ByVal what As Long, ByRef result As Variant) As Long

' Stream redirection
Declare PtrSafe Function XPRMsetdefstream Lib "xprmvb" Alias "vbXPRMsetdefstream" (ByVal model As LongPtr, ByVal wmd As Long, ByVal name As String) As Long

' Begin/end licensing
Declare PtrSafe Function XPRMbeginlicensing Lib "xprm_rt" (ByRef bIsLicensed As Long) As Long
Declare PtrSafe Function XPRMendlicensing Lib "xprm_rt" () As Long

' the functions below are now obsolete but are included for backwards compatibility
' type interrogating
Declare PtrSafe Function XPRM_TYP Lib "xprmvb" Alias "vbXPRM_TYP" (ByVal t As Long) As Long
Declare PtrSafe Function XPRM_STR Lib "xprmvb" Alias "vbXPRM_STR" (ByVal t As Long) As Long
Declare PtrSafe Function XPRM_GRP Lib "xprmvb" Alias "vbXPRM_GRP" (ByVal t As Long) As Long

' IO driver
Declare PtrSafe Function XPRM_IO_CB Lib "xprmvb" Alias "vbXPRM_IO_CB" (ByVal address As LongPtr) As String

' Callback-initialization IO driver
Declare PtrSafe Function XPRMcb_sendint Lib "xprm_rt" (ByVal cb As LongPtr, ByVal i As Long, ByVal bFlush As Long) As Long
Declare PtrSafe Function XPRMcb_sendreal Lib "xprm_rt" (ByVal cb As LongPtr, ByVal d As Double, ByVal bFlush As Long) As Long
Declare PtrSafe Function XPRMcb_sendstring Lib "xprm_rt" (ByVal cb As LongPtr, ByVal s As String, ByVal bFlush As Long) As Long
Declare PtrSafe Function XPRMcb_sendctrl Lib "xprm_rt" (ByVal cb As LongPtr, ByVal d As Byte, ByVal bFlush As Long) As Long


' Time functions
Declare PtrSafe Function XPRMdate2jdn Lib "xprmvb" Alias "vbXPRMdate2jdn" (ByVal y As Long, ByVal m As Long, ByVal d As Long) As Long
Declare PtrSafe Sub XPRMjdn2date Lib "xprmvb" Alias "vbXPRMjdn2date" (ByVal jd As Long, ByRef y As Long, ByRef m As Long, ByRef d As Long)
Declare PtrSafe Sub XPRMtime Lib "xprmvb" Alias "vbXPRMtime" (ByRef jdn As Long, ByRef t As Long, ByRef utc As Long)


' Debugging
Declare PtrSafe Function XPRMdbg_runmod Lib "xprm_rt" (ByVal model As LongPtr, ByRef returned As Long, ByVal parlist As String, ByVal dbgcb As LongPtr, ByVal dbgctx As LongPtr) As Long
Declare PtrSafe Function XPRMdbg_getnblndx Lib "xprm_rt" (ByVal model As LongPtr) As Long
Declare PtrSafe Function XPRMdbg_clearbrkp Lib "xprm_rt" (ByVal model As LongPtr, ByVal lindex As Long) As Long
Declare PtrSafe Function XPRMdbg_setbrkp Lib "xprm_rt" (ByVal model As LongPtr, ByVal lindex As Long) As Long
Declare PtrSafe Function XPRMdbg_setstacklev Lib "xprm_rt" (ByVal model As LongPtr, ByVal level As Long) As Long
Declare PtrSafe Function XPRMdbg_getlocation Lib "xprmvb" Alias "vbXPRMdbg_getlocation" (ByVal model As LongPtr, ByVal lindex As Long, ByRef line As Long, ByRef filename As String) As Long
Declare PtrSafe Function XPRMdbg_findproclndx Lib "xprm_rt" (ByVal model As LongPtr, ByVal proc As LongPtr) As Long

' Annotations
Declare PtrSafe Function XPRMgetnextanident Lib "xprmvb" Alias "vbXPRMgetnextanident" (ByVal model As LongPtr, ByRef ref As LongPtr) As String
Private Declare PtrSafe Function cXPRMgetannotations Lib "xprm_rt" Alias "XPRMgetannotations" (ByVal model As LongPtr, ByVal ident As LongPtr, ByVal prefix As LongPtr, ByRef annStrings As LongPtr, ByVal maxann As Long) As Long
Private Declare PtrSafe Function cXPRMgetdsoannotations Lib "xprm_rt" Alias "XPRMgetdsoannotations" (ByVal dso As LongPtr, ByVal prefix As LongPtr, ByRef annStrings As LongPtr, ByVal maxann As Long) As Long

Public Function XPRMgetannotations(ByVal model As Long, ByVal ident As String, ByVal prefix As String, ByRef annotations() As String, ByVal maxann As Long) As Long
    Dim i, nStrings As Long, nStringsFetched As Long
    Dim annptrs() As LongPtr
    ReDim annptrs(1 To maxann + 1)
    Dim identptr As LongPtr
    If ident = "" Then
        identptr = 0
    Else
        identptr = StrPtr(ident)
    End If
    
    nStrings = cXPRMgetannotations(model, identptr, StrPtr(prefix), annptrs(1), maxann)
    nStringsFetched = nStrings
    If nStringsFetched > maxann Then
        nStringsFetched = maxann
    End If
    
    If nStringsFetched > 0 Then
        For i = 1 To nStringsFetched
            annotations(LBound(annotations) + i - 1) = XPRMreadcstring(annptrs(i))
        Next
    End If
    XPRMgetannotations = nStrings
End Function

Public Function XPRMgetdsoannotations(ByVal dso As Long, ByVal prefix As String, ByRef annotations() As String, ByVal maxann As Long) As Long
    Dim i, nStrings As Long, nStringsFetched As Long
    Dim annptrs() As LongPtr
    ReDim annptrs(1 To maxann + 1)
    
    nStrings = cXPRMgetdsoannotations(model, StrPtr(prefix), annptrs(1), maxann)
    nStringsFetched = nStrings
    If nStringsFetched > maxann Then
        nStringsFetched = maxann
    End If
    
    If nStringsFetched > 0 Then
        For i = 1 To nStringsFetched
            annotations(LBound(annotations) + i - 1) = XPRMreadcstring(annptrs(i))
        Next
    End If
    XPRMgetannotations = nStrings
End Function

#Else

' Utilities
Declare Function XPRMreadcstring Lib "xprmvb" Alias "vbXPRMreadcstring" (ByVal stringptr As Long) As String

' Model functions
Declare Function XPRMcompmod Lib "xprm_mc" Alias "_XPRMcompmod@16" (ByVal options As String, ByVal srcfile As String, ByVal dstfile As String, ByVal userc As String) As Long
Declare Function XPRMcompmodsec Lib "xprm_mc" Alias "_XPRMcompmodsec@28" (ByVal options As String, ByVal srcfile As String, ByVal dstfile As String, ByVal userc As String, ByVal passfile As String, ByVal privkey As String, ByVal kfile As String) As Long
Declare Function XPRMexecmod Lib "xprm_mc" Alias "_XPRMexecmod@20" (ByVal options As String, ByVal srcfile As String, ByVal parlist As String, ByRef returned As Long, Optional ByRef rtmod As Long = 0) As Long
Declare Function XPRMfindmod Lib "xprm_rt" Alias "_XPRMfindmod@8" (ByVal name As String, ByVal number As Long) As Long
Declare Function XPRMgetnextmod Lib "xprm_rt" Alias "_XPRMgetnextmod@4" (ByVal model As Long) As Long
Declare Function XPRMrunmod Lib "xprm_rt" Alias "_XPRMrunmod@12" (ByVal model As Long, ByRef returned As Long, ByVal parlist As String) As Long
Declare Sub XPRMresetmod Lib "xprm_rt" Alias "_XPRMresetmod@4" (ByVal model As Long)
Declare Function XPRMisrunmod Lib "xprm_rt" Alias "_XPRMisrunmod@4" (ByVal model As Long) As Long
Declare Sub XPRMstoprunmod Lib "xprm_rt" Alias "_XPRMstoprunmod@4" (ByVal model As Long)
Declare Function XPRMloadmod Lib "xprm_rt" Alias "_XPRMloadmod@8" (ByVal bname As String, ByVal intname As String) As Long
Declare Function XPRMloadmodsec Lib "xprm_rt" Alias "_XPRMloadmodsec@24" (ByVal bname As String, ByVal intname As String, ByVal flags As String, ByVal passfile As String, ByVal privkey As String, ByVal keys As String) As Long
Declare Function XPRMunloadmod Lib "xprm_rt" Alias "_XPRMunloadmod@4" (ByVal model As Long) As Long
Declare Function XPRMgetmodprop Lib "xprmvb" Alias "vbXPRMgetmodprop" (ByVal model As Long, ByVal what As Long, ByRef result As Variant) As Long
Declare Function XPRMgetmodversion Lib "xprmvb" Alias "vbXPRMgetmodversion" (ByVal model As Long, ByVal what As Long, ByRef result As Variant) As Long

Declare Function XPRMgetdllpath Lib "xprmvb" Alias "vbXPRMgetlibpath" () As String
Declare Function XPRMgetlibpath Lib "xprmvb" Alias "vbXPRMgetlibpath" () As String
Declare Sub XPRMfreelibpath Lib "xprm_rt" Alias "_XPRMfreelibpath@0" ()


' Dictionary access
Declare Function XPRMfindident Lib "xprmvb" Alias "vbXPRMfindident" (ByVal model As Long, ByVal text As String, ByRef value As Variant) As Long
Declare Function XPRMgetnextident Lib "xprmvb" Alias "vbXPRMgetnextident" (ByVal model As Long, ByRef ref As Long) As String
Declare Function XPRMgetnextparam Lib "xprmvb" Alias "vbXPRMgetnextparam" (ByVal model As Long, ByRef ref As Long) As String
Declare Function XPRMgetnextdep Lib "xprmvb" Alias "vbXPRMgetnextdep" (ByVal model As Long, ByVal ref As Long, ByRef name As String, ByRef version As Long, ByRef dso_pkg As Long) As Long
Declare Function XPRMgetnextreq Lib "xprmvb" Alias "vbXPRMgetnextreq" (ByVal model As Long, ByVal ref As Long, ByRef name As String, ByRef typecode As Long, ByRef data As Long) As Long
Declare Function XPRMgetnextpbcomp Lib "xprm_rt" Alias "_XPRMgetnextbpcomp@16" (ByVal model As Long, ByVal ref As Long, ByVal code As Long, ByRef typecode As Long) As Long
Declare Function XPRMselectprob Lib "xprm_rt" Alias "_XPRMselectprob@12" (ByVal model As Long, ByVal code As Long, ByVal pb As Long) As Long

' Procedures/functions
Declare Function XPRMgetprocinfo Lib "xprmvb" Alias "vbXPRMgetprocinfo" (ByVal proc As Long, ByRef partyp As String, ByRef nbpar As Long, ByRef ftype As Long) As Long
Declare Function XPRMgetnextproc Lib "xprmvb" Alias "vbXPRMgetnextproc" (ByVal proc As Long) As Long

' Set access
Declare Function XPRMgetsetsize Lib "xprmvb" Alias "vbXPRMgetsetsize" (ByVal mmset As Long) As Long
Declare Function XPRMgetsettype Lib "xprmvb" Alias "vbXPRMgetsettype" (ByVal mmset As Long) As Long
Declare Function XPRMgetfirstsetndx Lib "xprmvb" Alias "vbXPRMgetfirstsetndx" (ByVal mmset As Long) As Long
Declare Function XPRMgetlastsetndx Lib "xprmvb" Alias "vbXPRMgetlastsetndx" (ByVal mmset As Long) As Long
Declare Sub XPRMgetelsetval Lib "xprmvb" Alias "vbXPRMgetelsetval" (ByVal mmset As Long, ByVal ndx As Long, ByRef value As Variant)
Declare Function XPRMgetelsetndx Lib "xprmvb" Alias "vbXPRMgetelsetndx" (ByVal model As Long, ByVal mmset As Long, ByRef value As Variant) As Long

' List access
Declare Function XPRMgetlistsize Lib "xprmvb" Alias "vbXPRMgetlistsize" (ByVal list As Long) As Long
Declare Function XPRMgetlisttype Lib "xprmvb" Alias "vbXPRMgetlisttype" (ByVal list As Long) As Long
Declare Function XPRMgetprevlistelt Lib "xprmvb" Alias "vbXPRMgetprevlistelt" (ByVal list As Long, ByVal ref As Long, ByRef typeref As Long, ByRef value As Variant) As Long
Declare Function XPRMgetnextlistelt Lib "xprmvb" Alias "vbXPRMgetnextlistelt" (ByVal list As Long, ByVal ref As Long, ByRef typeref As Long, ByRef value As Variant) As Long


' Array access
Declare Function XPRMgetarrval Lib "xprmvb" Alias "vbXPRMgetarrval" (ByVal mmarray As Long, ByRef indices() As Long, ByRef adr As Variant) As Long
Declare Sub XPRMgetarrsets Lib "xprmvb" Alias "vbXPRMgetarrsets" (ByVal mm_array As Long, ByRef sets() As Long)
Declare Function XPRMgetarrdim Lib "xprmvb" Alias "vbXPRMgetarrdim" (ByVal mmarray As Long) As Long
Declare Function XPRMgetarrtype Lib "xprmvb" Alias "vbXPRMgetarrtype" (ByVal mmarray As Long) As Long
Declare Function XPRMgetarrsize Lib "xprmvb" Alias "vbXPRMgetarrsize" (ByVal mmarray As Long) As Long
Declare Function XPRMgetfirstarrentry Lib "xprmvb" Alias "vbXPRMgetfirstarrentry" (ByVal mmarray As Long, ByRef indices() As Long) As Long
Declare Function XPRMgetfirstarrtruentry Lib "xprmvb" Alias "vbXPRMgetfirstarrtruentry" (ByVal mmarray As Long, ByRef indices() As Long) As Long
Declare Function XPRMgetlastarrentry Lib "xprmvb" Alias "vbXPRMgetlastarrentry" (ByVal mmarray As Long, ByRef indices() As Long) As Long
Declare Function XPRMgetnextarrentry Lib "xprmvb" Alias "vbXPRMgetnextarrentry" (ByVal mmarray As Long, ByRef indices() As Long) As Long
Declare Function XPRMgetnextarrtruentry Lib "xprmvb" Alias "vbXPRMgetnextarrtruentry" (ByVal mmarray As Long, ByRef indices() As Long) As Long
Declare Function XPRMchkarrind Lib "xprmvb" Alias "vbXPRMchkarrind" (ByVal mmarray As Long, ByRef indices() As Long) As Long
Declare Function XPRMcmpindices Lib "xprmvb" Alias "vbXPRMcmpindices" (ByVal nbdim As Long, ByRef ind1() As Long, ByRef ind2() As Long) As Long

' Record access
Declare Function XPRMgetnextfield Lib "xprmvb" Alias "vbXPRMgetnextfield" (ByVal model As Long, ByVal ref As Long, ByVal code As Long, ByRef name As String, ByRef typecode As Long, ByRef number As Long) As Long
' The VB version of XPRMgetfieldval takes an extra parameter (compared to the C version) telling it the type of the data in the field
Declare Sub XPRMgetfieldval Lib "xprmvb" Alias "vbXPRMgetfieldval" (ByVal model As Long, ByVal recordtype As Long, ByVal recordptr As Long, ByVal fieldnum As Long, ByVal fieldtype As Long, ByRef value As Variant)

' Solution
Declare Function XPRMgetvarnum Lib "xprmvb" Alias "vbXPRMgetvarnum" (ByVal model As Long, ByVal var As Long) As Long
Declare Function XPRMgetctrnum Lib "xprmvb" Alias "vbXPRMgetctrnum" (ByVal model As Long, ByVal ctr As Long) As Long
Declare Function XPRMgetprobstat Lib "xprmvb" Alias "vbXPRMgetprobstat" (ByVal model As Long) As Long
Declare Function XPRMgetobjval Lib "xprmvb" Alias "vbXPRMgetobjval" (ByVal model As Long) As Double
Declare Function XPRMgetvsol Lib "xprmvb" Alias "vbXPRMgetvsol" (ByVal model As Long, ByVal var As Long) As Double
Declare Function XPRMgetcsol Lib "xprmvb" Alias "vbXPRMgetcsol" (ByVal model As Long, ByVal ctr As Long) As Double
Declare Function XPRMgetrcost Lib "xprmvb" Alias "vbXPRMgetrcost" (ByVal model As Long, ByVal var As Long) As Double
Declare Function XPRMgetdual Lib "xprmvb" Alias "vbXPRMgetdual" (ByVal model As Long, ByVal crt As Long) As Double
Declare Function XPRMgetslack Lib "xprmvb" Alias "vbXPRMgetslack" (ByVal model As Long, ByVal ctr As Long) As Double
Declare Function XPRMgetact Lib "xprmvb" Alias "vbXPRMgetact" (ByVal model As Long, ByVal ctr As Long) As Double

' Miscellaneous
Declare Function XPRMinit Lib "xprm_rt" Alias "_XPRMinit@0" () As Long
Declare Function XPRMfinish Lib "xprmvb" Alias "vbXPRMfinish" () As Long
Declare Function XPRMfree Lib "xprmvb" Alias "vbXPRMfinish" () As Long
Declare Function XPRMgetlicerrmsg Lib "xprmvb" Alias "vbXPRMgetlicerrmsg" (ByRef msg As String) As Long
Declare Function XPRMlicense Lib "xprmvb" Alias "vbXPRMlicense" (ByRef oemnum As Long, ByRef oemmsg As String) As Long
Declare Function XPRMgetlicinfo Lib "xprmvb" Alias "vbXPRMgetlicinfo" () As String
Declare Sub XPRMsetlocaledir Lib "xprm_rt" Alias "_XPRMsetlocaledir@4" (ByVal path As String)
Declare Function XPRMgetlocaledir Lib "xprmvb" Alias "vbXPRMgetlocaledir" () As String
Declare Function XPRMgetversion Lib "xprmvb" Alias "vbXPRMgetversion" () As String
Declare Function XPRMgetversions Lib "xprm_rt" Alias "_XPRMgetversions@4" (ByVal whichone As Long) As Long
Declare Function XPRMexportprob Lib "xprmvb" Alias "vbXPRMexportprob" (ByVal model As Long, ByVal options As String, ByVal fname As String, ByVal obj As Long) As Long

Declare Sub XPRMskipalldep Lib "xprmvb" Alias "vbXPRMskipalldep" (ByVal model As Long, ByVal ref As Long)


Declare Function XPRMgettypeprop Lib "xprmvb" Alias "vbXPRMgettypeprop" (ByVal model As Long, ByVal ntyp As Long, ByVal what As Long, ByRef value As Variant) As Long
Declare Function XPRMfindtypecode Lib "xprm_rt" Alias "_XPRMfindtypecode@8" (ByVal model As Long, ByVal typenme As String) As Long

' DSO module
Declare Function XPRMregstatdso Lib "xprm_rt" Alias "_XPRMregstatdso@8" (ByVal name As String, ByVal dsoinit As Long) As Long
Declare Function XPRMpreloaddso Lib "xprm_rt" Alias "_XPRMpreloaddso@4" (ByVal name As String) As Long
Declare Function XPRMgetnextdsoproc Lib "xprmvb" Alias "vbXPRMgetnextdsoproc" (ByVal dso As Long, ByVal ref As Long, ByRef name As String, ByRef partyp As String, ByRef nbpar As Long, ByRef ftype As Long) As Long
Declare Function XPRMgetnextdsoconst Lib "xprmvb" Alias "vbXPRMgetnextdsoconst" (ByVal dso As Long, ByVal ref As Long, ByRef name As String, ByRef ctype As Long, ByRef value As Variant) As Long
Declare Function XPRMgetnextdsotype Lib "xprmvb" Alias "vbXPRMgetnextdsotype" (ByVal dso As Long, ByVal ref As Long, ByRef name As String, ByRef flag As Long) As Long
Declare Function XPRMgetnextdsoparam Lib "xprmvb" Alias "vbXPRMgetnextdsoparam" (ByVal dso As Long, ByVal ref As Long, ByRef name As String, ByRef desc As String, ByRef ptype As Long) As Long
Declare Function XPRMgetdsoparam Lib "xprmvb" Alias "vbXPRMgetdsoparam" (ByVal model As Long, ByVal dso As Long, ByVal name As String, ByRef ptype As Long, ByRef value As Variant) As Long
Declare Sub XPRMautounloaddso Lib "xprm_rt" Alias "_XPRMautounloaddso@4" (ByVal yesno As Long)
Declare Sub XPRMflushdso Lib "xprm_rt" Alias "_XPRMflushdso@0" ()
Declare Function XPRMgetnextdso Lib "xprm_rt" Alias "_XPRMgetnextdso@4" (ByVal dso As Long) As Long
Declare Function XPRMfinddso Lib "xprm_rt" Alias "_XPRMfinddso@4" (ByVal name As String) As Long
Declare Sub XPRMsetdsopath Lib "xprm_rt" Alias "_XPRMsetdsopath@4" (ByVal path As String)
Declare Function XPRMgetdsopath Lib "xprmvb" Alias "vbXPRMgetdsopath" (ByRef path As String) As Long
Declare Function XPRMdsotyptostr Lib "xprmvb" Alias "vbXPRMdsotyptostr" (ByVal model As Long, ByVal code As Long, ByVal value As Long, ByRef str As String) As Long
Declare Function XPRMgetdsoprop Lib "xprmvb" Alias "vbXPRMgetdsoprop" (ByVal dso As Long, ByVal what As Long, ByRef result As Variant) As Long


' Stream redirection
Declare Function XPRMsetdefstream Lib "xprmvb" Alias "vbXPRMsetdefstream" (ByVal model As Long, ByVal wmd As Long, ByVal name As String) As Long

' Begin/end licensing
Declare Function XPRMbeginlicensing Lib "xprm_rt" Alias "_XPRMbeginlicensing@4" (ByRef bIsLicensed As Long) As Long
Declare Function XPRMendlicensing Lib "xprm_rt" Alias "_XPRMendlicensing@0" () As Long

' the functions below are now obsolete but are included for backwards compatibility
' type interrogating
Declare Function XPRM_TYP Lib "xprmvb" Alias "vbXPRM_TYP" (ByVal t As Long) As Long
Declare Function XPRM_STR Lib "xprmvb" Alias "vbXPRM_STR" (ByVal t As Long) As Long
Declare Function XPRM_GRP Lib "xprmvb" Alias "vbXPRM_GRP" (ByVal t As Long) As Long

' IO driver
Declare Function XPRM_IO_CB Lib "xprmvb" Alias "vbXPRM_IO_CB" (ByVal address As Long) As String

' Callback-initialization IO driver
Declare Function XPRMcb_sendint Lib "xprm_rt" Alias "_XPRMcb_sendint@12" (ByVal cb As Long, ByVal i As Long, ByVal bFlush As Long) As Long
Declare Function XPRMcb_sendreal Lib "xprm_rt" Alias "_XPRMcb_sendreal@12" (ByVal cb As Long, ByVal d As Double, ByVal bFlush As Long) As Long
Declare Function XPRMcb_sendstring Lib "xprm_rt" Alias "_XPRMcb_sendstring@12" (ByVal cb As Long, ByVal s As String, ByVal bFlush As Long) As Long
Declare Function XPRMcb_sendctrl Lib "xprm_rt" Alias "_XPRMcb_sendctrl@9" (ByVal cb As Long, ByVal d As Byte, ByVal bFlush As Long) As Long


' Time functions
Declare Function XPRMdate2jdn Lib "xprmvb" Alias "vbXPRMdate2jdn" (ByVal y As Long, ByVal m As Long, ByVal d As Long) As Long
Declare Sub XPRMjdn2date Lib "xprmvb" Alias "vbXPRMjdn2date" (ByVal jd As Long, ByRef y As Long, ByRef m As Long, ByRef d As Long)
Declare Sub XPRMtime Lib "xprmvb" Alias "vbXPRMtime" (ByRef jdn As Long, ByRef t As Long, ByRef utc As Long)


' Debugging
Declare Function XPRMdbg_runmod Lib "xprm_rt" Alias "_XPRMdbg_runmod@20" (ByVal model As Long, ByRef returned As Long, ByVal parlist As String, ByVal dbgcb As Long, ByVal dbgctx As Long) As Long
Declare Function XPRMdbg_getnblndx Lib "xprm_rt" Alias "_XPRMdbg_getnblndx@4" (ByVal model As Long) As Long
Declare Function XPRMdbg_clearbrkp Lib "xprm_rt" Alias "_XPRMdbg_clearbrkp@8" (ByVal model As Long, ByVal lindex As Long) As Long
Declare Function XPRMdbg_setbrkp Lib "xprm_rt" Alias "_XPRMdbg_setbrkp@8" (ByVal model As Long, ByVal lindex As Long) As Long
Declare Function XPRMdbg_setstacklev Lib "xprm_rt" Alias "_XPRMdbg_setstacklev@8" (ByVal model As Long, ByVal level As Long) As Long
Declare Function XPRMdbg_getlocation Lib "xprmvb" Alias "vbXPRMdbg_getlocation" (ByVal model As Long, ByVal lindex As Long, ByRef line As Long, ByRef filename As String) As Long
Declare Function XPRMdbg_findproclndx Lib "xprm_rt" Alias "_XPRMdbg_findproclndx@8" (ByVal model As Long, ByVal proc As Long) As Long

Declare Function XPRMgetnextanident Lib "xprmvb" Alias "vbXPRMgetnextanident" (ByVal model As Long, ByRef ref As Long) As String
Private Declare Function cXPRMgetannotations Lib "xprm_rt" Alias "_XPRMgetannotations@20" (ByVal model As Long, ByVal ident As Long, ByVal prefix As Long, ByRef annStrings As Long, ByVal maxann As Long) As Long
Private Declare Function cXPRMgetdsoannotations Lib "xprm_rt" Alias "_XPRMgetdsoannotations@16" (ByVal dso As Long, ByVal prefix As Long, ByRef annStrings As Long, ByVal maxann As Long) As Long

Public Function XPRMgetannotations(ByVal model As Long, ByVal ident As String, ByVal prefix As String, ByRef annotations() As String, ByVal maxann As Long) As Long
    Dim i, nStrings As Long, nStringsFetched As Long
    Dim annptrs() As Long
    ReDim annptrs(1 To maxann + 1)
    Dim identptr As Long
    If ident = "" Then
        identptr = 0
    Else
        identptr = StrPtr(ident)
    End If
    
    nStrings = cXPRMgetannotations(model, identptr, StrPtr(prefix), annptrs(1), maxann)
    nStringsFetched = nStrings
    If nStringsFetched > maxann Then
        nStringsFetched = maxann
    End If
    
    If nStringsFetched > 0 Then
        For i = 1 To nStringsFetched
            annotations(LBound(annotations) + i - 1) = XPRMreadcstring(annptrs(i))
        Next
    End If
    XPRMgetannotations = nStrings
End Function

Public Function XPRMgetdsoannotations(ByVal dso As Long, ByVal prefix As String, ByRef annotations() As String, ByVal maxann As Long) As Long
    Dim i, nStrings As Long, nStringsFetched As Long
    Dim annptrs() As Long
    ReDim annptrs(1 To maxann + 1)
    
    nStrings = cXPRMgetdsoannotations(dso, StrPtr(prefix), annptrs(1), maxann)
    nStringsFetched = nStrings
    If nStringsFetched > maxann Then
        nStringsFetched = maxann
    End If
    
    If nStringsFetched > 0 Then
        For i = 1 To nStringsFetched
            annotations(LBound(annotations) + i - 1) = XPRMreadcstring(annptrs(i))
        Next
    End If
    XPRMgetdsoannotations = nStrings
End Function



#End If


' This function is intended to make it easier to wite code independent of 32bit-vs-64-bit and VBA6-vs-VBA7 concerns.
' The parameter "sets" should be an array of appropriate size, with element type being one of Variant, Long, LongPtr
' or LongLong. The actual XPRM API call will be made with a temporary array of Longs or LongPtrs (depending on VBA
' version) and the data will then be copied back if the call is successful.
Public Sub XPRMgetarrsetstovariant(ByVal mmarray As Variant, ByRef sets As Variant)
#If VBA7 <> 0 Then
    Dim tempsets() As LongPtr
#Else
    Dim tempsets() As Long
#End If
    XPRMarrayhelper_init tempsets, sets
    XPRMgetarrsets mmarray, tempsets
    XPRMarrayhelper_copy tempsets, sets
End Sub

Private Sub XPRMarrayhelper_redim(ByRef newarray As Variant, ByVal lb As Long, ByVal ub As Long)
#If VBA7 <> 0 Then
    ReDim newarray(lb To ub) As LongLong
#Else
    ReDim newarray(lb To ub) As Long
#End If
End Sub

Private Sub XPRMarrayhelper_init(ByRef newarray As Variant, ByRef oldarray As Variant, Optional ByVal copyold = False)
    XPRMarrayhelper_redim newarray, LBound(oldarray), UBound(oldarray)
    If copyold Then XPRMarrayhelper_copy oldarray, newarray
End Sub

Private Sub XPRMarrayhelper_copy(ByRef fromarray As Variant, ByRef toarray As Variant)
    If VarType(fromarray) = VarType(toarray) Then
        toarray = fromarray
        Exit Sub
    End If
        
    Dim i As Long
    For i = LBound(toarray) To UBound(toarray)
        toarray(i) = fromarray(i)
    Next i
End Sub


