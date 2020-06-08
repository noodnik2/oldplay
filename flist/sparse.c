/*

   sparse.c				Parse a Data String
					Copyright (C) 1990, by ==C=R=C==	

	char *inbuf, outbuf[SIZE_OUT+1] ;
	outlen= sparse(inbuf, outbuf, item, nsp) ;
	char *inbuf, *outbuf, *item ;
	int *nsp ;
	

   Parses buffer "inbuf" into buffer "outbuf", using C-like
   escape sequence processing.  Final length of "outbuf" is
   returned (no \0 is appended to string).

   Special escape sequences, defined in cescstr(), represent
   substitution values from item 'item'.  to *nsp is written
   the number of "special" values substituted.

   flserr(rc, msg, prior) is called for syntax error reporting.

   Restrictions:

	the final size of the parsed input string will not
	exceed SIZE_OUT.

*/

#include	<stdio.h>
#include	<fcntl.h>

#include	"compiler.h"
#include	"flist.h"

extern struct fl_arg fla ;

extern void bswap() ;

static char *fname(), *fnext() ;
static int insert(base, arg);

#define		SIZE_OUT	CMDSIZE	/* max. size of processed token */

int sparse(inbuf, outbuf, item, nsp)
char *inbuf, *outbuf, *item ;
int *nsp;
{
	register int i, j ;
	int l, c, n, outvals[SIZE_OUT] ;
	char *cfn, fn[FL_FNSIZE+1] ;

	if ((l= cescstr(inbuf, outvals, SIZE_OUT))< 0) {
		flserr(800, "string overflow", 2);
		return(-1);
	}
	for (n= i= j= 0; i< l; i++) {

		if ((c= outvals[i])< 0) {		/* special code  */

			fn[0]= '\0';
			if ((cfn= item) != NULL) strcpy(fn, cfn);

			switch(c) {

				case -1:		/*  extension */
					j+= insert(outbuf+j, fnext(fn));
					n++;
					break;

				case -2:		/*  file */
					j+= insert(outbuf+j, fn);
					n++;
					break;

				case -3:		/*  name */
					j+= insert(outbuf+j, fname(fn));
					n++;
					break;

				case -4:
					j+= insert(outbuf+j, OPTSTR);
					n++;
					break;

				case -5:
					j+= insert(outbuf+j, DIRFN);
					n++;
					break;

				default:
					flserr(801,
						"invalid reserved delimiter",
						2
					);
					return(-1);
					break;
			}
			continue ;
		}
		outbuf[j++]= (char)outvals[i] ;
	}
	(*nsp)= n;
	return(j) ;
}

static int insert(base, arg)
char *base, *arg ;
{
	register int i ;
 
	for (i= 0; arg[i]!='\0'; i++) {
		base[i]= arg[i] ;
	}
	base[i]= '\0';
	return(i);
}

/*
	also used by sortby()		01/27/90
*/
char *fnext(fn)
char *fn;
{
	register int i;

	for (i= strlen(fn); i> 0; i--) {
		if (fn[i-1]=='.') return(fn+i);
	}
	return("");
}

static char *fname(fn)
char *fn;
{
	register int i;

	for (i= strlen(fn); i> 0; i--) {
		if (fn[i-1]=='.') {
			fn[i-1]= '\0';
			break;
		}
	}
	return(fn);
}
