/************************/
/*         Mosel        */
/*                      */
/* File  mmxprs.h       */
/************************/
/* (c) Copyright Fair Isaac Corporation 2001-2011. All rights reserved */

#ifndef _MMXPRS_H
#define _MMXPRS_H

struct Xprsctx;
struct Mpsol;

typedef struct Mmxprs_imci
	{
	 void (*loadcbks_deprec)(struct Vimactx *,struct Xprsctx *);
	 int (*syncstat)(struct Vimactx *,struct Xprsctx *);
	 void (*loadcbks)(struct Vimactx *,struct Xprsctx *,int unload);
	 void (*setverbose)(struct Vimactx *,struct Xprsctx *,int verb);
         void *(*getxprspb)(struct Vimactx *,struct Xprsctx *,void *pbref);
	 int  (*getmpsol)(struct Vimactx *,struct Mpsol*,int ndx,struct Lpvar**,double *);
	} *mmxprs_imci;
#endif
