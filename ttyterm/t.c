
#include	<stdio.h>

#include	"compiler.h"

static void do_dnldir()  ;
static void setlist() ;

void main()
{
	char list[20480] ;
	int i, c ;

	for (i= 0; (i< sizeof(list)) && ((c= getchar()) != EOF); i++) {
		list[i]= (char)c ;
	}
	list[i]= '\0' ;
	setlist(list, i) ;
	do_dnldir(list) ;
}

void do_dnldir()
{
	register int i ;
	char *lp, list[10240];
	char xffn[257] ;
	int nfiles, fno ;

	fprintf(stderr, "\n") ;
	fprintf(stderr, "--Obtaining file list...\n") ;
	if ((nfiles= getlist(list, sizeof(list)))< 0) {
		fprintf(stderr, "--list aborted--\n") ;
		return ; 
	}
	
	fprintf(stderr, "--Transferring %d files...\n", nfiles) ;
	for (fno= 1, lp= list; (*lp) != '\0'; fno++) {
		for (i= 0; i< (sizeof(xffn)-1);) {
			xffn[i++]= (*lp++) ;
			if ((*lp)=='\n') {
				lp++ ;
				break ;
			}
		}	
		xffn[i]= '\0' ;
		fprintf(stderr,"--Remote file %d/%d: %s\n",fno,nfiles,xffn) ;
	}
}

static int getlist(lp, ml)
char *lp ;
int ml ;
{
	char ls_cmd[256] ;
	char schar, echar ;
	int i, ch, lastch, intext, nfiles ;

	schar= 1 ;
	echar= 2 ;

	i= 0;
	nfiles= 0 ;
	lastch= -1 ;
	for (intext= 0;;) {
		if ((ch= t_inc()) != (-1)) {
			if (intext) {
				if (ch==(int)echar) {
					intext= 0 ;
					break ;
				}
				if ((i+1)>= ml) {
					fprintf(stderr,
						"--filename list too long\n") ;
					lp[i]= '\0' ;
					return(-1) ;
				}
				switch(ch) {
					case '@':
					case '\t': case ' ':
					case '\r': case '*':
						ch= '\n' ;
						break ;
				}
				if (ch=='\n' && (lastch==ch || lastch==(-1))) {
					continue ;
				}
				else if (ch=='\n' && lastch=='/') {
					i-- ;
				}
				if (ch=='\n') nfiles++ ;
				lp[i++]= (char) ch ;
				lastch= ch ;
			}
			else {
				if (ch==(int)schar) {
					intext= 1 ;
				}
			}	
		}
	}
	lp[i]= '\0' ;
	return(nfiles) ;
}

static char *list_p ;
static int list_n ; 

static void setlist(list, n)
char *list;
int n ;
{
	list_p= list ;
	list_n= n ;
}

static int in_n= 0 ;
static int t_inc()
{
	if (in_n< list_n) {
		return((int)list_p[in_n++]) ;
	}
	return(-1) ;
}
