/*

   dsparse.c			Parse a Data String
	    			Copyright (C) 1989-1992, by Marty Ross	

	char *inbuf, outbuf[SIZE_OUT+1] ;
	outlen= dsparse(inbuf, outbuf);
	

   Parses buffer "inbuf" into buffer "outbuf", using C-like
   escape sequence processing.  Final length of "outbuf" is
   returned (no \0 is appended to string).

   Special escape sequences, defined in cescstr(), are not
   valid.  tokerr(rc, msg) is called for fatal error.

   Restrictions:

	the final size of the parsed input string will not
	exceed SIZE_OUT.

*/

#include	<stdio.h>
#include	<fcntl.h>

#include	"../compiler.h"
#include	"../flist.h"

extern void tokerr() ;
extern void bswap() ;

#define	SIZE_OUT	(CMDSIZE)	/* max. size of processed token */

int dsparse(inbuf, outbuf)
char *inbuf, *outbuf ;
{
	int i, j ;
	int l, c, outvals[SIZE_OUT] ;

	if ((l= cescstr(inbuf, outvals, SIZE_OUT))< 0) {
		tokerr(800, "string overflow");
	}
	for (i= j= 0; i< l; i++) {
		if ((c= outvals[i])< 0) {		/* special code  */
			tokerr(801, "invalid reserved delimiter");
		}
		outbuf[j++]= (char)outvals[i] ;
	}
	return(j) ;
}
