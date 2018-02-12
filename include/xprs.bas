Attribute VB_Name = "Xpress_Optimizer"
' Module XPRS.BAS                  (c) Fair Isaac Corporation 1983-2015. All rights reserved
' Function and Constant declarations for use with Xpress optimizer library

Option Explicit
Option Compare Text

' XPRESS constants used to get and set the optimiser control variables

' Infinity constants
Global Const XPRS_PLUSINFINITY = 1.0e+20
Global Const XPRS_MINUSINFINITY = -1.0e+20
Global Const MAXINT = 2147483647
Global Const XPVERSION = 28


#If (VBA7 <> 0) And (Win64 <> 0) Then
' 64-bit VB7 functions



' Integer control variables for XPRSprob
Global Const XPRS_EXTRAROWS               =     8004
Global Const XPRS_EXTRACOLS               =     8005
Global Const XPRS_LPITERLIMIT             =     8007
Global Const XPRS_LPLOG                   =     8009
Global Const XPRS_SCALING                 =     8010
Global Const XPRS_PRESOLVE                =     8011
Global Const XPRS_CRASH                   =     8012
Global Const XPRS_PRICINGALG              =     8013
Global Const XPRS_INVERTFREQ              =     8014
Global Const XPRS_INVERTMIN               =     8015
Global Const XPRS_MAXNODE                 =     8018
Global Const XPRS_MAXTIME                 =     8020
Global Const XPRS_MAXMIPSOL               =     8021
Global Const XPRS_DEFAULTALG              =     8023
Global Const XPRS_VARSELECTION            =     8025
Global Const XPRS_NODESELECTION           =     8026
Global Const XPRS_BACKTRACK               =     8027
Global Const XPRS_MIPLOG                  =     8028
Global Const XPRS_KEEPNROWS               =     8030
Global Const XPRS_MPSECHO                 =     8032
Global Const XPRS_MAXPAGELINES            =     8034
Global Const XPRS_OUTPUTLOG               =     8035
Global Const XPRS_BARSOLUTION             =     8038
Global Const XPRS_CACHESIZE               =     8043
Global Const XPRS_CROSSOVER               =     8044
Global Const XPRS_BARITERLIMIT            =     8045
Global Const XPRS_CHOLESKYALG             =     8046
Global Const XPRS_BAROUTPUT               =     8047
Global Const XPRS_CSTYLE                  =     8050
Global Const XPRS_EXTRAMIPENTS            =     8051
Global Const XPRS_REFACTOR                =     8052
Global Const XPRS_BARTHREADS              =     8053
Global Const XPRS_KEEPBASIS               =     8054
Global Const XPRS_VERSION                 =     8061
Global Const XPRS_BIGMMETHOD              =     8068
Global Const XPRS_MPSNAMELENGTH           =     8071
Global Const XPRS_PRESOLVEOPS             =     8077
Global Const XPRS_MIPPRESOLVE             =     8078
Global Const XPRS_MIPTHREADS              =     8079
Global Const XPRS_BARORDER                =     8080
Global Const XPRS_BREADTHFIRST            =     8082
Global Const XPRS_AUTOPERTURB             =     8084
Global Const XPRS_DENSECOLLIMIT           =     8086
Global Const XPRS_CALLBACKFROMMASTERTHREAD =     8090
Global Const XPRS_MAXMCOEFFBUFFERELEMS    =     8091
Global Const XPRS_REFINEOPS               =     8093
Global Const XPRS_LPREFINEITERLIMIT       =     8094
Global Const XPRS_MIPREFINEITERLIMIT      =     8095
Global Const XPRS_DUALIZEOPS              =     8097
Global Const XPRS_MAXMEMORY               =     8112
Global Const XPRS_CUTFREQ                 =     8116
Global Const XPRS_SYMSELECT               =     8117
Global Const XPRS_SYMMETRY                =     8118
Global Const XPRS_LPTHREADS               =     8124
Global Const XPRS_MIQCPALG                =     8125
Global Const XPRS_QCCUTS                  =     8126
Global Const XPRS_QCROOTALG               =     8127
Global Const XPRS_ALGAFTERNETWORK         =     8129
Global Const XPRS_TRACE                   =     8130
Global Const XPRS_MAXIIS                  =     8131
Global Const XPRS_CPUTIME                 =     8133
Global Const XPRS_COVERCUTS               =     8134
Global Const XPRS_GOMCUTS                 =     8135
Global Const XPRS_MPSFORMAT               =     8137
Global Const XPRS_CUTSTRATEGY             =     8138
Global Const XPRS_CUTDEPTH                =     8139
Global Const XPRS_TREECOVERCUTS           =     8140
Global Const XPRS_TREEGOMCUTS             =     8141
Global Const XPRS_CUTSELECT               =     8142
Global Const XPRS_TREECUTSELECT           =     8143
Global Const XPRS_DUALIZE                 =     8144
Global Const XPRS_DUALGRADIENT            =     8145
Global Const XPRS_SBITERLIMIT             =     8146
Global Const XPRS_SBBEST                  =     8147
Global Const XPRS_MAXCUTTIME              =     8149
Global Const XPRS_ACTIVESET               =     8152
Global Const XPRS_BARINDEFLIMIT           =     8153
Global Const XPRS_HEURSTRATEGY            =     8154
Global Const XPRS_HEURFREQ                =     8155
Global Const XPRS_HEURDEPTH               =     8156
Global Const XPRS_HEURMAXSOL              =     8157
Global Const XPRS_HEURNODES               =     8158
Global Const XPRS_LNPBEST                 =     8160
Global Const XPRS_LNPITERLIMIT            =     8161
Global Const XPRS_BRANCHCHOICE            =     8162
Global Const XPRS_BARREGULARIZE           =     8163
Global Const XPRS_SBSELECT                =     8164
Global Const XPRS_LOCALCHOICE             =     8170
Global Const XPRS_LOCALBACKTRACK          =     8171
Global Const XPRS_DUALSTRATEGY            =     8174
Global Const XPRS_L1CACHE                 =     8175
Global Const XPRS_HEURDIVESTRATEGY        =     8177
Global Const XPRS_HEURSELECT              =     8178
Global Const XPRS_BARSTART                =     8180
Global Const XPRS_BARNUMSTABILITY         =     8186
Global Const XPRS_EXTRASETS               =     8190
Global Const XPRS_FEASIBILITYPUMP         =     8193
Global Const XPRS_PRECOEFELIM             =     8194
Global Const XPRS_PREDOMCOL               =     8195
Global Const XPRS_HEURSEARCHFREQ          =     8196
Global Const XPRS_HEURDIVESPEEDUP         =     8197
Global Const XPRS_SBESTIMATE              =     8198
Global Const XPRS_BARCORES                =     8202
Global Const XPRS_MAXCHECKSONMAXTIME      =     8203
Global Const XPRS_MAXCHECKSONMAXCUTTIME   =     8204
Global Const XPRS_HISTORYCOSTS            =     8206
Global Const XPRS_ALGAFTERCROSSOVER       =     8208
Global Const XPRS_LINELENGTH              =     8209
Global Const XPRS_MUTEXCALLBACKS          =     8210
Global Const XPRS_BARCRASH                =     8211
Global Const XPRS_HEURDIVESOFTROUNDING    =     8215
Global Const XPRS_HEURSEARCHROOTSELECT    =     8216
Global Const XPRS_HEURSEARCHTREESELECT    =     8217
Global Const XPRS_MPS18COMPATIBLE         =     8223
Global Const XPRS_ROOTPRESOLVE            =     8224
Global Const XPRS_CROSSOVERDRP            =     8227
Global Const XPRS_FORCEOUTPUT             =     8229
Global Const XPRS_DETERMINISTIC           =     8232
Global Const XPRS_PREPROBING              =     8238
Global Const XPRS_EXTRAQCELEMENTS         =     8240
Global Const XPRS_EXTRAQCROWS             =     8241
Global Const XPRS_TREEMEMORYLIMIT         =     8242
Global Const XPRS_TREECOMPRESSION         =     8243
Global Const XPRS_TREEDIAGNOSTICS         =     8244
Global Const XPRS_MAXGLOBALFILESIZE       =     8245
Global Const XPRS_TEMPBOUNDS              =     8250
Global Const XPRS_IFCHECKCONVEXITY        =     8251
Global Const XPRS_PRIMALUNSHIFT           =     8252
Global Const XPRS_REPAIRINDEFINITEQ       =     8254
Global Const XPRS_MAXLOCALBACKTRACK       =     8257
Global Const XPRS_BACKTRACKTIE            =     8266
Global Const XPRS_BRANCHDISJ              =     8267
Global Const XPRS_MIPFRACREDUCE           =     8270
Global Const XPRS_CONCURRENTTHREADS       =     8274
Global Const XPRS_MAXSCALEFACTOR          =     8275
Global Const XPRS_HEURTHREADS             =     8276
Global Const XPRS_THREADS                 =     8278
Global Const XPRS_HEURBEFORELP            =     8280
Global Const XPRS_PREDOMROW               =     8281
Global Const XPRS_BRANCHSTRUCTURAL        =     8282
Global Const XPRS_QUADRATICUNSHIFT        =     8284
Global Const XPRS_BARPRESOLVEOPS          =     8286
Global Const XPRS_QSIMPLEXOPS             =     8288
Global Const XPRS_CONFLICTCUTS            =     8292
Global Const XPRS_CORESPERCPU             =     8296
Global Const XPRS_SLEEPONTHREADWAIT       =     8302
Global Const XPRS_PREDUPROW               =     8307
Global Const XPRS_CPUPLATFORM             =     8312
Global Const XPRS_BARALG                  =     8315
Global Const XPRS_TREEPRESOLVE            =     8320
Global Const XPRS_TREEPRESOLVE_KEEPBASIS  =     8321
Global Const XPRS_TREEPRESOLVEOPS         =     8322
Global Const XPRS_LPLOGSTYLE              =     8326
Global Const XPRS_RANDOMSEED              =     8328
Global Const XPRS_TREEQCCUTS              =     8331
Global Const XPRS_PRELINDEP               =     8333
Global Const XPRS_DUALTHREADS             =     8334
Global Const XPRS_PREOBJCUTDETECT         =     8336
Global Const XPRS_PREBNDREDQUAD           =     8337
Global Const XPRS_PREBNDREDCONE           =     8338
Global Const XPRS_PRECOMPONENTS           =     8339


' LongLong control variables for XPRSprob
Global Const XPRS_EXTRAELEMS              =     8006
Global Const XPRS_EXTRAPRESOLVE           =     8037
Global Const XPRS_EXTRASETELEMS           =     8191


' Double control variables for XPRSprob
Global Const XPRS_MATRIXTOL               =     7001
Global Const XPRS_PIVOTTOL                =     7002
Global Const XPRS_FEASTOL                 =     7003
Global Const XPRS_OUTPUTTOL               =     7004
Global Const XPRS_SOSREFTOL               =     7005
Global Const XPRS_OPTIMALITYTOL           =     7006
Global Const XPRS_ETATOL                  =     7007
Global Const XPRS_RELPIVOTTOL             =     7008
Global Const XPRS_MIPTOL                  =     7009
Global Const XPRS_MIPTOLTARGET            =     7010
Global Const XPRS_MIPADDCUTOFF            =     7012
Global Const XPRS_MIPABSCUTOFF            =     7013
Global Const XPRS_MIPRELCUTOFF            =     7014
Global Const XPRS_PSEUDOCOST              =     7015
Global Const XPRS_PENALTY                 =     7016
Global Const XPRS_BIGM                    =     7018
Global Const XPRS_MIPABSSTOP              =     7019
Global Const XPRS_MIPRELSTOP              =     7020
Global Const XPRS_CROSSOVERACCURACYTOL    =     7023
Global Const XPRS_CHOLESKYTOL             =     7032
Global Const XPRS_BARGAPSTOP              =     7033
Global Const XPRS_BARDUALSTOP             =     7034
Global Const XPRS_BARPRIMALSTOP           =     7035
Global Const XPRS_BARSTEPSTOP             =     7036
Global Const XPRS_ELIMTOL                 =     7042
Global Const XPRS_PERTURB                 =     7044
Global Const XPRS_MARKOWITZTOL            =     7047
Global Const XPRS_MIPABSGAPNOTIFY         =     7064
Global Const XPRS_MIPRELGAPNOTIFY         =     7065
Global Const XPRS_PPFACTOR                =     7069
Global Const XPRS_REPAIRINDEFINITEQMAX    =     7071
Global Const XPRS_BARGAPTARGET            =     7073
Global Const XPRS_SBEFFORT                =     7086
Global Const XPRS_HEURDIVERANDOMIZE       =     7089
Global Const XPRS_HEURSEARCHEFFORT        =     7090
Global Const XPRS_CUTFACTOR               =     7091
Global Const XPRS_EIGENVALUETOL           =     7097
Global Const XPRS_INDLINBIGM              =     7099
Global Const XPRS_TREEMEMORYSAVINGTARGET  =     7100
Global Const XPRS_GLOBALFILEBIAS          =     7101
Global Const XPRS_INDPRELINBIGM           =     7102
Global Const XPRS_RELAXTREEMEMORYLIMIT    =     7105
Global Const XPRS_MIPABSGAPNOTIFYOBJ      =     7108
Global Const XPRS_MIPABSGAPNOTIFYBOUND    =     7109
Global Const XPRS_PRESOLVEMAXGROW         =     7110
Global Const XPRS_HEURSEARCHTARGETSIZE    =     7112
Global Const XPRS_CROSSOVERRELPIVOTTOL    =     7113
Global Const XPRS_CROSSOVERRELPIVOTTOLSAFE =     7114
Global Const XPRS_DETLOGFREQ              =     7116
Global Const XPRS_MAXIMPLIEDBOUND         =     7120
Global Const XPRS_FEASTOLTARGET           =     7121
Global Const XPRS_OPTIMALITYTOLTARGET     =     7122


' String control variables for XPRSprob
Global Const XPRS_MPSRHSNAME              =     6001
Global Const XPRS_MPSOBJNAME              =     6002
Global Const XPRS_MPSRANGENAME            =     6003
Global Const XPRS_MPSBOUNDNAME            =     6004
Global Const XPRS_OUTPUTMASK              =     6005


' Integer attributes for XPRSprob
Global Const XPRS_ROWS                    =     1001
Global Const XPRS_SETS                    =     1004
Global Const XPRS_PRIMALINFEAS            =     1007
Global Const XPRS_DUALINFEAS              =     1008
Global Const XPRS_SIMPLEXITER             =     1009
Global Const XPRS_LPSTATUS                =     1010
Global Const XPRS_MIPSTATUS               =     1011
Global Const XPRS_CUTS                    =     1012
Global Const XPRS_NODES                   =     1013
Global Const XPRS_NODEDEPTH               =     1014
Global Const XPRS_ACTIVENODES             =     1015
Global Const XPRS_MIPSOLNODE              =     1016
Global Const XPRS_MIPSOLS                 =     1017
Global Const XPRS_COLS                    =     1018
Global Const XPRS_SPAREROWS               =     1019
Global Const XPRS_SPARECOLS               =     1020
Global Const XPRS_SPAREMIPENTS            =     1022
Global Const XPRS_ERRORCODE               =     1023
Global Const XPRS_MIPINFEAS               =     1024
Global Const XPRS_PRESOLVESTATE           =     1026
Global Const XPRS_PARENTNODE              =     1027
Global Const XPRS_NAMELENGTH              =     1028
Global Const XPRS_QELEMS                  =     1030
Global Const XPRS_NUMIIS                  =     1031
Global Const XPRS_MIPENTS                 =     1032
Global Const XPRS_BRANCHVAR               =     1036
Global Const XPRS_MIPTHREADID             =     1037
Global Const XPRS_ALGORITHM               =     1049
Global Const XPRS_ORIGINALROWS            =     1124
Global Const XPRS_CALLBACKCOUNT_OPTNODE   =     1136
Global Const XPRS_CALLBACKCOUNT_CUTMGR    =     1137
Global Const XPRS_ORIGINALQELEMS          =     1157
Global Const XPRS_MAXPROBNAMELENGTH       =     1158
Global Const XPRS_STOPSTATUS              =     1179
Global Const XPRS_ORIGINALMIPENTS         =     1191
Global Const XPRS_ORIGINALSETS            =     1194
Global Const XPRS_SPARESETS               =     1203
Global Const XPRS_CHECKSONMAXTIME         =     1208
Global Const XPRS_CHECKSONMAXCUTTIME      =     1209
Global Const XPRS_ORIGINALCOLS            =     1214
Global Const XPRS_QCELEMS                 =     1232
Global Const XPRS_QCONSTRAINTS            =     1234
Global Const XPRS_ORIGINALQCELEMS         =     1237
Global Const XPRS_ORIGINALQCONSTRAINTS    =     1239
Global Const XPRS_PEAKTOTALTREEMEMORYUSAGE =     1240
Global Const XPRS_CURRENTNODE             =     1248
Global Const XPRS_TREEMEMORYUSAGE         =     1251
Global Const XPRS_GLOBALFILESIZE          =     1252
Global Const XPRS_GLOBALFILEUSAGE         =     1253
Global Const XPRS_INDICATORS              =     1254
Global Const XPRS_ORIGINALINDICATORS      =     1255
Global Const XPRS_CORESDETECTED           =     1260
Global Const XPRS_BARSING                 =     1281
Global Const XPRS_BARSINGR                =     1282
Global Const XPRS_PRESOLVEINDEX           =     1284
Global Const XPRS_CONES                   =     1307
Global Const XPRS_CONEELEMS               =     1308
Global Const XPRS_BARITER                 =     5001
Global Const XPRS_BARAASIZE               =     5002
Global Const XPRS_BARLSIZE                =     5003
Global Const XPRS_BARDENSECOL             =     5004
Global Const XPRS_BARCROSSOVER            =     5005


' LongLong attributes for XPRSprob
Global Const XPRS_SETMEMBERS              =     1005
Global Const XPRS_ELEMS                   =     1006
Global Const XPRS_SPAREELEMS              =     1021
Global Const XPRS_ORIGINALSETMEMBERS      =     1195
Global Const XPRS_SPARESETELEMS           =     1204


' Double attributes for XPRSprob
Global Const XPRS_LPOBJVAL                =     2001
Global Const XPRS_SUMPRIMALINF            =     2002
Global Const XPRS_MIPOBJVAL               =     2003
Global Const XPRS_BESTBOUND               =     2004
Global Const XPRS_OBJRHS                  =     2005
Global Const XPRS_MIPBESTOBJVAL           =     2006
Global Const XPRS_OBJSENSE                =     2008
Global Const XPRS_BRANCHVALUE             =     2009
Global Const XPRS_PENALTYVALUE            =     2061
Global Const XPRS_CURRMIPCUTOFF           =     2062
Global Const XPRS_BARCONDA                =     2063
Global Const XPRS_BARCONDD                =     2064
Global Const XPRS_MAXABSPRIMALINFEAS      =     2073
Global Const XPRS_MAXRELPRIMALINFEAS      =     2074
Global Const XPRS_MAXABSDUALINFEAS        =     2075
Global Const XPRS_MAXRELDUALINFEAS        =     2076
Global Const XPRS_PRIMALDUALINTEGRAL      =     2079
Global Const XPRS_BARPRIMALOBJ            =     4001
Global Const XPRS_BARDUALOBJ              =     4002
Global Const XPRS_BARPRIMALINF            =     4003
Global Const XPRS_BARDUALINF              =     4004
Global Const XPRS_BARCGAP                 =     4005


' String attributes for XPRSprob
Global Const XPRS_MATRIXNAME              =     3001
Global Const XPRS_BOUNDNAME               =     3002
Global Const XPRS_OBJNAME                 =     3003
Global Const XPRS_RHSNAME                 =     3004
Global Const XPRS_RANGENAME               =     3005


' Integer control variables for XPRSmipsolpool
Global Const XPRS_MSP_DUPLICATESOLUTIONSPOLICY =     6203
Global Const XPRS_MSP_INCLUDEPROBNAMEINLOGGING =     6211
Global Const XPRS_MSP_WRITESLXSOLLOGGING  =     6212
Global Const XPRS_MSP_ENABLESLACKSTORAGE  =     6213
Global Const XPRS_MSP_OUTPUTLOG           =     6214
Global Const XPRS_MSP_SOL_BITFIELDSUSR    =     6406


' Double control variables for XPRSmipsolpool
Global Const XPRS_MSP_DEFAULTUSERSOLFEASTOL =     6209
Global Const XPRS_MSP_DEFAULTUSERSOLMIPTOL =     6210
Global Const XPRS_MSP_SOL_FEASTOL         =     6402
Global Const XPRS_MSP_SOL_MIPTOL          =     6403


' Integer attributes for XPRSmipsolpool
Global Const XPRS_MSP_SOLUTIONS           =     6208
Global Const XPRS_MSP_PRB_VALIDSOLS       =     6300
Global Const XPRS_MSP_PRB_FEASIBLESOLS    =     6301
Global Const XPRS_MSP_SOL_COLS            =     6400
Global Const XPRS_MSP_SOL_NONZEROS        =     6401
Global Const XPRS_MSP_SOL_ISUSERSOLUTION  =     6404
Global Const XPRS_MSP_SOL_ISREPROCESSEDUSERSOLUTION =     6405
Global Const XPRS_MSP_SOL_BITFIELDSSYS    =     6407
Global Const XPRS_MSP_SOLPRB_INFEASCOUNT  =     6501
Global Const XPRS_MSP_SOLPRB_INFCNT_PRIMAL =     6503
Global Const XPRS_MSP_SOLPRB_INFCNT_MIP   =     6505
Global Const XPRS_MSP_SOLPRB_INFCNT_SLACK =     6507
Global Const XPRS_MSP_SOLPRB_INFMXI_SLACK =     6509
Global Const XPRS_MSP_SOLPRB_INFCNT_COLUMN =     6511
Global Const XPRS_MSP_SOLPRB_INFMXI_COLUMN =     6513
Global Const XPRS_MSP_SOLPRB_INFCNT_DELAYEDROW =     6515
Global Const XPRS_MSP_SOLPRB_INFMXI_DELAYEDROW =     6517
Global Const XPRS_MSP_SOLPRB_INFCNT_INT   =     6519
Global Const XPRS_MSP_SOLPRB_INFMXI_INT   =     6521
Global Const XPRS_MSP_SOLPRB_INFCNT_BIN   =     6523
Global Const XPRS_MSP_SOLPRB_INFMXI_BIN   =     6525
Global Const XPRS_MSP_SOLPRB_INFCNT_SC    =     6527
Global Const XPRS_MSP_SOLPRB_INFMXI_SC    =     6529
Global Const XPRS_MSP_SOLPRB_INFCNT_SI    =     6531
Global Const XPRS_MSP_SOLPRB_INFMXI_SI    =     6533
Global Const XPRS_MSP_SOLPRB_INFCNT_PI    =     6535
Global Const XPRS_MSP_SOLPRB_INFMXI_PI    =     6537
Global Const XPRS_MSP_SOLPRB_INFCNT_SET1  =     6539
Global Const XPRS_MSP_SOLPRB_INFMXI_SET1  =     6541
Global Const XPRS_MSP_SOLPRB_INFCNT_SET2  =     6543
Global Const XPRS_MSP_SOLPRB_INFMXI_SET2  =     6545


' Double attributes for XPRSmipsolpool
Global Const XPRS_MSP_SOLPRB_OBJ          =     6500
Global Const XPRS_MSP_SOLPRB_INFSUM_PRIMAL =     6502
Global Const XPRS_MSP_SOLPRB_INFSUM_MIP   =     6504
Global Const XPRS_MSP_SOLPRB_INFSUM_SLACK =     6506
Global Const XPRS_MSP_SOLPRB_INFMAX_SLACK =     6508
Global Const XPRS_MSP_SOLPRB_INFSUM_COLUMN =     6510
Global Const XPRS_MSP_SOLPRB_INFMAX_COLUMN =     6512
Global Const XPRS_MSP_SOLPRB_INFSUM_DELAYEDROW =     6514
Global Const XPRS_MSP_SOLPRB_INFMAX_DELAYEDROW =     6516
Global Const XPRS_MSP_SOLPRB_INFSUM_INT   =     6518
Global Const XPRS_MSP_SOLPRB_INFMAX_INT   =     6520
Global Const XPRS_MSP_SOLPRB_INFSUM_BIN   =     6522
Global Const XPRS_MSP_SOLPRB_INFMAX_BIN   =     6524
Global Const XPRS_MSP_SOLPRB_INFSUM_SC    =     6526
Global Const XPRS_MSP_SOLPRB_INFMAX_SC    =     6528
Global Const XPRS_MSP_SOLPRB_INFSUM_SI    =     6530
Global Const XPRS_MSP_SOLPRB_INFMAX_SI    =     6532
Global Const XPRS_MSP_SOLPRB_INFSUM_PI    =     6534
Global Const XPRS_MSP_SOLPRB_INFMAX_PI    =     6536
Global Const XPRS_MSP_SOLPRB_INFSUM_SET1  =     6538
Global Const XPRS_MSP_SOLPRB_INFMAX_SET1  =     6540
Global Const XPRS_MSP_SOLPRB_INFSUM_SET2  =     6542
Global Const XPRS_MSP_SOLPRB_INFMAX_SET2  =     6544


' Integer control variables for XPRSmipsolenum
Global Const XPRS_MSE_CALLBACKCULLSOLS_MIPOBJECT =     6601
Global Const XPRS_MSE_CALLBACKCULLSOLS_DIVERSITY =     6602
Global Const XPRS_MSE_CALLBACKCULLSOLS_MODOBJECT =     6603
Global Const XPRS_MSE_OPTIMIZEDIVERSITY   =     6607
Global Const XPRS_MSE_OUTPUTLOG           =     6610


' Double control variables for XPRSmipsolenum
Global Const XPRS_MSE_OUTPUTTOL           =     6609


' Integer attributes for XPRSmipsolenum
Global Const XPRS_MSE_SOLUTIONS           =     6600
Global Const XPRS_MSE_METRIC_MIPOBJECT    =     6604
Global Const XPRS_MSE_METRIC_DIVERSITY    =     6605
Global Const XPRS_MSE_METRIC_MODOBJECT    =     6606


' Double attributes for XPRSmipsolenum
Global Const XPRS_MSE_DIVERSITYSUM        =     6608


' Integer control variables for XPRSprobperturber
Global Const XPRS_PTB_PERMUTE_ACTIVE      =     6700
Global Const XPRS_PTB_PERMUTE_SEEDLAST    =     6701
Global Const XPRS_PTB_PERTURB_COST2_ACTIVE =     6710
Global Const XPRS_PTB_PERTURB_COST2_SEEDLAST =     6711
Global Const XPRS_PTB_SHIFTSCALE_COLS_ACTIVE =     6720
Global Const XPRS_PTB_SHIFTSCALE_COLS_SEEDLAST =     6721
Global Const XPRS_PTB_SHIFTSCALE_COLS_SHIFT_ACTIVE_I =     6725
Global Const XPRS_PTB_SHIFTSCALE_COLS_NEGATE_ACTIVE_I =     6726
Global Const XPRS_PTB_SHIFTSCALE_COLS_COMPLEMENT_ACTIVE_I =     6727
Global Const XPRS_PTB_SHIFTSCALE_COLS_COMPLEMENT_ACTIVE_B =     6728
Global Const XPRS_PTB_SHIFTSCALE_ROWS_ACTIVE =     6760
Global Const XPRS_PTB_SHIFTSCALE_ROWS_SEEDLAST =     6761


' Double control variables for XPRSprobperturber
Global Const XPRS_PTB_PERMUTE_INTENSITY_ROW =     6702
Global Const XPRS_PTB_PERMUTE_INTENSITY_COL =     6703
Global Const XPRS_PTB_SHIFTSCALE_COLS_INTENSITY =     6722
Global Const XPRS_PTB_SHIFTSCALE_COLS_MAXRANGEFORCOMPLEMENTING =     6729
Global Const XPRS_PTB_SHIFTSCALE_ROWS_INTENSITY =     6762


' Integer attributes for XPRSprobperturber
Global Const XPRS_PTB_PERMUTE_PERMCOUNT_ROW =     6704
Global Const XPRS_PTB_PERMUTE_PERMCOUNT_COL =     6705
Global Const XPRS_PTB_PERTURB_COST2_CHANGEDCOLCOUNT =     6712
Global Const XPRS_PTB_SHIFTSCALE_COLS_CHANGEDCOLCOUNT =     6723


' Double attributes for XPRSprobperturber
Global Const XPRS_PTB_PERTURB_COST2_TOTALABSCOSTCHANGE =     6713
Global Const XPRS_PTB_SHIFTSCALE_COLS_FIXEDOBJDELTA =     6724


' Enumeration values relating to LPSTATUS
Global Const XPRS_LP_UNSTARTED = 0
Global Const XPRS_LP_OPTIMAL = 1
Global Const XPRS_LP_INFEAS = 2
Global Const XPRS_LP_CUTOFF = 3
Global Const XPRS_LP_UNFINISHED = 4
Global Const XPRS_LP_UNBOUNDED = 5
Global Const XPRS_LP_CUTOFF_IN_DUAL = 6
Global Const XPRS_LP_UNSOLVED = 7
Global Const XPRS_LP_NONCONVEX = 8

' Enumeration values relating to MIPSTATUS
Global Const XPRS_MIP_NOT_LOADED = 0
Global Const XPRS_MIP_LP_NOT_OPTIMAL = 1
Global Const XPRS_MIP_LP_OPTIMAL = 2
Global Const XPRS_MIP_NO_SOL_FOUND = 3
Global Const XPRS_MIP_SOLUTION = 4
Global Const XPRS_MIP_INFEAS = 5
Global Const XPRS_MIP_OPTIMAL = 6
Global Const XPRS_MIP_UNBOUNDED = 7

' Enumeration values relating to BARORDER
Global Const XPRS_BAR_DEFAULT = 0
Global Const XPRS_BAR_MIN_DEGREE = 1
Global Const XPRS_BAR_MIN_LOCAL_FILL = 2
Global Const XPRS_BAR_NESTED_DISSECTION = 3

' Enumeration values relating to DEFAULTALG
Global Const XPRS_ALG_DEFAULT = 1
Global Const XPRS_ALG_DUAL = 2
Global Const XPRS_ALG_PRIMAL = 3
Global Const XPRS_ALG_BARRIER = 4
Global Const XPRS_ALG_NETWORK = 5

' Enumeration values relating to XPRSinterrupt
Global Const XPRS_STOP_NONE = 0
Global Const XPRS_STOP_TIMELIMIT = 1
Global Const XPRS_STOP_CTRLC = 2
Global Const XPRS_STOP_NODELIMIT = 3
Global Const XPRS_STOP_ITERLIMIT = 4
Global Const XPRS_STOP_MIPGAP = 5
Global Const XPRS_STOP_SOLLIMIT = 6
Global Const XPRS_STOP_USER = 9

' Enumeration values relating to AlwaysNeverOrAutomatic
Global Const XPRS_ANA_AUTOMATIC = -1
Global Const XPRS_ANA_NEVER = 0
Global Const XPRS_ANA_ALWAYS = 1

' Enumeration values relating to OnOrOff
Global Const XPRS_BOOL_OFF = 0
Global Const XPRS_BOOL_ON = 1

' Enumeration values relating to BACKTRACK
Global Const XPRS_BACKTRACKALG_BEST_ESTIMATE = 2
Global Const XPRS_BACKTRACKALG_BEST_BOUND = 3
Global Const XPRS_BACKTRACKALG_DEEPEST_NODE = 4
Global Const XPRS_BACKTRACKALG_HIGHEST_NODE = 5
Global Const XPRS_BACKTRACKALG_EARLIEST_NODE = 6
Global Const XPRS_BACKTRACKALG_LATEST_NODE = 7
Global Const XPRS_BACKTRACKALG_RANDOM = 8
Global Const XPRS_BACKTRACKALG_MIN_INFEAS = 9
Global Const XPRS_BACKTRACKALG_BEST_ESTIMATE_MIN_INFEAS = 10
Global Const XPRS_BACKTRACKALG_DEEPEST_BEST_ESTIMATE = 11

' Enumeration values relating to BRANCHCHOICE
Global Const XPRS_BRANCH_MIN_EST_FIRST = 0
Global Const XPRS_BRANCH_MAX_EST_FIRST = 1

' Enumeration values relating to CHOLESKYALG
Global Const XPRS_ALG_PULL_CHOLESKY = 0
Global Const XPRS_ALG_PUSH_CHOLESKY = 1

' Enumeration values relating to CROSSOVERDRP
Global Const XPRS_XDRPBEFORE_CROSSOVER = 1
Global Const XPRS_XDRPINSIDE_CROSSOVER = 2
Global Const XPRS_XDRPAGGRESSIVE_BEFORE_CROSSOVER = 4

' Enumeration values relating to DUALGRADIENT
Global Const XPRS_DUALGRADIENT_AUTOMATIC = -1
Global Const XPRS_DUALGRADIENT_DEVEX = 0
Global Const XPRS_DUALGRADIENT_STEEPESTEDGE = 1

' Enumeration values relating to DUALSTRATEGY
Global Const XPRS_DUALSTRATEGY_REMOVE_INFEAS_WITH_PRIMAL = 0
Global Const XPRS_DUALSTRATEGY_REMOVE_INFEAS_WITH_DUAL = 1

' Enumeration values relating to FEASIBILITYPUMP
Global Const XPRS_FEASIBILITYPUMP_NEVER = 0
Global Const XPRS_FEASIBILITYPUMP_ALWAYS = 1
Global Const XPRS_FEASIBILITYPUMP_LASTRESORT = 2

' Enumeration values relating to HEURSEARCHSELECT
Global Const XPRS_HEURSEARCH_LOCAL_SEARCH_LARGE_NEIGHBOURHOOD = 0
Global Const XPRS_HEURSEARCH_LOCAL_SEARCH_NODE_NEIGHBOURHOOD = 1
Global Const XPRS_HEURSEARCH_LOCAL_SEARCH_SOLUTION_NEIGHBOURHOOD = 2

' Enumeration values relating to HEURSTRATEGY
Global Const XPRS_HEURSTRATEGY_AUTOMATIC = -1
Global Const XPRS_HEURSTRATEGY_NONE = 0
Global Const XPRS_HEURSTRATEGY_BASIC = 1
Global Const XPRS_HEURSTRATEGY_ENHANCED = 2
Global Const XPRS_HEURSTRATEGY_EXTENSIVE = 3

' Enumeration values relating to NODESELECTION
Global Const XPRS_NODESELECTION_LOCAL_FIRST = 1
Global Const XPRS_NODESELECTION_BEST_FIRST = 2
Global Const XPRS_NODESELECTION_LOCAL_DEPTH_FIRST = 3
Global Const XPRS_NODESELECTION_BEST_FIRST_THEN_LOCAL_FIRST = 4
Global Const XPRS_NODESELECTION_DEPTH_FIRST = 5

' Enumeration values relating to OUTPUTLOG
Global Const XPRS_OUTPUTLOG_NO_OUTPUT = 0
Global Const XPRS_OUTPUTLOG_FULL_OUTPUT = 1
Global Const XPRS_OUTPUTLOG_ERRORS_AND_WARNINGS = 2
Global Const XPRS_OUTPUTLOG_ERRORS = 3

' Enumeration values relating to PREPROBING
Global Const XPRS_PREPROBING_AUTOMATIC = -1
Global Const XPRS_PREPROBING_DISABLED = 0
Global Const XPRS_PREPROBING_LIGHT = 1
Global Const XPRS_PREPROBING_FULL = 2
Global Const XPRS_PREPROBING_FULL_AND_REPEAT = 3

' Enumeration values relating to PRESOLVEOPS
Global Const XPRS_PRESOLVEOPS_SINGLETONCOLUMNREMOVAL = 1
Global Const XPRS_PRESOLVEOPS_SINGLETONROWREMOVAL = 2
Global Const XPRS_PRESOLVEOPS_FORCINGROWREMOVAL = 4
Global Const XPRS_PRESOLVEOPS_DUALREDUCTIONS = 8
Global Const XPRS_PRESOLVEOPS_REDUNDANTROWREMOVAL = 16
Global Const XPRS_PRESOLVEOPS_DUPLICATECOLUMNREMOVAL = 32
Global Const XPRS_PRESOLVEOPS_DUPLICATEROWREMOVAL = 64
Global Const XPRS_PRESOLVEOPS_STRONGDUALREDUCTIONS = 128
Global Const XPRS_PRESOLVEOPS_VARIABLEELIMINATIONS = 256
Global Const XPRS_PRESOLVEOPS_NOIPREDUCTIONS = 512
Global Const XPRS_PRESOLVEOPS_NOADVANCEDIPREDUCTIONS = 2048
Global Const XPRS_PRESOLVEOPS_LINEARLYDEPENDANTROWREMOVAL = 16384
Global Const XPRS_PRESOLVEOPS_NOINTEGERVARIABLEANDSOSDETECTION = 32768

' Enumeration values relating to PRESOLVESTATE
Global Const XPRS_PRESOLVESTATE_PROBLEMLOADED = 1
Global Const XPRS_PRESOLVESTATE_PROBLEMLPPRESOLVED = 2
Global Const XPRS_PRESOLVESTATE_PROBLEMMIPPRESOLVED = 4
Global Const XPRS_PRESOLVESTATE_SOLUTIONVALID = 128

' Enumeration values relating to MIPPRESOLVE
Global Const XPRS_MIPPRESOLVE_REDUCED_COST_FIXING = 1
Global Const XPRS_MIPPRESOLVE_LOGIC_PREPROCESSING = 2
Global Const XPRS_MIPPRESOLVE_ALLOW_CHANGE_BOUNDS = 8

' Enumeration values relating to PRESOLVE
Global Const XPRS_PRESOLVE_NOPRIMALINFEASIBILITY = -1
Global Const XPRS_PRESOLVE_NONE = 0
Global Const XPRS_PRESOLVE_DEFAULT = 1
Global Const XPRS_PRESOLVE_KEEPREDUNDANTBOUNDS = 2

' Enumeration values relating to PRICINGALG
Global Const XPRS_PRICING_PARTIAL = -1
Global Const XPRS_PRICING_DEFAULT = 0
Global Const XPRS_PRICING_DEVEX = 1

' Enumeration values relating to CUTSTRATEGY
Global Const XPRS_CUTSTRATEGY_DEFAULT = -1
Global Const XPRS_CUTSTRATEGY_NONE = 0
Global Const XPRS_CUTSTRATEGY_CONSERVATIVE = 1
Global Const XPRS_CUTSTRATEGY_MODERATE = 2
Global Const XPRS_CUTSTRATEGY_AGGRESSIVE = 3

' Enumeration values relating to VARSELECTION
Global Const XPRS_VARSELECTION_AUTOMATIC = -1
Global Const XPRS_VARSELECTION_MIN_UPDOWN_PSEUDO_COSTS = 1
Global Const XPRS_VARSELECTION_SUM_UPDOWN_PSEUDO_COSTS = 2
Global Const XPRS_VARSELECTION_MAX_UPDOWN_PSEUDO_COSTS_PLUS_TWICE_MIN = 3
Global Const XPRS_VARSELECTION_MAX_UPDOWN_PSEUDO_COSTS = 4
Global Const XPRS_VARSELECTION_DOWN_PSEUDO_COST = 5
Global Const XPRS_VARSELECTION_UP_PSEUDO_COST = 6

' Enumeration values relating to SCALING
Global Const XPRS_SCALING_ROW_SCALING = 1
Global Const XPRS_SCALING_COLUMN_SCALING = 2
Global Const XPRS_SCALING_ROW_SCALING_AGAIN = 4
Global Const XPRS_SCALING_MAXIMUM = 8
Global Const XPRS_SCALING_CURTIS_REID = 16
Global Const XPRS_SCALING_BY_MAX_ELEM_NOT_GEO_MEAN = 32
Global Const XPRS_SCALING_OBJECTIVE_SCALING = 64
Global Const XPRS_SCALING_EXCLUDE_QUADRATIC_FROM_SCALE_FACTOR = 128
Global Const XPRS_SCALING_IGNORE_QUADRATIC_ROW_PART = 256
Global Const XPRS_SCALING_BEFORE_PRESOLVE = 512
Global Const XPRS_SCALING_NO_SCALING_ROWS_UP = 1024
Global Const XPRS_SCALING_NO_SCALING_COLUMNS_DOWN = 2048
Global Const XPRS_SCALING_SIMPLEX_OBJECTIVE_SCALING = 4096
Global Const XPRS_SCALING_RHS_SCALING = 8192
Global Const XPRS_SCALING_NO_AGGRESSIVE_Q_SCALING = 16384
Global Const XPRS_SCALING_SLACK_SCALING = 32768

' Enumeration values relating to CUTSELECT
Global Const XPRS_CUTSELECT_CLIQUE = (32+1823)
Global Const XPRS_CUTSELECT_MIR = (64+1823)
Global Const XPRS_CUTSELECT_COVER = (128+1823)
Global Const XPRS_CUTSELECT_FLOWPATH = (2048+1823)
Global Const XPRS_CUTSELECT_IMPLICATION = (4096+1823)
Global Const XPRS_CUTSELECT_LIFT_AND_PROJECT = (8192+1823)
Global Const XPRS_CUTSELECT_DISABLE_CUT_ROWS = (16384+1823)
Global Const XPRS_CUTSELECT_GUB_COVER = (32768+1823)
Global Const XPRS_CUTSELECT_DEFAULT = (-1)

' Enumeration values relating to REFINEOPS
Global Const XPRS_REFINEOPS_LPOPTIMAL = 1
Global Const XPRS_REFINEOPS_MIPSOLUTION = 2
Global Const XPRS_REFINEOPS_MIPOPTIMAL = 4
Global Const XPRS_REFINEOPS_MIPNODELP = 8

' Enumeration values relating to DUALIZEOPS
Global Const XPRS_DUALIZEOPS_SWITCHALGORITHM = 1

' Enumeration values relating to TREEDIAGNOSTICS
Global Const XPRS_TREEDIAGNOSTICS_MEMORY_USAGE_SUMMARIES = 1
Global Const XPRS_TREEDIAGNOSTICS_MEMORY_SAVED_REPORTS = 2

' Enumeration values relating to BARPRESOLVEOPS
Global Const XPRS_BARPRESOLVEOPS_STANDARD_PRESOLVE = 0
Global Const XPRS_BARPRESOLVEOPS_EXTRA_BARRIER_PRESOLVE = 1

' Enumeration values relating to PRECOEFELIM
Global Const XPRS_PRECOEFELIM_DISABLED = 0
Global Const XPRS_PRECOEFELIM_AGGRESSIVE = 1
Global Const XPRS_PRECOEFELIM_CAUTIOUS = 2

' Enumeration values relating to PREDOMROW
Global Const XPRS_PREDOMROW_AUTOMATIC = -1
Global Const XPRS_PREDOMROW_DISABLED = 0
Global Const XPRS_PREDOMROW_CAUTIOUS = 1
Global Const XPRS_PREDOMROW_MEDIUM = 2
Global Const XPRS_PREDOMROW_AGGRESSIVE = 3

' Enumeration values relating to PREDOMCOL
Global Const XPRS_PREDOMCOL_AUTOMATIC = -1
Global Const XPRS_PREDOMCOL_DISABLED = 0
Global Const XPRS_PREDOMCOL_CAUTIOUS = 1
Global Const XPRS_PREDOMCOL_AGGRESSIVE = 2

' Enumeration values relating to PRIMALUNSHIFT
Global Const XPRS_PRIMALUNSHIFT_ALLOW_DUAL_UNSHIFT = 0
Global Const XPRS_PRIMALUNSHIFT_NO_DUAL_UNSHIFT = 1

' Enumeration values relating to REPAIRINDEFINITEQ
Global Const XPRS_REPAIRINDEFINITEQ_REPAIR_IF_POSSIBLE = 0
Global Const XPRS_REPAIRINDEFINITEQ_NO_REPAIR = 1

' Enumeration values relating to Minimize/Maximize
Global Const XPRS_OBJ_MINIMIZE = 1
Global Const XPRS_OBJ_MAXIMIZE = -1

' Enumeration values relating to Set/GetControl/Attribinfo
Global Const XPRS_TYPE_NOTDEFINED = 0
Global Const XPRS_TYPE_INT = 1
Global Const XPRS_TYPE_INT64 = 2
Global Const XPRS_TYPE_DOUBLE = 3
Global Const XPRS_TYPE_STRING = 4

' Enumeration values relating to QCONVEXITY
Global Const XPRS_QCONVEXITY_UNKNOWN = -1
Global Const XPRS_QCONVEXITY_NONCONVEX = 0
Global Const XPRS_QCONVEXITY_CONVEX = 1
Global Const XPRS_QCONVEXITY_REPAIRABLE = 2
Global Const XPRS_QCONVEXITY_CONVEXCONE = 3
Global Const XPRS_QCONVEXITY_CONECONVERTABLE = 4

' Enumeration values relating to SOLINFO
Global Const XPRS_SOLINFO_ABSPRIMALINFEAS = 0
Global Const XPRS_SOLINFO_RELPRIMALINFEAS = 1
Global Const XPRS_SOLINFO_ABSDUALINFEAS = 2
Global Const XPRS_SOLINFO_RELDUALINFEAS = 3
Global Const XPRS_SOLINFO_MAXMIPFRACTIONAL = 4

' Bitfield values relating to XPRSmipsolpool_soln_flags
Global Const XPRS_ISUSERSOLUTION = 1
Global Const XPRS_ISREPROCESSEDUSERSOLUTION = 2


' Optimiser DLL Function Declarations
' NB You may have to change the path of "XPRS.DLL" so that VB can find the DLL.
Declare PtrSafe Function XPRScopycallbacks Lib "XPRS.DLL" (ByVal dest As LongPtr, ByVal src As LongPtr) As Long
Declare PtrSafe Function XPRScopycontrols Lib "XPRS.DLL" (ByVal dest As LongPtr, ByVal src As LongPtr) As Long
Declare PtrSafe Function XPRScopyprob Lib "XPRS.DLL" (ByVal dest As LongPtr, ByVal src As LongPtr, ByVal probname As String) As Long
Declare PtrSafe Function XPRScreateprob Lib "XPRS.DLL" (ByRef probholder As LongPtr) As Long
Declare PtrSafe Function XPRSdestroyprob Lib "XPRS.DLL" (ByVal prob As LongPtr) As Long

Declare PtrSafe Function XPRSinit Lib "XPRS.DLL" (ByVal path As String) As Long
Declare PtrSafe Function XPRSfree Lib "XPRS.DLL" () As Long
Private Declare PtrSafe Function XPRSgetbanner_C Lib "XPRS.DLL" Alias "XPRSgetbanner" (ByVal banner As String) As Long
Private Declare PtrSafe Function XPRSgetversion_C Lib "XPRS.DLL" Alias "XPRSgetversion" (ByVal version As String) As Long
Declare PtrSafe Function XPRSgetdaysleft Lib "XPRS.DLL" (ByRef daysleft As Long) As Long
Declare PtrSafe Function XPRSsetcheckedmode Lib "XPRS.DLL" (ByVal checked_mode As Long) As Long
Declare PtrSafe Function XPRSgetcheckedmode Lib "XPRS.DLL" (ByRef r_checked_mode As Long) As Long
Private Declare PtrSafe Function XPRSlicense_C Lib "XPRS.DLL" Alias "XPRSlicense" (ByRef i1 As Long, ByVal c1 As String) As Long
Declare PtrSafe Function XPRSbeginlicensing Lib "XPRS.DLL" (ByRef r_dontAlreadyHaveLicense As Long) As Long
Declare PtrSafe Function XPRSendlicensing Lib "XPRS.DLL" () As Long
Private Declare PtrSafe Function XPRSgetlicerrmsg_C Lib "XPRS.DLL" Alias "XPRSgetlicerrmsg" (ByVal msg As String, ByVal len_ As Long) As Long

Declare PtrSafe Function XPRSsetlogfile Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal logname As String) As Long
Declare PtrSafe Function XPRSsetintcontrol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal index As Long, ByVal ivalue As LongLong) As Long
Declare PtrSafe Function XPRSsetdblcontrol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal index As Long, ByVal dvalue As Double) As Long
Declare PtrSafe Function XPRSinterrupt Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal reason As Long) As Long
Private Declare PtrSafe Function XPRSgetprobname_C Lib "XPRS.DLL" Alias "XPRSgetprobname" (ByVal prob As LongPtr, ByVal svalue As String) As Long
Declare PtrSafe Function XPRSgetqobj Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal icol As Long, ByVal jcol As Long, ByRef dval As Double) As Long
Declare PtrSafe Function XPRSsetprobname Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal svalue As String) As Long
Declare PtrSafe Function XPRSsetstrcontrol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal index As Long, ByVal svalue As String) As Long
Declare PtrSafe Function XPRSgetintcontrol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal index As Long, ByRef ivalue As LongLong) As Long
Declare PtrSafe Function XPRSgetdblcontrol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal index As Long, ByRef dvalue As Double) As Long
Private Declare PtrSafe Function XPRSgetstrcontrol_C Lib "XPRS.DLL" Alias "XPRSgetstrcontrol" (ByVal prob As LongPtr, ByVal index As Long, ByVal svalue As String) As Long
Private Declare PtrSafe Function XPRSgetstringcontrol_C Lib "XPRS.DLL" Alias "XPRSgetstringcontrol" (ByVal prob As LongPtr, ByVal index As Long, ByVal svalue As String, ByVal svaluesize As Long, ByRef controlsize As Long) As Long
Declare PtrSafe Function XPRSgetintattrib Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal index As Long, ByRef ivalue As LongLong) As Long
Private Declare PtrSafe Function XPRSgetstrattrib_C Lib "XPRS.DLL" Alias "XPRSgetstrattrib" (ByVal prob As LongPtr, ByVal index As Long, ByVal cvalue As String) As Long
Private Declare PtrSafe Function XPRSgetstringattrib_C Lib "XPRS.DLL" Alias "XPRSgetstringattrib" (ByVal prob As LongPtr, ByVal index As Long, ByVal cvalue As String, ByVal cvaluesize As Long, ByRef controlsize As Long) As Long
Declare PtrSafe Function XPRSgetdblattrib Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal index As Long, ByRef dvalue As Double) As Long
Declare PtrSafe Function XPRSsetdefaultcontrol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal index As Long) As Long
Declare PtrSafe Function XPRSsetdefaults Lib "XPRS.DLL" (ByVal prob As LongPtr) As Long
Declare PtrSafe Function XPRSgetcontrolinfo Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sCaName As String, ByRef iHeaderId As Long, ByRef iTypeinfo As Long) As Long
Declare PtrSafe Function XPRSgetattribinfo Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sCaName As String, ByRef iHeaderId As Long, ByRef iTypeinfo As Long) As Long

Declare PtrSafe Function XPRSgoal Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal filename As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSreadprob Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sprobname As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSloadlp Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, drange As Double, dobj As Double, mstart As Long, mnel As Long, mrwind As Long, dmatval As Double, dlb As Double, dub As Double) As Long
Declare PtrSafe Function XPRSloadqp Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, drange As Double, dobj As Double, mstart As Long, mnel As Long, mrwind As Long, dmatval As Double, dlb As Double, dub As Double, ByVal nquads As LongLong, mqcol1 As Long, mqcol2 As Long, dqval As Double) As Long
Declare PtrSafe Function XPRSloadqglobal Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal probname As String, ByVal ncols As Long, ByVal nrows As Long, qsenx As Byte, rhsx As Double, range As Double, objx As Double, matbeg As Long, matcnt As Long, matind As Long, dmtval As Double, bndl As Double, bndu As Double, ByVal nquads As LongLong, mqcol1 As Long, mqcol2 As Long, dqval As Double, ByVal ngents As Long, ByVal nsets As Long, qgtype As Byte, mgcols As Long, dlim As Double, qstype As Byte, msstart As Long, mscols As Long, dref As Double) As Long
Declare PtrSafe Function XPRSfixglobals Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ifround As Long) As Long
Declare PtrSafe Function XPRSloadmodelcuts Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nmodcuts As Long, mrows As Long) As Long
Declare PtrSafe Function XPRSloaddelayedrows Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nrows As Long, mrows As Long) As Long
Declare PtrSafe Function XPRSloaddirs Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ndirs As Long, mcols As Long, mpri As Long, sbr As Byte, dupc As Double, ddpc As Double) As Long
Declare PtrSafe Function XPRSloadbranchdirs Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ndirs As Long, mcols As Long, mbranch As Long) As Long
Declare PtrSafe Function XPRSloadpresolvedirs Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ndirs As Long, mcols As Long, mpri As Long, sbr As Byte, dupc As Double, ddpc As Double) As Long
Declare PtrSafe Function XPRSgetdirs Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef ndirs As Long, mcols As Long, mpri As Long, sbr As Byte, dupc As Double, ddpc As Double) As Long
Declare PtrSafe Function XPRSloadglobal Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, drange As Double, dobj As Double, mstart As Long, mnel As Long, mrwind As Long, dmatval As Double, dlb As Double, dub As Double, ByVal ngents As Long, ByVal nsets As Long, qgtype As Byte, mgcols As Long, dlim As Double, stype As Byte, msstart As Long, mscols As Long, dref As Double) As Long
Declare PtrSafe Function XPRSaddnames Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal itype As Long, sname As Byte, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSaddsetnames Lib "XPRS.DLL" (ByVal prob As LongPtr, sname As Byte, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSscale Lib "XPRS.DLL" (ByVal prob As LongPtr, mrscal As Long, mcscal As Long) As Long
Declare PtrSafe Function XPRSreaddirs Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String) As Long
Declare PtrSafe Function XPRSwritedirs Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String) As Long
Declare PtrSafe Function XPRSsetindicators Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nrows As Long, mrows As Long, inds As Long, comps As Long) As Long
Declare PtrSafe Function XPRSgetindicators Lib "XPRS.DLL" (ByVal prob As LongPtr, inds As Long, comps As Long, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSdelindicators Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSdumpcontrols Lib "XPRS.DLL" (ByVal prob As LongPtr) As Long

Declare PtrSafe Function XPRSminim Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSmaxim Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSlpoptimize Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSmipoptimize Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSrange Lib "XPRS.DLL" (ByVal prob As LongPtr) As Long
Declare PtrSafe Function XPRSgetrowrange Lib "XPRS.DLL" (ByVal prob As LongPtr, upact As Double, loact As Double, uup As Double, udn As Double) As Long
Declare PtrSafe Function XPRSgetcolrange Lib "XPRS.DLL" (ByVal prob As LongPtr, upact As Double, loact As Double, uup As Double, udn As Double, ucost As Double, lcost As Double) As Long
Declare PtrSafe Function XPRSgetpivotorder Lib "XPRS.DLL" (ByVal prob As LongPtr, mpiv As Long) As Long
Declare PtrSafe Function XPRSgetpresolvemap Lib "XPRS.DLL" (ByVal prob As LongPtr, rowmap As Long, colmap As Long) As Long
Declare PtrSafe Function XPRSreadbasis Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSwritebasis Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSglobal Lib "XPRS.DLL" (ByVal prob As LongPtr) As Long
Declare PtrSafe Function XPRSinitglobal Lib "XPRS.DLL" (ByVal prob As LongPtr) As Long
Declare PtrSafe Function XPRSwriteprtsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSalter Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String) As Long
Declare PtrSafe Function XPRSwritesol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSwritebinsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSreadbinsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSwriteslxsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSreadslxsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSwriteprtrange Lib "XPRS.DLL" (ByVal prob As LongPtr) As Long
Declare PtrSafe Function XPRSwriterange Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSgetsol Lib "XPRS.DLL" (ByVal prob As LongPtr, dx As Double, dslack As Double, dual As Double, dj As Double) As Long
Declare PtrSafe Function XPRSgetpresolvesol Lib "XPRS.DLL" (ByVal prob As LongPtr, dx As Double, dslack As Double, dual As Double, dj As Double) As Long
Declare PtrSafe Function XPRSgetinfeas Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef npv As Long, ByRef nps As Long, ByRef nds As Long, ByRef ndv As Long, mx As Long, mslack As Long, mdual As Long, mdj As Long) As Long
Declare PtrSafe Function XPRSgetscaledinfeas Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef npv As Long, ByRef nps As Long, ByRef nds As Long, ByRef ndv As Long, mx As Long, mslack As Long, mdual As Long, mdj As Long) As Long
Declare PtrSafe Function XPRSgetunbvec Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef icol As Long) As Long
Declare PtrSafe Function XPRSbtran Lib "XPRS.DLL" (ByVal prob As LongPtr, dwork As Double) As Long
Declare PtrSafe Function XPRSftran Lib "XPRS.DLL" (ByVal prob As LongPtr, dwork As Double) As Long
Declare PtrSafe Function XPRSgetobj Lib "XPRS.DLL" (ByVal prob As LongPtr, dobj As Double, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetrhs Lib "XPRS.DLL" (ByVal prob As LongPtr, drhs As Double, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetrhsrange Lib "XPRS.DLL" (ByVal prob As LongPtr, drng As Double, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetlb Lib "XPRS.DLL" (ByVal prob As LongPtr, dbdl As Double, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetub Lib "XPRS.DLL" (ByVal prob As LongPtr, dbdu As Double, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetcols Lib "XPRS.DLL" (ByVal prob As LongPtr, mstart As Long, mrwind As Long, dmatval As Double, ByVal maxcoeffs As LongLong, ByRef ncoeffs As LongLong, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetrows Lib "XPRS.DLL" (ByVal prob As LongPtr, mstart As Long, mclind As Long, dmatval As Double, ByVal maxcoeffs As LongLong, ByRef ncoeffs As LongLong, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetcoef Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal irow As Long, ByVal icol As Long, ByRef dval As Double) As Long
Declare PtrSafe Function XPRSgetmqobj Lib "XPRS.DLL" (ByVal prob As LongPtr, mstart As Long, mclind As Long, dobjval As Double, ByVal maxcoeffs As LongLong, ByRef ncoeffs As LongLong, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetbarnumstability Lib "XPRS.DLL" (ByVal prob As LongPtr, dColumnStability As Long, dRowStability As Long) As Long

Declare PtrSafe Function XPRSiisclear Lib "XPRS.DLL" (ByVal prob As LongPtr) As Long
Declare PtrSafe Function XPRSiisfirst Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal iismode As Long, ByRef status_code As Long) As Long
Declare PtrSafe Function XPRSiisnext Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef status_code As Long) As Long
Declare PtrSafe Function XPRSiisstatus Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef iiscount As Long, rowsizes As Long, colsizes As Long, suminfeas As Double, numinfeas As Long) As Long
Declare PtrSafe Function XPRSiisall Lib "XPRS.DLL" (ByVal prob As LongPtr) As Long
Declare PtrSafe Function XPRSiiswrite Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal number As Long, ByVal fn As String, ByVal filetype As Long, ByVal typeflags As String) As Long
Declare PtrSafe Function XPRSiisisolations Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal number As Long) As Long
Declare PtrSafe Function XPRSgetiisdata Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal number As Long, ByRef rownumber As Long, ByRef colnumber As Long, miisrow As Long, miiscol As Long, constrainttype As Byte, colbndtype As Byte, duals As Double, rdcs As Double, isolationrows As Byte, isolationcols As Byte) As Long
Declare PtrSafe Function XPRSgetiis Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef ncols As Long, ByRef nrows As Long, miiscol As Long, miisrow As Long) As Long
Declare PtrSafe Function XPRSgetpresolvebasis Lib "XPRS.DLL" (ByVal prob As LongPtr, mrowstatus As Long, mcolstatus As Long) As Long
Declare PtrSafe Function XPRSloadpresolvebasis Lib "XPRS.DLL" (ByVal prob As LongPtr, mrowstatus As Long, mcolstatus As Long) As Long
Declare PtrSafe Function XPRSgetglobal Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef ngents As Long, ByRef nsets As Long, sgtype As Byte, mgcols As Long, dlim As Double, sstype As Byte, msstart As Long, mscols As Long, dref As Double) As Long
Declare PtrSafe Function XPRSwriteprob Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sfilename As String, ByVal sflags As String) As Long
Declare PtrSafe Function XPRSgetnames Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal itype As Long, sbuff As Byte, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetrowtype Lib "XPRS.DLL" (ByVal prob As LongPtr, srowtype As Byte, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSloadsecurevecs Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nrows As Long, ByVal ncols As Long, mrow As Long, mcol As Long) As Long
Declare PtrSafe Function XPRSgetcoltype Lib "XPRS.DLL" (ByVal prob As LongPtr, coltype As Byte, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetbasis Lib "XPRS.DLL" (ByVal prob As LongPtr, mrowstatus As Long, mcolstatus As Long) As Long
Declare PtrSafe Function XPRSloadbasis Lib "XPRS.DLL" (ByVal prob As LongPtr, mrowstatus As Long, mcolstatus As Long) As Long
Declare PtrSafe Function XPRSgetindex Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal itype As Long, ByVal sname As String, ByRef iseq As Long) As Long
Declare PtrSafe Function XPRSaddrows Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nrows As Long, ByVal ncoeffs As LongLong, srowtype As Byte, drhs As Double, drng As Double, mstart As Long, mclind As Long, dmatval As Double) As Long
Declare PtrSafe Function XPRSdelrows Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nrows As Long, mindex As Long) As Long
Declare PtrSafe Function XPRSaddcols Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ncols As Long, ByVal ncoeffs As LongLong, dobj As Double, mstart As Long, mrwind As Long, dmatval As Double, dbdl As Double, dbdu As Double) As Long
Declare PtrSafe Function XPRSdelcols Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ncols As Long, mindex As Long) As Long
Declare PtrSafe Function XPRSchgcoltype Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ncols As Long, mindex As Long, coltype As Byte) As Long
Declare PtrSafe Function XPRSchgrowtype Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nrows As Long, mindex As Long, srowtype As Byte) As Long
Declare PtrSafe Function XPRSchgbounds Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nbnds As Long, mindex As Long, sboundtype As Byte, dbnd As Double) As Long
Declare PtrSafe Function XPRSchgobj Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ncols As Long, mindex As Long, dobj As Double) As Long
Declare PtrSafe Function XPRSchgcoef Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal irow As Long, ByVal icol As Long, ByVal dval As Double) As Long
Declare PtrSafe Function XPRSchgmcoef Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ncoeffs As LongLong, mrow As Long, mcol As Long, dval As Double) As Long
Declare PtrSafe Function XPRSchgmqobj Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ncols As LongLong, mcol1 As Long, mcol2 As Long, dval As Double) As Long
Declare PtrSafe Function XPRSchgqobj Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal icol As Long, ByVal jcol As Long, ByVal dval As Double) As Long
Declare PtrSafe Function XPRSchgrhs Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nrows As Long, mindex As Long, drhs As Double) As Long
Declare PtrSafe Function XPRSchgrhsrange Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nrows As Long, mindex As Long, drng As Double) As Long
Declare PtrSafe Function XPRSchgobjsense Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal objsense As Long) As Long
Declare PtrSafe Function XPRSchgglblimit Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ncols As Long, mindex As Long, dlimit As Double) As Long
Declare PtrSafe Function XPRSsave Lib "XPRS.DLL" (ByVal prob As LongPtr) As Long
Declare PtrSafe Function XPRSrestore Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sprobname As String, ByVal force As String) As Long
Declare PtrSafe Function XPRSpivot Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal in_ As Long, ByVal out As Long) As Long
Declare PtrSafe Function XPRSgetpivots Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal in_ As Long, mout As Long, dout As Double, ByRef dobjo As Double, ByRef npiv As Long, ByVal maxpiv As Long) As Long
Declare PtrSafe Function XPRSaddcuts Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ncuts As Long, mtype As Long, qrtype As Byte, drhs As Double, mstart As Long, mcols As Long, dmatval As Double) As Long
Declare PtrSafe Function XPRSdelcuts Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ibasis As Long, ByVal itype As Long, ByVal interp As Long, ByVal delta As Double, ByVal ncuts As Long, mcutind As Long) As Long
Declare PtrSafe Function XPRSdelcpcuts Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal itype As Long, ByVal interp As Long, ByVal ncuts As Long, mcutind As Long) As Long
Declare PtrSafe Function XPRSgetcutlist Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal itype As Long, ByVal interp As Long, ByRef ncuts As Long, ByVal maxcuts As Long, mcutind As Long) As Long
Declare PtrSafe Function XPRSgetcpcutlist Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal itype As Long, ByVal interp As Long, ByVal delta As Double, ByRef ncuts As Long, ByVal maxcuts As Long, mcutind As Long, dviol As Double) As Long
Declare PtrSafe Function XPRSgetcpcuts Lib "XPRS.DLL" (ByVal prob As LongPtr, mindex As Long, ByVal ncuts As Long, ByVal size As LongLong, mtype As Long, qrtype As Byte, mstart As Long, mcols As Long, dmatval As Double, drhs As Double) As Long
Declare PtrSafe Function XPRSloadcuts Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal itype As Long, ByVal interp As Long, ByVal ncuts As Long, mcutind As Long) As Long
Declare PtrSafe Function XPRSstorecuts Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ncuts As Long, ByVal nodupl As Long, mtype As Long, qrtype As Byte, drhs As Double, mstart As Long, mindex As Long, mcols As Long, dmatval As Double) As Long
Declare PtrSafe Function XPRSpresolverow Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal qrtype As Byte, ByVal nzo As Long, mcolso As Long, dvalo As Double, ByVal drhso As Double, ByVal maxcoeffs As Long, ByRef nzp As Long, mcolsp As Long, dvalp As Double, ByRef drhsp As Double, ByRef status As Long) As Long
Declare PtrSafe Function XPRSloadmipsol Lib "XPRS.DLL" (ByVal prob As LongPtr, dsol As Double, ByRef status As Long) As Long
Declare PtrSafe Function XPRSaddmipsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ilength As Long, mipsolval As Double, mipsolcol As Long, ByVal solname As String) As Long
Declare PtrSafe Function XPRSstorebounds Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nbnds As Long, mcols As Long, qbtype As Byte, dbnd As Double, ByRef mindex As LongPtr) As Long
Declare PtrSafe Function XPRSsetbranchcuts Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nbcuts As Long, mindex As Long) As Long
Declare PtrSafe Function XPRSsetbranchbounds Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal mindex As LongPtr) As Long
Private Declare PtrSafe Function XPRSgetlasterror_C Lib "XPRS.DLL" Alias "XPRSgetlasterror" (ByVal prob As LongPtr, ByVal errmsg As String) As Long
Declare PtrSafe Function XPRSbasiscondition Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef condnum As Double, ByRef scondnum As Double) As Long
Declare PtrSafe Function XPRSgetmipsol Lib "XPRS.DLL" (ByVal prob As LongPtr, dx As Double, dslack As Double) As Long
Declare PtrSafe Function XPRSgetlpsol Lib "XPRS.DLL" (ByVal prob As LongPtr, dx As Double, dslack As Double, dual As Double, dj As Double) As Long
Declare PtrSafe Function XPRSpostsolve Lib "XPRS.DLL" (ByVal prob As LongPtr) As Long
Declare PtrSafe Function XPRSdelsets Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nsets As Long, msindex As Long) As Long
Declare PtrSafe Function XPRSaddsets Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal newsets As Long, ByVal newnz As LongLong, qstype As Byte, msstart As Long, mscols As Long, dref As Double) As Long
Declare PtrSafe Function XPRSstrongbranch Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nbnds As Long, mindex As Long, sboundtype As Byte, dbnd As Double, ByVal itrlimit As Long, dsbobjval As Double, msbstatus As Long) As Long
Declare PtrSafe Function XPRSestimaterowdualranges Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nRows As Long, rowIndices As Long, ByVal iterationLimit As Long, minDualActivity As Double, maxDualActivity As Double) As Long
Declare PtrSafe Function XPRSgetprimalray Lib "XPRS.DLL" (ByVal prob As LongPtr, dpray As Double, ByRef hasray As Long) As Long
Declare PtrSafe Function XPRSgetdualray Lib "XPRS.DLL" (ByVal prob As LongPtr, ddray As Double, ByRef hasray As Long) As Long
Declare PtrSafe Function XPRSsetmessagestatus Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal errcode As Long, ByVal bEnabledStatus As Long) As Long
Declare PtrSafe Function XPRSgetmessagestatus Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal errcode As Long, ByRef bEnabledStatus As Long) As Long
Declare PtrSafe Function XPRSrepairweightedinfeas Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef scode As Long, lrp_array As Double, grp_array As Double, lbp_array As Double, ubp_array As Double, ByVal second_phase As Byte, ByVal delta As Double, ByVal optflags As String) As Long
Declare PtrSafe Function XPRSrepairweightedinfeasbounds Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef scode As Long, lrp_array As Double, grp_array As Double, lbp_array As Double, ubp_array As Double, lrb_array As Double, grb_array As Double, lbb_array As Double, ubb_array As Double, ByVal second_phase As Byte, ByVal delta As Double, ByVal optflags As String) As Long
Declare PtrSafe Function XPRSrepairinfeas Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef scode As Long, ByVal ptype As Byte, ByVal phase2 As Byte, ByVal globalflags As Byte, ByVal lrp As Double, ByVal grp As Double, ByVal lbp As Double, ByVal ubp As Double, ByVal delta As Double) As Long
Declare PtrSafe Function XPRSgetcutslack Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal cut As LongPtr, ByRef dslack As Double) As Long
Declare PtrSafe Function XPRSgetcutmap Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ncuts As Long, cuts As Long, cutmap As Long) As Long
Declare PtrSafe Function XPRSbasisstability Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal typecode As Long, ByVal norm As Long, ByVal ifscaled As Long, ByRef dval As Double) As Long
Declare PtrSafe Function XPRScalcslacks Lib "XPRS.DLL" (ByVal prob As LongPtr, solution As Double, calculatedslacks As Double) As Long
Declare PtrSafe Function XPRScalcreducedcosts Lib "XPRS.DLL" (ByVal prob As LongPtr, duals As Double, solution As Double, calculateddjs As Double) As Long
Declare PtrSafe Function XPRScalcobjective Lib "XPRS.DLL" (ByVal prob As LongPtr, solution As Double, ByRef objective As Double) As Long
Declare PtrSafe Function XPRSrefinemipsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal options As Long, ByVal sflags As String, solution As Double, refined_solution As Double, ByRef refinestatus As Long) As Long
Declare PtrSafe Function XPRScalcsolinfo Lib "XPRS.DLL" (ByVal prob As LongPtr, solution As Double, dual As Double, ByVal Property As Long, ByRef Value As Double) As Long
Declare PtrSafe Function XPRSgetnamelist Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal itype As Long, sbuff As Byte, ByVal names_len As Long, ByRef names_len_reqd As Long, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetnamelistobject Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal itype As Long, ByRef r_nl As LongPtr) As Long
Declare PtrSafe Function XPRSlogfilehandler Lib "XPRS.DLL" (ByVal obj As LongPtr, ByVal vUserContext As String, ByVal vSystemThreadId As LongPtr, ByVal sMsg As String, ByVal iMsgType As Long, ByVal iMsgCode As Long) As Long
Private Declare PtrSafe Function XPRSgetobjecttypename_C Lib "XPRS.DLL" Alias "XPRSgetobjecttypename" (ByVal obj As LongPtr, ByRef sObjectName As Long) As Long
Declare PtrSafe Function XPRSstrongbranchcb Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nbnds As Long, mindex As Long, sboundtype As Byte, dbnd As Double, ByVal itrlimit As Long, dsbobjval As Double, msbstatus As Long, ByVal sbsolvecb As LongPtr, ByVal vContext As String) As Long
Declare PtrSafe Function XPRSsetcblplog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_lplog As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcblplog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_lplog As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcblplog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_lplog As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecblplog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_lplog As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbgloballog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_globallog As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbgloballog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_globallog As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbgloballog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_globallog As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbgloballog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_globallog As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbcutlog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_cutlog As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbcutlog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_cutlog As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbcutlog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_cutlog As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbcutlog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_cutlog As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbbarlog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_barlog As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbbarlog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_barlog As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbbarlog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_barlog As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbbarlog Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_barlog As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbcutmgr Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_cutmgr As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbcutmgr Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_cutmgr As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbcutmgr Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_cutmgr As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbcutmgr Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_cutmgr As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbchgnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_chgnode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbchgnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_chgnode As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbchgnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_chgnode As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbchgnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_chgnode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcboptnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_optnode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcboptnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_optnode As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcboptnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_optnode As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecboptnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_optnode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbprenode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_prenode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbprenode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_prenode As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbprenode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_prenode As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbprenode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_prenode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbinfnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_infnode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbinfnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_infnode As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbinfnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_infnode As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbinfnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_infnode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbnodecutoff Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_nodecutoff As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbnodecutoff Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_nodecutoff As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbnodecutoff Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_nodecutoff As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbnodecutoff Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_nodecutoff As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbintsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_intsol As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbintsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_intsol As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbintsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_intsol As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbintsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_intsol As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbpreintsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_preintsol As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbpreintsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_preintsol As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbpreintsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_preintsol As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbpreintsol Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_preintsol As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbchgbranch Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_chgbranch As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbchgbranch Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_chgbranch As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbchgbranch Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_chgbranch As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbchgbranch Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_chgbranch As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbestimate Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_estimate As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbestimate Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_estimate As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbestimate Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_estimate As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbestimate Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_estimate As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbsepnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_sepnode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbsepnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_sepnode As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbsepnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_sepnode As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbsepnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_sepnode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbmessage Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_message As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbmessage Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_message As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbmessage Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_message As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbmessage Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_message As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbmipthread Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_mipthread As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbmipthread Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_mipthread As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbmipthread Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_mipthread As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbmipthread Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_mipthread As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbdestroymt Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_destroymt As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbdestroymt Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_destroymt As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbdestroymt Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_destroymt As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbdestroymt Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_destroymt As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbnewnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_newnode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbnewnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_newnode As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbnewnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_newnode As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbnewnode Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_newnode As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbbariteration Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_bariteration As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbbariteration Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_bariteration As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbbariteration Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_bariteration As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbbariteration Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_bariteration As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbchgbranchobject Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_chgbranchobject As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbchgbranchobject Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_chgbranchobject As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbchgbranchobject Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_chgbranchobject As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbchgbranchobject Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_chgbranchobject As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbgapnotify Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_gapnotify As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbgapnotify Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_gapnotify As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbgapnotify Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_gapnotify As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbgapnotify Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_gapnotify As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSsetcbusersolnotify Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_usersolnotify As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRSgetcbusersolnotify Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef f_usersolnotify As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRSaddcbusersolnotify Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_usersolnotify As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRSremovecbusersolnotify Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_usersolnotify As LongPtr, ByVal p As String) As Long

Declare PtrSafe Function XPRSobjsa Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal ncols As Long, mindex As Long, lower As Double, upper As Double) As Long
Declare PtrSafe Function XPRSrhssa Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal nrows As Long, mindex As Long, lower As Double, upper As Double) As Long

Declare PtrSafe Function XPRSstartrecord Lib "XPRS.DLL" (ByVal path As String, ByVal cfgfilepath As String, ByVal flags As Long) As Long
Declare PtrSafe Function XPRSstoprecord Lib "XPRS.DLL" () As Long

Declare PtrSafe Function XPRS_ge_setcbmsghandler Lib "XPRS.DLL" (ByVal f_msghandler As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRS_ge_getcbmsghandler Lib "XPRS.DLL" (ByRef f_msghandler As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRS_ge_addcbmsghandler Lib "XPRS.DLL" (ByVal f_msghandler As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRS_ge_removecbmsghandler Lib "XPRS.DLL" (ByVal f_msghandler As LongPtr, ByVal p As String) As Long

Private Declare PtrSafe Function XPRS_ge_getlasterror_C Lib "XPRS.DLL" Alias "XPRS_ge_getlasterror" (ByRef iMsgCode As Long, ByVal msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long

Declare PtrSafe Function XPRS_msp_create Lib "XPRS.DLL" (ByRef msp As LongPtr) As Long
Declare PtrSafe Function XPRS_msp_destroy Lib "XPRS.DLL" (ByVal msp As LongPtr) As Long
Declare PtrSafe Function XPRS_msp_probattach Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob As LongPtr) As Long
Declare PtrSafe Function XPRS_msp_probdetach Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob As LongPtr) As Long
Declare PtrSafe Function XPRS_msp_getsollist Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob_to_rank_against As LongPtr, ByVal iRankAttrib As Long, ByVal bRankAscending As Long, ByVal iRankFirstIndex_Ob As Long, ByVal iRankLastIndex_Ob As Long, iSolutionIds_Zb As Long, ByRef nReturnedSolIds As Long, ByRef nSols As Long) As Long
Declare PtrSafe Function XPRS_msp_getsollist2 Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob_to_rank_against As LongPtr, ByVal iRankAttrib As Long, ByVal bRankAscending As Long, ByVal iRankFirstIndex_Ob As Long, ByVal iRankLastIndex_Ob As Long, ByVal bUseUserBitFilter As Long, ByVal iUserBitMask As Long, ByVal iUserBitPattern As Long, ByVal bUseInternalBitFilter As Long, ByVal iInternalBitMask As Long, ByVal iInternalBitPattern As Long, iSolutionIds_Zb As Long, ByRef nReturnedSolIds As Long, ByRef nSols As Long) As Long
Declare PtrSafe Function XPRS_msp_getsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, x As Double, ByVal iColFirst As Long, ByVal iColLast As Long, ByRef nValuesReturned As Long) As Long
Declare PtrSafe Function XPRS_msp_getslack Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob_to_rank_against As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, slack As Double, ByVal iRowFirst As Long, ByVal iRowLast As Long, ByRef nValuesReturned As Long) As Long
Declare PtrSafe Function XPRS_msp_loadsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByRef iSolutionId As Long, x As Double, ByVal nCols As Long, ByVal sSolutionName As String, ByRef bNameModifiedForUniqueness As Long, ByRef iSolutionIdOfExistingDuplicatePreventedLoad As Long) As Long
Declare PtrSafe Function XPRS_msp_delsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long) As Long
Declare PtrSafe Function XPRS_msp_getintattribprobsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob_to_rank_against As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iAttribId As Long, ByRef Dst As Long) As Long
Declare PtrSafe Function XPRS_msp_getdblattribprobsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob_to_rank_against As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iAttribId As Long, ByRef Dst As Double) As Long
Declare PtrSafe Function XPRS_msp_getintattribprob Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob As LongPtr, ByVal iAttribId As Long, ByRef Dst As Long) As Long
Declare PtrSafe Function XPRS_msp_getdblattribprob Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob As LongPtr, ByVal iAttribId As Long, ByRef Dst As Double) As Long
Declare PtrSafe Function XPRS_msp_getintattribsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iAttribId As Long, ByRef Dst As Long) As Long
Declare PtrSafe Function XPRS_msp_getdblattribsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iAttribId As Long, ByRef Dst As Double) As Long
Declare PtrSafe Function XPRS_msp_getintcontrolsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iControlId As Long, ByRef Val As Long) As Long
Declare PtrSafe Function XPRS_msp_getdblcontrolsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iControlId As Long, ByRef Val As Double) As Long
Declare PtrSafe Function XPRS_msp_setintcontrolsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iControlId As Long, ByVal Val As Long) As Long
Declare PtrSafe Function XPRS_msp_setdblcontrolsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iControlId As Long, ByVal Val As Double) As Long
Declare PtrSafe Function XPRS_msp_getintattribprobextreme Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob_to_rank_against As LongPtr, ByVal bGet_Max_Otherwise_Min As Long, ByRef iSolutionId As Long, ByVal iAttribId As Long, ByRef ExtremeVal As Long) As Long
Declare PtrSafe Function XPRS_msp_getdblattribprobextreme Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob_to_rank_against As LongPtr, ByVal bGet_Max_Otherwise_Min As Long, ByRef iSolutionId As Long, ByVal iAttribId As Long, ByRef ExtremeVal As Double) As Long
Declare PtrSafe Function XPRS_msp_getintattrib Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iAttribId As Long, ByRef Val As Long) As Long
Declare PtrSafe Function XPRS_msp_getdblattrib Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iAttribId As Long, ByRef Val As Double) As Long
Declare PtrSafe Function XPRS_msp_getintcontrol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iControlId As Long, ByRef Val As Long) As Long
Declare PtrSafe Function XPRS_msp_getdblcontrol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iControlId As Long, ByRef Val As Double) As Long
Declare PtrSafe Function XPRS_msp_setintcontrol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iControlId As Long, ByVal Val As Long) As Long
Declare PtrSafe Function XPRS_msp_setdblcontrol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iControlId As Long, ByVal Val As Double) As Long
Declare PtrSafe Function XPRS_msp_setsolname Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal iSolutionId As Long, ByVal sNewSolutionBaseName As String, ByRef bNameModifiedForUniqueness As Long, ByRef iSolutionIdStatus_ As Long) As Long
Private Declare PtrSafe Function XPRS_msp_getsolname_C Lib "XPRS.DLL" Alias "XPRS_msp_getsolname" (ByVal msp As LongPtr, ByVal iSolutionId As Long, ByVal sname As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long, ByRef iSolutionIdStatus_ As Long) As Long
Declare PtrSafe Function XPRS_msp_findsolbyname Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal sSolutionName As String, ByRef iSolutionId As Long) As Long
Declare PtrSafe Function XPRS_msp_writeslxsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal prob_context As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal sFileName As String, ByVal sFlags As String) As Long
Declare PtrSafe Function XPRS_msp_readslxsol Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal col_name_list As LongPtr, ByVal sFileName As String, ByVal sFlags As String, ByRef iSolutionId_Beg As Long, ByRef iSolutionId_End As Long) As Long
Private Declare PtrSafe Function XPRS_msp_getlasterror_C Lib "XPRS.DLL" Alias "XPRS_msp_getlasterror" (ByVal msp As LongPtr, ByRef iMsgCode As Long, ByVal msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
Declare PtrSafe Function XPRS_msp_setcbmsghandler Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal f_msghandler As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRS_msp_getcbmsghandler Lib "XPRS.DLL" (ByVal msp As LongPtr, ByRef f_msghandler As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRS_msp_addcbmsghandler Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal f_msghandler As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRS_msp_removecbmsghandler Lib "XPRS.DLL" (ByVal msp As LongPtr, ByVal f_msghandler As LongPtr, ByVal p As String) As Long

Declare PtrSafe Function XPRS_nml_create Lib "XPRS.DLL" (ByRef r_nl As LongPtr) As Long
Declare PtrSafe Function XPRS_nml_destroy Lib "XPRS.DLL" (ByVal nml As LongPtr) As Long
Declare PtrSafe Function XPRS_nml_getnamecount Lib "XPRS.DLL" (ByVal nml As LongPtr, ByRef count As Long) As Long
Declare PtrSafe Function XPRS_nml_getmaxnamelen Lib "XPRS.DLL" (ByVal nml As LongPtr, ByRef namlen As Long) As Long
Declare PtrSafe Function XPRS_nml_getnames Lib "XPRS.DLL" (ByVal nml As LongPtr, ByVal padlen As Long, buf As Byte, ByVal buflen As Long, ByRef r_buflen_reqd As Long, ByVal firstIndex As Long, ByVal lastIndex As Long) As Long
Declare PtrSafe Function XPRS_nml_addnames Lib "XPRS.DLL" (ByVal nml As LongPtr, buf As Byte, ByVal firstIndex As Long, ByVal lastIndex As Long) As Long
Declare PtrSafe Function XPRS_nml_removenames Lib "XPRS.DLL" (ByVal nml As LongPtr, ByVal firstIndex As Long, ByVal lastIndex As Long) As Long
Declare PtrSafe Function XPRS_nml_findname Lib "XPRS.DLL" (ByVal nml As LongPtr, ByVal name As String, ByRef r_index As Long) As Long
Declare PtrSafe Function XPRS_nml_copynames Lib "XPRS.DLL" (ByVal dst As LongPtr, ByVal src As LongPtr) As Long
Private Declare PtrSafe Function XPRS_nml_getlasterror_C Lib "XPRS.DLL" Alias "XPRS_nml_getlasterror" (ByVal nml As LongPtr, ByRef iMsgCode As Long, ByVal msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long

Declare PtrSafe Function XPRSgetqrowcoeff Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal irow As Long, ByVal icol As Long, ByVal jcol As Long, ByRef dval As Double) As Long
Declare PtrSafe Function XPRSgetqrowqmatrix Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal irow As Long, mstart As Long, mclind As Long, dobjval As Double, ByVal maxcoeffs As Long, ByRef ncoeffs As Long, ByVal first As Long, ByVal last As Long) As Long
Declare PtrSafe Function XPRSgetqrowqmatrixtriplets Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal irow As Long, ByRef nqelem As Long, mqcol1 As Long, mqcol2 As Long, dqe As Double) As Long
Declare PtrSafe Function XPRSchgqrowcoeff Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal irow As Long, ByVal icol As Long, ByVal jcol As Long, ByVal dval As Double) As Long
Declare PtrSafe Function XPRSgetqrows Lib "XPRS.DLL" (ByVal prob As LongPtr, ByRef qmn As Long, qcrows As Long) As Long
Declare PtrSafe Function XPRSaddqmatrix Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal irow As Long, ByVal nqtr As LongLong, mqc1 As Long, mqc2 As Long, dqew As Double) As Long
Declare PtrSafe Function XPRSdelqmatrix Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal irow As Long) As Long
Declare PtrSafe Function XPRSloadqcqp Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, drange As Double, dobj As Double, mstart As Long, mnel As Long, mrwind As Long, dmatval As Double, dlb As Double, dub As Double, ByVal nquads As LongLong, mqcol1 As Long, mqcol2 As Long, dqval As Double, ByVal qmn As Long, qcrows As Long, qcnquads As Long, qcmqcol1 As Long, qcmqcol2 As Long, qcdqval As Double) As Long
Declare PtrSafe Function XPRSloadqcqpglobal Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, drange As Double, dobj As Double, matbeg As Long, matcnt As Long, matrow As Long, dmatval As Double, dlb As Double, dub As Double, ByVal nquads As LongLong, mqcol1 As Long, mqcol2 As Long, dqval As Double, ByVal qmn As Long, qcrows As Long, qcnquads As Long, qcmqcol1 As Long, qcmqcol2 As Long, qcdqval As Double, ByVal ngents As Long, ByVal nsets As Long, qgtype As Byte, mgcols As Long, dlim As Double, qstype As Byte, msstart As Long, mscols As Long, dref As Double) As Long

Declare PtrSafe Function XPRS_mse_create Lib "XPRS.DLL" (ByRef mse As LongPtr) As Long
Declare PtrSafe Function XPRS_mse_destroy Lib "XPRS.DLL" (ByVal mse As LongPtr) As Long
Declare PtrSafe Function XPRS_mse_getsollist Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal iMetricId As Long, ByVal iRankFirstIndex_Ob As Long, ByVal iRankLastIndex_Ob As Long, iSolutionIds As Long, ByRef nReturnedSolIds As Long, ByRef nSols As Long) As Long
Declare PtrSafe Function XPRS_mse_getsolmetric Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal iSolutionId As Long, ByRef iSolutionIdStatus As Long, ByVal iMetricId As Long, ByRef dMetric As Double) As Long
Declare PtrSafe Function XPRS_mse_getcullchoice Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal iMetricId As Long, cull_sol_id_list As Long, ByVal nMaxSolsToCull As Long, ByRef nSolsToCull As Long, ByVal dNewSolMetric As Double, x As Double, ByVal nCols As Long, ByRef bRejectSoln As Long) As Long
Declare PtrSafe Function XPRS_mse_minim Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal prob As LongPtr, ByVal msp As LongPtr, ByVal f_mse_handler As LongPtr, ByVal p As String, ByRef nMaxSols As Long) As Long
Declare PtrSafe Function XPRS_mse_maxim Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal prob As LongPtr, ByVal msp As LongPtr, ByVal f_mse_handler As LongPtr, ByVal p As String, ByRef nMaxSols As Long) As Long
Declare PtrSafe Function XPRS_mse_opt Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal prob As LongPtr, ByVal msp As LongPtr, ByVal f_mse_handler As LongPtr, ByVal p As String, ByRef nMaxSols As Long) As Long
Declare PtrSafe Function XPRS_mse_getintattrib Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal iAttribId As Long, ByRef Val As Long) As Long
Declare PtrSafe Function XPRS_mse_getdblattrib Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal iAttribId As Long, ByRef Val As Double) As Long
Declare PtrSafe Function XPRS_mse_getintcontrol Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal iAttribId As Long, ByRef Val As Long) As Long
Declare PtrSafe Function XPRS_mse_getdblcontrol Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal iAttribId As Long, ByRef Val As Double) As Long
Declare PtrSafe Function XPRS_mse_setintcontrol Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal iAttribId As Long, ByVal Val As Long) As Long
Declare PtrSafe Function XPRS_mse_setdblcontrol Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal iAttribId As Long, ByVal Val As Double) As Long
Private Declare PtrSafe Function XPRS_mse_getlasterror_C Lib "XPRS.DLL" Alias "XPRS_mse_getlasterror" (ByVal mse As LongPtr, ByRef iMsgCode As Long, ByVal msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
Declare PtrSafe Function XPRS_mse_setsolbasename Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal sSolutionBaseName As String) As Long
Private Declare PtrSafe Function XPRS_mse_getsolbasename_C Lib "XPRS.DLL" Alias "XPRS_mse_getsolbasename" (ByVal mse As LongPtr, ByVal sname As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
Declare PtrSafe Function XPRS_mse_setcbgetsolutiondiff Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal f_mse_getsolutiondiff As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRS_mse_getcbgetsolutiondiff Lib "XPRS.DLL" (ByVal mse As LongPtr, ByRef f_mse_getsolutiondiff As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRS_mse_addcbgetsolutiondiff Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal f_mse_getsolutiondiff As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRS_mse_removecbgetsolutiondiff Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal f_mse_getsolutiondiff As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRS_mse_setcbmsghandler Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal f_msghandler As LongPtr, ByVal p As String) As Long
Declare PtrSafe Function XPRS_mse_getcbmsghandler Lib "XPRS.DLL" (ByVal mse As LongPtr, ByRef f_msghandler As LongPtr, ByRef p As String) As Long
Declare PtrSafe Function XPRS_mse_addcbmsghandler Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal f_msghandler As LongPtr, ByVal p As String, ByVal priority As Long) As Long
Declare PtrSafe Function XPRS_mse_removecbmsghandler Lib "XPRS.DLL" (ByVal mse As LongPtr, ByVal f_msghandler As LongPtr, ByVal p As String) As Long

Declare PtrSafe Function XPRS_bo_create Lib "XPRS.DLL" (ByRef p_object As LongPtr, ByVal prob As LongPtr, ByVal isoriginal As Long) As Long
Declare PtrSafe Function XPRS_bo_destroy Lib "XPRS.DLL" (ByVal obranch As LongPtr) As Long
Declare PtrSafe Function XPRS_bo_store Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByRef p_status As Long) As Long
Declare PtrSafe Function XPRS_bo_addbranches Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByVal nbranches As Long) As Long
Declare PtrSafe Function XPRS_bo_getbranches Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByRef p_nbranches As Long) As Long
Declare PtrSafe Function XPRS_bo_setpriority Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByVal ipriority As Long) As Long
Declare PtrSafe Function XPRS_bo_setpreferredbranch Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByVal ibranch As Long) As Long
Declare PtrSafe Function XPRS_bo_addbounds Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByVal ibranch As Long, ByVal nbounds As Long, cbndtype As Byte, mbndcol As Long, dbndval As Double) As Long
Declare PtrSafe Function XPRS_bo_getbounds Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByVal ibranch As Long, ByRef p_nbounds As Long, ByVal nbounds_size As Long, cbndtype As Byte, mbndcol As Long, dbndval As Double) As Long
Declare PtrSafe Function XPRS_bo_addrows Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByVal ibranch As Long, ByVal nrows As Long, ByVal nelems As Long, crtype As Byte, drrhs As Double, mrbeg As Long, mcol As Long, dval As Double) As Long
Declare PtrSafe Function XPRS_bo_getrows Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByVal ibranch As Long, ByRef p_nrows As Long, ByVal nrows_size As Long, ByRef p_nelems As Long, ByVal nelems_size As Long, crtype As Byte, drrhs As Double, mrbeg As Long, mcol As Long, dval As Double) As Long
Declare PtrSafe Function XPRS_bo_addcuts Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByVal ibranch As Long, ByVal ncuts As Long, mcutind As Long) As Long
Declare PtrSafe Function XPRS_bo_getid Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByRef p_id As Long) As Long
Private Declare PtrSafe Function XPRS_bo_getlasterror_C Lib "XPRS.DLL" Alias "XPRS_bo_getlasterror" (ByVal obranch As LongPtr, ByRef iMsgCode As Long, ByVal msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
Declare PtrSafe Function XPRS_bo_validate Lib "XPRS.DLL" (ByVal obranch As LongPtr, ByRef p_status As Long) As Long


Declare PtrSafe Function XPRSsetcbmessageVB Lib "XPRS.DLL" (ByVal prob As LongPtr, ByVal f_message As LongPtr, ByRef p As LongPtr) As Long

' OS-provided utility functions we'll use
Private Declare PtrSafe Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length as Long)

' XPRS-provided utility functions we'll use
Private Declare PtrSafe Function CStrLen Lib "XPRS.DLL" Alias "XPRS_vbhelp_strlen" (ByVal lpStr As LongPtr, ByRef r_StrLen As Long) As Long

' Declarations of functions that allow NULL pointers to be passed

Declare PtrSafe Function XPRSloadlpr Lib "XPRS.DLL" Alias "XPRSloadlp" (ByVal prob As LongPtr, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, ByVal drange As String, dobj As Double, mstart As Long, ByVal mnel As String, mrwind As Long, dmatval As Double, dlb As Double, dub As Double) As Long
Declare PtrSafe Function XPRSloaddirsx Lib "XPRS.DLL" Alias "XPRSloaddirs" (ByVal prob As LongPtr, ByVal ndirs As Long, mcols As Long, mpri As Long, ByVal sbr As String, ByVal dupc As String, ByVal ddpc As String) As Long
Declare PtrSafe Function XPRSgetsolx Lib "XPRS.DLL" Alias "XPRSgetsol" (ByVal prob As LongPtr, dx As Double, ByVal dslack As String, ByVal dual As String, ByVal dj As String) As Long
Declare PtrSafe Function XPRSgetsolslack Lib "XPRS.DLL" Alias "XPRSgetsol" (ByVal prob As LongPtr, ByVal dx As String, dslack As Double, ByVal dual As String, ByVal dj As String) As Long
Declare PtrSafe Function XPRSgetsoldual Lib "XPRS.DLL" Alias "XPRSgetsol" (ByVal prob As LongPtr, ByVal dx As String, ByVal dslack As String, dual As Double, ByVal dj As String) As Long
Declare PtrSafe Function XPRSgetsoldj Lib "XPRS.DLL" Alias "XPRSgetsol" (ByVal prob As LongPtr, ByVal dx As String, ByVal dslack As String, ByVal dual As String, dj As Double) As Long
Declare PtrSafe Function XPRSgetpresolvesolx Lib "XPRS.DLL" Alias "XPRSgetpresolvesol" (ByVal prob As LongPtr, dx As Double, ByVal dslack As String, ByVal dual As String, ByVal dj As String) As Long
Declare PtrSafe Function XPRSgetpresolvesolslack Lib "XPRS.DLL" Alias "XPRSgetpresolvesol" (ByVal prob As LongPtr, ByVal dx As String, dslack As Double, ByVal dual As String, ByVal dj As String) As Long
Declare PtrSafe Function XPRSgetpresolvesoldual Lib "XPRS.DLL" Alias "XPRSgetpresolvesol" (ByVal prob As LongPtr, ByVal dx As String, ByVal dslack As String, dual As Double, ByVal dj As String) As Long
Declare PtrSafe Function XPRSgetpresolvesoldj Lib "XPRS.DLL" Alias "XPRSgetpresolvesol" (ByVal prob As LongPtr, ByVal dx As String, ByVal dslack As String, ByVal dual As String, dj As Double) As Long
Declare PtrSafe Function XPRSgetglobalx Lib "XPRS.DLL" Alias "XPRSgetglobal" (ByVal prob As LongPtr, ByRef ngents As Long, ByRef nsets As Long, sgtype As Byte, mgcols As Long, ByVal dlim As String, ByVal sstype As String, ByVal msstart As String, ByVal mscols As String, ByVal dref As String) As Long
Declare PtrSafe Function XPRSaddrowsr Lib "XPRS.DLL" Alias "XPRSaddrows" (ByVal prob As LongPtr, ByVal nrows As Long, ByVal ncoeffs As Long, srowtype As Byte, drhs As Double, ByVal drng As String, mstart As Long, mclind As Long, dmatval As Double) As Long

' Utility function to create a VB string when given the pointer to a C string
Private Function StringFromPointer(StringPointer As LongPtr) As String
  If StringPointer=0 Then
    StringFromPointer = vbNullString
  Else
  	Dim string_length As Long
  	CStrLen StringPointer, string_length
  	Dim str As String
  	str = Space$(string_length)
  	CopyMemory ByVal str, ByVal StringPointer, string_length
  	StringFromPointer = str
  End If
End Function


' VB wrappers for functions that need them
Function XPRSgetbanner(ByRef banner As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_banner As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetbanner_C(c_banner)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_banner, vbNullChar) > 1 Then
    banner = Left(c_banner, InStr(1, c_banner, vbNullChar) - 1)
  End If
  XPRSgetbanner = rval
End Function


Function XPRSgetversion(ByRef version As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_version As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetversion_C(c_version)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_version, vbNullChar) > 1 Then
    version = Left(c_version, InStr(1, c_version, vbNullChar) - 1)
  End If
  XPRSgetversion = rval
End Function


Function XPRSlicense(ByRef i1 As Long, ByRef c1 As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_c1 As String * 255
  c_c1 = c1
  Mid(c_c1, Len(c1) + 1, 1) = vbNullChar
  ' Call XPRS.DLL
  rval = XPRSlicense_C(i1, c_c1)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_c1, vbNullChar) > 1 Then
    c1 = Left(c_c1, InStr(1, c_c1, vbNullChar) - 1)
  End If
  XPRSlicense = rval
End Function



Function XPRSgetprobname(ByVal prob As LongPtr, ByRef svalue As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_svalue As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetprobname_C(prob, c_svalue)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_svalue, vbNullChar) > 1 Then
    svalue = Left(c_svalue, InStr(1, c_svalue, vbNullChar) - 1)
  End If
  XPRSgetprobname = rval
End Function


Function XPRSgetstrcontrol(ByVal prob As LongPtr, ByVal index As Long, ByRef svalue As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_svalue As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetstrcontrol_C(prob, index, c_svalue)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_svalue, vbNullChar) > 1 Then
    svalue = Left(c_svalue, InStr(1, c_svalue, vbNullChar) - 1)
  End If
  XPRSgetstrcontrol = rval
End Function


Function XPRSgetstringcontrol(ByVal prob As LongPtr, ByVal index As Long, ByRef svalue As String, ByVal svaluesize As Long, ByRef controlsize As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_svalue As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetstringcontrol_C(prob, index, c_svalue, svaluesize, controlsize)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_svalue, vbNullChar) > 1 Then
    svalue = Left(c_svalue, InStr(1, c_svalue, vbNullChar) - 1)
  End If
  XPRSgetstringcontrol = rval
End Function


Function XPRSgetstrattrib(ByVal prob As LongPtr, ByVal index As Long, ByRef cvalue As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_cvalue As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetstrattrib_C(prob, index, c_cvalue)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_cvalue, vbNullChar) > 1 Then
    cvalue = Left(c_cvalue, InStr(1, c_cvalue, vbNullChar) - 1)
  End If
  XPRSgetstrattrib = rval
End Function


Function XPRSgetstringattrib(ByVal prob As LongPtr, ByVal index As Long, ByRef cvalue As String, ByVal cvaluesize As Long, ByRef controlsize As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_cvalue As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetstringattrib_C(prob, index, c_cvalue, cvaluesize, controlsize)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_cvalue, vbNullChar) > 1 Then
    cvalue = Left(c_cvalue, InStr(1, c_cvalue, vbNullChar) - 1)
  End If
  XPRSgetstringattrib = rval
End Function



Function XPRSgetlasterror(ByVal prob As LongPtr, ByRef errmsg As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_errmsg As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetlasterror_C(prob, c_errmsg)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_errmsg, vbNullChar) > 1 Then
    errmsg = Left(c_errmsg, InStr(1, c_errmsg, vbNullChar) - 1)
  End If
  XPRSgetlasterror = rval
End Function


Function XPRSgetobjecttypename(ByVal obj As LongPtr, ByRef sObjectName As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_sObjectName As Long
  ' Call XPRS.DLL
  rval = XPRSgetobjecttypename_C(obj, c_sObjectName)
  ' Performing any necessary type conversions on output arguments
  sObjectName = StringFromPointer(c_sObjectName)
  XPRSgetobjecttypename = rval
End Function



Function XPRS_ge_getlasterror(ByRef iMsgCode As Long, ByRef msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRS_ge_getlasterror_C(iMsgCode, c_msg, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRS_ge_getlasterror = rval
End Function



Function XPRS_msp_getsolname(ByVal msp As LongPtr, ByVal iSolutionId As Long, ByRef sname As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long, ByRef iSolutionIdStatus_ As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_sname As String * 255
  ' Call XPRS.DLL
  rval = XPRS_msp_getsolname_C(msp, iSolutionId, c_sname, iStringBufferBytes, iBytesInInternalString, iSolutionIdStatus_)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_sname, vbNullChar) > 1 Then
    sname = Left(c_sname, InStr(1, c_sname, vbNullChar) - 1)
  End If
  XPRS_msp_getsolname = rval
End Function


Function XPRS_msp_getlasterror(ByVal msp As LongPtr, ByRef iMsgCode As Long, ByRef msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRS_msp_getlasterror_C(msp, iMsgCode, c_msg, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRS_msp_getlasterror = rval
End Function



Function XPRS_nml_getlasterror(ByVal nml As LongPtr, ByRef iMsgCode As Long, ByRef msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRS_nml_getlasterror_C(nml, iMsgCode, c_msg, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRS_nml_getlasterror = rval
End Function



Function XPRS_mse_getlasterror(ByVal mse As LongPtr, ByRef iMsgCode As Long, ByRef msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRS_mse_getlasterror_C(mse, iMsgCode, c_msg, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRS_mse_getlasterror = rval
End Function


Function XPRS_mse_getsolbasename(ByVal mse As LongPtr, ByRef sname As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_sname As String * 255
  ' Call XPRS.DLL
  rval = XPRS_mse_getsolbasename_C(mse, c_sname, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_sname, vbNullChar) > 1 Then
    sname = Left(c_sname, InStr(1, c_sname, vbNullChar) - 1)
  End If
  XPRS_mse_getsolbasename = rval
End Function



Function XPRS_bo_getlasterror(ByVal obranch As LongPtr, ByRef iMsgCode As Long, ByRef msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRS_bo_getlasterror_C(obranch, iMsgCode, c_msg, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRS_bo_getlasterror = rval
End Function




Function XPRSgetlicerrmsg(ByRef msg As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetlicerrmsg_C(c_msg, 255)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRSgetlicerrmsg = rval
End Function

Function XPRSgetbannerVB(ByRef banner As String) As Long
  XPRSgetbannerVB = XPRSgetbanner(banner)
End Function

Function XPRSgetversionVB(ByRef version As String) As Long
  XPRSgetversionVB = XPRSgetversion(version)
End Function

Function XPRSlicenseVB(ByRef i1 As Long, ByRef c1 As String) As Long
  XPRSlicenseVB = XPRSlicense(i1, c1)
End Function

Function XPRSgetlicerrmsgVB(ByRef msg As String) As Long
  XPRSgetlicerrmsgVB = XPRSgetlicerrmsg(msg)
End Function

Function XPRSgetprobnameVB(ByVal prob As LongPtr, ByRef svalue As String) As Long
  XPRSgetprobnameVB = XPRSgetprobname(prob, svalue)
End Function

Function XPRSgetstrcontrolVB(ByVal prob As LongPtr , ByVal index As Long, ByRef svalue As String) As Long
  XPRSgetstrcontrolVB = XPRSgetstrcontrol(prob, index, svalue)
End Function

Function XPRSgetstrattribVB(ByVal prob As LongPtr, ByVal index As Long, ByRef cvalue As String) As Long
  XPRSgetstrattribVB = XPRSgetstrattrib(prob, index, cvalue)
End Function

Function XPRSgetlasterrorVB(ByVal prob As LongPtr, ByRef errmsg As String) As Long
  XPRSgetlasterrorVB = XPRSgetlasterror(prob, errmsg)
End Function







#Else
' 32-bit VB6 functions



' Integer control variables for XPRSprob
Global Const XPRS_EXTRAROWS               =     8004
Global Const XPRS_EXTRACOLS               =     8005
Global Const XPRS_LPITERLIMIT             =     8007
Global Const XPRS_LPLOG                   =     8009
Global Const XPRS_SCALING                 =     8010
Global Const XPRS_PRESOLVE                =     8011
Global Const XPRS_CRASH                   =     8012
Global Const XPRS_PRICINGALG              =     8013
Global Const XPRS_INVERTFREQ              =     8014
Global Const XPRS_INVERTMIN               =     8015
Global Const XPRS_MAXNODE                 =     8018
Global Const XPRS_MAXTIME                 =     8020
Global Const XPRS_MAXMIPSOL               =     8021
Global Const XPRS_DEFAULTALG              =     8023
Global Const XPRS_VARSELECTION            =     8025
Global Const XPRS_NODESELECTION           =     8026
Global Const XPRS_BACKTRACK               =     8027
Global Const XPRS_MIPLOG                  =     8028
Global Const XPRS_KEEPNROWS               =     8030
Global Const XPRS_MPSECHO                 =     8032
Global Const XPRS_MAXPAGELINES            =     8034
Global Const XPRS_OUTPUTLOG               =     8035
Global Const XPRS_BARSOLUTION             =     8038
Global Const XPRS_CACHESIZE               =     8043
Global Const XPRS_CROSSOVER               =     8044
Global Const XPRS_BARITERLIMIT            =     8045
Global Const XPRS_CHOLESKYALG             =     8046
Global Const XPRS_BAROUTPUT               =     8047
Global Const XPRS_CSTYLE                  =     8050
Global Const XPRS_EXTRAMIPENTS            =     8051
Global Const XPRS_REFACTOR                =     8052
Global Const XPRS_BARTHREADS              =     8053
Global Const XPRS_KEEPBASIS               =     8054
Global Const XPRS_VERSION                 =     8061
Global Const XPRS_BIGMMETHOD              =     8068
Global Const XPRS_MPSNAMELENGTH           =     8071
Global Const XPRS_PRESOLVEOPS             =     8077
Global Const XPRS_MIPPRESOLVE             =     8078
Global Const XPRS_MIPTHREADS              =     8079
Global Const XPRS_BARORDER                =     8080
Global Const XPRS_BREADTHFIRST            =     8082
Global Const XPRS_AUTOPERTURB             =     8084
Global Const XPRS_DENSECOLLIMIT           =     8086
Global Const XPRS_CALLBACKFROMMASTERTHREAD =     8090
Global Const XPRS_MAXMCOEFFBUFFERELEMS    =     8091
Global Const XPRS_REFINEOPS               =     8093
Global Const XPRS_LPREFINEITERLIMIT       =     8094
Global Const XPRS_MIPREFINEITERLIMIT      =     8095
Global Const XPRS_DUALIZEOPS              =     8097
Global Const XPRS_MAXMEMORY               =     8112
Global Const XPRS_CUTFREQ                 =     8116
Global Const XPRS_SYMSELECT               =     8117
Global Const XPRS_SYMMETRY                =     8118
Global Const XPRS_LPTHREADS               =     8124
Global Const XPRS_MIQCPALG                =     8125
Global Const XPRS_QCCUTS                  =     8126
Global Const XPRS_QCROOTALG               =     8127
Global Const XPRS_ALGAFTERNETWORK         =     8129
Global Const XPRS_TRACE                   =     8130
Global Const XPRS_MAXIIS                  =     8131
Global Const XPRS_CPUTIME                 =     8133
Global Const XPRS_COVERCUTS               =     8134
Global Const XPRS_GOMCUTS                 =     8135
Global Const XPRS_MPSFORMAT               =     8137
Global Const XPRS_CUTSTRATEGY             =     8138
Global Const XPRS_CUTDEPTH                =     8139
Global Const XPRS_TREECOVERCUTS           =     8140
Global Const XPRS_TREEGOMCUTS             =     8141
Global Const XPRS_CUTSELECT               =     8142
Global Const XPRS_TREECUTSELECT           =     8143
Global Const XPRS_DUALIZE                 =     8144
Global Const XPRS_DUALGRADIENT            =     8145
Global Const XPRS_SBITERLIMIT             =     8146
Global Const XPRS_SBBEST                  =     8147
Global Const XPRS_MAXCUTTIME              =     8149
Global Const XPRS_ACTIVESET               =     8152
Global Const XPRS_BARINDEFLIMIT           =     8153
Global Const XPRS_HEURSTRATEGY            =     8154
Global Const XPRS_HEURFREQ                =     8155
Global Const XPRS_HEURDEPTH               =     8156
Global Const XPRS_HEURMAXSOL              =     8157
Global Const XPRS_HEURNODES               =     8158
Global Const XPRS_LNPBEST                 =     8160
Global Const XPRS_LNPITERLIMIT            =     8161
Global Const XPRS_BRANCHCHOICE            =     8162
Global Const XPRS_BARREGULARIZE           =     8163
Global Const XPRS_SBSELECT                =     8164
Global Const XPRS_LOCALCHOICE             =     8170
Global Const XPRS_LOCALBACKTRACK          =     8171
Global Const XPRS_DUALSTRATEGY            =     8174
Global Const XPRS_L1CACHE                 =     8175
Global Const XPRS_HEURDIVESTRATEGY        =     8177
Global Const XPRS_HEURSELECT              =     8178
Global Const XPRS_BARSTART                =     8180
Global Const XPRS_BARNUMSTABILITY         =     8186
Global Const XPRS_EXTRASETS               =     8190
Global Const XPRS_FEASIBILITYPUMP         =     8193
Global Const XPRS_PRECOEFELIM             =     8194
Global Const XPRS_PREDOMCOL               =     8195
Global Const XPRS_HEURSEARCHFREQ          =     8196
Global Const XPRS_HEURDIVESPEEDUP         =     8197
Global Const XPRS_SBESTIMATE              =     8198
Global Const XPRS_BARCORES                =     8202
Global Const XPRS_MAXCHECKSONMAXTIME      =     8203
Global Const XPRS_MAXCHECKSONMAXCUTTIME   =     8204
Global Const XPRS_HISTORYCOSTS            =     8206
Global Const XPRS_ALGAFTERCROSSOVER       =     8208
Global Const XPRS_LINELENGTH              =     8209
Global Const XPRS_MUTEXCALLBACKS          =     8210
Global Const XPRS_BARCRASH                =     8211
Global Const XPRS_HEURDIVESOFTROUNDING    =     8215
Global Const XPRS_HEURSEARCHROOTSELECT    =     8216
Global Const XPRS_HEURSEARCHTREESELECT    =     8217
Global Const XPRS_MPS18COMPATIBLE         =     8223
Global Const XPRS_ROOTPRESOLVE            =     8224
Global Const XPRS_CROSSOVERDRP            =     8227
Global Const XPRS_FORCEOUTPUT             =     8229
Global Const XPRS_DETERMINISTIC           =     8232
Global Const XPRS_PREPROBING              =     8238
Global Const XPRS_EXTRAQCELEMENTS         =     8240
Global Const XPRS_EXTRAQCROWS             =     8241
Global Const XPRS_TREEMEMORYLIMIT         =     8242
Global Const XPRS_TREECOMPRESSION         =     8243
Global Const XPRS_TREEDIAGNOSTICS         =     8244
Global Const XPRS_MAXGLOBALFILESIZE       =     8245
Global Const XPRS_TEMPBOUNDS              =     8250
Global Const XPRS_IFCHECKCONVEXITY        =     8251
Global Const XPRS_PRIMALUNSHIFT           =     8252
Global Const XPRS_REPAIRINDEFINITEQ       =     8254
Global Const XPRS_MAXLOCALBACKTRACK       =     8257
Global Const XPRS_BACKTRACKTIE            =     8266
Global Const XPRS_BRANCHDISJ              =     8267
Global Const XPRS_MIPFRACREDUCE           =     8270
Global Const XPRS_CONCURRENTTHREADS       =     8274
Global Const XPRS_MAXSCALEFACTOR          =     8275
Global Const XPRS_HEURTHREADS             =     8276
Global Const XPRS_THREADS                 =     8278
Global Const XPRS_HEURBEFORELP            =     8280
Global Const XPRS_PREDOMROW               =     8281
Global Const XPRS_BRANCHSTRUCTURAL        =     8282
Global Const XPRS_QUADRATICUNSHIFT        =     8284
Global Const XPRS_BARPRESOLVEOPS          =     8286
Global Const XPRS_QSIMPLEXOPS             =     8288
Global Const XPRS_CONFLICTCUTS            =     8292
Global Const XPRS_CORESPERCPU             =     8296
Global Const XPRS_SLEEPONTHREADWAIT       =     8302
Global Const XPRS_PREDUPROW               =     8307
Global Const XPRS_CPUPLATFORM             =     8312
Global Const XPRS_BARALG                  =     8315
Global Const XPRS_TREEPRESOLVE            =     8320
Global Const XPRS_TREEPRESOLVE_KEEPBASIS  =     8321
Global Const XPRS_TREEPRESOLVEOPS         =     8322
Global Const XPRS_LPLOGSTYLE              =     8326
Global Const XPRS_RANDOMSEED              =     8328
Global Const XPRS_TREEQCCUTS              =     8331
Global Const XPRS_PRELINDEP               =     8333
Global Const XPRS_DUALTHREADS             =     8334
Global Const XPRS_PREOBJCUTDETECT         =     8336
Global Const XPRS_PREBNDREDQUAD           =     8337
Global Const XPRS_PREBNDREDCONE           =     8338
Global Const XPRS_PRECOMPONENTS           =     8339


' Double control variables for XPRSprob
Global Const XPRS_MATRIXTOL               =     7001
Global Const XPRS_PIVOTTOL                =     7002
Global Const XPRS_FEASTOL                 =     7003
Global Const XPRS_OUTPUTTOL               =     7004
Global Const XPRS_SOSREFTOL               =     7005
Global Const XPRS_OPTIMALITYTOL           =     7006
Global Const XPRS_ETATOL                  =     7007
Global Const XPRS_RELPIVOTTOL             =     7008
Global Const XPRS_MIPTOL                  =     7009
Global Const XPRS_MIPTOLTARGET            =     7010
Global Const XPRS_MIPADDCUTOFF            =     7012
Global Const XPRS_MIPABSCUTOFF            =     7013
Global Const XPRS_MIPRELCUTOFF            =     7014
Global Const XPRS_PSEUDOCOST              =     7015
Global Const XPRS_PENALTY                 =     7016
Global Const XPRS_BIGM                    =     7018
Global Const XPRS_MIPABSSTOP              =     7019
Global Const XPRS_MIPRELSTOP              =     7020
Global Const XPRS_CROSSOVERACCURACYTOL    =     7023
Global Const XPRS_CHOLESKYTOL             =     7032
Global Const XPRS_BARGAPSTOP              =     7033
Global Const XPRS_BARDUALSTOP             =     7034
Global Const XPRS_BARPRIMALSTOP           =     7035
Global Const XPRS_BARSTEPSTOP             =     7036
Global Const XPRS_ELIMTOL                 =     7042
Global Const XPRS_PERTURB                 =     7044
Global Const XPRS_MARKOWITZTOL            =     7047
Global Const XPRS_MIPABSGAPNOTIFY         =     7064
Global Const XPRS_MIPRELGAPNOTIFY         =     7065
Global Const XPRS_PPFACTOR                =     7069
Global Const XPRS_REPAIRINDEFINITEQMAX    =     7071
Global Const XPRS_BARGAPTARGET            =     7073
Global Const XPRS_SBEFFORT                =     7086
Global Const XPRS_HEURDIVERANDOMIZE       =     7089
Global Const XPRS_HEURSEARCHEFFORT        =     7090
Global Const XPRS_CUTFACTOR               =     7091
Global Const XPRS_EIGENVALUETOL           =     7097
Global Const XPRS_INDLINBIGM              =     7099
Global Const XPRS_TREEMEMORYSAVINGTARGET  =     7100
Global Const XPRS_GLOBALFILEBIAS          =     7101
Global Const XPRS_INDPRELINBIGM           =     7102
Global Const XPRS_RELAXTREEMEMORYLIMIT    =     7105
Global Const XPRS_MIPABSGAPNOTIFYOBJ      =     7108
Global Const XPRS_MIPABSGAPNOTIFYBOUND    =     7109
Global Const XPRS_PRESOLVEMAXGROW         =     7110
Global Const XPRS_HEURSEARCHTARGETSIZE    =     7112
Global Const XPRS_CROSSOVERRELPIVOTTOL    =     7113
Global Const XPRS_CROSSOVERRELPIVOTTOLSAFE =     7114
Global Const XPRS_DETLOGFREQ              =     7116
Global Const XPRS_MAXIMPLIEDBOUND         =     7120
Global Const XPRS_FEASTOLTARGET           =     7121
Global Const XPRS_OPTIMALITYTOLTARGET     =     7122


' String control variables for XPRSprob
Global Const XPRS_MPSRHSNAME              =     6001
Global Const XPRS_MPSOBJNAME              =     6002
Global Const XPRS_MPSRANGENAME            =     6003
Global Const XPRS_MPSBOUNDNAME            =     6004
Global Const XPRS_OUTPUTMASK              =     6005


' Integer attributes for XPRSprob
Global Const XPRS_ROWS                    =     1001
Global Const XPRS_SETS                    =     1004
Global Const XPRS_PRIMALINFEAS            =     1007
Global Const XPRS_DUALINFEAS              =     1008
Global Const XPRS_SIMPLEXITER             =     1009
Global Const XPRS_LPSTATUS                =     1010
Global Const XPRS_MIPSTATUS               =     1011
Global Const XPRS_CUTS                    =     1012
Global Const XPRS_NODES                   =     1013
Global Const XPRS_NODEDEPTH               =     1014
Global Const XPRS_ACTIVENODES             =     1015
Global Const XPRS_MIPSOLNODE              =     1016
Global Const XPRS_MIPSOLS                 =     1017
Global Const XPRS_COLS                    =     1018
Global Const XPRS_SPAREROWS               =     1019
Global Const XPRS_SPARECOLS               =     1020
Global Const XPRS_SPAREMIPENTS            =     1022
Global Const XPRS_ERRORCODE               =     1023
Global Const XPRS_MIPINFEAS               =     1024
Global Const XPRS_PRESOLVESTATE           =     1026
Global Const XPRS_PARENTNODE              =     1027
Global Const XPRS_NAMELENGTH              =     1028
Global Const XPRS_QELEMS                  =     1030
Global Const XPRS_NUMIIS                  =     1031
Global Const XPRS_MIPENTS                 =     1032
Global Const XPRS_BRANCHVAR               =     1036
Global Const XPRS_MIPTHREADID             =     1037
Global Const XPRS_ALGORITHM               =     1049
Global Const XPRS_ORIGINALROWS            =     1124
Global Const XPRS_CALLBACKCOUNT_OPTNODE   =     1136
Global Const XPRS_CALLBACKCOUNT_CUTMGR    =     1137
Global Const XPRS_ORIGINALQELEMS          =     1157
Global Const XPRS_MAXPROBNAMELENGTH       =     1158
Global Const XPRS_STOPSTATUS              =     1179
Global Const XPRS_ORIGINALMIPENTS         =     1191
Global Const XPRS_ORIGINALSETS            =     1194
Global Const XPRS_SPARESETS               =     1203
Global Const XPRS_CHECKSONMAXTIME         =     1208
Global Const XPRS_CHECKSONMAXCUTTIME      =     1209
Global Const XPRS_ORIGINALCOLS            =     1214
Global Const XPRS_QCELEMS                 =     1232
Global Const XPRS_QCONSTRAINTS            =     1234
Global Const XPRS_ORIGINALQCELEMS         =     1237
Global Const XPRS_ORIGINALQCONSTRAINTS    =     1239
Global Const XPRS_PEAKTOTALTREEMEMORYUSAGE =     1240
Global Const XPRS_CURRENTNODE             =     1248
Global Const XPRS_TREEMEMORYUSAGE         =     1251
Global Const XPRS_GLOBALFILESIZE          =     1252
Global Const XPRS_GLOBALFILEUSAGE         =     1253
Global Const XPRS_INDICATORS              =     1254
Global Const XPRS_ORIGINALINDICATORS      =     1255
Global Const XPRS_CORESDETECTED           =     1260
Global Const XPRS_BARSING                 =     1281
Global Const XPRS_BARSINGR                =     1282
Global Const XPRS_PRESOLVEINDEX           =     1284
Global Const XPRS_CONES                   =     1307
Global Const XPRS_CONEELEMS               =     1308
Global Const XPRS_BARITER                 =     5001
Global Const XPRS_BARAASIZE               =     5002
Global Const XPRS_BARLSIZE                =     5003
Global Const XPRS_BARDENSECOL             =     5004
Global Const XPRS_BARCROSSOVER            =     5005


' Double attributes for XPRSprob
Global Const XPRS_LPOBJVAL                =     2001
Global Const XPRS_SUMPRIMALINF            =     2002
Global Const XPRS_MIPOBJVAL               =     2003
Global Const XPRS_BESTBOUND               =     2004
Global Const XPRS_OBJRHS                  =     2005
Global Const XPRS_MIPBESTOBJVAL           =     2006
Global Const XPRS_OBJSENSE                =     2008
Global Const XPRS_BRANCHVALUE             =     2009
Global Const XPRS_PENALTYVALUE            =     2061
Global Const XPRS_CURRMIPCUTOFF           =     2062
Global Const XPRS_BARCONDA                =     2063
Global Const XPRS_BARCONDD                =     2064
Global Const XPRS_MAXABSPRIMALINFEAS      =     2073
Global Const XPRS_MAXRELPRIMALINFEAS      =     2074
Global Const XPRS_MAXABSDUALINFEAS        =     2075
Global Const XPRS_MAXRELDUALINFEAS        =     2076
Global Const XPRS_PRIMALDUALINTEGRAL      =     2079
Global Const XPRS_BARPRIMALOBJ            =     4001
Global Const XPRS_BARDUALOBJ              =     4002
Global Const XPRS_BARPRIMALINF            =     4003
Global Const XPRS_BARDUALINF              =     4004
Global Const XPRS_BARCGAP                 =     4005


' String attributes for XPRSprob
Global Const XPRS_MATRIXNAME              =     3001
Global Const XPRS_BOUNDNAME               =     3002
Global Const XPRS_OBJNAME                 =     3003
Global Const XPRS_RHSNAME                 =     3004
Global Const XPRS_RANGENAME               =     3005


' Integer control variables for XPRSmipsolpool
Global Const XPRS_MSP_DUPLICATESOLUTIONSPOLICY =     6203
Global Const XPRS_MSP_INCLUDEPROBNAMEINLOGGING =     6211
Global Const XPRS_MSP_WRITESLXSOLLOGGING  =     6212
Global Const XPRS_MSP_ENABLESLACKSTORAGE  =     6213
Global Const XPRS_MSP_OUTPUTLOG           =     6214
Global Const XPRS_MSP_SOL_BITFIELDSUSR    =     6406


' Double control variables for XPRSmipsolpool
Global Const XPRS_MSP_DEFAULTUSERSOLFEASTOL =     6209
Global Const XPRS_MSP_DEFAULTUSERSOLMIPTOL =     6210
Global Const XPRS_MSP_SOL_FEASTOL         =     6402
Global Const XPRS_MSP_SOL_MIPTOL          =     6403


' Integer attributes for XPRSmipsolpool
Global Const XPRS_MSP_SOLUTIONS           =     6208
Global Const XPRS_MSP_PRB_VALIDSOLS       =     6300
Global Const XPRS_MSP_PRB_FEASIBLESOLS    =     6301
Global Const XPRS_MSP_SOL_COLS            =     6400
Global Const XPRS_MSP_SOL_NONZEROS        =     6401
Global Const XPRS_MSP_SOL_ISUSERSOLUTION  =     6404
Global Const XPRS_MSP_SOL_ISREPROCESSEDUSERSOLUTION =     6405
Global Const XPRS_MSP_SOL_BITFIELDSSYS    =     6407
Global Const XPRS_MSP_SOLPRB_INFEASCOUNT  =     6501
Global Const XPRS_MSP_SOLPRB_INFCNT_PRIMAL =     6503
Global Const XPRS_MSP_SOLPRB_INFCNT_MIP   =     6505
Global Const XPRS_MSP_SOLPRB_INFCNT_SLACK =     6507
Global Const XPRS_MSP_SOLPRB_INFMXI_SLACK =     6509
Global Const XPRS_MSP_SOLPRB_INFCNT_COLUMN =     6511
Global Const XPRS_MSP_SOLPRB_INFMXI_COLUMN =     6513
Global Const XPRS_MSP_SOLPRB_INFCNT_DELAYEDROW =     6515
Global Const XPRS_MSP_SOLPRB_INFMXI_DELAYEDROW =     6517
Global Const XPRS_MSP_SOLPRB_INFCNT_INT   =     6519
Global Const XPRS_MSP_SOLPRB_INFMXI_INT   =     6521
Global Const XPRS_MSP_SOLPRB_INFCNT_BIN   =     6523
Global Const XPRS_MSP_SOLPRB_INFMXI_BIN   =     6525
Global Const XPRS_MSP_SOLPRB_INFCNT_SC    =     6527
Global Const XPRS_MSP_SOLPRB_INFMXI_SC    =     6529
Global Const XPRS_MSP_SOLPRB_INFCNT_SI    =     6531
Global Const XPRS_MSP_SOLPRB_INFMXI_SI    =     6533
Global Const XPRS_MSP_SOLPRB_INFCNT_PI    =     6535
Global Const XPRS_MSP_SOLPRB_INFMXI_PI    =     6537
Global Const XPRS_MSP_SOLPRB_INFCNT_SET1  =     6539
Global Const XPRS_MSP_SOLPRB_INFMXI_SET1  =     6541
Global Const XPRS_MSP_SOLPRB_INFCNT_SET2  =     6543
Global Const XPRS_MSP_SOLPRB_INFMXI_SET2  =     6545


' Double attributes for XPRSmipsolpool
Global Const XPRS_MSP_SOLPRB_OBJ          =     6500
Global Const XPRS_MSP_SOLPRB_INFSUM_PRIMAL =     6502
Global Const XPRS_MSP_SOLPRB_INFSUM_MIP   =     6504
Global Const XPRS_MSP_SOLPRB_INFSUM_SLACK =     6506
Global Const XPRS_MSP_SOLPRB_INFMAX_SLACK =     6508
Global Const XPRS_MSP_SOLPRB_INFSUM_COLUMN =     6510
Global Const XPRS_MSP_SOLPRB_INFMAX_COLUMN =     6512
Global Const XPRS_MSP_SOLPRB_INFSUM_DELAYEDROW =     6514
Global Const XPRS_MSP_SOLPRB_INFMAX_DELAYEDROW =     6516
Global Const XPRS_MSP_SOLPRB_INFSUM_INT   =     6518
Global Const XPRS_MSP_SOLPRB_INFMAX_INT   =     6520
Global Const XPRS_MSP_SOLPRB_INFSUM_BIN   =     6522
Global Const XPRS_MSP_SOLPRB_INFMAX_BIN   =     6524
Global Const XPRS_MSP_SOLPRB_INFSUM_SC    =     6526
Global Const XPRS_MSP_SOLPRB_INFMAX_SC    =     6528
Global Const XPRS_MSP_SOLPRB_INFSUM_SI    =     6530
Global Const XPRS_MSP_SOLPRB_INFMAX_SI    =     6532
Global Const XPRS_MSP_SOLPRB_INFSUM_PI    =     6534
Global Const XPRS_MSP_SOLPRB_INFMAX_PI    =     6536
Global Const XPRS_MSP_SOLPRB_INFSUM_SET1  =     6538
Global Const XPRS_MSP_SOLPRB_INFMAX_SET1  =     6540
Global Const XPRS_MSP_SOLPRB_INFSUM_SET2  =     6542
Global Const XPRS_MSP_SOLPRB_INFMAX_SET2  =     6544


' Integer control variables for XPRSmipsolenum
Global Const XPRS_MSE_CALLBACKCULLSOLS_MIPOBJECT =     6601
Global Const XPRS_MSE_CALLBACKCULLSOLS_DIVERSITY =     6602
Global Const XPRS_MSE_CALLBACKCULLSOLS_MODOBJECT =     6603
Global Const XPRS_MSE_OPTIMIZEDIVERSITY   =     6607
Global Const XPRS_MSE_OUTPUTLOG           =     6610


' Double control variables for XPRSmipsolenum
Global Const XPRS_MSE_OUTPUTTOL           =     6609


' Integer attributes for XPRSmipsolenum
Global Const XPRS_MSE_SOLUTIONS           =     6600
Global Const XPRS_MSE_METRIC_MIPOBJECT    =     6604
Global Const XPRS_MSE_METRIC_DIVERSITY    =     6605
Global Const XPRS_MSE_METRIC_MODOBJECT    =     6606


' Double attributes for XPRSmipsolenum
Global Const XPRS_MSE_DIVERSITYSUM        =     6608


' Integer control variables for XPRSprobperturber
Global Const XPRS_PTB_PERMUTE_ACTIVE      =     6700
Global Const XPRS_PTB_PERMUTE_SEEDLAST    =     6701
Global Const XPRS_PTB_PERTURB_COST2_ACTIVE =     6710
Global Const XPRS_PTB_PERTURB_COST2_SEEDLAST =     6711
Global Const XPRS_PTB_SHIFTSCALE_COLS_ACTIVE =     6720
Global Const XPRS_PTB_SHIFTSCALE_COLS_SEEDLAST =     6721
Global Const XPRS_PTB_SHIFTSCALE_COLS_SHIFT_ACTIVE_I =     6725
Global Const XPRS_PTB_SHIFTSCALE_COLS_NEGATE_ACTIVE_I =     6726
Global Const XPRS_PTB_SHIFTSCALE_COLS_COMPLEMENT_ACTIVE_I =     6727
Global Const XPRS_PTB_SHIFTSCALE_COLS_COMPLEMENT_ACTIVE_B =     6728
Global Const XPRS_PTB_SHIFTSCALE_ROWS_ACTIVE =     6760
Global Const XPRS_PTB_SHIFTSCALE_ROWS_SEEDLAST =     6761


' Double control variables for XPRSprobperturber
Global Const XPRS_PTB_PERMUTE_INTENSITY_ROW =     6702
Global Const XPRS_PTB_PERMUTE_INTENSITY_COL =     6703
Global Const XPRS_PTB_SHIFTSCALE_COLS_INTENSITY =     6722
Global Const XPRS_PTB_SHIFTSCALE_COLS_MAXRANGEFORCOMPLEMENTING =     6729
Global Const XPRS_PTB_SHIFTSCALE_ROWS_INTENSITY =     6762


' Integer attributes for XPRSprobperturber
Global Const XPRS_PTB_PERMUTE_PERMCOUNT_ROW =     6704
Global Const XPRS_PTB_PERMUTE_PERMCOUNT_COL =     6705
Global Const XPRS_PTB_PERTURB_COST2_CHANGEDCOLCOUNT =     6712
Global Const XPRS_PTB_SHIFTSCALE_COLS_CHANGEDCOLCOUNT =     6723


' Double attributes for XPRSprobperturber
Global Const XPRS_PTB_PERTURB_COST2_TOTALABSCOSTCHANGE =     6713
Global Const XPRS_PTB_SHIFTSCALE_COLS_FIXEDOBJDELTA =     6724


' Enumeration values relating to LPSTATUS
Global Const XPRS_LP_UNSTARTED = 0
Global Const XPRS_LP_OPTIMAL = 1
Global Const XPRS_LP_INFEAS = 2
Global Const XPRS_LP_CUTOFF = 3
Global Const XPRS_LP_UNFINISHED = 4
Global Const XPRS_LP_UNBOUNDED = 5
Global Const XPRS_LP_CUTOFF_IN_DUAL = 6
Global Const XPRS_LP_UNSOLVED = 7
Global Const XPRS_LP_NONCONVEX = 8

' Enumeration values relating to MIPSTATUS
Global Const XPRS_MIP_NOT_LOADED = 0
Global Const XPRS_MIP_LP_NOT_OPTIMAL = 1
Global Const XPRS_MIP_LP_OPTIMAL = 2
Global Const XPRS_MIP_NO_SOL_FOUND = 3
Global Const XPRS_MIP_SOLUTION = 4
Global Const XPRS_MIP_INFEAS = 5
Global Const XPRS_MIP_OPTIMAL = 6
Global Const XPRS_MIP_UNBOUNDED = 7

' Enumeration values relating to BARORDER
Global Const XPRS_BAR_DEFAULT = 0
Global Const XPRS_BAR_MIN_DEGREE = 1
Global Const XPRS_BAR_MIN_LOCAL_FILL = 2
Global Const XPRS_BAR_NESTED_DISSECTION = 3

' Enumeration values relating to DEFAULTALG
Global Const XPRS_ALG_DEFAULT = 1
Global Const XPRS_ALG_DUAL = 2
Global Const XPRS_ALG_PRIMAL = 3
Global Const XPRS_ALG_BARRIER = 4
Global Const XPRS_ALG_NETWORK = 5

' Enumeration values relating to XPRSinterrupt
Global Const XPRS_STOP_NONE = 0
Global Const XPRS_STOP_TIMELIMIT = 1
Global Const XPRS_STOP_CTRLC = 2
Global Const XPRS_STOP_NODELIMIT = 3
Global Const XPRS_STOP_ITERLIMIT = 4
Global Const XPRS_STOP_MIPGAP = 5
Global Const XPRS_STOP_SOLLIMIT = 6
Global Const XPRS_STOP_USER = 9

' Enumeration values relating to AlwaysNeverOrAutomatic
Global Const XPRS_ANA_AUTOMATIC = -1
Global Const XPRS_ANA_NEVER = 0
Global Const XPRS_ANA_ALWAYS = 1

' Enumeration values relating to OnOrOff
Global Const XPRS_BOOL_OFF = 0
Global Const XPRS_BOOL_ON = 1

' Enumeration values relating to BACKTRACK
Global Const XPRS_BACKTRACKALG_BEST_ESTIMATE = 2
Global Const XPRS_BACKTRACKALG_BEST_BOUND = 3
Global Const XPRS_BACKTRACKALG_DEEPEST_NODE = 4
Global Const XPRS_BACKTRACKALG_HIGHEST_NODE = 5
Global Const XPRS_BACKTRACKALG_EARLIEST_NODE = 6
Global Const XPRS_BACKTRACKALG_LATEST_NODE = 7
Global Const XPRS_BACKTRACKALG_RANDOM = 8
Global Const XPRS_BACKTRACKALG_MIN_INFEAS = 9
Global Const XPRS_BACKTRACKALG_BEST_ESTIMATE_MIN_INFEAS = 10
Global Const XPRS_BACKTRACKALG_DEEPEST_BEST_ESTIMATE = 11

' Enumeration values relating to BRANCHCHOICE
Global Const XPRS_BRANCH_MIN_EST_FIRST = 0
Global Const XPRS_BRANCH_MAX_EST_FIRST = 1

' Enumeration values relating to CHOLESKYALG
Global Const XPRS_ALG_PULL_CHOLESKY = 0
Global Const XPRS_ALG_PUSH_CHOLESKY = 1

' Enumeration values relating to CROSSOVERDRP
Global Const XPRS_XDRPBEFORE_CROSSOVER = 1
Global Const XPRS_XDRPINSIDE_CROSSOVER = 2
Global Const XPRS_XDRPAGGRESSIVE_BEFORE_CROSSOVER = 4

' Enumeration values relating to DUALGRADIENT
Global Const XPRS_DUALGRADIENT_AUTOMATIC = -1
Global Const XPRS_DUALGRADIENT_DEVEX = 0
Global Const XPRS_DUALGRADIENT_STEEPESTEDGE = 1

' Enumeration values relating to DUALSTRATEGY
Global Const XPRS_DUALSTRATEGY_REMOVE_INFEAS_WITH_PRIMAL = 0
Global Const XPRS_DUALSTRATEGY_REMOVE_INFEAS_WITH_DUAL = 1

' Enumeration values relating to FEASIBILITYPUMP
Global Const XPRS_FEASIBILITYPUMP_NEVER = 0
Global Const XPRS_FEASIBILITYPUMP_ALWAYS = 1
Global Const XPRS_FEASIBILITYPUMP_LASTRESORT = 2

' Enumeration values relating to HEURSEARCHSELECT
Global Const XPRS_HEURSEARCH_LOCAL_SEARCH_LARGE_NEIGHBOURHOOD = 0
Global Const XPRS_HEURSEARCH_LOCAL_SEARCH_NODE_NEIGHBOURHOOD = 1
Global Const XPRS_HEURSEARCH_LOCAL_SEARCH_SOLUTION_NEIGHBOURHOOD = 2

' Enumeration values relating to HEURSTRATEGY
Global Const XPRS_HEURSTRATEGY_AUTOMATIC = -1
Global Const XPRS_HEURSTRATEGY_NONE = 0
Global Const XPRS_HEURSTRATEGY_BASIC = 1
Global Const XPRS_HEURSTRATEGY_ENHANCED = 2
Global Const XPRS_HEURSTRATEGY_EXTENSIVE = 3

' Enumeration values relating to NODESELECTION
Global Const XPRS_NODESELECTION_LOCAL_FIRST = 1
Global Const XPRS_NODESELECTION_BEST_FIRST = 2
Global Const XPRS_NODESELECTION_LOCAL_DEPTH_FIRST = 3
Global Const XPRS_NODESELECTION_BEST_FIRST_THEN_LOCAL_FIRST = 4
Global Const XPRS_NODESELECTION_DEPTH_FIRST = 5

' Enumeration values relating to OUTPUTLOG
Global Const XPRS_OUTPUTLOG_NO_OUTPUT = 0
Global Const XPRS_OUTPUTLOG_FULL_OUTPUT = 1
Global Const XPRS_OUTPUTLOG_ERRORS_AND_WARNINGS = 2
Global Const XPRS_OUTPUTLOG_ERRORS = 3

' Enumeration values relating to PREPROBING
Global Const XPRS_PREPROBING_AUTOMATIC = -1
Global Const XPRS_PREPROBING_DISABLED = 0
Global Const XPRS_PREPROBING_LIGHT = 1
Global Const XPRS_PREPROBING_FULL = 2
Global Const XPRS_PREPROBING_FULL_AND_REPEAT = 3

' Enumeration values relating to PRESOLVEOPS
Global Const XPRS_PRESOLVEOPS_SINGLETONCOLUMNREMOVAL = 1
Global Const XPRS_PRESOLVEOPS_SINGLETONROWREMOVAL = 2
Global Const XPRS_PRESOLVEOPS_FORCINGROWREMOVAL = 4
Global Const XPRS_PRESOLVEOPS_DUALREDUCTIONS = 8
Global Const XPRS_PRESOLVEOPS_REDUNDANTROWREMOVAL = 16
Global Const XPRS_PRESOLVEOPS_DUPLICATECOLUMNREMOVAL = 32
Global Const XPRS_PRESOLVEOPS_DUPLICATEROWREMOVAL = 64
Global Const XPRS_PRESOLVEOPS_STRONGDUALREDUCTIONS = 128
Global Const XPRS_PRESOLVEOPS_VARIABLEELIMINATIONS = 256
Global Const XPRS_PRESOLVEOPS_NOIPREDUCTIONS = 512
Global Const XPRS_PRESOLVEOPS_NOADVANCEDIPREDUCTIONS = 2048
Global Const XPRS_PRESOLVEOPS_LINEARLYDEPENDANTROWREMOVAL = 16384
Global Const XPRS_PRESOLVEOPS_NOINTEGERVARIABLEANDSOSDETECTION = 32768

' Enumeration values relating to PRESOLVESTATE
Global Const XPRS_PRESOLVESTATE_PROBLEMLOADED = 1
Global Const XPRS_PRESOLVESTATE_PROBLEMLPPRESOLVED = 2
Global Const XPRS_PRESOLVESTATE_PROBLEMMIPPRESOLVED = 4
Global Const XPRS_PRESOLVESTATE_SOLUTIONVALID = 128

' Enumeration values relating to MIPPRESOLVE
Global Const XPRS_MIPPRESOLVE_REDUCED_COST_FIXING = 1
Global Const XPRS_MIPPRESOLVE_LOGIC_PREPROCESSING = 2
Global Const XPRS_MIPPRESOLVE_ALLOW_CHANGE_BOUNDS = 8

' Enumeration values relating to PRESOLVE
Global Const XPRS_PRESOLVE_NOPRIMALINFEASIBILITY = -1
Global Const XPRS_PRESOLVE_NONE = 0
Global Const XPRS_PRESOLVE_DEFAULT = 1
Global Const XPRS_PRESOLVE_KEEPREDUNDANTBOUNDS = 2

' Enumeration values relating to PRICINGALG
Global Const XPRS_PRICING_PARTIAL = -1
Global Const XPRS_PRICING_DEFAULT = 0
Global Const XPRS_PRICING_DEVEX = 1

' Enumeration values relating to CUTSTRATEGY
Global Const XPRS_CUTSTRATEGY_DEFAULT = -1
Global Const XPRS_CUTSTRATEGY_NONE = 0
Global Const XPRS_CUTSTRATEGY_CONSERVATIVE = 1
Global Const XPRS_CUTSTRATEGY_MODERATE = 2
Global Const XPRS_CUTSTRATEGY_AGGRESSIVE = 3

' Enumeration values relating to VARSELECTION
Global Const XPRS_VARSELECTION_AUTOMATIC = -1
Global Const XPRS_VARSELECTION_MIN_UPDOWN_PSEUDO_COSTS = 1
Global Const XPRS_VARSELECTION_SUM_UPDOWN_PSEUDO_COSTS = 2
Global Const XPRS_VARSELECTION_MAX_UPDOWN_PSEUDO_COSTS_PLUS_TWICE_MIN = 3
Global Const XPRS_VARSELECTION_MAX_UPDOWN_PSEUDO_COSTS = 4
Global Const XPRS_VARSELECTION_DOWN_PSEUDO_COST = 5
Global Const XPRS_VARSELECTION_UP_PSEUDO_COST = 6

' Enumeration values relating to SCALING
Global Const XPRS_SCALING_ROW_SCALING = 1
Global Const XPRS_SCALING_COLUMN_SCALING = 2
Global Const XPRS_SCALING_ROW_SCALING_AGAIN = 4
Global Const XPRS_SCALING_MAXIMUM = 8
Global Const XPRS_SCALING_CURTIS_REID = 16
Global Const XPRS_SCALING_BY_MAX_ELEM_NOT_GEO_MEAN = 32
Global Const XPRS_SCALING_OBJECTIVE_SCALING = 64
Global Const XPRS_SCALING_EXCLUDE_QUADRATIC_FROM_SCALE_FACTOR = 128
Global Const XPRS_SCALING_IGNORE_QUADRATIC_ROW_PART = 256
Global Const XPRS_SCALING_BEFORE_PRESOLVE = 512
Global Const XPRS_SCALING_NO_SCALING_ROWS_UP = 1024
Global Const XPRS_SCALING_NO_SCALING_COLUMNS_DOWN = 2048
Global Const XPRS_SCALING_SIMPLEX_OBJECTIVE_SCALING = 4096
Global Const XPRS_SCALING_RHS_SCALING = 8192
Global Const XPRS_SCALING_NO_AGGRESSIVE_Q_SCALING = 16384
Global Const XPRS_SCALING_SLACK_SCALING = 32768

' Enumeration values relating to CUTSELECT
Global Const XPRS_CUTSELECT_CLIQUE = (32+1823)
Global Const XPRS_CUTSELECT_MIR = (64+1823)
Global Const XPRS_CUTSELECT_COVER = (128+1823)
Global Const XPRS_CUTSELECT_FLOWPATH = (2048+1823)
Global Const XPRS_CUTSELECT_IMPLICATION = (4096+1823)
Global Const XPRS_CUTSELECT_LIFT_AND_PROJECT = (8192+1823)
Global Const XPRS_CUTSELECT_DISABLE_CUT_ROWS = (16384+1823)
Global Const XPRS_CUTSELECT_GUB_COVER = (32768+1823)
Global Const XPRS_CUTSELECT_DEFAULT = (-1)

' Enumeration values relating to REFINEOPS
Global Const XPRS_REFINEOPS_LPOPTIMAL = 1
Global Const XPRS_REFINEOPS_MIPSOLUTION = 2
Global Const XPRS_REFINEOPS_MIPOPTIMAL = 4
Global Const XPRS_REFINEOPS_MIPNODELP = 8

' Enumeration values relating to DUALIZEOPS
Global Const XPRS_DUALIZEOPS_SWITCHALGORITHM = 1

' Enumeration values relating to TREEDIAGNOSTICS
Global Const XPRS_TREEDIAGNOSTICS_MEMORY_USAGE_SUMMARIES = 1
Global Const XPRS_TREEDIAGNOSTICS_MEMORY_SAVED_REPORTS = 2

' Enumeration values relating to BARPRESOLVEOPS
Global Const XPRS_BARPRESOLVEOPS_STANDARD_PRESOLVE = 0
Global Const XPRS_BARPRESOLVEOPS_EXTRA_BARRIER_PRESOLVE = 1

' Enumeration values relating to PRECOEFELIM
Global Const XPRS_PRECOEFELIM_DISABLED = 0
Global Const XPRS_PRECOEFELIM_AGGRESSIVE = 1
Global Const XPRS_PRECOEFELIM_CAUTIOUS = 2

' Enumeration values relating to PREDOMROW
Global Const XPRS_PREDOMROW_AUTOMATIC = -1
Global Const XPRS_PREDOMROW_DISABLED = 0
Global Const XPRS_PREDOMROW_CAUTIOUS = 1
Global Const XPRS_PREDOMROW_MEDIUM = 2
Global Const XPRS_PREDOMROW_AGGRESSIVE = 3

' Enumeration values relating to PREDOMCOL
Global Const XPRS_PREDOMCOL_AUTOMATIC = -1
Global Const XPRS_PREDOMCOL_DISABLED = 0
Global Const XPRS_PREDOMCOL_CAUTIOUS = 1
Global Const XPRS_PREDOMCOL_AGGRESSIVE = 2

' Enumeration values relating to PRIMALUNSHIFT
Global Const XPRS_PRIMALUNSHIFT_ALLOW_DUAL_UNSHIFT = 0
Global Const XPRS_PRIMALUNSHIFT_NO_DUAL_UNSHIFT = 1

' Enumeration values relating to REPAIRINDEFINITEQ
Global Const XPRS_REPAIRINDEFINITEQ_REPAIR_IF_POSSIBLE = 0
Global Const XPRS_REPAIRINDEFINITEQ_NO_REPAIR = 1

' Enumeration values relating to Minimize/Maximize
Global Const XPRS_OBJ_MINIMIZE = 1
Global Const XPRS_OBJ_MAXIMIZE = -1

' Enumeration values relating to Set/GetControl/Attribinfo
Global Const XPRS_TYPE_NOTDEFINED = 0
Global Const XPRS_TYPE_INT = 1
Global Const XPRS_TYPE_INT64 = 2
Global Const XPRS_TYPE_DOUBLE = 3
Global Const XPRS_TYPE_STRING = 4

' Enumeration values relating to QCONVEXITY
Global Const XPRS_QCONVEXITY_UNKNOWN = -1
Global Const XPRS_QCONVEXITY_NONCONVEX = 0
Global Const XPRS_QCONVEXITY_CONVEX = 1
Global Const XPRS_QCONVEXITY_REPAIRABLE = 2
Global Const XPRS_QCONVEXITY_CONVEXCONE = 3
Global Const XPRS_QCONVEXITY_CONECONVERTABLE = 4

' Enumeration values relating to SOLINFO
Global Const XPRS_SOLINFO_ABSPRIMALINFEAS = 0
Global Const XPRS_SOLINFO_RELPRIMALINFEAS = 1
Global Const XPRS_SOLINFO_ABSDUALINFEAS = 2
Global Const XPRS_SOLINFO_RELDUALINFEAS = 3
Global Const XPRS_SOLINFO_MAXMIPFRACTIONAL = 4

' Bitfield values relating to XPRSmipsolpool_soln_flags
Global Const XPRS_ISUSERSOLUTION = 1
Global Const XPRS_ISREPROCESSEDUSERSOLUTION = 2


' Optimiser DLL Function Declarations
' NB You may have to change the path of "XPRS.DLL" so that VB can find the DLL.
Declare Function XPRScopycallbacks Lib "XPRS.DLL" (ByVal dest As Long, ByVal src As Long) As Long
Declare Function XPRScopycontrols Lib "XPRS.DLL" (ByVal dest As Long, ByVal src As Long) As Long
Declare Function XPRScopyprob Lib "XPRS.DLL" (ByVal dest As Long, ByVal src As Long, ByVal probname As String) As Long
Declare Function XPRScreateprob Lib "XPRS.DLL" (ByRef probholder As Long) As Long
Declare Function XPRSdestroyprob Lib "XPRS.DLL" (ByVal prob As Long) As Long

Declare Function XPRSinit Lib "XPRS.DLL" (ByVal path As String) As Long
Declare Function XPRSfree Lib "XPRS.DLL" () As Long
Private Declare Function XPRSgetbanner_C Lib "XPRS.DLL" Alias "XPRSgetbanner" (ByVal banner As String) As Long
Private Declare Function XPRSgetversion_C Lib "XPRS.DLL" Alias "XPRSgetversion" (ByVal version As String) As Long
Declare Function XPRSgetdaysleft Lib "XPRS.DLL" (ByRef daysleft As Long) As Long
Declare Function XPRSsetcheckedmode Lib "XPRS.DLL" (ByVal checked_mode As Long) As Long
Declare Function XPRSgetcheckedmode Lib "XPRS.DLL" (ByRef r_checked_mode As Long) As Long
Private Declare Function XPRSlicense_C Lib "XPRS.DLL" Alias "XPRSlicense" (ByRef i1 As Long, ByVal c1 As String) As Long
Declare Function XPRSbeginlicensing Lib "XPRS.DLL" (ByRef r_dontAlreadyHaveLicense As Long) As Long
Declare Function XPRSendlicensing Lib "XPRS.DLL" () As Long
Private Declare Function XPRSgetlicerrmsg_C Lib "XPRS.DLL" Alias "XPRSgetlicerrmsg" (ByVal msg As String, ByVal len_ As Long) As Long

Declare Function XPRSsetlogfile Lib "XPRS.DLL" (ByVal prob As Long, ByVal logname As String) As Long
Declare Function XPRSsetintcontrol Lib "XPRS.DLL" (ByVal prob As Long, ByVal index As Long, ByVal ivalue As Long) As Long
Declare Function XPRSsetdblcontrol Lib "XPRS.DLL" (ByVal prob As Long, ByVal index As Long, ByVal dvalue As Double) As Long
Declare Function XPRSinterrupt Lib "XPRS.DLL" (ByVal prob As Long, ByVal reason As Long) As Long
Private Declare Function XPRSgetprobname_C Lib "XPRS.DLL" Alias "XPRSgetprobname" (ByVal prob As Long, ByVal svalue As String) As Long
Declare Function XPRSgetqobj Lib "XPRS.DLL" (ByVal prob As Long, ByVal icol As Long, ByVal jcol As Long, ByRef dval As Double) As Long
Declare Function XPRSsetprobname Lib "XPRS.DLL" (ByVal prob As Long, ByVal svalue As String) As Long
Declare Function XPRSsetstrcontrol Lib "XPRS.DLL" (ByVal prob As Long, ByVal index As Long, ByVal svalue As String) As Long
Declare Function XPRSgetintcontrol Lib "XPRS.DLL" (ByVal prob As Long, ByVal index As Long, ByRef ivalue As Long) As Long
Declare Function XPRSgetdblcontrol Lib "XPRS.DLL" (ByVal prob As Long, ByVal index As Long, ByRef dvalue As Double) As Long
Private Declare Function XPRSgetstrcontrol_C Lib "XPRS.DLL" Alias "XPRSgetstrcontrol" (ByVal prob As Long, ByVal index As Long, ByVal svalue As String) As Long
Private Declare Function XPRSgetstringcontrol_C Lib "XPRS.DLL" Alias "XPRSgetstringcontrol" (ByVal prob As Long, ByVal index As Long, ByVal svalue As String, ByVal svaluesize As Long, ByRef controlsize As Long) As Long
Declare Function XPRSgetintattrib Lib "XPRS.DLL" (ByVal prob As Long, ByVal index As Long, ByRef ivalue As Long) As Long
Private Declare Function XPRSgetstrattrib_C Lib "XPRS.DLL" Alias "XPRSgetstrattrib" (ByVal prob As Long, ByVal index As Long, ByVal cvalue As String) As Long
Private Declare Function XPRSgetstringattrib_C Lib "XPRS.DLL" Alias "XPRSgetstringattrib" (ByVal prob As Long, ByVal index As Long, ByVal cvalue As String, ByVal cvaluesize As Long, ByRef controlsize As Long) As Long
Declare Function XPRSgetdblattrib Lib "XPRS.DLL" (ByVal prob As Long, ByVal index As Long, ByRef dvalue As Double) As Long
Declare Function XPRSsetdefaultcontrol Lib "XPRS.DLL" (ByVal prob As Long, ByVal index As Long) As Long
Declare Function XPRSsetdefaults Lib "XPRS.DLL" (ByVal prob As Long) As Long
Declare Function XPRSgetcontrolinfo Lib "XPRS.DLL" (ByVal prob As Long, ByVal sCaName As String, ByRef iHeaderId As Long, ByRef iTypeinfo As Long) As Long
Declare Function XPRSgetattribinfo Lib "XPRS.DLL" (ByVal prob As Long, ByVal sCaName As String, ByRef iHeaderId As Long, ByRef iTypeinfo As Long) As Long

Declare Function XPRSgoal Lib "XPRS.DLL" (ByVal prob As Long, ByVal filename As String, ByVal sflags As String) As Long
Declare Function XPRSreadprob Lib "XPRS.DLL" (ByVal prob As Long, ByVal sprobname As String, ByVal sflags As String) As Long
Declare Function XPRSloadlp Lib "XPRS.DLL" (ByVal prob As Long, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, drange As Double, dobj As Double, mstart As Long, mnel As Long, mrwind As Long, dmatval As Double, dlb As Double, dub As Double) As Long
Declare Function XPRSloadqp Lib "XPRS.DLL" (ByVal prob As Long, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, drange As Double, dobj As Double, mstart As Long, mnel As Long, mrwind As Long, dmatval As Double, dlb As Double, dub As Double, ByVal nquads As Long, mqcol1 As Long, mqcol2 As Long, dqval As Double) As Long
Declare Function XPRSloadqglobal Lib "XPRS.DLL" (ByVal prob As Long, ByVal probname As String, ByVal ncols As Long, ByVal nrows As Long, qsenx As Byte, rhsx As Double, range As Double, objx As Double, matbeg As Long, matcnt As Long, matind As Long, dmtval As Double, bndl As Double, bndu As Double, ByVal nquads As Long, mqcol1 As Long, mqcol2 As Long, dqval As Double, ByVal ngents As Long, ByVal nsets As Long, qgtype As Byte, mgcols As Long, dlim As Double, qstype As Byte, msstart As Long, mscols As Long, dref As Double) As Long
Declare Function XPRSfixglobals Lib "XPRS.DLL" (ByVal prob As Long, ByVal ifround As Long) As Long
Declare Function XPRSloadmodelcuts Lib "XPRS.DLL" (ByVal prob As Long, ByVal nmodcuts As Long, mrows As Long) As Long
Declare Function XPRSloaddelayedrows Lib "XPRS.DLL" (ByVal prob As Long, ByVal nrows As Long, mrows As Long) As Long
Declare Function XPRSloaddirs Lib "XPRS.DLL" (ByVal prob As Long, ByVal ndirs As Long, mcols As Long, mpri As Long, sbr As Byte, dupc As Double, ddpc As Double) As Long
Declare Function XPRSloadbranchdirs Lib "XPRS.DLL" (ByVal prob As Long, ByVal ndirs As Long, mcols As Long, mbranch As Long) As Long
Declare Function XPRSloadpresolvedirs Lib "XPRS.DLL" (ByVal prob As Long, ByVal ndirs As Long, mcols As Long, mpri As Long, sbr As Byte, dupc As Double, ddpc As Double) As Long
Declare Function XPRSgetdirs Lib "XPRS.DLL" (ByVal prob As Long, ByRef ndirs As Long, mcols As Long, mpri As Long, sbr As Byte, dupc As Double, ddpc As Double) As Long
Declare Function XPRSloadglobal Lib "XPRS.DLL" (ByVal prob As Long, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, drange As Double, dobj As Double, mstart As Long, mnel As Long, mrwind As Long, dmatval As Double, dlb As Double, dub As Double, ByVal ngents As Long, ByVal nsets As Long, qgtype As Byte, mgcols As Long, dlim As Double, stype As Byte, msstart As Long, mscols As Long, dref As Double) As Long
Declare Function XPRSaddnames Lib "XPRS.DLL" (ByVal prob As Long, ByVal itype As Long, sname As Byte, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSaddsetnames Lib "XPRS.DLL" (ByVal prob As Long, sname As Byte, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSscale Lib "XPRS.DLL" (ByVal prob As Long, mrscal As Long, mcscal As Long) As Long
Declare Function XPRSreaddirs Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String) As Long
Declare Function XPRSwritedirs Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String) As Long
Declare Function XPRSsetindicators Lib "XPRS.DLL" (ByVal prob As Long, ByVal nrows As Long, mrows As Long, inds As Long, comps As Long) As Long
Declare Function XPRSgetindicators Lib "XPRS.DLL" (ByVal prob As Long, inds As Long, comps As Long, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSdelindicators Lib "XPRS.DLL" (ByVal prob As Long, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSdumpcontrols Lib "XPRS.DLL" (ByVal prob As Long) As Long

Declare Function XPRSminim Lib "XPRS.DLL" (ByVal prob As Long, ByVal sflags As String) As Long
Declare Function XPRSmaxim Lib "XPRS.DLL" (ByVal prob As Long, ByVal sflags As String) As Long
Declare Function XPRSlpoptimize Lib "XPRS.DLL" (ByVal prob As Long, ByVal sflags As String) As Long
Declare Function XPRSmipoptimize Lib "XPRS.DLL" (ByVal prob As Long, ByVal sflags As String) As Long
Declare Function XPRSrange Lib "XPRS.DLL" (ByVal prob As Long) As Long
Declare Function XPRSgetrowrange Lib "XPRS.DLL" (ByVal prob As Long, upact As Double, loact As Double, uup As Double, udn As Double) As Long
Declare Function XPRSgetcolrange Lib "XPRS.DLL" (ByVal prob As Long, upact As Double, loact As Double, uup As Double, udn As Double, ucost As Double, lcost As Double) As Long
Declare Function XPRSgetpivotorder Lib "XPRS.DLL" (ByVal prob As Long, mpiv As Long) As Long
Declare Function XPRSgetpresolvemap Lib "XPRS.DLL" (ByVal prob As Long, rowmap As Long, colmap As Long) As Long
Declare Function XPRSreadbasis Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String, ByVal sflags As String) As Long
Declare Function XPRSwritebasis Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String, ByVal sflags As String) As Long
Declare Function XPRSglobal Lib "XPRS.DLL" (ByVal prob As Long) As Long
Declare Function XPRSinitglobal Lib "XPRS.DLL" (ByVal prob As Long) As Long
Declare Function XPRSwriteprtsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String, ByVal sflags As String) As Long
Declare Function XPRSalter Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String) As Long
Declare Function XPRSwritesol Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String, ByVal sflags As String) As Long
Declare Function XPRSwritebinsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String, ByVal sflags As String) As Long
Declare Function XPRSreadbinsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String, ByVal sflags As String) As Long
Declare Function XPRSwriteslxsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String, ByVal sflags As String) As Long
Declare Function XPRSreadslxsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String, ByVal sflags As String) As Long
Declare Function XPRSwriteprtrange Lib "XPRS.DLL" (ByVal prob As Long) As Long
Declare Function XPRSwriterange Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String, ByVal sflags As String) As Long
Declare Function XPRSgetsol Lib "XPRS.DLL" (ByVal prob As Long, dx As Double, dslack As Double, dual As Double, dj As Double) As Long
Declare Function XPRSgetpresolvesol Lib "XPRS.DLL" (ByVal prob As Long, dx As Double, dslack As Double, dual As Double, dj As Double) As Long
Declare Function XPRSgetinfeas Lib "XPRS.DLL" (ByVal prob As Long, ByRef npv As Long, ByRef nps As Long, ByRef nds As Long, ByRef ndv As Long, mx As Long, mslack As Long, mdual As Long, mdj As Long) As Long
Declare Function XPRSgetscaledinfeas Lib "XPRS.DLL" (ByVal prob As Long, ByRef npv As Long, ByRef nps As Long, ByRef nds As Long, ByRef ndv As Long, mx As Long, mslack As Long, mdual As Long, mdj As Long) As Long
Declare Function XPRSgetunbvec Lib "XPRS.DLL" (ByVal prob As Long, ByRef icol As Long) As Long
Declare Function XPRSbtran Lib "XPRS.DLL" (ByVal prob As Long, dwork As Double) As Long
Declare Function XPRSftran Lib "XPRS.DLL" (ByVal prob As Long, dwork As Double) As Long
Declare Function XPRSgetobj Lib "XPRS.DLL" (ByVal prob As Long, dobj As Double, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetrhs Lib "XPRS.DLL" (ByVal prob As Long, drhs As Double, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetrhsrange Lib "XPRS.DLL" (ByVal prob As Long, drng As Double, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetlb Lib "XPRS.DLL" (ByVal prob As Long, dbdl As Double, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetub Lib "XPRS.DLL" (ByVal prob As Long, dbdu As Double, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetcols Lib "XPRS.DLL" (ByVal prob As Long, mstart As Long, mrwind As Long, dmatval As Double, ByVal maxcoeffs As Long, ByRef ncoeffs As Long, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetrows Lib "XPRS.DLL" (ByVal prob As Long, mstart As Long, mclind As Long, dmatval As Double, ByVal maxcoeffs As Long, ByRef ncoeffs As Long, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetcoef Lib "XPRS.DLL" (ByVal prob As Long, ByVal irow As Long, ByVal icol As Long, ByRef dval As Double) As Long
Declare Function XPRSgetmqobj Lib "XPRS.DLL" (ByVal prob As Long, mstart As Long, mclind As Long, dobjval As Double, ByVal maxcoeffs As Long, ByRef ncoeffs As Long, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetbarnumstability Lib "XPRS.DLL" (ByVal prob As Long, dColumnStability As Long, dRowStability As Long) As Long

Declare Function XPRSiisclear Lib "XPRS.DLL" (ByVal prob As Long) As Long
Declare Function XPRSiisfirst Lib "XPRS.DLL" (ByVal prob As Long, ByVal iismode As Long, ByRef status_code As Long) As Long
Declare Function XPRSiisnext Lib "XPRS.DLL" (ByVal prob As Long, ByRef status_code As Long) As Long
Declare Function XPRSiisstatus Lib "XPRS.DLL" (ByVal prob As Long, ByRef iiscount As Long, rowsizes As Long, colsizes As Long, suminfeas As Double, numinfeas As Long) As Long
Declare Function XPRSiisall Lib "XPRS.DLL" (ByVal prob As Long) As Long
Declare Function XPRSiiswrite Lib "XPRS.DLL" (ByVal prob As Long, ByVal number As Long, ByVal fn As String, ByVal filetype As Long, ByVal typeflags As String) As Long
Declare Function XPRSiisisolations Lib "XPRS.DLL" (ByVal prob As Long, ByVal number As Long) As Long
Declare Function XPRSgetiisdata Lib "XPRS.DLL" (ByVal prob As Long, ByVal number As Long, ByRef rownumber As Long, ByRef colnumber As Long, miisrow As Long, miiscol As Long, constrainttype As Byte, colbndtype As Byte, duals As Double, rdcs As Double, isolationrows As Byte, isolationcols As Byte) As Long
Declare Function XPRSgetiis Lib "XPRS.DLL" (ByVal prob As Long, ByRef ncols As Long, ByRef nrows As Long, miiscol As Long, miisrow As Long) As Long
Declare Function XPRSgetpresolvebasis Lib "XPRS.DLL" (ByVal prob As Long, mrowstatus As Long, mcolstatus As Long) As Long
Declare Function XPRSloadpresolvebasis Lib "XPRS.DLL" (ByVal prob As Long, mrowstatus As Long, mcolstatus As Long) As Long
Declare Function XPRSgetglobal Lib "XPRS.DLL" (ByVal prob As Long, ByRef ngents As Long, ByRef nsets As Long, sgtype As Byte, mgcols As Long, dlim As Double, sstype As Byte, msstart As Long, mscols As Long, dref As Double) As Long
Declare Function XPRSwriteprob Lib "XPRS.DLL" (ByVal prob As Long, ByVal sfilename As String, ByVal sflags As String) As Long
Declare Function XPRSgetnames Lib "XPRS.DLL" (ByVal prob As Long, ByVal itype As Long, sbuff As Byte, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetrowtype Lib "XPRS.DLL" (ByVal prob As Long, srowtype As Byte, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSloadsecurevecs Lib "XPRS.DLL" (ByVal prob As Long, ByVal nrows As Long, ByVal ncols As Long, mrow As Long, mcol As Long) As Long
Declare Function XPRSgetcoltype Lib "XPRS.DLL" (ByVal prob As Long, coltype As Byte, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetbasis Lib "XPRS.DLL" (ByVal prob As Long, mrowstatus As Long, mcolstatus As Long) As Long
Declare Function XPRSloadbasis Lib "XPRS.DLL" (ByVal prob As Long, mrowstatus As Long, mcolstatus As Long) As Long
Declare Function XPRSgetindex Lib "XPRS.DLL" (ByVal prob As Long, ByVal itype As Long, ByVal sname As String, ByRef iseq As Long) As Long
Declare Function XPRSaddrows Lib "XPRS.DLL" (ByVal prob As Long, ByVal nrows As Long, ByVal ncoeffs As Long, srowtype As Byte, drhs As Double, drng As Double, mstart As Long, mclind As Long, dmatval As Double) As Long
Declare Function XPRSdelrows Lib "XPRS.DLL" (ByVal prob As Long, ByVal nrows As Long, mindex As Long) As Long
Declare Function XPRSaddcols Lib "XPRS.DLL" (ByVal prob As Long, ByVal ncols As Long, ByVal ncoeffs As Long, dobj As Double, mstart As Long, mrwind As Long, dmatval As Double, dbdl As Double, dbdu As Double) As Long
Declare Function XPRSdelcols Lib "XPRS.DLL" (ByVal prob As Long, ByVal ncols As Long, mindex As Long) As Long
Declare Function XPRSchgcoltype Lib "XPRS.DLL" (ByVal prob As Long, ByVal ncols As Long, mindex As Long, coltype As Byte) As Long
Declare Function XPRSchgrowtype Lib "XPRS.DLL" (ByVal prob As Long, ByVal nrows As Long, mindex As Long, srowtype As Byte) As Long
Declare Function XPRSchgbounds Lib "XPRS.DLL" (ByVal prob As Long, ByVal nbnds As Long, mindex As Long, sboundtype As Byte, dbnd As Double) As Long
Declare Function XPRSchgobj Lib "XPRS.DLL" (ByVal prob As Long, ByVal ncols As Long, mindex As Long, dobj As Double) As Long
Declare Function XPRSchgcoef Lib "XPRS.DLL" (ByVal prob As Long, ByVal irow As Long, ByVal icol As Long, ByVal dval As Double) As Long
Declare Function XPRSchgmcoef Lib "XPRS.DLL" (ByVal prob As Long, ByVal ncoeffs As Long, mrow As Long, mcol As Long, dval As Double) As Long
Declare Function XPRSchgmqobj Lib "XPRS.DLL" (ByVal prob As Long, ByVal ncols As Long, mcol1 As Long, mcol2 As Long, dval As Double) As Long
Declare Function XPRSchgqobj Lib "XPRS.DLL" (ByVal prob As Long, ByVal icol As Long, ByVal jcol As Long, ByVal dval As Double) As Long
Declare Function XPRSchgrhs Lib "XPRS.DLL" (ByVal prob As Long, ByVal nrows As Long, mindex As Long, drhs As Double) As Long
Declare Function XPRSchgrhsrange Lib "XPRS.DLL" (ByVal prob As Long, ByVal nrows As Long, mindex As Long, drng As Double) As Long
Declare Function XPRSchgobjsense Lib "XPRS.DLL" (ByVal prob As Long, ByVal objsense As Long) As Long
Declare Function XPRSchgglblimit Lib "XPRS.DLL" (ByVal prob As Long, ByVal ncols As Long, mindex As Long, dlimit As Double) As Long
Declare Function XPRSsave Lib "XPRS.DLL" (ByVal prob As Long) As Long
Declare Function XPRSrestore Lib "XPRS.DLL" (ByVal prob As Long, ByVal sprobname As String, ByVal force As String) As Long
Declare Function XPRSpivot Lib "XPRS.DLL" (ByVal prob As Long, ByVal in_ As Long, ByVal out As Long) As Long
Declare Function XPRSgetpivots Lib "XPRS.DLL" (ByVal prob As Long, ByVal in_ As Long, mout As Long, dout As Double, ByRef dobjo As Double, ByRef npiv As Long, ByVal maxpiv As Long) As Long
Declare Function XPRSaddcuts Lib "XPRS.DLL" (ByVal prob As Long, ByVal ncuts As Long, mtype As Long, qrtype As Byte, drhs As Double, mstart As Long, mcols As Long, dmatval As Double) As Long
Declare Function XPRSdelcuts Lib "XPRS.DLL" (ByVal prob As Long, ByVal ibasis As Long, ByVal itype As Long, ByVal interp As Long, ByVal delta As Double, ByVal ncuts As Long, mcutind As Long) As Long
Declare Function XPRSdelcpcuts Lib "XPRS.DLL" (ByVal prob As Long, ByVal itype As Long, ByVal interp As Long, ByVal ncuts As Long, mcutind As Long) As Long
Declare Function XPRSgetcutlist Lib "XPRS.DLL" (ByVal prob As Long, ByVal itype As Long, ByVal interp As Long, ByRef ncuts As Long, ByVal maxcuts As Long, mcutind As Long) As Long
Declare Function XPRSgetcpcutlist Lib "XPRS.DLL" (ByVal prob As Long, ByVal itype As Long, ByVal interp As Long, ByVal delta As Double, ByRef ncuts As Long, ByVal maxcuts As Long, mcutind As Long, dviol As Double) As Long
Declare Function XPRSgetcpcuts Lib "XPRS.DLL" (ByVal prob As Long, mindex As Long, ByVal ncuts As Long, ByVal size As Long, mtype As Long, qrtype As Byte, mstart As Long, mcols As Long, dmatval As Double, drhs As Double) As Long
Declare Function XPRSloadcuts Lib "XPRS.DLL" (ByVal prob As Long, ByVal itype As Long, ByVal interp As Long, ByVal ncuts As Long, mcutind As Long) As Long
Declare Function XPRSstorecuts Lib "XPRS.DLL" (ByVal prob As Long, ByVal ncuts As Long, ByVal nodupl As Long, mtype As Long, qrtype As Byte, drhs As Double, mstart As Long, mindex As Long, mcols As Long, dmatval As Double) As Long
Declare Function XPRSpresolverow Lib "XPRS.DLL" (ByVal prob As Long, ByVal qrtype As Byte, ByVal nzo As Long, mcolso As Long, dvalo As Double, ByVal drhso As Double, ByVal maxcoeffs As Long, ByRef nzp As Long, mcolsp As Long, dvalp As Double, ByRef drhsp As Double, ByRef status As Long) As Long
Declare Function XPRSloadmipsol Lib "XPRS.DLL" (ByVal prob As Long, dsol As Double, ByRef status As Long) As Long
Declare Function XPRSaddmipsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal ilength As Long, mipsolval As Double, mipsolcol As Long, ByVal solname As String) As Long
Declare Function XPRSstorebounds Lib "XPRS.DLL" (ByVal prob As Long, ByVal nbnds As Long, mcols As Long, qbtype As Byte, dbnd As Double, ByRef mindex As Long) As Long
Declare Function XPRSsetbranchcuts Lib "XPRS.DLL" (ByVal prob As Long, ByVal nbcuts As Long, mindex As Long) As Long
Declare Function XPRSsetbranchbounds Lib "XPRS.DLL" (ByVal prob As Long, ByVal mindex As Long) As Long
Private Declare Function XPRSgetlasterror_C Lib "XPRS.DLL" Alias "XPRSgetlasterror" (ByVal prob As Long, ByVal errmsg As String) As Long
Declare Function XPRSbasiscondition Lib "XPRS.DLL" (ByVal prob As Long, ByRef condnum As Double, ByRef scondnum As Double) As Long
Declare Function XPRSgetmipsol Lib "XPRS.DLL" (ByVal prob As Long, dx As Double, dslack As Double) As Long
Declare Function XPRSgetlpsol Lib "XPRS.DLL" (ByVal prob As Long, dx As Double, dslack As Double, dual As Double, dj As Double) As Long
Declare Function XPRSpostsolve Lib "XPRS.DLL" (ByVal prob As Long) As Long
Declare Function XPRSdelsets Lib "XPRS.DLL" (ByVal prob As Long, ByVal nsets As Long, msindex As Long) As Long
Declare Function XPRSaddsets Lib "XPRS.DLL" (ByVal prob As Long, ByVal newsets As Long, ByVal newnz As Long, qstype As Byte, msstart As Long, mscols As Long, dref As Double) As Long
Declare Function XPRSstrongbranch Lib "XPRS.DLL" (ByVal prob As Long, ByVal nbnds As Long, mindex As Long, sboundtype As Byte, dbnd As Double, ByVal itrlimit As Long, dsbobjval As Double, msbstatus As Long) As Long
Declare Function XPRSestimaterowdualranges Lib "XPRS.DLL" (ByVal prob As Long, ByVal nRows As Long, rowIndices As Long, ByVal iterationLimit As Long, minDualActivity As Double, maxDualActivity As Double) As Long
Declare Function XPRSgetprimalray Lib "XPRS.DLL" (ByVal prob As Long, dpray As Double, ByRef hasray As Long) As Long
Declare Function XPRSgetdualray Lib "XPRS.DLL" (ByVal prob As Long, ddray As Double, ByRef hasray As Long) As Long
Declare Function XPRSsetmessagestatus Lib "XPRS.DLL" (ByVal prob As Long, ByVal errcode As Long, ByVal bEnabledStatus As Long) As Long
Declare Function XPRSgetmessagestatus Lib "XPRS.DLL" (ByVal prob As Long, ByVal errcode As Long, ByRef bEnabledStatus As Long) As Long
Declare Function XPRSrepairweightedinfeas Lib "XPRS.DLL" (ByVal prob As Long, ByRef scode As Long, lrp_array As Double, grp_array As Double, lbp_array As Double, ubp_array As Double, ByVal second_phase As Byte, ByVal delta As Double, ByVal optflags As String) As Long
Declare Function XPRSrepairweightedinfeasbounds Lib "XPRS.DLL" (ByVal prob As Long, ByRef scode As Long, lrp_array As Double, grp_array As Double, lbp_array As Double, ubp_array As Double, lrb_array As Double, grb_array As Double, lbb_array As Double, ubb_array As Double, ByVal second_phase As Byte, ByVal delta As Double, ByVal optflags As String) As Long
Declare Function XPRSrepairinfeas Lib "XPRS.DLL" (ByVal prob As Long, ByRef scode As Long, ByVal ptype As Byte, ByVal phase2 As Byte, ByVal globalflags As Byte, ByVal lrp As Double, ByVal grp As Double, ByVal lbp As Double, ByVal ubp As Double, ByVal delta As Double) As Long
Declare Function XPRSgetcutslack Lib "XPRS.DLL" (ByVal prob As Long, ByVal cut As Long, ByRef dslack As Double) As Long
Declare Function XPRSgetcutmap Lib "XPRS.DLL" (ByVal prob As Long, ByVal ncuts As Long, cuts As Long, cutmap As Long) As Long
Declare Function XPRSbasisstability Lib "XPRS.DLL" (ByVal prob As Long, ByVal typecode As Long, ByVal norm As Long, ByVal ifscaled As Long, ByRef dval As Double) As Long
Declare Function XPRScalcslacks Lib "XPRS.DLL" (ByVal prob As Long, solution As Double, calculatedslacks As Double) As Long
Declare Function XPRScalcreducedcosts Lib "XPRS.DLL" (ByVal prob As Long, duals As Double, solution As Double, calculateddjs As Double) As Long
Declare Function XPRScalcobjective Lib "XPRS.DLL" (ByVal prob As Long, solution As Double, ByRef objective As Double) As Long
Declare Function XPRSrefinemipsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal options As Long, ByVal sflags As String, solution As Double, refined_solution As Double, ByRef refinestatus As Long) As Long
Declare Function XPRScalcsolinfo Lib "XPRS.DLL" (ByVal prob As Long, solution As Double, dual As Double, ByVal Property As Long, ByRef Value As Double) As Long
Declare Function XPRSgetnamelist Lib "XPRS.DLL" (ByVal prob As Long, ByVal itype As Long, sbuff As Byte, ByVal names_len As Long, ByRef names_len_reqd As Long, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetnamelistobject Lib "XPRS.DLL" (ByVal prob As Long, ByVal itype As Long, ByRef r_nl As Long) As Long
Declare Function XPRSlogfilehandler Lib "XPRS.DLL" (ByVal obj As Long, ByVal vUserContext As String, ByVal vSystemThreadId As Long, ByVal sMsg As String, ByVal iMsgType As Long, ByVal iMsgCode As Long) As Long
Private Declare Function XPRSgetobjecttypename_C Lib "XPRS.DLL" Alias "XPRSgetobjecttypename" (ByVal obj As Long, ByRef sObjectName As Long) As Long
Declare Function XPRSstrongbranchcb Lib "XPRS.DLL" (ByVal prob As Long, ByVal nbnds As Long, mindex As Long, sboundtype As Byte, dbnd As Double, ByVal itrlimit As Long, dsbobjval As Double, msbstatus As Long, ByVal sbsolvecb As Long, ByVal vContext As String) As Long
Declare Function XPRSsetcblplog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_lplog As Long, ByVal p As String) As Long
Declare Function XPRSgetcblplog Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_lplog As Long, ByRef p As String) As Long
Declare Function XPRSaddcblplog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_lplog As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecblplog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_lplog As Long, ByVal p As String) As Long
Declare Function XPRSsetcbgloballog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_globallog As Long, ByVal p As String) As Long
Declare Function XPRSgetcbgloballog Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_globallog As Long, ByRef p As String) As Long
Declare Function XPRSaddcbgloballog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_globallog As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbgloballog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_globallog As Long, ByVal p As String) As Long
Declare Function XPRSsetcbcutlog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_cutlog As Long, ByVal p As String) As Long
Declare Function XPRSgetcbcutlog Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_cutlog As Long, ByRef p As String) As Long
Declare Function XPRSaddcbcutlog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_cutlog As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbcutlog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_cutlog As Long, ByVal p As String) As Long
Declare Function XPRSsetcbbarlog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_barlog As Long, ByVal p As String) As Long
Declare Function XPRSgetcbbarlog Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_barlog As Long, ByRef p As String) As Long
Declare Function XPRSaddcbbarlog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_barlog As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbbarlog Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_barlog As Long, ByVal p As String) As Long
Declare Function XPRSsetcbcutmgr Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_cutmgr As Long, ByVal p As String) As Long
Declare Function XPRSgetcbcutmgr Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_cutmgr As Long, ByRef p As String) As Long
Declare Function XPRSaddcbcutmgr Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_cutmgr As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbcutmgr Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_cutmgr As Long, ByVal p As String) As Long
Declare Function XPRSsetcbchgnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_chgnode As Long, ByVal p As String) As Long
Declare Function XPRSgetcbchgnode Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_chgnode As Long, ByRef p As String) As Long
Declare Function XPRSaddcbchgnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_chgnode As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbchgnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_chgnode As Long, ByVal p As String) As Long
Declare Function XPRSsetcboptnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_optnode As Long, ByVal p As String) As Long
Declare Function XPRSgetcboptnode Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_optnode As Long, ByRef p As String) As Long
Declare Function XPRSaddcboptnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_optnode As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecboptnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_optnode As Long, ByVal p As String) As Long
Declare Function XPRSsetcbprenode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_prenode As Long, ByVal p As String) As Long
Declare Function XPRSgetcbprenode Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_prenode As Long, ByRef p As String) As Long
Declare Function XPRSaddcbprenode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_prenode As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbprenode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_prenode As Long, ByVal p As String) As Long
Declare Function XPRSsetcbinfnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_infnode As Long, ByVal p As String) As Long
Declare Function XPRSgetcbinfnode Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_infnode As Long, ByRef p As String) As Long
Declare Function XPRSaddcbinfnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_infnode As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbinfnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_infnode As Long, ByVal p As String) As Long
Declare Function XPRSsetcbnodecutoff Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_nodecutoff As Long, ByVal p As String) As Long
Declare Function XPRSgetcbnodecutoff Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_nodecutoff As Long, ByRef p As String) As Long
Declare Function XPRSaddcbnodecutoff Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_nodecutoff As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbnodecutoff Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_nodecutoff As Long, ByVal p As String) As Long
Declare Function XPRSsetcbintsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_intsol As Long, ByVal p As String) As Long
Declare Function XPRSgetcbintsol Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_intsol As Long, ByRef p As String) As Long
Declare Function XPRSaddcbintsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_intsol As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbintsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_intsol As Long, ByVal p As String) As Long
Declare Function XPRSsetcbpreintsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_preintsol As Long, ByVal p As String) As Long
Declare Function XPRSgetcbpreintsol Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_preintsol As Long, ByRef p As String) As Long
Declare Function XPRSaddcbpreintsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_preintsol As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbpreintsol Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_preintsol As Long, ByVal p As String) As Long
Declare Function XPRSsetcbchgbranch Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_chgbranch As Long, ByVal p As String) As Long
Declare Function XPRSgetcbchgbranch Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_chgbranch As Long, ByRef p As String) As Long
Declare Function XPRSaddcbchgbranch Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_chgbranch As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbchgbranch Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_chgbranch As Long, ByVal p As String) As Long
Declare Function XPRSsetcbestimate Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_estimate As Long, ByVal p As String) As Long
Declare Function XPRSgetcbestimate Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_estimate As Long, ByRef p As String) As Long
Declare Function XPRSaddcbestimate Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_estimate As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbestimate Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_estimate As Long, ByVal p As String) As Long
Declare Function XPRSsetcbsepnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_sepnode As Long, ByVal p As String) As Long
Declare Function XPRSgetcbsepnode Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_sepnode As Long, ByRef p As String) As Long
Declare Function XPRSaddcbsepnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_sepnode As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbsepnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_sepnode As Long, ByVal p As String) As Long
Declare Function XPRSsetcbmessage Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_message As Long, ByVal p As String) As Long
Declare Function XPRSgetcbmessage Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_message As Long, ByRef p As String) As Long
Declare Function XPRSaddcbmessage Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_message As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbmessage Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_message As Long, ByVal p As String) As Long
Declare Function XPRSsetcbmipthread Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_mipthread As Long, ByVal p As String) As Long
Declare Function XPRSgetcbmipthread Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_mipthread As Long, ByRef p As String) As Long
Declare Function XPRSaddcbmipthread Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_mipthread As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbmipthread Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_mipthread As Long, ByVal p As String) As Long
Declare Function XPRSsetcbdestroymt Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_destroymt As Long, ByVal p As String) As Long
Declare Function XPRSgetcbdestroymt Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_destroymt As Long, ByRef p As String) As Long
Declare Function XPRSaddcbdestroymt Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_destroymt As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbdestroymt Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_destroymt As Long, ByVal p As String) As Long
Declare Function XPRSsetcbnewnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_newnode As Long, ByVal p As String) As Long
Declare Function XPRSgetcbnewnode Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_newnode As Long, ByRef p As String) As Long
Declare Function XPRSaddcbnewnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_newnode As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbnewnode Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_newnode As Long, ByVal p As String) As Long
Declare Function XPRSsetcbbariteration Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_bariteration As Long, ByVal p As String) As Long
Declare Function XPRSgetcbbariteration Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_bariteration As Long, ByRef p As String) As Long
Declare Function XPRSaddcbbariteration Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_bariteration As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbbariteration Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_bariteration As Long, ByVal p As String) As Long
Declare Function XPRSsetcbchgbranchobject Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_chgbranchobject As Long, ByVal p As String) As Long
Declare Function XPRSgetcbchgbranchobject Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_chgbranchobject As Long, ByRef p As String) As Long
Declare Function XPRSaddcbchgbranchobject Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_chgbranchobject As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbchgbranchobject Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_chgbranchobject As Long, ByVal p As String) As Long
Declare Function XPRSsetcbgapnotify Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_gapnotify As Long, ByVal p As String) As Long
Declare Function XPRSgetcbgapnotify Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_gapnotify As Long, ByRef p As String) As Long
Declare Function XPRSaddcbgapnotify Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_gapnotify As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbgapnotify Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_gapnotify As Long, ByVal p As String) As Long
Declare Function XPRSsetcbusersolnotify Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_usersolnotify As Long, ByVal p As String) As Long
Declare Function XPRSgetcbusersolnotify Lib "XPRS.DLL" (ByVal prob As Long, ByRef f_usersolnotify As Long, ByRef p As String) As Long
Declare Function XPRSaddcbusersolnotify Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_usersolnotify As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRSremovecbusersolnotify Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_usersolnotify As Long, ByVal p As String) As Long

Declare Function XPRSobjsa Lib "XPRS.DLL" (ByVal prob As Long, ByVal ncols As Long, mindex As Long, lower As Double, upper As Double) As Long
Declare Function XPRSrhssa Lib "XPRS.DLL" (ByVal prob As Long, ByVal nrows As Long, mindex As Long, lower As Double, upper As Double) As Long

Declare Function XPRSstartrecord Lib "XPRS.DLL" (ByVal path As String, ByVal cfgfilepath As String, ByVal flags As Long) As Long
Declare Function XPRSstoprecord Lib "XPRS.DLL" () As Long

Declare Function XPRS_ge_setcbmsghandler Lib "XPRS.DLL" (ByVal f_msghandler As Long, ByVal p As String) As Long
Declare Function XPRS_ge_getcbmsghandler Lib "XPRS.DLL" (ByRef f_msghandler As Long, ByRef p As String) As Long
Declare Function XPRS_ge_addcbmsghandler Lib "XPRS.DLL" (ByVal f_msghandler As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRS_ge_removecbmsghandler Lib "XPRS.DLL" (ByVal f_msghandler As Long, ByVal p As String) As Long

Private Declare Function XPRS_ge_getlasterror_C Lib "XPRS.DLL" Alias "XPRS_ge_getlasterror" (ByRef iMsgCode As Long, ByVal msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long

Declare Function XPRS_msp_create Lib "XPRS.DLL" (ByRef msp As Long) As Long
Declare Function XPRS_msp_destroy Lib "XPRS.DLL" (ByVal msp As Long) As Long
Declare Function XPRS_msp_probattach Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob As Long) As Long
Declare Function XPRS_msp_probdetach Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob As Long) As Long
Declare Function XPRS_msp_getsollist Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob_to_rank_against As Long, ByVal iRankAttrib As Long, ByVal bRankAscending As Long, ByVal iRankFirstIndex_Ob As Long, ByVal iRankLastIndex_Ob As Long, iSolutionIds_Zb As Long, ByRef nReturnedSolIds As Long, ByRef nSols As Long) As Long
Declare Function XPRS_msp_getsollist2 Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob_to_rank_against As Long, ByVal iRankAttrib As Long, ByVal bRankAscending As Long, ByVal iRankFirstIndex_Ob As Long, ByVal iRankLastIndex_Ob As Long, ByVal bUseUserBitFilter As Long, ByVal iUserBitMask As Long, ByVal iUserBitPattern As Long, ByVal bUseInternalBitFilter As Long, ByVal iInternalBitMask As Long, ByVal iInternalBitPattern As Long, iSolutionIds_Zb As Long, ByRef nReturnedSolIds As Long, ByRef nSols As Long) As Long
Declare Function XPRS_msp_getsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, x As Double, ByVal iColFirst As Long, ByVal iColLast As Long, ByRef nValuesReturned As Long) As Long
Declare Function XPRS_msp_getslack Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob_to_rank_against As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, slack As Double, ByVal iRowFirst As Long, ByVal iRowLast As Long, ByRef nValuesReturned As Long) As Long
Declare Function XPRS_msp_loadsol Lib "XPRS.DLL" (ByVal msp As Long, ByRef iSolutionId As Long, x As Double, ByVal nCols As Long, ByVal sSolutionName As String, ByRef bNameModifiedForUniqueness As Long, ByRef iSolutionIdOfExistingDuplicatePreventedLoad As Long) As Long
Declare Function XPRS_msp_delsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long) As Long
Declare Function XPRS_msp_getintattribprobsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob_to_rank_against As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iAttribId As Long, ByRef Dst As Long) As Long
Declare Function XPRS_msp_getdblattribprobsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob_to_rank_against As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iAttribId As Long, ByRef Dst As Double) As Long
Declare Function XPRS_msp_getintattribprob Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob As Long, ByVal iAttribId As Long, ByRef Dst As Long) As Long
Declare Function XPRS_msp_getdblattribprob Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob As Long, ByVal iAttribId As Long, ByRef Dst As Double) As Long
Declare Function XPRS_msp_getintattribsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iAttribId As Long, ByRef Dst As Long) As Long
Declare Function XPRS_msp_getdblattribsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iAttribId As Long, ByRef Dst As Double) As Long
Declare Function XPRS_msp_getintcontrolsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iControlId As Long, ByRef Val As Long) As Long
Declare Function XPRS_msp_getdblcontrolsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iControlId As Long, ByRef Val As Double) As Long
Declare Function XPRS_msp_setintcontrolsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iControlId As Long, ByVal Val As Long) As Long
Declare Function XPRS_msp_setdblcontrolsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal iControlId As Long, ByVal Val As Double) As Long
Declare Function XPRS_msp_getintattribprobextreme Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob_to_rank_against As Long, ByVal bGet_Max_Otherwise_Min As Long, ByRef iSolutionId As Long, ByVal iAttribId As Long, ByRef ExtremeVal As Long) As Long
Declare Function XPRS_msp_getdblattribprobextreme Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob_to_rank_against As Long, ByVal bGet_Max_Otherwise_Min As Long, ByRef iSolutionId As Long, ByVal iAttribId As Long, ByRef ExtremeVal As Double) As Long
Declare Function XPRS_msp_getintattrib Lib "XPRS.DLL" (ByVal msp As Long, ByVal iAttribId As Long, ByRef Val As Long) As Long
Declare Function XPRS_msp_getdblattrib Lib "XPRS.DLL" (ByVal msp As Long, ByVal iAttribId As Long, ByRef Val As Double) As Long
Declare Function XPRS_msp_getintcontrol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iControlId As Long, ByRef Val As Long) As Long
Declare Function XPRS_msp_getdblcontrol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iControlId As Long, ByRef Val As Double) As Long
Declare Function XPRS_msp_setintcontrol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iControlId As Long, ByVal Val As Long) As Long
Declare Function XPRS_msp_setdblcontrol Lib "XPRS.DLL" (ByVal msp As Long, ByVal iControlId As Long, ByVal Val As Double) As Long
Declare Function XPRS_msp_setsolname Lib "XPRS.DLL" (ByVal msp As Long, ByVal iSolutionId As Long, ByVal sNewSolutionBaseName As String, ByRef bNameModifiedForUniqueness As Long, ByRef iSolutionIdStatus_ As Long) As Long
Private Declare Function XPRS_msp_getsolname_C Lib "XPRS.DLL" Alias "XPRS_msp_getsolname" (ByVal msp As Long, ByVal iSolutionId As Long, ByVal sname As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long, ByRef iSolutionIdStatus_ As Long) As Long
Declare Function XPRS_msp_findsolbyname Lib "XPRS.DLL" (ByVal msp As Long, ByVal sSolutionName As String, ByRef iSolutionId As Long) As Long
Declare Function XPRS_msp_writeslxsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal prob_context As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus_ As Long, ByVal sFileName As String, ByVal sFlags As String) As Long
Declare Function XPRS_msp_readslxsol Lib "XPRS.DLL" (ByVal msp As Long, ByVal col_name_list As Long, ByVal sFileName As String, ByVal sFlags As String, ByRef iSolutionId_Beg As Long, ByRef iSolutionId_End As Long) As Long
Private Declare Function XPRS_msp_getlasterror_C Lib "XPRS.DLL" Alias "XPRS_msp_getlasterror" (ByVal msp As Long, ByRef iMsgCode As Long, ByVal msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
Declare Function XPRS_msp_setcbmsghandler Lib "XPRS.DLL" (ByVal msp As Long, ByVal f_msghandler As Long, ByVal p As String) As Long
Declare Function XPRS_msp_getcbmsghandler Lib "XPRS.DLL" (ByVal msp As Long, ByRef f_msghandler As Long, ByRef p As String) As Long
Declare Function XPRS_msp_addcbmsghandler Lib "XPRS.DLL" (ByVal msp As Long, ByVal f_msghandler As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRS_msp_removecbmsghandler Lib "XPRS.DLL" (ByVal msp As Long, ByVal f_msghandler As Long, ByVal p As String) As Long

Declare Function XPRS_nml_create Lib "XPRS.DLL" (ByRef r_nl As Long) As Long
Declare Function XPRS_nml_destroy Lib "XPRS.DLL" (ByVal nml As Long) As Long
Declare Function XPRS_nml_getnamecount Lib "XPRS.DLL" (ByVal nml As Long, ByRef count As Long) As Long
Declare Function XPRS_nml_getmaxnamelen Lib "XPRS.DLL" (ByVal nml As Long, ByRef namlen As Long) As Long
Declare Function XPRS_nml_getnames Lib "XPRS.DLL" (ByVal nml As Long, ByVal padlen As Long, buf As Byte, ByVal buflen As Long, ByRef r_buflen_reqd As Long, ByVal firstIndex As Long, ByVal lastIndex As Long) As Long
Declare Function XPRS_nml_addnames Lib "XPRS.DLL" (ByVal nml As Long, buf As Byte, ByVal firstIndex As Long, ByVal lastIndex As Long) As Long
Declare Function XPRS_nml_removenames Lib "XPRS.DLL" (ByVal nml As Long, ByVal firstIndex As Long, ByVal lastIndex As Long) As Long
Declare Function XPRS_nml_findname Lib "XPRS.DLL" (ByVal nml As Long, ByVal name As String, ByRef r_index As Long) As Long
Declare Function XPRS_nml_copynames Lib "XPRS.DLL" (ByVal dst As Long, ByVal src As Long) As Long
Private Declare Function XPRS_nml_getlasterror_C Lib "XPRS.DLL" Alias "XPRS_nml_getlasterror" (ByVal nml As Long, ByRef iMsgCode As Long, ByVal msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long

Declare Function XPRSgetqrowcoeff Lib "XPRS.DLL" (ByVal prob As Long, ByVal irow As Long, ByVal icol As Long, ByVal jcol As Long, ByRef dval As Double) As Long
Declare Function XPRSgetqrowqmatrix Lib "XPRS.DLL" (ByVal prob As Long, ByVal irow As Long, mstart As Long, mclind As Long, dobjval As Double, ByVal maxcoeffs As Long, ByRef ncoeffs As Long, ByVal first As Long, ByVal last As Long) As Long
Declare Function XPRSgetqrowqmatrixtriplets Lib "XPRS.DLL" (ByVal prob As Long, ByVal irow As Long, ByRef nqelem As Long, mqcol1 As Long, mqcol2 As Long, dqe As Double) As Long
Declare Function XPRSchgqrowcoeff Lib "XPRS.DLL" (ByVal prob As Long, ByVal irow As Long, ByVal icol As Long, ByVal jcol As Long, ByVal dval As Double) As Long
Declare Function XPRSgetqrows Lib "XPRS.DLL" (ByVal prob As Long, ByRef qmn As Long, qcrows As Long) As Long
Declare Function XPRSaddqmatrix Lib "XPRS.DLL" (ByVal prob As Long, ByVal irow As Long, ByVal nqtr As Long, mqc1 As Long, mqc2 As Long, dqew As Double) As Long
Declare Function XPRSdelqmatrix Lib "XPRS.DLL" (ByVal prob As Long, ByVal irow As Long) As Long
Declare Function XPRSloadqcqp Lib "XPRS.DLL" (ByVal prob As Long, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, drange As Double, dobj As Double, mstart As Long, mnel As Long, mrwind As Long, dmatval As Double, dlb As Double, dub As Double, ByVal nquads As Long, mqcol1 As Long, mqcol2 As Long, dqval As Double, ByVal qmn As Long, qcrows As Long, qcnquads As Long, qcmqcol1 As Long, qcmqcol2 As Long, qcdqval As Double) As Long
Declare Function XPRSloadqcqpglobal Lib "XPRS.DLL" (ByVal prob As Long, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, drange As Double, dobj As Double, matbeg As Long, matcnt As Long, matrow As Long, dmatval As Double, dlb As Double, dub As Double, ByVal nquads As Long, mqcol1 As Long, mqcol2 As Long, dqval As Double, ByVal qmn As Long, qcrows As Long, qcnquads As Long, qcmqcol1 As Long, qcmqcol2 As Long, qcdqval As Double, ByVal ngents As Long, ByVal nsets As Long, qgtype As Byte, mgcols As Long, dlim As Double, qstype As Byte, msstart As Long, mscols As Long, dref As Double) As Long

Declare Function XPRS_mse_create Lib "XPRS.DLL" (ByRef mse As Long) As Long
Declare Function XPRS_mse_destroy Lib "XPRS.DLL" (ByVal mse As Long) As Long
Declare Function XPRS_mse_getsollist Lib "XPRS.DLL" (ByVal mse As Long, ByVal iMetricId As Long, ByVal iRankFirstIndex_Ob As Long, ByVal iRankLastIndex_Ob As Long, iSolutionIds As Long, ByRef nReturnedSolIds As Long, ByRef nSols As Long) As Long
Declare Function XPRS_mse_getsolmetric Lib "XPRS.DLL" (ByVal mse As Long, ByVal iSolutionId As Long, ByRef iSolutionIdStatus As Long, ByVal iMetricId As Long, ByRef dMetric As Double) As Long
Declare Function XPRS_mse_getcullchoice Lib "XPRS.DLL" (ByVal mse As Long, ByVal iMetricId As Long, cull_sol_id_list As Long, ByVal nMaxSolsToCull As Long, ByRef nSolsToCull As Long, ByVal dNewSolMetric As Double, x As Double, ByVal nCols As Long, ByRef bRejectSoln As Long) As Long
Declare Function XPRS_mse_minim Lib "XPRS.DLL" (ByVal mse As Long, ByVal prob As Long, ByVal msp As Long, ByVal f_mse_handler As Long, ByVal p As String, ByRef nMaxSols As Long) As Long
Declare Function XPRS_mse_maxim Lib "XPRS.DLL" (ByVal mse As Long, ByVal prob As Long, ByVal msp As Long, ByVal f_mse_handler As Long, ByVal p As String, ByRef nMaxSols As Long) As Long
Declare Function XPRS_mse_opt Lib "XPRS.DLL" (ByVal mse As Long, ByVal prob As Long, ByVal msp As Long, ByVal f_mse_handler As Long, ByVal p As String, ByRef nMaxSols As Long) As Long
Declare Function XPRS_mse_getintattrib Lib "XPRS.DLL" (ByVal mse As Long, ByVal iAttribId As Long, ByRef Val As Long) As Long
Declare Function XPRS_mse_getdblattrib Lib "XPRS.DLL" (ByVal mse As Long, ByVal iAttribId As Long, ByRef Val As Double) As Long
Declare Function XPRS_mse_getintcontrol Lib "XPRS.DLL" (ByVal mse As Long, ByVal iAttribId As Long, ByRef Val As Long) As Long
Declare Function XPRS_mse_getdblcontrol Lib "XPRS.DLL" (ByVal mse As Long, ByVal iAttribId As Long, ByRef Val As Double) As Long
Declare Function XPRS_mse_setintcontrol Lib "XPRS.DLL" (ByVal mse As Long, ByVal iAttribId As Long, ByVal Val As Long) As Long
Declare Function XPRS_mse_setdblcontrol Lib "XPRS.DLL" (ByVal mse As Long, ByVal iAttribId As Long, ByVal Val As Double) As Long
Private Declare Function XPRS_mse_getlasterror_C Lib "XPRS.DLL" Alias "XPRS_mse_getlasterror" (ByVal mse As Long, ByRef iMsgCode As Long, ByVal msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
Declare Function XPRS_mse_setsolbasename Lib "XPRS.DLL" (ByVal mse As Long, ByVal sSolutionBaseName As String) As Long
Private Declare Function XPRS_mse_getsolbasename_C Lib "XPRS.DLL" Alias "XPRS_mse_getsolbasename" (ByVal mse As Long, ByVal sname As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
Declare Function XPRS_mse_setcbgetsolutiondiff Lib "XPRS.DLL" (ByVal mse As Long, ByVal f_mse_getsolutiondiff As Long, ByVal p As String) As Long
Declare Function XPRS_mse_getcbgetsolutiondiff Lib "XPRS.DLL" (ByVal mse As Long, ByRef f_mse_getsolutiondiff As Long, ByRef p As String) As Long
Declare Function XPRS_mse_addcbgetsolutiondiff Lib "XPRS.DLL" (ByVal mse As Long, ByVal f_mse_getsolutiondiff As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRS_mse_removecbgetsolutiondiff Lib "XPRS.DLL" (ByVal mse As Long, ByVal f_mse_getsolutiondiff As Long, ByVal p As String) As Long
Declare Function XPRS_mse_setcbmsghandler Lib "XPRS.DLL" (ByVal mse As Long, ByVal f_msghandler As Long, ByVal p As String) As Long
Declare Function XPRS_mse_getcbmsghandler Lib "XPRS.DLL" (ByVal mse As Long, ByRef f_msghandler As Long, ByRef p As String) As Long
Declare Function XPRS_mse_addcbmsghandler Lib "XPRS.DLL" (ByVal mse As Long, ByVal f_msghandler As Long, ByVal p As String, ByVal priority As Long) As Long
Declare Function XPRS_mse_removecbmsghandler Lib "XPRS.DLL" (ByVal mse As Long, ByVal f_msghandler As Long, ByVal p As String) As Long

Declare Function XPRS_bo_create Lib "XPRS.DLL" (ByRef p_object As Long, ByVal prob As Long, ByVal isoriginal As Long) As Long
Declare Function XPRS_bo_destroy Lib "XPRS.DLL" (ByVal obranch As Long) As Long
Declare Function XPRS_bo_store Lib "XPRS.DLL" (ByVal obranch As Long, ByRef p_status As Long) As Long
Declare Function XPRS_bo_addbranches Lib "XPRS.DLL" (ByVal obranch As Long, ByVal nbranches As Long) As Long
Declare Function XPRS_bo_getbranches Lib "XPRS.DLL" (ByVal obranch As Long, ByRef p_nbranches As Long) As Long
Declare Function XPRS_bo_setpriority Lib "XPRS.DLL" (ByVal obranch As Long, ByVal ipriority As Long) As Long
Declare Function XPRS_bo_setpreferredbranch Lib "XPRS.DLL" (ByVal obranch As Long, ByVal ibranch As Long) As Long
Declare Function XPRS_bo_addbounds Lib "XPRS.DLL" (ByVal obranch As Long, ByVal ibranch As Long, ByVal nbounds As Long, cbndtype As Byte, mbndcol As Long, dbndval As Double) As Long
Declare Function XPRS_bo_getbounds Lib "XPRS.DLL" (ByVal obranch As Long, ByVal ibranch As Long, ByRef p_nbounds As Long, ByVal nbounds_size As Long, cbndtype As Byte, mbndcol As Long, dbndval As Double) As Long
Declare Function XPRS_bo_addrows Lib "XPRS.DLL" (ByVal obranch As Long, ByVal ibranch As Long, ByVal nrows As Long, ByVal nelems As Long, crtype As Byte, drrhs As Double, mrbeg As Long, mcol As Long, dval As Double) As Long
Declare Function XPRS_bo_getrows Lib "XPRS.DLL" (ByVal obranch As Long, ByVal ibranch As Long, ByRef p_nrows As Long, ByVal nrows_size As Long, ByRef p_nelems As Long, ByVal nelems_size As Long, crtype As Byte, drrhs As Double, mrbeg As Long, mcol As Long, dval As Double) As Long
Declare Function XPRS_bo_addcuts Lib "XPRS.DLL" (ByVal obranch As Long, ByVal ibranch As Long, ByVal ncuts As Long, mcutind As Long) As Long
Declare Function XPRS_bo_getid Lib "XPRS.DLL" (ByVal obranch As Long, ByRef p_id As Long) As Long
Private Declare Function XPRS_bo_getlasterror_C Lib "XPRS.DLL" Alias "XPRS_bo_getlasterror" (ByVal obranch As Long, ByRef iMsgCode As Long, ByVal msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
Declare Function XPRS_bo_validate Lib "XPRS.DLL" (ByVal obranch As Long, ByRef p_status As Long) As Long


Declare  Function XPRSsetcbmessageVB Lib "XPRS.DLL" (ByVal prob As Long, ByVal f_message As Long, ByRef p As Long) As Long

' OS-provided utility functions we'll use
Private Declare  Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length as Long)

' XPRS-provided utility functions we'll use
Private Declare  Function CStrLen Lib "XPRS.DLL" Alias "XPRS_vbhelp_strlen" (ByVal lpStr As Long, ByRef r_StrLen As Long) As Long

' Declarations of functions that allow NULL pointers to be passed

Declare  Function XPRSloadlpr Lib "XPRS.DLL" Alias "XPRSloadlp" (ByVal prob As Long, ByVal sprobname As String, ByVal ncols As Long, ByVal nrows As Long, srowtypes As Byte, drhs As Double, ByVal drange As String, dobj As Double, mstart As Long, ByVal mnel As String, mrwind As Long, dmatval As Double, dlb As Double, dub As Double) As Long
Declare  Function XPRSloaddirsx Lib "XPRS.DLL" Alias "XPRSloaddirs" (ByVal prob As Long, ByVal ndirs As Long, mcols As Long, mpri As Long, ByVal sbr As String, ByVal dupc As String, ByVal ddpc As String) As Long
Declare  Function XPRSgetsolx Lib "XPRS.DLL" Alias "XPRSgetsol" (ByVal prob As Long, dx As Double, ByVal dslack As String, ByVal dual As String, ByVal dj As String) As Long
Declare  Function XPRSgetsolslack Lib "XPRS.DLL" Alias "XPRSgetsol" (ByVal prob As Long, ByVal dx As String, dslack As Double, ByVal dual As String, ByVal dj As String) As Long
Declare  Function XPRSgetsoldual Lib "XPRS.DLL" Alias "XPRSgetsol" (ByVal prob As Long, ByVal dx As String, ByVal dslack As String, dual As Double, ByVal dj As String) As Long
Declare  Function XPRSgetsoldj Lib "XPRS.DLL" Alias "XPRSgetsol" (ByVal prob As Long, ByVal dx As String, ByVal dslack As String, ByVal dual As String, dj As Double) As Long
Declare  Function XPRSgetpresolvesolx Lib "XPRS.DLL" Alias "XPRSgetpresolvesol" (ByVal prob As Long, dx As Double, ByVal dslack As String, ByVal dual As String, ByVal dj As String) As Long
Declare  Function XPRSgetpresolvesolslack Lib "XPRS.DLL" Alias "XPRSgetpresolvesol" (ByVal prob As Long, ByVal dx As String, dslack As Double, ByVal dual As String, ByVal dj As String) As Long
Declare  Function XPRSgetpresolvesoldual Lib "XPRS.DLL" Alias "XPRSgetpresolvesol" (ByVal prob As Long, ByVal dx As String, ByVal dslack As String, dual As Double, ByVal dj As String) As Long
Declare  Function XPRSgetpresolvesoldj Lib "XPRS.DLL" Alias "XPRSgetpresolvesol" (ByVal prob As Long, ByVal dx As String, ByVal dslack As String, ByVal dual As String, dj As Double) As Long
Declare  Function XPRSgetglobalx Lib "XPRS.DLL" Alias "XPRSgetglobal" (ByVal prob As Long, ByRef ngents As Long, ByRef nsets As Long, sgtype As Byte, mgcols As Long, ByVal dlim As String, ByVal sstype As String, ByVal msstart As String, ByVal mscols As String, ByVal dref As String) As Long
Declare  Function XPRSaddrowsr Lib "XPRS.DLL" Alias "XPRSaddrows" (ByVal prob As Long, ByVal nrows As Long, ByVal ncoeffs As Long, srowtype As Byte, drhs As Double, ByVal drng As String, mstart As Long, mclind As Long, dmatval As Double) As Long

' Utility function to create a VB string when given the pointer to a C string
Private Function StringFromPointer(StringPointer As Long) As String
  If StringPointer=0 Then
    StringFromPointer = vbNullString
  Else
  	Dim string_length As Long
  	CStrLen StringPointer, string_length
  	Dim str As String
  	str = Space$(string_length)
  	CopyMemory ByVal str, ByVal StringPointer, string_length
  	StringFromPointer = str
  End If
End Function


' VB wrappers for functions that need them
Function XPRSgetbanner(ByRef banner As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_banner As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetbanner_C(c_banner)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_banner, vbNullChar) > 1 Then
    banner = Left(c_banner, InStr(1, c_banner, vbNullChar) - 1)
  End If
  XPRSgetbanner = rval
End Function


Function XPRSgetversion(ByRef version As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_version As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetversion_C(c_version)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_version, vbNullChar) > 1 Then
    version = Left(c_version, InStr(1, c_version, vbNullChar) - 1)
  End If
  XPRSgetversion = rval
End Function


Function XPRSlicense(ByRef i1 As Long, ByRef c1 As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_c1 As String * 255
  c_c1 = c1
  Mid(c_c1, Len(c1) + 1, 1) = vbNullChar
  ' Call XPRS.DLL
  rval = XPRSlicense_C(i1, c_c1)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_c1, vbNullChar) > 1 Then
    c1 = Left(c_c1, InStr(1, c_c1, vbNullChar) - 1)
  End If
  XPRSlicense = rval
End Function



Function XPRSgetprobname(ByVal prob As Long, ByRef svalue As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_svalue As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetprobname_C(prob, c_svalue)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_svalue, vbNullChar) > 1 Then
    svalue = Left(c_svalue, InStr(1, c_svalue, vbNullChar) - 1)
  End If
  XPRSgetprobname = rval
End Function


Function XPRSgetstrcontrol(ByVal prob As Long, ByVal index As Long, ByRef svalue As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_svalue As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetstrcontrol_C(prob, index, c_svalue)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_svalue, vbNullChar) > 1 Then
    svalue = Left(c_svalue, InStr(1, c_svalue, vbNullChar) - 1)
  End If
  XPRSgetstrcontrol = rval
End Function


Function XPRSgetstringcontrol(ByVal prob As Long, ByVal index As Long, ByRef svalue As String, ByVal svaluesize As Long, ByRef controlsize As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_svalue As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetstringcontrol_C(prob, index, c_svalue, svaluesize, controlsize)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_svalue, vbNullChar) > 1 Then
    svalue = Left(c_svalue, InStr(1, c_svalue, vbNullChar) - 1)
  End If
  XPRSgetstringcontrol = rval
End Function


Function XPRSgetstrattrib(ByVal prob As Long, ByVal index As Long, ByRef cvalue As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_cvalue As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetstrattrib_C(prob, index, c_cvalue)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_cvalue, vbNullChar) > 1 Then
    cvalue = Left(c_cvalue, InStr(1, c_cvalue, vbNullChar) - 1)
  End If
  XPRSgetstrattrib = rval
End Function


Function XPRSgetstringattrib(ByVal prob As Long, ByVal index As Long, ByRef cvalue As String, ByVal cvaluesize As Long, ByRef controlsize As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_cvalue As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetstringattrib_C(prob, index, c_cvalue, cvaluesize, controlsize)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_cvalue, vbNullChar) > 1 Then
    cvalue = Left(c_cvalue, InStr(1, c_cvalue, vbNullChar) - 1)
  End If
  XPRSgetstringattrib = rval
End Function



Function XPRSgetlasterror(ByVal prob As Long, ByRef errmsg As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_errmsg As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetlasterror_C(prob, c_errmsg)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_errmsg, vbNullChar) > 1 Then
    errmsg = Left(c_errmsg, InStr(1, c_errmsg, vbNullChar) - 1)
  End If
  XPRSgetlasterror = rval
End Function


Function XPRSgetobjecttypename(ByVal obj As Long, ByRef sObjectName As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_sObjectName As Long
  ' Call XPRS.DLL
  rval = XPRSgetobjecttypename_C(obj, c_sObjectName)
  ' Performing any necessary type conversions on output arguments
  sObjectName = StringFromPointer(c_sObjectName)
  XPRSgetobjecttypename = rval
End Function



Function XPRS_ge_getlasterror(ByRef iMsgCode As Long, ByRef msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRS_ge_getlasterror_C(iMsgCode, c_msg, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRS_ge_getlasterror = rval
End Function



Function XPRS_msp_getsolname(ByVal msp As Long, ByVal iSolutionId As Long, ByRef sname As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long, ByRef iSolutionIdStatus_ As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_sname As String * 255
  ' Call XPRS.DLL
  rval = XPRS_msp_getsolname_C(msp, iSolutionId, c_sname, iStringBufferBytes, iBytesInInternalString, iSolutionIdStatus_)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_sname, vbNullChar) > 1 Then
    sname = Left(c_sname, InStr(1, c_sname, vbNullChar) - 1)
  End If
  XPRS_msp_getsolname = rval
End Function


Function XPRS_msp_getlasterror(ByVal msp As Long, ByRef iMsgCode As Long, ByRef msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRS_msp_getlasterror_C(msp, iMsgCode, c_msg, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRS_msp_getlasterror = rval
End Function



Function XPRS_nml_getlasterror(ByVal nml As Long, ByRef iMsgCode As Long, ByRef msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRS_nml_getlasterror_C(nml, iMsgCode, c_msg, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRS_nml_getlasterror = rval
End Function



Function XPRS_mse_getlasterror(ByVal mse As Long, ByRef iMsgCode As Long, ByRef msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRS_mse_getlasterror_C(mse, iMsgCode, c_msg, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRS_mse_getlasterror = rval
End Function


Function XPRS_mse_getsolbasename(ByVal mse As Long, ByRef sname As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_sname As String * 255
  ' Call XPRS.DLL
  rval = XPRS_mse_getsolbasename_C(mse, c_sname, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_sname, vbNullChar) > 1 Then
    sname = Left(c_sname, InStr(1, c_sname, vbNullChar) - 1)
  End If
  XPRS_mse_getsolbasename = rval
End Function



Function XPRS_bo_getlasterror(ByVal obranch As Long, ByRef iMsgCode As Long, ByRef msg As String, ByVal iStringBufferBytes As Long, ByRef iBytesInInternalString As Long) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRS_bo_getlasterror_C(obranch, iMsgCode, c_msg, iStringBufferBytes, iBytesInInternalString)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRS_bo_getlasterror = rval
End Function




Function XPRSgetlicerrmsg(ByRef msg As String) As Long
  Dim rval As Long
  ' Performing any necessary type conversions on input arguments
  Dim c_msg As String * 255
  ' Call XPRS.DLL
  rval = XPRSgetlicerrmsg_C(c_msg, 255)
  ' Performing any necessary type conversions on output arguments
  If InStr(1, c_msg, vbNullChar) > 1 Then
    msg = Left(c_msg, InStr(1, c_msg, vbNullChar) - 1)
  End If
  XPRSgetlicerrmsg = rval
End Function

Function XPRSgetbannerVB(ByRef banner As String) As Long
  XPRSgetbannerVB = XPRSgetbanner(banner)
End Function

Function XPRSgetversionVB(ByRef version As String) As Long
  XPRSgetversionVB = XPRSgetversion(version)
End Function

Function XPRSlicenseVB(ByRef i1 As Long, ByRef c1 As String) As Long
  XPRSlicenseVB = XPRSlicense(i1, c1)
End Function

Function XPRSgetlicerrmsgVB(ByRef msg As String) As Long
  XPRSgetlicerrmsgVB = XPRSgetlicerrmsg(msg)
End Function

Function XPRSgetprobnameVB(ByVal prob As Long, ByRef svalue As String) As Long
  XPRSgetprobnameVB = XPRSgetprobname(prob, svalue)
End Function

Function XPRSgetstrcontrolVB(ByVal prob As Long , ByVal index As Long, ByRef svalue As String) As Long
  XPRSgetstrcontrolVB = XPRSgetstrcontrol(prob, index, svalue)
End Function

Function XPRSgetstrattribVB(ByVal prob As Long, ByVal index As Long, ByRef cvalue As String) As Long
  XPRSgetstrattribVB = XPRSgetstrattrib(prob, index, cvalue)
End Function

Function XPRSgetlasterrorVB(ByVal prob As Long, ByRef errmsg As String) As Long
  XPRSgetlasterrorVB = XPRSgetlasterror(prob, errmsg)
End Function







#End If
