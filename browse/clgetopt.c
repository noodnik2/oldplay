#define		CLGETOPT_ROOT
#define		STDERR		stderr

#include	<stdio.h>
#include	"compiler.h"
#include	"clgetopt.h"

char *strchr() ;

char *findopt();
void clsyntax();
void cl1error();

/*

	(int) c= clgetopt(argc, argv, (char *)optionlist)

		optionlist records the tolerated options.  A colon after
		an option letter indicates that the option takes an arg-
		ument.  e.g.  "abo:" or "ao:b" or "o:ba" are all equiv-
		alent.  a and b are options, and o is an option requiring
		an option argument.  

		Error messages presented by clgetopt():

	command:  option x not allowed.
	
	or
	
	command:  "xxx" syntax invalid.

	
		Warnings:
			clgetopt() does not check for repeated options.
			The last specified will be used.

*/
int clgetopt(argc, argv, optionlist)
int argc ;
char *argv[] ;
char *optionlist ;
{		   
	char *arg, *olp;
	int oc;

	for (cloptarg= NULL;; cloptind++, cloptofs= 1) {
		if (cloptind>=argc) return(-1);		/* At end of CL */
		arg= argv[cloptind];
		if (arg[0]!=CLOPCHAR) return(-1);	/* Not option   */
		oc= (int) arg[cloptofs];
		if (oc==CLOPCHAR) return(-1);		/* STDIN file   */
		if (oc!=0) break;
		if (cloptofs==1) {			/* forced EOCL  */
			cloptind++;
			return(-1);
		}
	} 

	cloptofs++;
	if ((olp= findopt(optionlist, oc))==NULL) {	/* find option  */
		return(oc);
	}

	if (olp[1]!=':') {				/* noargletter  */
		return(oc);
	}
	
	if (arg[2]!='\0') {				/* argletter    */
		clsyntax(0, argv[cloptind]);
		return('?');				/* not alone    */
	}
	
	if ((cloptind+1)>=argc) {			/* get optarg   */
		clsyntax(0, argv[cloptind]);
		return('?');				/* not there    */
	}

	cloptarg= argv[++cloptind];
	cloptind++; cloptofs= 1;
	return(oc);

}


/*

	(char *)s= clcmdname(s, progname)
	
	Returns basename of progname in s.
	(e.g., "/usr/lclbin/cmd" becomes "cmd").

	Called by clerror().

*/
char *clcmdname(s, progname)
char *s;
char *progname;
{
	int i;

	for (i= strlen(progname); i>0; i--) {
		if (progname[i-1]==CLPSCHAR) break;
	}
	strcpy(s, progname+i);
	return(s) ;
}


/*

	(char *)s= clrmcma(s)

	Examines s.  If it contains any white space characters
	(' ', '\t', '\n'), leaves s unmodified.
	Otherwise, converts all commas in s to ' '.
	In all cases, returns s.

*/
char *clrmcma(s)
char *s;
{
	register char *q;

	for (q= s; *q; q++) {					/* Examine s */
		if ((*q)==' ' || (*q)=='\t' || (*q)=='\n') {
			return(s);
		}
	}	
	for (q= s; *q; q++) {					/* Convert s */
		if ((*q)==',') {
			(*q)= ' ';
		}
	}
	return(s);
}

/*

	(int) flg= cl1arg(s)

	verifies that s contains a single <optarg>.
	It is not necessary, but you may use clrcma(s) before
	calling cl1arg(s).
	if clopterr!=0, cl1arg issues the error message

	command:  "xxx" invalid.
	
	if none or more than one argument is found.

	Returns 0 if 1 argument found, 1 if none or more than one
	argument found.

*/
int cl1arg(s)
char *s;
{
	register char *q;

	if (s[0]=='\0') {			/* no argument found */
		clsyntax(1, s);
		return(1);
	}

	for (q= s; *q; q++) {				/* Examine s */
		if ((*q)==' ' || (*q)==',') {
			clsyntax(1, s);		
			return(1);			/* more than 1 */
		}
	}	

	return(0);
}


/*

	(int) flg= cl1fn(s, dvok)


	verifies that s contains a single <optarg> and that <optarg>
	can be interpreted as a filename or, if dvok!=0, as a device
	name (just tolerates /dev/...), otherwise "/dev/..." not okay.
	If clopterr!=0, issues the error message
	
	command:  "xxx" found where filename expected
	
	or 

	command:  "xxx" found where device or filename expected


	cl1fn calls cl1arg(), but suppresses its message.

	Returns 0 if 1 argument found, 1 if none or more than
	one argument found.

*/

int cl1fn(s, dvok)
char *s;
int dvok;
{
	int rc, oclopterr ;

	oclopterr= clopterr;
	clopterr= 0;
	rc= cl1arg(s); 
	clopterr= oclopterr;
		
	if (rc) {
		clsyntax(dvok? 3: 2, s);
		return(1);
	}

	if (!dvok) {
		if (memcmp(s, "/dev/", 5)==0) {
			clsyntax(3, s);
			return(1);
		}
	}
	
	return(0);
}

/*
	int clerror((char *)msg);

	iff clopterr!=0, then presents the error
	message "msg", prefixed by the command name.

	warning: the command name must be shorter than 32 characters.

*/

void clerror(msg)
char *msg;
{
	char cmdname[32];

	fprintf(STDERR, "%s: %s\n", 
		clcmdname(cmdname, clpname), msg);
}

static char *findopt(optionlist, oc)
char *optionlist;
int oc;
{
	char *olp;
	char emsg[32];

	if ((olp= strchr(optionlist, (char)oc))==NULL) {
		sprintf(emsg, "option %c not allowed", oc);
		cl1error(emsg);
		return(NULL);
	}
	return(olp);
}

static void clsyntax(type, token)
int type;
char *token;
{
	char emsg[163];
	static char *etype[]= { 
		"\"%s\" syntax invalid.",
		"\"%s\" invalid.",
		"\"%s\" found where filename expected.",
		"\"%s\" found where device or filename expected."
	} ;

	sprintf(emsg, etype[type], token);
	cl1error(emsg);
}

static void cl1error(emsg)
char *emsg;
{
	if (clopterr!=0) {
		clerror(emsg);
	}
}
