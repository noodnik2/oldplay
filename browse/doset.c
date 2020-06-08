/*

	doset				set browse run-time parameters
					Copyright (C) 1990, by ==C=R=C==

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"browse.h"

extern struct br_arg bra ;
extern struct br_opt bro ;
extern struct br_scn brs ;

char *strim() ;

int doset(arg)
char *arg ;
{
	int rc ;

	if (arg!=NULL) {
		if (isarg(arg, &rc)) return(rc) ;
	}
	return(notset(arg)) ;
}

void brsettab(brp)
struct br_draw *brp ;
{
	(brp->tabmode)= (
			(!bro.o_tab) &&
			(!brp->hexmode) &&
			(!brp->binmode) &&
			(!brp->ebcmode) 
		) ; 
}

/*
	static subroutines
*/

static int notset(arg)
char *arg ;
{
	int i, l ;

	if (arg != NULL) {
		l= strlen(arg) ;
		for (i= l+1; i> 0; i--) arg[i]= arg[i-1] ;
		arg[0]= '?' ;
	}
	beep() ;
	return(0) ;
}

#define	WORDSIZE	10

static int isarg(arg, rcp)
char *arg;
int *rcp ;
{
	char word[WORDSIZE+1], *val ;
	int i, rc ;

	(*rcp)= 0 ;
	arg= strim(arg) ;
	if (arg[0]=='?') return(0) ;

	for (i= 0; i< WORDSIZE; i++) {
		if (arg[i]=='\0' || arg[i]==' ') break;
		word[i]= arg[i] ;
	}
	word[i]= '\0' ;

	val= (arg[i]!='\0')? strim(arg+i+1): NULL ;

	if (cmdcmp(word, "tabexpand", 1)) {
		if (val!=NULL) return(onoff(&NEWTABMODE, val)) ;
	}
	else if (cmdcmp(word, "ebcdic", 1)) {
		if (val!=NULL) {
			rc= onoff(&NEWEBCMODE, val) ;
			brsettab(&NDRAW) ;
			return(rc) ;
		}
	}
	else if (cmdcmp(word, "binary", 1)) {
		if (val!=NULL) {
			rc= onoff(&NEWBINMODE, val) ;
			brsettab(&NDRAW) ;
			return(rc) ;
		}
	}
	else if (cmdcmp(word, "hexdisplay", 1)) {
		if (val!=NULL) {
			rc= hexmode(&NEWHEXMODE, val) ;
			brsettab(&NDRAW) ;
			return(rc) ;
		}
	}
	else if (cmdcmp(word, "case", 1)) {
		if (val!=NULL) return(casemode(&CASEMODE, val)) ;
	}
	else if (cmdcmp(word, "scale", 1)) {
		if (val!=NULL) return(rulermode(&NEWRLINE, val)) ;
	}

	beep() ;
	(*rcp)= -1 ;
	return(0) ;
}

static int rulermode(rp, vs)
int *rp;
char *vs;
{
	char word[10] ;
	int i, nrline ;

	for (i= 0; i< WORDSIZE; i++) {
		if (vs[i]=='\0' || vs[i]==' ') break;
		word[i]= vs[i] ;
	}
	word[i]= '\0' ;

	if (cmdcmp(word, "on", 2)) {
		nrline= 0;
		vs= strim(vs+i) ;
		if (vs[0]!='\0') {
			if (sscanf(vs, "%d", &nrline)!=1) {
				beep();
				return(0) ;
			}
			if ((nrline< 0) || (nrline>=F_LINES)) {
				beep();
				return(0);
			}
		}
		(*rp)= nrline ;
	}	
	else if (cmdcmp(word, "off", 2)) (*rp)= F_LINES ;
	else { beep(); return(0); }
	return(1) ;
}

static int casemode(cp, vs)
int *cp;
char *vs;
{
	if (cmdcmp(vs, "ignore", 1)) (*cp)= BCM_IGNORE ;
	else if (cmdcmp(vs, "respect", 1)) (*cp)= BCM_RESPECT ;
	else { beep(); return(0); }
	return(1) ;
}

static int onoff(ip, vs)
int *ip;
char *vs;
{
	if (cmdcmp(vs, "on", 2)) (*ip)= 1 ;
	else if (cmdcmp(vs, "off", 2)) (*ip)= 0 ;
	else { beep(); return(0); }
	return(1) ;
}

static int hexmode(ip, vs)
int *ip;
char *vs;
{
	if (cmdcmp(vs, "on", 2)) (*ip)= -1 ;
	else if (cmdcmp(vs, "off", 2)) (*ip)= 0 ;
	else if (cmdcmp(vs, "char", 1)) (*ip)= 1 ;
	else { beep(); return(0); }
	return(1) ;
}
