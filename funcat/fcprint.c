/*	Revision:	1
*/
/*

	fcprint.c		Funcat - Print Results
				Copyright (C) 1993, Contahal Ltd.

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"fc.h"

extern fc_opts fcopts ;
extern fc_fstate fcfs ;
extern fc_gstate fcgs ;

static void proc_chprint() ;
static void proc_chprint_fnline() ;

void proc_bufprint()
{
	char c ;
	register int i ;

	if (!FCO_SHOWSTATIC) {
		if (memcmp("static", FCB_BUF+FCS_POS_FN, 6) == 0) {
			return ;
		}
	}

	for (i= 0; i< FCB_IDX; i++) {
		c= FCB_BUF[i] ;
		if (c> ' ') break ;
	}

	if (FCO_LINES) {
		proc_chprint_fnline() ;
	}

	while(i< FCB_IDX) {
		proc_chprint(FCB_BUF[i++]) ;
	}

	proc_chprint('\n') ;
	proc_chprint('.') ;
	proc_chprint('.') ;
	proc_chprint('.') ;
	proc_chprint('\n') ;
	proc_chprint('\n') ;
}

static void proc_chprint(c)
int c ;
{
	static int tab_flag= 1 ;

	if (FCO_INDENT) {
		if (c == '\n') tab_flag= 1 ;
		else if (tab_flag) {
			FCF_PUTC('\t') ;
			tab_flag= 0 ;
		}
	}
	FCF_PUTC(c) ;
}

static void proc_chprint_fnline()
{
	char lno_buf[25] ;
	int i ;

	sprintf(lno_buf, "%d", (FCF_FNLINE+1)) ;
	for (i= 0; lno_buf[i] != '\0'; i++) {
		FCF_PUTC(lno_buf[i]) ;
	}
}
