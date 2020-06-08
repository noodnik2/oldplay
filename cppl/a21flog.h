/*	Revision:		16
*/

#ifndef	A21FLOG_H_INC
#define	A21FLOG_H_INC

/*

	a21flog.h		qad-ARC21 A21FLOG Utility Header
				Copyright (C) 1994-1995, Contahal Ltd.

*/

/*
	sub-#includes
*/
#include	"a21flogt.h"
#include	<stdarg.h>

/*
	function prototypes
*/

HA21LOG a21flog_open(M_SLPCSTR, int, int) ;
M_SBOOL a21flog_close(int) ;
M_SBOOL a21flog_write(LPA21LMID, M_SLPCSTR, int) ;
M_SBOOL a21flog_puts(LPA21LMID, M_SLPCSTR) ;
M_SBOOL a21flog_putmodulename(LPA21LMID, M_SPID) ;
M_SBOOL a21flog_vprintf(LPA21LMID, M_SLPCSTR, va_list) ;
M_SBOOL a21flog_printf(LPA21LMID, M_SLPCSTR, ...) ;
M_SBOOL a21flog_tabto(LPA21LMID lplmid, int column) ;
M_SBOOL a21flog_setlmgn(LPA21LMID lplmid, int lmgn) ;

/* #ifndef A21FLOG_H_INC */
#endif

/* end of a21flog.h */
