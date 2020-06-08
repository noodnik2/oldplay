/*

	pderror.c			Copyright (C) 1990, by ==C=R=C==
					Display reserved Stata RGS/PD error


	PD_EXISTS= pderrex(s)
	char *s ; 
		presents "%s already exists" error message

	PD_NOTFND= pderrnf(s)
	char *s ; 
		presents "%s not found" error message 

	PD_BADARG= pderrinv(s)
	char *s ; 
		presents "%s invalid" error message

	PD_BADARG= pderror(i)
	int i ; 
		Presents standard error message
			3.	no windowing software
			4.	Syntax error
		
*/

#include	"pd.h"
#include	"pderr.h"

void pderrmsg() ; 

int pderrex(s)
char *s ; 
{
	char buf[PD_MAXLINELEN+80] ; 

	sprintf(buf,"%s already exists",s) ; 
	pderrmsg(buf) ; 
	return(PD_EXISTS) ; 
}

int pderrnf(s)
char *s ; 
{
	char buf[PD_MAXLINELEN+80] ; 

	sprintf(buf,"%s not found",s) ; 
	pderrmsg(buf) ; 
	return(PD_NOTFND) ; 
}

int pderrinv(s)
char *s ; 
{
	char buf[PD_MAXLINELEN+80] ; 

	sprintf(buf,"'%s' invalid",s) ; 
	pderrmsg(buf) ; 
	return(PD_BADARG) ; 
}

int pderror(i)
int i ; 
{
	char buf[80] ; 

	switch(i) { 

		case 3:
			pderrmsg("no windowing software") ;
			break ;

		case 4:
			pderrmsg("syntax error") ;
			break ;

		default:
			sprintf(buf, "unknown error %d", i) ; 
			pderrmsg(buf) ;
			break ;

	}
	return(PD_BADARG) ; 
}
