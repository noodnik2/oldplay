#include	"compiler.h"
#include	"pd.h"
#include	"pderr.h"

extern int pdcmdeb() ; 
extern int pdcmddb() ; 
extern int pdcmddbg() ; 
extern int pdcmdwdir() ; 
extern int pdmdhello() ; 
extern int pdmdbye() ; 
extern int pdcmddcw() ; 

extern int pdcmdawd() ; 
extern int pdcmdabo() ; 
extern int pdcmdasv() ; 
extern int pdcmdaee() ; 
extern int pdcmdawdr() ; 
extern int pdcmdauw() ; 
extern int pdcmdasp() ; 
extern int pdcmdaclr() ; 
extern int pdcmdacmap() ;

struct cmdtabdf { 
	char	*txt ; 
	int	bin ; 
	int	(*cmd)() ; 
} ; 

static struct cmdtabdf noarg[] = { 
	"EB",0xfa, pdcmdeb,
	"DB",0xf9, pdcmddb,
	"DEBUG",0xf7, pdcmddbg,
	"WDIR",0xcf, pdcmdwdir,
	"DCW",0xc1, pdcmddcw,
	"SE",0xb1, pdmdbye,
	"SI",0xb0, pdmdhello,
	"\0",0x00, pdcmdeb
} ; 

static struct cmdtabdf wargs[] = {
	"SP",0xe0, pdcmdasp,
	"SV",0xfb, pdcmdasv,
	"EE",0xf8, pdcmdaee,
	"WD",0xd1, pdcmdawd,
	"WDROP",0xd3, pdcmdawdr,
	"UW",0xd4, pdcmdauw,
	"CMAP",0xe1, pdcmdacmap,
	"C",0xc0, pdcmdaclr,
	"\0",0x00, pdcmdeb
} ; 

/*
	rc= pdaxeq(s) 

	s contains the ASCII command string of the form:

		(1)	cmd\0
		(2)	cmd arg\0

	If there are arguments, there are at least one blank between the 
	command and its argument.
*/

int pdaxeq(s)
char *s ; 
{
	register int i ; 
	char *arg, cmd[PD_MAXLINELEN] ; 

	for (i= 0; noarg[i].txt[0]; i++) {
		if (strcmp(s, noarg[i].txt)==0) {
			return((*(noarg[i].cmd))()) ; 
		}
	}

	for (i= 0; s[i] != ' '; i++) { 
		if (s[i] == '\0') return(GO_ERRST) ; 
		cmd[i]= s[i] ; 
	}
	cmd[i]= '\0' ; 

	arg= &s[i] ; 
	while(*arg==' ') arg++ ; 
	if (*arg == '\0') return(GO_ERRST) ; 
	
	for (i= 0; wargs[i].txt[0]; i++) { 
		if (strcmp(cmd, wargs[i].txt)==0) { 
			return((*(wargs[i].cmd))(arg)) ; 
		}
	}
	return(GO_ERRST) ; 
}
