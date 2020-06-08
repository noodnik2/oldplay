/*

	ebchelp.c			Print help for ebcdic command
					Copyright (C) 1989, by ==C=R=C==

*/

#include	<stdio.h>
#include	"../compiler/compiler.h"

static char *ebchs[]= {
"Usage:	  ebcdic [-options] ebc_file.in asc_file.out",
"    or",
"         (ebc_process) | ebcdic [-options] | (asc_process)",
"Options:",
"	  -h		print this help message",
"	  -q		quiet, supress informatory messages",
"	  -z		enable diagnostic messages",
"	  -E e_table	select alternate translation table",
"",
"Changes all EBCDIC text characters in input to their ASCII equivalent.",
"The mapping of EBCDIC to ASCII is specified by the ebcdic(5) trans-",
"lation table.  Any characters not located in the table are left un-",
"changed.  An error message is printed if non-EBCDIC text characters",
"are encountered unless the -q option is used.",
""
} ;

#define		NEBCHS	(sizeof(ebchs)/sizeof(ebchs[0]))

void ebchelp()
{
	int i;
	FILE *ofp;

	ofp= stdout ;
	for (i= 0; i< NEBCHS; i++) {
		fputs(ebchs[i], ofp);
		fputs("\n", ofp);
	}
}
