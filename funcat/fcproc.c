/*
	
	fcproc.c			funcat: Catalog C-Program functions
					Copyright (C) 1991, Contahal Ltd.


	THIS IS A VERY MESSY PROGRAM.  I'M SORRY.

	Because I was sick during the writing of this program
	is no excuse.  My excuse is that to parse 'C' code by
	without writing a grammatical parser is like building
	an F-16 out of balsa wood.  It's just a toy.

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"fc.h"

#define		FCB_SAVE(c) \
		if (FCB_IDX == 0) FCF_FNLINE= FCF_LINE ;\
		FCB_BUF[FCB_IDX++]= c

#define		FCB_POS			(FCB_IDX-1)

#define		FCC_ISDATA(c)		(c> 32)

#define		proc_bufflush()		FCB_IDX= 0
#define		proc_bufopen()		proc_bufflush()
#define		proc_bufclose()		proc_bufflush()

extern fc_opts fcopts ;
extern fc_fstate fcfs ;
extern fc_gstate fcgs ;

static void proc_ovfl() ;

int proc_read()
{
	int pcrc, c ;

	pcrc= FCE_SUCCESS ;
	proc_bufopen() ;
	while((c= proc_getc()) != EOF) {
		if (c == '\n') fcfs.line_no++ ;
		if ((pcrc= proc_bufwrite(c)) != FCE_SUCCESS) {
			break ;
		}
	}
	proc_bufclose() ;
	return(pcrc) ;
}

#define		PG_NOC	(-99)

static int proc_getc()
{
	int c ;
	static int oc= PG_NOC ;
	static int dec_cmt= 0 ;

	if (oc != PG_NOC) {
		c= oc ;
		oc= PG_NOC ;
		return(c) ;
	}

	if (dec_cmt) {
		if ((--FCS_CMT_LVL) == 0) {
			FCS_IN_CMT= 0 ;
		}
		dec_cmt= 0 ;
	}

	c= FCF_GETC() ;

	if (FCS_IN_CMT || (!FCS_IN_DQ && !FCS_IN_SQ && !FCS_IN_BQ)) {
		if (c == '/') {
			if ((oc= FCF_GETC()) == '*') {
				if ((FCS_CMT_LVL++) == 0) {
					FCS_IN_CMT= 1 ;
				}
			}
		}
		else if (c == '*') {
			if ((oc= FCF_GETC()) == '/') {
				dec_cmt= 1 ;
			}
		}
	}

	return(c) ;
}

static int proc_bufwrite(c)
int c ;
{
	if (FCB_IDX == FCB_SIZE) {
		proc_ovfl() ;
		return(FCE_BUFOVFL) ;
	}

	if (proc_nl(c)) {
		if (!FCS_IN_CMT || FCO_SHOWCMT) {
			FCB_SAVE(c) ;
		}
	}

	if (proc_bchk(c)) {
		if (proc_qchk(c)) {
			proc_fun(c) ;
		}
	}

	if (c == '\n') FCS_IN_LDATA= 0 ;
	else if (FCC_ISDATA(c)) FCS_IN_LDATA= 1 ;

	return(FCE_SUCCESS) ;
}

static void proc_ovfl()
{
	char emsg[256] ;

	sprintf(
		emsg,
		"buffer (size=%d) overflow - use 'b' option to increase",
		FCB_SIZE
	) ;
	fcfnmsg(emsg) ;
}

/*
	at this point we are guaranteed NOT to be inside
	a comment or a literal string.

	check for "function"

*/
static int proc_fun(c)
int c ;
{
	if (FCS_IN_CMT || FCS_IN_DQ || FCS_IN_SQ || FCS_IN_BQ) {
		return(1) ;
	}

	if (FCS_FNB_LVL> 0) {
		if (c == '}') FCS_FNB_LVL-- ;
		else if (c == '{') FCS_FNB_LVL++ ;
		proc_bufflush() ;
		return(0) ;
	}

	if (FCS_IN_CDL) {
		if (c == '\n') {
			FCS_IN_CDL= 0 ;
			proc_bufflush() ;
		}
		return(0) ;
	}

	if ((c == '#') && !FCS_IN_LDATA) {
		FCS_IN_CDL= 1 ;
		return(0) ;
	}

	if (FCS_IN_FPARM) {
		if (c == '(') {
			FCS_FLP_LVL++ ;
		}
		else if (c == ')') {
			if ((--FCS_FLP_LVL) == 0) {
				FCS_IN_FPARM= 0 ;
				FCS_IN_FDECP= 0 ;
				FCS_IN_FDECL= 1 ;
			}
		}
		return(0) ;
	}

	if (FCS_IN_FDECL) {
		if (c == '{') {
			proc_bufprint() ;
			proc_bufflush() ;
			FCS_FNB_LVL++ ;
			FCS_IN_FDECL= 0 ;
			FCS_IN_FN= 0 ;
		}
		else if (!FCS_IN_FDECP) {
			if ((c == ',') || (c == ';')) {	/* fn fwd dcl */
				FCS_IN_FDECL= 0 ;
				FCS_IN_FN= 0 ;
 				proc_bufflush() ;
			}
			else if (FCC_ISDATA(c)) FCS_IN_FDECP= 1 ;
		}
		return(0) ;
	}
	
	if (FCS_IN_FN) {
		if (c == '(') {
			FCS_IN_FPARM= 1 ;
			FCS_FLP_LVL= 1 ;
			FCS_POS_FLP= FCB_POS ;
			return(0) ;
		}
	}

	if (c == ';') {
		proc_bufflush() ;
		FCS_IN_FN= 0 ;
		return(0) ;
	}

	if (FCC_ISDATA(c)) {
		if (!FCS_IN_FN) {
			FCS_POS_FN= FCB_POS ;
			FCS_IN_FN= 1 ;
		}
		return(0) ;
	}

	return(FCS_IN_FN? 0: 1) ;
}

/*
	processes backslash-quoted literals

	returns 1 if character did NOT come from literal
*/
static int proc_bchk(c)
int c ;
{
	if (FCS_IN_CMT) {
		return(1) ;
	}

	if (FCS_IN_BQ) {
		FCS_IN_BQ= 0 ;
		return(0) ;
	}
	if (c == '\\') {
		FCS_IN_BQ= 1 ;
		return(0) ;
	}
	return(1) ;
}

/*
	processes character-quoted literals

	returns 1 if character did NOT come from literal
*/
static int proc_qchk(c)
int c ;
{
	if (FCS_IN_CMT) {
		return(1) ;
	}

	if (FCS_IN_SQ) {
		if (c == '\'') FCS_IN_SQ= 0 ;
		return(0) ;
	}
	if (FCS_IN_DQ) {
		if (c == '"') FCS_IN_DQ= 0 ;
		return(0) ;
	}

	if (c == '\'') {
		FCS_IN_SQ= 1 ;
		return(0) ;
	}
	if (c == '"') {
		FCS_IN_DQ= 1 ;
		return(0) ;
	}

	return(1) ;
}

/*
	processes newline sequences

	returns 1 when character did NOT come from
	newline sequence
*/
static int proc_nl(c)
int c ;
{
	if (FCS_IN_NL) {
		if (c == '\n') return(0) ;
		FCS_IN_NL= 0 ;
	}
	else if (c == '\n') {
		FCS_IN_NL= 1 ;
	}
	return(1) ;
}
