#define		DEBUG		1

/*

	pdcmdmode(screenbuf)

		parse PD commands until PD takes long jump out.
		send parsed commands to PD command interpreter.

*/

#include	<stdio.h> 
#include	"compiler.h"
#include	"pd.h"
#include	"pderr.h"

#if	DEBUG==1
extern FILE *pdfile ;
#endif

void pdcmdmode(scn) 
char *scn ;
{ 
	int rc ; 
	char line[PD_MAXLINELEN] ; 

	for(;;) { 
		rc= pd_rdline(line) ; 
		if (rc==3 || rc==4) { 		/* too long or timeout */
			pdrc= PD_ABORT ; 
			pderrst() ; 
		}
		else if (rc==0) { 		/* not NOOP	*/
			if (strcmp(line, "P")==0) {
				pdrc= PD_OKAY ; 
				return ; 
			}
			if (strcmp(line, "TTY")==0)  {
				pdrc= pdtty(scn) ; 
			}
			else 	pdrc= pdaxeq(line) ; 
			if (pdrc == GO_ERRST) { 
#if DEBUG==1
				fprintf(pdfile, "pdcmdm: badcmd='%s'\n", line) ;
#endif
				pdrc= PD_ABORT ; 
				pderrst() ; 
			}
		}
	}
	/*NOTREACHED*/
}

/*
	rc= pd_rdline(s)

	Returns:
		0.	command loaded in s
		2.	command is ECM or NOOP
		3.	command is too long 
		4.	command timeout; incomplete
*/

static int pd_rdline(s)
char *s ; 
{
	int buf ; 

	buf= pdgetch() ;
	if (buf=='\r') buf= pdgetch() ;
	if (buf< 0) return(4) ;
	if (buf==pdecm || buf=='\n') return(2) ; 
	*s= (char) buf ; 
	return(pd_getarg(s+1)) ; 
}


/*
	rc= pd_getarg(s) 

	Returns:
		0	arg is loaded
		3	line is too long
		4	line timed out

	pd_getarg() is used to read the rest of a ASCII command to and
	to obtain the argument to obtain the argument in BINARY mode.  
	The string ends on \n.  \r's are ignored.

*/
static int pd_getarg(s)
char *s ; 
{
	int buf, l ;

	for (l= 0; l< (PD_MAXLINELEN-2);) { 
		if ((buf= pdgetch())< 0) return(4) ;
		if (buf == '\n') { 
			s[l]= '\0' ; 
			return(0) ; 
		}
		if (buf != '\r') s[l++]= (char) buf ; 
	}
	return(3) ; 
}
