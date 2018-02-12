/************************/
/*         Mosel        */
/*                      */
/* File  mmhttp.h       */
/************************/
/* (c) Copyright Fair Isaac Corporation 2014. All rights reserved */

#ifndef _MMHTTP_H
#define _MMHTTP_H

#define HTTP_GET      1
#define HTTP_POST     2
#define HTTP_PUT      4
#define HTTP_DELETE   8

struct Hctx;
typedef struct Mmhttp_imci
	{
	 int (*httpsend)(struct Vimactx *ctx,struct Hctx *hctx,int *status,int qtype,const char *query,const char *querydatafile,const char *resultfile,const char *extrahdr);
	} *mmhttp_imci;
#endif
