/************************/
/*         Mosel        */
/*                      */
/* File  mmsystem.h     */
/************************/
/* (c) Copyright Fair Isaac Corporation 2001-2011. All rights reserved */

#ifndef _MMSYSTEM_H
#define _MMSYSTEM_H

struct Sysctx;

typedef struct Mmsystem_imci
	{
	 int (*setenv)(struct Vimactx *ctx,struct Sysctx *sysctx,const char *name,const char *value);
	 const char *(*getenv)(struct Vimactx *ctx,struct Sysctx *sysctx,const char *name);
	 int (*system)(struct Vimactx *ctx,struct Sysctx *sysctx,const char *cmd);
	 int (*gettxtsize)(struct Vimactx *ctx,struct Sysctx *sysctx,void *txt);
	 char *(*gettxtbuf)(struct Vimactx *ctx,struct Sysctx *sysctx,void *txt);
	 char *(*txtresize)(struct Vimactx *ctx,struct Sysctx *sysctx,void *txt,int size);
#ifdef _WIN32
	 char *(*envproc)(struct Vimactx *ctx,struct Sysctx *sysctx,char *todel);
#else
	 void (*envfork)(struct Vimactx *ctx,struct Sysctx *sysctx);
#endif
	} *mmsystem_imci;
#endif
