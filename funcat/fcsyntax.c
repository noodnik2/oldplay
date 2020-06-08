/*	Revision:	1
*/
/*

	fcsyntax.c			funcat: Catalog C-Program functions
					Copyright (C) 1991-1993, Contahal Ltd.

*/

#include	<stdio.h>

#include	"compiler.h"

void fcsyntax()
{
	static char *lines[]= {
"",
"funcat 0.02  catalog C-program functions",
"Copyright (C) 1991-1993, Contahal Ltd.",
"",
"syntax:",
#if	OPSYS==OPMSDOS
"\tfuncat [/h] [/c] [/s] [/p] [/l] [/i] [/b #] [file-list]",
#else
"\tfuncat [-h] [-c] [-s] [-p] [-l] [-i] [-b #] [file-list]",
#endif
"",
"options:",
"\th\tPrints this syntax diagram to stderr",	
"\tc\tShow comments appearing in or just before fn decl",	
"\ti\tIndent output lines with tab character",
"\tl\tShow source line numbers of functions",
"\tp\tPrefix output with name of C-source (input file)",
"\ts\tShow \"static\" functions",
"\tb #\tSets 'collection buffer size' to #.  Use if overflow.",
"",
"     funcat reads one or more input C-source files (or if no 'file-list'",
"is specified, then stdin is used), and writes to stdout just the function",
"declarations, including parameters.  This is useful when you want to know",
"what external functions exist in a module, or set of modules, and how to",
"call the functions.",
""
} ;
#define	NLINES	(sizeof(lines)/sizeof(lines[0]))

	int i ;

	for (i= 0; i< NLINES; i++) {
		fprintf(stderr, "%s\n", lines[i]) ;
	}
}
