/*

	docmd				Handle user command processing
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

int docmd(cmd)
char *cmd ;
{
	int rc ;

	if (iscmd(cmd, &rc)) return(rc) ;
	return(notcmd(cmd)) ;
}

/*
	static subroutines
*/

static int notcmd(cmd)
char *cmd ;
{
	int i, l ;

	l= strlen(cmd) ;
	for (i= l+1; i> 0; i--) cmd[i]= cmd[i-1] ;
	cmd[0]= '?' ;
	return(0) ;
}

#define	WORDSIZE	10

static int iscmd(cmd, rcp)
char *cmd;
int *rcp ;
{
	char word[WORDSIZE+1], *arg ;
	int i ;
	long iarg ; 

	cmd= strim(cmd) ;
	if ((cmd[0]=='\0') || (cmd[0]=='?')) return(0) ;

	for (i= 0; i< WORDSIZE; i++) {
		if (cmd[i]=='\0' || cmd[i]==' ') break;
		word[i]= cmd[i] ;
	}
	word[i]= '\0' ;

	if ((arg= (cmd[i]!='\0')? strim(cmd+i+1): NULL) != NULL) {
		if (sscanf(arg, "%ld", &iarg)!=1) iarg= 1L ;
	}
	else iarg= 1L ;

	if ( cmdcmp(word, "quit", 1) ||
	     cmdcmp(word, "exit", 2) ) {
		(*rcp)= 1 ;			/* signal eof */
		return(-1) ;
	}

	else if (cmdcmp(word, "backward", 1))
		NEWTOPLINE= TOPLINE-(iarg*(long)F_LINES) ;
	else if (cmdcmp(word, "down", 1)) NEWTOPLINE= TOPLINE+iarg ;
	else if (cmdcmp(word, "enter", 1)) doenter(arg) ;
	else if (cmdcmp(word, "findstr", 1)) dofindstr(arg) ;
	else if (cmdcmp(word, "findbin", 5)) dofindbin(arg) ;
	else if (cmdcmp(word, "forward", 2)) 
		NEWTOPLINE= TOPLINE+(iarg*(long)F_LINES) ;
	else if (cmdcmp(word, "goto", 1)) NEWTOPLINE= (iarg-1L) ;
	else if (cmdcmp(word, "help", 1)) dohelp(arg) ;
	else if (cmdcmp(word, "left", 1)) NEWINDENT= INDENT-(int)iarg ;
	else if (cmdcmp(word, "next", 1)) NEWTOPLINE= TOPLINE+(int)iarg ;
	else if (cmdcmp(word, "right", 1)) NEWINDENT= INDENT+(int)iarg ;
	else if (cmdcmp(word, "set", 1)) doset(arg) ;
	else if (cmdcmp(word, "shell", 2)) dosyscmd(arg) ;
	else if (cmdcmp(word, "top", 2)) NEWTOPLINE= 0 ;
	else if (cmdcmp(word, "up", 1)) NEWTOPLINE= TOPLINE-iarg ;
	else if (cmd[0]=='!') dosyscmd(cmd+1) ;
	else if (cmd[0]=='/') dofindstr(cmd+1) ;
	else if (sscanf(word, "%ld", &iarg)==1) NEWTOPLINE= (iarg-1L) ;
	else return(0) ;

	if (NEWTOPLINE< 0L) NEWTOPLINE= 0L ;
	if (NEWINDENT< 0) NEWINDENT= 0 ;
	(*rcp)= 0 ;
	return(1) ;
}

/*
	dofindbin	finds non-text character in file.
			sets TOPLINE if found.
*/
static int dofindbin(s)
char *s ;
{
	int i, j, l ;
	char rec[RECSIZEMAX+1] ;
	unsigned char c ;

	if (s[0]!='\0') {
		beep() ;
		return(0) ;		/* error! */
	}

	for (i= TOPLINE+1;; i++) {
		if (rrd(i, rec, RECSIZE, &l)) break ;
		for (j= 0; j< l; j++) {
			c= (unsigned char) rec[j] ;
			if (c=='\r' || c=='\n' || c=='\t') continue ;
			if ((c<' ') || (c>=0x80)) break ;
		}
		if (j< l) {
			NEWTOPLINE= i ;
			return(1) ;
		}
	}	
	beep() ;
	return(0) ;
}

/*
	dofindstr	finds text string in file.
			sets TOPLINE if found.
*/
static int dofindstr(s)
char *s;
{
	int i, l;
	char rec[RECSIZEMAX+1] ;

	if (CASEMODE==BCM_IGNORE) mklowcase(s) ;
	if (s[0]!='\0') {
		for (i= TOPLINE+1;; i++) {
			if (rrd(i, rec, RECSIZE, &l)) break ;
			rec[l]= '\0' ;
			if (CASEMODE==BCM_IGNORE) mklowcase(rec) ;
			if (index(rec, s)>= 0) {
				NEWTOPLINE= i ;
				return(1) ;
			}
		}
	}	
	beep() ;
	return(0) ;	/* not found */
}

static int doenter(cmd)
char *cmd ;
{
	char cmdbuf[1024] ;

	sprintf(cmdbuf, "browse %s", cmd) ;
	return(dosyscmd(cmdbuf)) ;
}

static int dohelp(cmd)
char *cmd ;
{
	if (cmd==NULL) {
		clear() ;
		refresh() ;
		scnmode(0) ;
		syntax() ;
		domore() ;
		scnmode(1) ;
		clear() ;
		redraw() ;
		return(0) ;
	}	
	beep() ;
	return(-1);
}

static int dosyscmd(cmd)
char *cmd ;
{
	if (cmd!=NULL) {
		clear() ;
		refresh() ;
		scnmode(0) ;
		system(cmd) ;
		scnmode(1) ;
		clear() ;
		redraw() ;
		return(0) ;
	}	
	beep() ;
	return(-1);
}
