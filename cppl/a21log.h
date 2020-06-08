/*	Revision:		39
*/

#ifndef	A21LOG_H_INC
#define	A21LOG_H_INC

/*

	a21log.h		qad-ARC21 A21LOG.DLL Utility Header
				Copyright (C) 1994-1995, Contahal Ltd.

*/

#include	"a21logt.h"

#if	UINTFC==UINTFC_MSW
#include	<windows.h>
#endif

/*
	stdarg.h included to allow variable number
		 of argument calling conventions.
*/
#include	<stdarg.h>

#ifdef	__cplusplus
extern "C" {
#endif

#if	UINTFC==UINTFC_MSW
HA21LOG FAR PASCAL a21log_open(LPCSTR, int, int) ;
BOOL FAR PASCAL a21log_close(int) ;
BOOL FAR PASCAL a21log_write(LPA21LMID, LPCSTR, int) ;
BOOL FAR PASCAL a21log_puts(LPA21LMID, LPCSTR) ;
BOOL FAR PASCAL a21log_putmodulename(LPA21LMID, HINSTANCE) ;
BOOL FAR PASCAL a21log_tabto(LPA21LMID, int) ;
BOOL FAR PASCAL a21log_setlmgn(LPA21LMID, int) ;
int FAR _cdecl a21log_vprintf(LPA21LMID, LPCSTR, va_list) ;
int FAR _cdecl a21log_printf(LPA21LMID, LPCSTR, ...) ;
#endif

#ifdef	__cplusplus
/* extern "C" */
}
#endif

/* #ifndef A21LOG_H_INC */
#endif

/* end of a21log.h */
