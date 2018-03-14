/************************/
/*         Mosel        */
/*                      */
/* File  mosel_dso.h    */
/************************/
/* (c) Copyright Fair Isaac Corporation 2001-2014. All rights reserved. */

struct Vimactx;
union Alltypes;

#ifdef MM_NICOMPAT
 #if MM_NICOMPAT < 3000000 || MM_NICOMPAT > MM_VERSNUM
  #error "Compatibility version not supported"
 #elif MM_NICOMPAT < 3002000
  #define MM_NIVERS	5005
 #elif MM_NICOMPAT < 3003000
  #define MM_NIVERS	5007
 #elif MM_NICOMPAT < 3004002
  #define MM_NIVERS	5008
 #elif MM_NICOMPAT < 3005003
  #define MM_NIVERS	5009
 #elif MM_NICOMPAT < 3007000
  #define MM_NIVERS	5010
 #elif MM_NICOMPAT < 3009000
  #define MM_NIVERS	5011
 #endif
#endif

						/* Interface version */
#ifndef MM_NIVERS
#define MM_NIVERS	5012
#endif
						/* Make module version */
#define MM_MKVER(M,m,r) ((r)+(m)*1000+(M)*1000000)
						/* Make module priority */
#define MM_MKPRIORITY(p) ((void*)((size_t)(p)))
							/* Services codes */
#define MM_SRV_PARAM	0		/* Encode a parameter */
				/* int findparm(char *,int *,int,void*,void*) */
#define MM_SRV_RESET	1		/* Reset a DSo for a run */
				/* void *reset(ctx, void *,int) */
#define MM_SRV_CHKVER	2		/* Check version numbers */
				/* int chkvers(int) */
#define MM_SRV_UNLOAD	3		/* Before unloading the DSO */
				/* void unload(void) */
#define MM_SRV_PARLST	4		/* Enumerate the parameter names */
		/* void *nextpar(void *,const char **,const char **,int *) */
#define MM_SRV_IMCI	5	/* Inter-Module Communication Interface */
				/* void * */
#define MM_SRV_DEPLST	6		/* Module dependency list */
				/* const char ** */
#define MM_SRV_CERTIF	7		/* Module certification */
				/* void certif(void *,void *) */
#define MM_SRV_IODRVS	8		/* table of IO drivers */
				/* mm_iodrvtab * */
#define MM_SRV_ONEXIT	9		/* End of execution of the model */
				/* void onexit(mm_context,void *,int) */
#define MM_SRV_CHKRES	10		/* Check restrictions */
				/* int chkres(int) */
#define MM_SRV_PRIORITY	11		/* Set module priority level (int) */
				/* MM_MKPRIORITY(level) */
#define MM_SRV_UPDVERS	12		/* update version number (compilation)*/
				/* void updvers(int,int,int*) */
#define MM_SRV_IMPLST	13		/* implied dependency list */
				/* const char ** */
#define MM_SRV_ANNOT	14		/* Annotation definitions */
				/* const char ** */

typedef int (*mm_srv_findparm)(const char *,int *,int,struct Vimactx*,void*);
					

#define MM_FCT_GETPAR	0		/* Read a library parameter */
				/* {int,text,real} getpar(int) */
#define MM_FCT_SETPAR	1		/* Set a library parameter */
				/* void setpar(int,{int,text,real}) */

                                        /* updvers event types */
#define MM_UPDV_INIT    0       /* module just loaded for compilation */
#define MM_UPDV_FUNC    1       /* procedure/function number */
#define MM_UPDV_TYPE    2       /* type code */
#define MM_UPDV_GPAR    3       /* 'getparam' on parameter number */
#define MM_UPDV_SPAR    4       /* 'setparam' on parameter number */
#define MM_UPDV_ENDP    5       /* end of parsing */

					/* 'findparm': values of 'why' */
#define MM_FNDP_MCREAD	0	/* compilation: getparam */
#define MM_FNDP_MCWRITE	1	/* compilation: setparam */
#define MM_FNDP_RTWRITE	2	/* setparam via 'run' arguments */
#define MM_FNDP_NIREAD	3	/* 'getparam' from another module */
#define MM_FNDP_RTREAD	4	/* 'getparam' from RT library */

					/* Type properties */
#define MM_DTYP_PNCTX	1	/* 'tostring' does not use ctx */
#define MM_DTYP_RFCNT	2	/* create/delete supports reference count */
#define MM_DTYP_APPND	4	/* 'copy' supports 'append' (+=) */
#define MM_DTYP_ORSET	8	/* 'copy' supports 'reset' only */
#define MM_DTYP_PROB	16	/* type represents a problem */

				/* Decoding of last parameter of 'copy' fct */
#define MM_CPY_COPY	0
#define MM_CPY_RESET	(1<<12)
#define MM_CPY_APPEND	(2<<12)
#define MM_CPY(o)	((o)&0x1F000)

				/* Decoding of last parameter of 'compare' fct*/
#define MM_COMPARE_EQ	0
#define MM_COMPARE_NEQ	(1<<12)
#define MM_COMPARE(o)	((o)&0x1F000)
#define MM_COMPARE_ERROR	-13

				/* Control codes for IO drivers */
#define MM_IOCTRL_FCTS	-2
#define MM_IOCTRL_DSO	-1
#define MM_IOCTRL_NAME	0
#define MM_IOCTRL_OPEN	1
#define MM_IOCTRL_CLOSE	2
#define MM_IOCTRL_READ	3
#define MM_IOCTRL_WRITE	4
#define MM_IOCTRL_IFROM	5
#define MM_IOCTRL_ITO	6
#define MM_IOCTRL_INFO	7
#define MM_IOCTRL_ERROR	8
#define MM_IOCTRL_RM	9
#define MM_IOCTRL_MV	10
#define MM_IOCTRL_DUP	11
#define MM_IOCTRL_SKIP	12

			/* Type decoding for records in IO drivers */
#define MM_INIT_SHT_FLD		12
#define MM_INIT_SHT_SKIP	(MM_INIT_SHT_FLD+10)
#define MM_INIT_FLD(o)		(((o)>>MM_INIT_SHT_FLD)&1023)
#define MM_INIT_SKIP(o)		((o)>>MM_INIT_SHT_SKIP)
#define MM_INIT_MSK_SKIP	(1023u<<MM_INIT_SHT_SKIP)

typedef struct Dsoconst
	{
	 char *name;
	 int type;
	 union
	 {
	  char *s;
	  int i;
	  double *d;
	 } val;
	} mm_dsoconst;

typedef struct Dsovimfct
	{
	 char *name;
	 int code;
	 int type;
	 int nbpar;
	 char *typpar;
	 int (*vimfct)(struct Vimactx *,void *);
	} mm_dsofct;

typedef struct Dsotype
	{
	 char *name;
	 int code;
	 int props;
	 void *(*create)(struct Vimactx *,void *,void *,int);
	 void (*fdelete)(struct Vimactx *,void *,void *,int);
	 int (*tostring)(struct Vimactx *,void *,void *,char *,int,int);
	 int (*fromstring)(struct Vimactx *,void *,void *,const char *,int,const char **);
	 int (*copy)(struct Vimactx *,void *,void *,void *,int);
	 int (*compare)(struct Vimactx *,void *,void *,void *,int);
	} mm_dsotyp;

typedef struct
	{
	 int code;
	 void *fct;
	} mm_dsoserv;

typedef struct Dsointer
	{
	 int nbconst;
	 mm_dsoconst *tabconst;
	 int nbvmfct;
	 mm_dsofct *tabvmfct;
	 int nbtypes;
	 mm_dsotyp *tabtypes;
	 int nbserv;
	 mm_dsoserv *tabserv;
	} mm_dsointer;

typedef struct
	{
	 int code;
	 void *fct;
	} mm_iofcttab;

typedef struct
	{
	 const char *name;
	 mm_iofcttab *fcttab;
	} mm_iodrvtab;

				/* Functions for IO drivers */
typedef void *(*mm_iodrv_open)(struct Vimactx *,int *mode,const char *filename);
typedef int (*mm_iodrv_close)(struct Vimactx *,void *fd,int mode);
typedef long (*mm_iodrv_sync)(struct Vimactx *,void *fd,void *buffer,unsigned long size);
typedef int (*mm_iodrv_skip)(struct Vimactx *,void *fd,int size);
typedef int (*mm_iodrv_err)(struct Vimactx *,void *fd,char *msg,unsigned long size);
typedef int (*mm_iodrv_initfrom)(struct Vimactx *,void *fd,int nbrec,const char **labels,int *types,union Alltypes **adrs,int *nbread);
typedef int (*mm_iodrv_initto)(struct Vimactx *,void *fd,int nbrec,const char **labels,int *types,union Alltypes **adrs);
typedef int (*mm_iodrv_rm)(struct Vimactx *,const char *);
typedef int (*mm_iodrv_mv)(struct Vimactx *,const char *,const char *);


typedef struct Iodrvlst* mm_iodrv;
typedef struct Mosel_ni* mm_nifct;
typedef struct Mosel_nix* mm_nixfct;

