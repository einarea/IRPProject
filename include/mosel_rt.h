/************************/
/*         Mosel        */
/*                      */
/* File  mosel_rt.h     */
/************************/
/* (c) Copyright Fair Isaac Corporation 2001-2014. All rights reserved */

#ifndef _MOSEL_RT_H
#define _MOSEL_RT_H

#define MM_VERSION "3.10.0"
#define MM_VERSNUM 3010000

#if defined(_WIN32) || defined(_WIN64)
#define MM_RTC __stdcall
#else
#define MM_RTC
#endif

			/* Possible types */
#define MM_TYP_NOT	0
#define MM_TYP_INT	1
#define MM_TYP_REAL	2
#define MM_TYP_STRING	3
#define MM_TYP_BOOL	4
#define MM_TYP_MPVAR	5
#define MM_TYP_LINCTR	6
#define MM_TYP_EXTN	0xFFF

			/* Possible structures */
#define MM_STR_CONST 0
#define MM_STR_REF   (1<<12)
#define MM_STR_ARR   (2<<12)
#define MM_STR_SET   (3<<12)
#define MM_STR_LIST  (4<<12)
#define MM_STR_PROC  (5<<12)
#define MM_STR_MEM   (7<<12)
#define MM_STR_UTYP  (8<<12)
#define MM_STR_NTYP  (9<<12)
#define MM_STR_REC   (10<<12)
#define MM_STR_PROB  (11<<12)

			/* Possible set/array structure */
#define MM_GRP_FIX   0
#define MM_GRP_DYN   (1<<17)
#define MM_GRP_RNG   0
#define MM_GRP_GEN   (1<<18)
#define MM_ARR_DENSE (MM_GRP_FIX|MM_GRP_RNG)
#define MM_ARR_SPARSE (MM_GRP_DYN|MM_GRP_GEN)

			/* DSO parameters coding read/write */
#define MM_CPAR_READ  MM_STR_REF
#define MM_CPAR_WRITE MM_STR_ARR

			/* DSO type properties encoding */
#define MM_MTP_CREAT 1
#define MM_MTP_DELET 2
#define MM_MTP_TOSTR 4
#define MM_MTP_FRSTR 8
#define MM_MTP_PRTBL 16
#define MM_MTP_RFCNT 32
#define MM_MTP_COPY  64
#define MM_MTP_APPND 128
#define MM_MTP_ORSET 256
#define MM_MTP_PROB  512
#define MM_MTP_CMP   1024

				/* Mask */
#define MM_MSK_TYP   0xFFF
#define MM_MSK_STR   0x1F000
#define MM_MSK_GRP   0x60000
#define MM_MSK_FIN   0x80000
#define MM_MSK_LOC   0xFFF00000

#define MM_TYP(o) ((o)&MM_MSK_TYP)	/* Object type */
#define MM_STR(o) ((o)&MM_MSK_STR)	/* Object structure */
#define MM_GRP(o) ((o)&MM_MSK_GRP)	/* Object storage class [set|array] */
#define MM_IS_PUBLIC(o) (((o)&MM_MSK_LOC)==0)

						/* Problem status */
#define MM_PBSOL	1	/* A solution is available */
				/* bits 2-4 are the result of an optimisation */
#define MM_PBOPT	2	/* Optimal solution found */
#define MM_PBUNF	4	/* Optimisation unfinished */
#define MM_PBINF	(2|4)	/* Problem infeasible */
#define MM_PBUNB	8	/* Problem is unbounded */
#define MM_PBOTH	(8|2)	/* Optimisation failed (cutoff) */
#define MM_PBMOD	16	/* modified since last matrix generation */
#define MM_PBOMOD	32	/* objective modified since last generation */

#define MM_PBRES	(2|4|8)

#define MM_PBCHG	16	/* modified since last matrix generation */
#define MM_PBOBJ	32	/* objective modified since last generation */

					/* Returned values from "runmodel" */
#define MM_RT_OK	0
#define MM_RT_INSTR	1	/* Invalid instruction */
#define MM_RT_ENDING	2	/* Normal termination (debugger only) */
#define MM_RT_MATHERR	3	/* Math error */
#define MM_RT_UNKN_PF	5	/* Call to an unknown procedure/function */
#define MM_RT_UNKN_SYS	9	/* Call to an unknown system function */
#define MM_RT_PROB	10	/* Error when opening/closing a problem */
#define MM_RT_ERROR	11	/* Runtime Error code */
#define MM_RT_EXIT	12	/* Termination via exit(code) [callproc only] */
#define MM_RT_IOERR	13	/* IO Error code */
#define MM_RT_BREAK	14	/* Stopped on a breakpoint (debugger only) */
#define MM_RT_NIFCT	15	/* Stopped in native function (debugger only) */
#define MM_RT_NULL	16	/* NULL reference */
#define MM_RT_LICERR	17	/* License error */
#define MM_RT_STOP	128	/* Stopped */

					/* Return values for debugger */
#define MM_DBG_STOP	-3
#define MM_DBG_NEXT	-2
#define MM_DBG_STEP	-1
#define MM_DBG_CONT	0
					/* Monitor events */
#define MM_EVT_LOAD	1
#define MM_EVT_UNLOAD	2
#define MM_EVT_RUN	3
#define MM_EVT_ENDRUN	4

					/* Boolean constants */
#define MM_TRUE		1
#define MM_FALSE	0

					/* Constants for the IO module */
#define MM_F_TEXT	0
#define MM_F_BINARY	1
#define MM_F_READ	0
#define MM_F_INPUT	0
#define MM_F_WRITE	2
#define MM_F_OUTPUT	2
#define MM_F_APPEND	4
#define MM_F_ERROR	8
#define MM_F_LINBUF	16
#define MM_F_INIT	32
#define MM_F_SILENT	64
#define MM_F_IOERR	128
#define MM_F_DELCLOSE	256

					/* Properties for get[mod|dso]prop */
#define MM_PROP_NAME	0
#define MM_PROP_ID	1
#define MM_PROP_VERSION	2
#define MM_PROP_SYSCOM	3
#define MM_PROP_USRCOM	4
#define MM_PROP_SIZE	5
#define MM_PROP_NBREF	6
#define MM_PROP_DATE	7
#define MM_PROP_PATH	8
#define MM_PROP_IMCI	9
#define MM_PROP_PRIORITY 10
#define MM_PROP_SECSTAT 11
#define MM_PROP_SKEYFP  12
					/* Model security status decoding */
#define MM_SECSTAT_CRYPTED    1
#define MM_SECSTAT_SIGNED     2
#define MM_SECSTAT_VERIFIED   4
#define MM_SECSTAT_UNVERIFIED 8

					/* Properties for gettypeprop */
#define MM_TPROP_NAME	0
#define MM_TPROP_FEAT	1
#define MM_TPROP_EXP	2
#define MM_TPROP_PBID	3
					/* Control characters for CB init */
#define MM_CBC_SKIP	0
#define MM_CBC_OPENLST  1
#define MM_CBC_CLOSELST 2
#define MM_CBC_OPENNDX  3
#define MM_CBC_CLOSENDX 4

#define MM_KEEPOBJ	((void *)1l)	/* For 'exportprob' */

					/******* Restrictions *******/
#define MM_RESTR_NOWRITE 1	/* no write */
#define MM_RESTR_NOREAD  2	/* no read (=> nowrite) */
#define MM_RESTR_NOEXEC  4	/* no command execution */
#define MM_RESTR_WDONLY  8	/* access only to current workdir */
#define MM_RESTR_NOTMP   16	/* no tmp dir */
#define MM_RESTR_NODB    32	/* no DB access */
#define MM_RESTR_FAC     64	/* running from FAC */

					/******* Time functions *******/
#define MM_TIME_UTC   ((int*)0)
#define MM_TIME_LOCAL ((int*)1)
					/******* Maximum path len *******/
#define MM_MAXPATHLEN 1024
					/******* Restrictions check *******/
#define MM_RCHK_READ	0	/* == MM_F_READ */
#define MM_RCHK_WRITE	2	/* == MM_F_WRITE */
#define MM_RCHK_NOCHK	1
#define MM_RCHK_IODRV	4

typedef struct Model* mm_model;
typedef struct CBInit* mm_cbinit;
typedef struct Dsolist* mm_dsolib;

typedef int mm_integer;
typedef double mm_real;
typedef int mm_boolean;
typedef const char* mm_string;
typedef struct Lpvar* mm_mpvar;
typedef struct Lpctr* mm_linctr;
typedef struct Array* mm_array;
typedef union SetR* mm_set;
typedef struct List* mm_list;
typedef void* mm_ref;
typedef struct Htentry* mm_proc;
typedef struct Memblk
	{
	 void *ref;
	 size_t size;
	} mm_memblk;

typedef union Alltypes		/** AllTypes union **/
	{
	 mm_integer	integer;
	 mm_real	real;
	 mm_boolean	boolean;
	 mm_string	string;
	 mm_mpvar	mpvar;
	 mm_linctr	linctr;
	 mm_set		set;
	 mm_list	list;
	 mm_array	array;
	 mm_ref		ref;
	 mm_proc	proc;
	 mm_memblk	*memblk;
	 size_t		size;
	} mm_alltypes;

typedef struct
	{
	 mm_model model;
	 int (MM_RTC *dbgcb)(void *,int,int);
	 void *dbgctx;
	 int rts;
	} mm_evtd_run;

/************************** Prototypes ****************************/
#ifndef _MOSEL_NI_H
#ifdef __cplusplus
extern "C" {
#endif
struct Vimactx;

const char * MM_RTC XPRMgetlibpath(void);
void MM_RTC XPRMfreelibpath(void);
							/* Model functions */
mm_model MM_RTC XPRMfindmod(const char *name,int number);
mm_model MM_RTC XPRMgetnextmod(mm_model model);
void MM_RTC XPRMsetmonitor(mm_model model,int (MM_RTC *monitor)(void *mctx,int evt, void *data,size_t datasize),void *monctx);
int MM_RTC XPRMrunmod(mm_model model,int *returned, const char *parlist);
void MM_RTC XPRMresetmod(mm_model model);
int MM_RTC XPRMisrunmod(mm_model model);
void MM_RTC XPRMstoprunmod(mm_model model);
void MM_RTC XPRMtermrunmod(mm_model model);
int MM_RTC XPRMgetmodprop(mm_model model,int what,mm_alltypes *result);
mm_model MM_RTC XPRMloadmod(const char *bname,const char *intname);
mm_model MM_RTC XPRMloadmodsec(const char *bname,const char *intname,const char *flags,const char *passfile,const char *privkey,const char *keys);
mm_model mmloadmod(struct Vimactx *ctx,const char *bname,const char *intname,const char *options,const char *passfile,const char *privkey,const char *keys);
int MM_RTC XPRMunloadmod(mm_model model);
int mmunloadmod(struct Vimactx *ctx,mm_model model);
							/* Debugger functions */
int MM_RTC XPRMdbg_runmod(mm_model model,int *returned, const char *parlist,
	int (MM_RTC *dbgcb)(void *dctx,int vmstat, int lindex),void *dbgctx);
void MM_RTC XPRMdbg_getlndx(mm_model model,int *nbl,int *lines,int *nbf, const char **files);
int MM_RTC XPRMdbg_getnblndx(mm_model model);
int MM_RTC XPRMdbg_clearbrkp(mm_model model,int lindex);
int MM_RTC XPRMdbg_setbrkp(mm_model model,int lindex);
int MM_RTC XPRMdbg_setstacklev(mm_model model,int level);
int MM_RTC XPRMdbg_getlocation(mm_model model,int lindex,int *line,
							const char **filename);
int MM_RTC XPRMdbg_findproclndx(mm_model model,mm_proc proc);

							/* Dictionary access */
int MM_RTC XPRMfindident(mm_model model,const char *text,mm_alltypes *value);
const char * MM_RTC XPRMgetnextident(mm_model model, void **ref);
const char * MM_RTC XPRMgetnextanident(mm_model model, void **ref);
int MM_RTC XPRMgetannotations(mm_model model, const char *ident, const char *prefix, const char **ann, int maxann);
const char * MM_RTC XPRMgetnextparam(mm_model model, void **ref);
void * MM_RTC XPRMgetnextdep(mm_model model, void *ref, const char **name,
						int *version, int *dso_pkg);
void * MM_RTC XPRMgetnextreq(mm_model model, void *ref,const char **name,
						int *type,void **data);
void * MM_RTC XPRMgetnextpbcomp(mm_model model, void *ref,int code,int *type);
int MM_RTC XPRMselectprob(mm_model model,int code,void *pb);
						/* Procedures/functions */
int MM_RTC XPRMgetprocinfo(mm_proc proc,const char **partyp,int *nbpar,int *type);
mm_proc MM_RTC XPRMgetnextproc(mm_proc proc);
							/* Set access */
int MM_RTC XPRMgetsetsize(mm_set set);
int MM_RTC XPRMgetsettype(mm_set set);
int MM_RTC XPRMgetfirstsetndx(mm_set set);
int MM_RTC XPRMgetlastsetndx(mm_set set);
mm_alltypes* MM_RTC XPRMgetelsetval(mm_set set, int ndx,mm_alltypes *value);
int MM_RTC XPRMgetelsetndx(mm_model model, mm_set set, mm_alltypes *value);
							/* List access */
int MM_RTC XPRMgetlistsize(mm_list list);
int MM_RTC XPRMgetlisttype(mm_list list);
void* MM_RTC XPRMgetnextlistelt(mm_list list,void *ref,int *type,mm_alltypes *value);
void* MM_RTC XPRMgetprevlistelt(mm_list list,void *ref,int *type,mm_alltypes *value);
							/* Array access */
int MM_RTC XPRMgetarrval(mm_array tab, int indices[], void *adr);
void MM_RTC XPRMgetarrsets(mm_array tab,mm_set sets[]);
int MM_RTC XPRMgetarrdim(mm_array tab);
int MM_RTC XPRMgetarrtype(mm_array tab);
int MM_RTC XPRMgetarrsize(mm_array tab);
int MM_RTC XPRMgetfirstarrentry(mm_array tab,int indices[]);
int MM_RTC XPRMgetfirstarrtruentry(mm_array tab,int indices[]);
int MM_RTC XPRMgetlastarrentry(mm_array tab,int indices[]);
int MM_RTC XPRMgetnextarrentry(mm_array tab,int indices[]);
int MM_RTC XPRMgetnextarrtruentry(mm_array tab,int indices[]);
int MM_RTC XPRMchkarrind(mm_array tab,const int indices[]);
int MM_RTC XPRMcmpindices(int nbdim,const int ind1[],const int ind2[]);
							/* Record access */
void* MM_RTC XPRMgetnextfield(mm_model model, void *ref,int code,const char **name, int *type,int *number);
void MM_RTC XPRMgetfieldval(mm_model model, int code, void *ref, int num,mm_alltypes *value);
							/* Solution */
int MM_RTC XPRMgetvarnum(mm_model model, mm_mpvar var);
int MM_RTC XPRMgetctrnum(mm_model model, mm_linctr ctr);
int MM_RTC XPRMgetprobstat(mm_model model);
double MM_RTC XPRMgetobjval(mm_model model);
double MM_RTC XPRMgetvsol(mm_model model, mm_mpvar var);
double MM_RTC XPRMgetcsol(mm_model model, mm_linctr ctr);
double MM_RTC XPRMgetrcost(mm_model model, mm_mpvar var);
double MM_RTC XPRMgetdual(mm_model model, mm_linctr ctr);
double MM_RTC XPRMgetslack(mm_model model, mm_linctr ctr);
double MM_RTC XPRMgetact(mm_model model, mm_linctr ctr);
							/* CB IO driver */
int MM_RTC XPRMcb_sendint(mm_cbinit cb,int i,int flush);
int MM_RTC XPRMcb_sendreal(mm_cbinit cb,double d,int flush);
int MM_RTC XPRMcb_sendstring(mm_cbinit cb,const char *text,int len,int flush);
int MM_RTC XPRMcb_sendctrl(mm_cbinit cb,unsigned char ctrl,int flush);
							/* Miscellaneous */
int MM_RTC XPRMinit(void);
int MM_RTC XPRMfinish(void);
int MM_RTC XPRMsetrestrictions(unsigned int restr);
int MM_RTC XPRMgetlicerrmsg(char *msg,int len);
int MM_RTC XPRMlicense(int *oemnum, char *oemmsg);
int MM_RTC XPRMbeginlicensing(int *haslic);
int MM_RTC XPRMendlicensing();
const char * MM_RTC XPRMgetlicinfo(void);
void MM_RTC XPRMsetlocaledir(const char *path);
const char * MM_RTC XPRMgetlocaledir(void);
void MM_RTC XPRMsetdefworkdir(const char *path);
const char * MM_RTC XPRMgetdefworkdir(void);
int MM_RTC XPRMremovetmpdir(void);
int MM_RTC XPRMsetdefstream(mm_model model,int wmd,const char *name);
void MM_RTC XPRMsetonexit(void (*onexit)(int,char *));
const char * MM_RTC XPRMgetversion(void);
int MM_RTC XPRMgetversions(int whichone);
int MM_RTC XPRMexportprob(mm_model model, const char *options,const char *fname, mm_linctr obj);
int MM_RTC XPRMdate2jdn(int y,int m, int d);
void MM_RTC XPRMjdn2date(int jd,int *y,int *m, int *d);
void MM_RTC XPRMtime(int *jdn,int *t,int *utc);
int MM_RTC XPRMpathcheck(const char *str,char *path,int rlen,int acc);
int MM_RTC XPRMfreememblk(mm_memblk*);

struct Mosel_ni;
struct Dsointer;
int MM_RTC XPRMregstatdso(const char *name,int (*dsoinit)(struct Mosel_ni *,int *,int *,struct Dsointer **));
mm_dsolib MM_RTC XPRMpreloaddso(const char *name);
void * MM_RTC XPRMgetnextdsoproc(mm_dsolib dso,void *ref,const char **name,
				const char **partyp,int *nbpar, int *type);
void * MM_RTC XPRMgetnextdsodep(mm_dsolib dso, void *ref,const char **name);
void * MM_RTC XPRMgetnextdsoconst(mm_dsolib dso,void *ref,const char **name,
						int *type, mm_alltypes *value);
void * MM_RTC XPRMgetnextdsotype(mm_dsolib dso,void *ref,const char **name, unsigned int *flag);
void * MM_RTC XPRMgetnextdsoparam(mm_dsolib dso,void *ref,const char **name,
						const char **desc,int *type);
int MM_RTC XPRMgetdsoannotations(mm_dsolib dso, const char *prefix, const char **ann, int maxann);
int MM_RTC XPRMgetdsoparam(mm_model model,mm_dsolib dso, const char *name,
						int *type, mm_alltypes *value);
int MM_RTC XPRMgettypeprop(mm_model model, int ntyp, int what,
							mm_alltypes *value);
int MM_RTC XPRMfindtypecode(mm_model model, const char *typenme);
int MM_RTC XPRMdsotyptostr(mm_model model,int code, void *value,
							char *str, int size);
void MM_RTC XPRMautounloaddso(int yesno);
void MM_RTC XPRMflushdso(void);
mm_dsolib MM_RTC XPRMgetnextdso(mm_dsolib dso);
mm_dsolib MM_RTC XPRMfinddso(const char *name);
void MM_RTC XPRMsetdsopath(const char *path);
int MM_RTC XPRMgetdsopath(char *path,int len);
int MM_RTC XPRMgetdsoprop(mm_dsolib dso,int what,mm_alltypes *result);
void* MM_RTC XPRMgetnextiodrv(void *prev,const char **name,const char **module,const char **info);

#ifdef __cplusplus
}
#endif
#endif

#endif
