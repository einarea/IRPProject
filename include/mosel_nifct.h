/************************/
/*         Mosel        */
/*                      */
/* File  mosel_nifct.h  */
/************************/
/* (c) Copyright Fair Isaac Corporation 2001-2013. All rights reserved */

struct Mosel_ni {
							/* Execution */
int (*getparam)(struct Vimactx *ctx,int parnum,union Alltypes *value);
int (*callproc)(struct Vimactx *ctx,struct Htentry *proc, union Alltypes *parst);
int (*calldsofct)(struct Vimactx *ctx,struct Dsolist *dso,int code);
void (*stoprun)(struct Vimactx *ctx);
							/* Set access */
int (MM_RTC *getsetsize)(union SetR *set);
int (MM_RTC *getsettype)(union SetR *set);
int (MM_RTC *getfirstsetndx)(union SetR *set);
int (MM_RTC *getlastsetndx)(union SetR *set);
int (*resetset)(struct Vimactx *ctx,union SetR *set);
int (*getelsetndx)(struct Vimactx *ctx,union SetR *set,union Alltypes *value);
int (*isinset)(struct Vimactx *ctx,union SetR *set,union Alltypes *value);
int (*addelset)(struct Vimactx *ctx,union SetR *set,union Alltypes *value, int *ndx);

							/* Table access */
int (MM_RTC *getarrval)(struct Array *tab,const int indices[], void *adr);
void (MM_RTC *getarrsets)(struct Array *tab,union SetR *sets[]);
int (MM_RTC *getarrdim)(struct Array *tab);
int (MM_RTC *getarrtype)(struct Array *tab);
int (MM_RTC *getarrsize)(struct Array *tab);
int (MM_RTC *getfirstarrentry)(struct Array *tab,int indices[]);
int (MM_RTC *getfirstarrtruentry)(struct Array *tab,int indices[]);
int (MM_RTC *getlastarrentry)(struct Array *tab,int indices[]);
int (MM_RTC *getnextarrentry)(struct Array *tab,int indices[]);
int (MM_RTC *getnextarrtruentry)(struct Array *tab,int indices[]);
int (MM_RTC *chkarrind)(struct Array *tab,const int indices[]);
int (MM_RTC *cmpindices)(int nbdim,const int ind1[],const int ind2[]);
int (*setarrval)(struct Vimactx *ctx,struct Array *tab,const int indices[],union Alltypes *val);
int (*setarrvalint)(struct Vimactx *ctx,struct Array *tab,const int indices[],int val);
int (*setarrvalreal)(struct Vimactx *ctx,struct Array *tab,const int indices[],double val);
int (*setarrvalstr)(struct Vimactx *ctx,struct Array *tab,const int indices[],const char *val);
int (*setarrvalbool)(struct Vimactx *ctx,struct Array *tab,const int indices[],int val);
							/* Problem/Solution */
int (*getprobstat)(struct Vimactx *ctx);
double (*getobjval)(struct Vimactx *ctx);
double (*getvsol)(struct Vimactx *ctx, struct Lpvar *var);
double (*getcsol)(struct Vimactx *ctx, struct Lpctr *ctr);
double (*getrcost)(struct Vimactx *ctx, struct Lpvar *var);
double (*getdual)(struct Vimactx *ctx, struct Lpctr *ctr);
double (*getslack)(struct Vimactx *ctx, struct Lpctr *ctr);
double (*getact)(struct Vimactx *ctx, struct Lpctr *ctr);
int (*getctrtyp)(struct Lpctr *ctr);
void* (*getctrnextterm)(struct Vimactx *ctx, struct Lpctr *ctr,void *prev,
					struct Lpvar **var,double *coeff);
void (*setctrhidden)(struct Vimactx *ctx,struct Lpctr *lpctr, int hide);
void *empty_4;
void *empty_5;
int (*exportprob)(struct Vimactx *ctx,const char *options,const char *fname, struct Lpctr *obj);
							/* Input/output */
int (*gettypeprop)(struct Vimactx *ctx,int code,int what,union Alltypes *result);
int (*fopen)(struct Vimactx *ctx,int flags,const char *fname);
int (*fclose)(struct Vimactx *ctx,int flag);
int (*fselect)(struct Vimactx *ctx,int flag);
int (*fgetid)(struct Vimactx *ctx,int flag);
int (*feof)(struct Vimactx *ctx);
int (*printf)(struct Vimactx *ctx,const char *format, ...);
char *(*fgets)(struct Vimactx *ctx,char *s,int size);
int (*fflush)(struct Vimactx *ctx);
							/* Dictionary */
int (*findident)(struct Vimactx *ctx,const char *text,union Alltypes *value);
const char* (*getnextident)(struct Vimactx *ctx, void **ref);
#if MM_NIVERS >= 5010
struct Dsolist *(*opendso)(struct Vimactx *ctx,const char *name, void **imci);
#else
void *unused_0;
#endif
int (MM_RTC *getprocinfo)(struct Htentry *proc,const char **partyp,int *nbpar,int *t);
struct Htentry * (MM_RTC *getnextproc)(struct Htentry *proc);
							/* Miscellaneous */
const char* (*regstring)(struct Vimactx *ctx,const char *string);
const char* (MM_RTC *getlocaledir)(void);
char* (*normfname)(char *fname, const char *ext, int force);
int (*setglobal)(struct Vimactx *ctx,const char *text,union Alltypes *value);
struct Dsolist * (MM_RTC *finddso)(const char *name);
#if MM_NIVERS >= 5010
void (*closedso)(struct Vimactx *ctx,const char *name);
#else
void *unused_1;
#endif
void **(*getdsoctx)(struct Vimactx *ctx,struct Dsolist *dso, void **imci);
int (*getdsoparam)(struct Vimactx *ctx, struct Dsolist *dso,
			const char *name, int *type,union Alltypes *value);
double (*getrand)(struct Vimactx *ctx);
int (MM_RTC *getversions)(int whichone);
void (*dispmsg)(struct  Vimactx*,const char *format,...);
union Alltypes* (*getelsetval)(struct Vimactx *ctx,union SetR *set, int ndx,union Alltypes *value);
int (*fgetinfo)(struct Vimactx *ctx,int *mode,int *line,int *col,const char **iodrv,const char **filename);
long (*fread)(struct Vimactx *ctx,void *where, long nb);
long (*fwrite)(struct Vimactx *ctx,const void *where, long nb);
int (*fremove)(struct Vimactx *ctx,const char *filename);
int (*fcopy)(struct Vimactx *ctx,const char *src,const char *dst);
int (*fmove)(struct Vimactx *ctx,const char *src,const char *dst);
int (*chkinterrupt)(struct Vimactx *ctx);
struct Vimactx *(*getcurrctx)(struct Model *model);
void (*mapset)(struct Vimactx *ctx,union SetR *ndx);
void (*unmapset)(struct Vimactx *ctx,union SetR *ndx);
void *(*newref)(struct Vimactx *ctx,int type,void *ref);
void (*delref)(struct Vimactx *ctx,int type,void *ref);
int (MM_RTC *getdsoprop)(struct Dsolist *dso,int what,union Alltypes *result);
int (MM_RTC *getlistsize)(struct List *list);
int (MM_RTC *getlisttype)(struct List *list);
void *(MM_RTC *getnextlistelt)(struct List *list,void *ref,int *type,union Alltypes *elt);
void *(MM_RTC *getprevlistelt)(struct List *list,void *ref,int *type,union Alltypes *elt);
int (*addellist)(struct Vimactx *ctx,struct List *list,int type,union Alltypes  *elt);
int (*insellist)(struct Vimactx *ctx,struct List *list,int type,union Alltypes  *elt);
int (*resetlist)(struct Vimactx *ctx,struct List *list);
void *(*getnextfield)(struct Vimactx *ctx,void *ref,int code,const char **name, int *type,int *number);
void (*getfieldval)(struct Vimactx *ctx, int code, void *ref, int num,union Alltypes *value);
int (*setfieldval)(struct Vimactx *ctx, int code, void *ref, int num,union Alltypes *value);
int (MM_RTC *date2jdn)(const int y,int m, const int d);
void (MM_RTC *jdn2date)(const int jd,int *y,int *m, int *d);
void (*time)(struct Vimactx *ctx, int *jdn,int *t,int *utc);
int (*dsotyptostr)(struct Vimactx *ctx,int code,void *val,char *str, int siz);
void *empty_3;
int (*copyval)(struct Vimactx *ctx,int code,void *dst,void *src);
int (*findtypecode)(struct Vimactx *ctx,const char *typenme);
int (*dsotypfromstr)(struct Vimactx *ctx,int code,void *dst,const char *src,const char **endptr);
int (*getvarnum)(struct Vimactx *ctx,struct Lpvar *var);
int (*getctrnum)(struct Vimactx *ctx,struct Lpctr *ctr);
void *(*getnextpbcomp)(struct Vimactx *ctx, void *ref,int code,int *type);
void (*fnlset)(struct Vimactx *ctx,union SetR *set);
int (*beginarrinit)(struct Vimactx *ctx,struct Array *tab);
int (*endarrinit)(struct Vimactx *ctx,struct Array *tab,int status);
#if MM_NIVERS >= 5007
int (*pathcheck)(struct Vimactx *ctx,const char *str,char *path,int rlen,int acc);
#if MM_NIVERS >= 5008
const char* (*getnextparam)(struct Vimactx *ctx, void **ref);
int (*fskip)(struct Vimactx *ctx,int nb);
#if MM_NIVERS >= 5009
struct Array* (*newarray)(struct Vimactx *ctx,int type, int nbdim, union SetR **sets);
void (*setioerrmsg)(struct Vimactx *ctx, const char *msg,int ecode);
#if MM_NIVERS >= 5012
int (*dbggetlocation)(struct Vimactx *ctx,int lref,int *line, const char **filename);
int (*buildnames)(struct Vimactx *ctx,int nbt,int *types,int (**fls)(struct Vimactx*,void*,int,void*),int (**snm)(struct Vimactx*,void*,int,void*,const char*),void **fctx);
struct MMhashmap *(*hmnew)(struct Vimactx *ctx,unsigned int minsize,int flags);
int (*hmdel)(struct Vimactx *ctx,struct MMhashmap *hm);
size_t (*hmset)(struct Vimactx *ctx,struct MMhashmap *hm,size_t key,size_t val,int how);
size_t (*hmget)(struct Vimactx *ctx,struct MMhashmap *hm,size_t key);
unsigned int (*hmdump)(struct Vimactx *ctx,struct MMhashmap *hm,size_t *keys,size_t *vals,unsigned int nbmax);
#endif
#endif
#endif
#endif
void *mmempty;					/* Some space for `NULL' */
};
