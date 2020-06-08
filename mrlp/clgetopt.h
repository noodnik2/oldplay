/*
	
	clgetopt.h:			Command Line parser definitions

	#defines:
	
		MAXOPTIONLEN		max size of option argument
		CLOPTCHAR		option marker character
			
		char *clpname		global name of executing program
		char *cloptarg		global current option argument
		int cloptind		global current option index
		int cloptofs		global current option offset
		int clopterr		global current option error level

		clinit()		macro function: initialize parser

	note:	the globals are external if CLGETOPT_ROOT is #undefined
		when this header file is included.  

*/


#define		MAXOPTIONLEN	81

#ifndef		CLGETOPT_ROOT

extern char *clpname ;
extern char *cloptarg ;
extern int cloptind ;
extern int cloptofs ;
extern int clopterr ;

#else

char *clpname ;
char *cloptarg ;
int cloptind ;
int cloptofs ;
int clopterr ;

#endif

#if	OPSYS==OPMSDOS
#define	CLOPCHAR	'/'
#define	CLPSCHAR	'\\'
#endif

#if	OPSYS==OPUNIX
#define	CLOPCHAR	'-'
#define	CLPSCHAR	'/'
#endif

#define	clinit(argv)   		\
	clpname= argv[0];	\
	cloptarg= NULL;		\
	cloptind= 1;   		\
	cloptofs= 1;   		\
	clopterr= 1;
