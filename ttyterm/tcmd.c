
/*

	terminal driver:	command processor

*/

#include	<stdio.h>
#include	"compiler.h"
#include	"term.h"

void nothing() ;
static void parse(), tcmd_syntax(), tcmderr() ;
extern int std_sendfile(), std_rcvfile() ;

static char *ljust() ;
static char *get_cmd() ;
static int proc_cmd() ;

static int tcmd_quit= 0 ;
static int tcmd_stay= 0 ;

int tcmd()
{
	char *cmdline, cmdbuf[256] ;

	tcmd_quit= 0 ;
	fprintf(stderr, "\n") ;
	do {

		cmdline= get_cmd(cmdbuf) ;	/* get request from user */
		proc_cmd(cmdline) ;		/* process the request */

	} while((cmdline[0]!='\0') && (tcmd_stay) && (!tcmd_quit)) ;

	return(tcmd_quit) ;
}

/*
	static procedures
*/

static int proc_cmd(cmdline)
char *cmdline ;
{
	char line[256], word[256], emsg[256] ;

						/* null command */
	if (cmdline[0]=='\0') {
		return ;
	}

						/* process cmdline commands */
	if (cmdline[0]=='!') {
		system(cmdline+1) ;
		return ;
	}

						/* process word commands */
	strcpy(line, cmdline) ;
	parse(word, line, sizeof(word)) ;

	if ( cmdcmp(word, "exit", 1)
	     || cmdcmp(word, "quit", 1)) {
		tcmd_quit= 1 ;
		return ;
	}

	if (cmdcmp(word, "takedir", 5)) {
		do_dnldir() ;
		return ;
	}

	if (cmdcmp(word, "putdir", 4)) {
		do_upldir() ;
		return ;
	}

	if (cmdcmp(word, "take", 2)) {
		do2fnc(line, std_rcvfile) ;
		return ;
	}

	if (cmdcmp(word, "put", 2)) {
		do2fnc(line, std_sendfile) ;
		return ;
	}

	if (strcmp(word, "cd")==0) {
		parse(word, line, sizeof(word)) ;
		if (chdir(word)) {
			sprintf(emsg, "can't change to: %s\n", word);
			tcmderr(emsg);
		}
		return ;
	}

	if (cmdcmp(word, "set", 2)) {
		parse(word, line, sizeof(word)) ;
		if (cmdcmp(word, "stay", 3)) {
			parse(word, line, sizeof(word)) ;
			if (cmdonoff(word, &tcmd_stay)==0) {
				return ;
			}
		}
		if (cmdcmp(word, "xfer", 2)) {
			parse(word, line, sizeof(word)) ;
			if (cmdcmp(word, "xmodem", 2)) {
				parse(word, line, sizeof(word)) ;
				if ((word[0]=='\0')
					|| (cmdcmp(word, "text", 1))) {
					tsetxfer(XFER_XMT) ;
					return ;
				}
				if (cmdcmp(word, "binary", 1)) {
					tsetxfer(XFER_XMB) ;
					return ;
				}
			}
			if (cmdcmp(word, "text", 1)) {
				tsetxfer(XFER_TEXT) ;
				return ;
			}
		}
		tcmderr("invalid set command") ;
		return ;
	}

	if ( (cmdcmp(word, "help", 1))
	     || (strcmp(word, "?")==0)
	) {
		tcmd_syntax() ;
		return ; 
	}

	tcmderr("unrecognized ttyterm command") ;
}

static int cmdonoff(cmd, onoffp)
char *cmd ;
int *onoffp ;
{
	if (strcmp(cmd, "on")==0) {
		(*onoffp)= 1 ;
		return(0) ;
	}
	if (strcmp(cmd, "off")==0) {
		(*onoffp)= 0 ;
		return(0) ;
	}
	return(1) ;
}

static char *get_cmd(cmdbuf)
char *cmdbuf ;
{
	fprintf(stderr, "--> ");
	fflush(stderr) ;
	gets(cmdbuf) ;
	return(ljust(cmdbuf)); 
}

static void tcmderr(msg)
char *msg ;
{
	fprintf(stderr, "--error: %s\n", msg) ;
}

void nothing()
{
	fprintf(stderr, "--(nothing done)\n") ;
}

static int do2fnc(parms, cmdproc)
char parms[] ;
int (*cmdproc)() ;
{
	char fn1[256], fn2[256] ;

	parse(fn1, parms, sizeof(fn1)) ;
	parse(fn2, parms, sizeof(fn2)) ;
	if (fn1[0] == '\0') {
		nothing() ;
		return ;
	}
	if (fn2[0] == '\0') {
		strcpy(fn2, fn1) ;
	}
	cmdproc(fn1, fn2) ;
}

#define		IS_SPACE(x)	(((x)==' ') || ((x)=='\t'))

static void parse(word, line, ml)
char word[], line[] ;
int ml ;
{
	register int i ;
	char *cp ;
	
	for (i= 0, cp= ljust(line); (*cp) != '\0'; cp++) {
		if (IS_SPACE(cp[0])) break ;
		word[i++]= (*cp) ;
		if (i>= ml) {
			i-- ;
			break ;
		}
	}
	word[i]= '\0' ;

	for (i= 0, cp= ljust(cp); (*cp) != '\0'; cp++) {
		line[i++]= (*cp) ;
	}
	line[i]= '\0' ;

}

static char *ljust(s)
char *s ;
{
	while(IS_SPACE(*s)) s++ ;
	return(s) ;
}

static int cmdcmp(s1, s2, l)
char *s1, *s2 ;
int l ;
{
	register int i ;

	for (i= 0; (s1[i]!='\0') && (s2[i]!='\0'); i++) {
		if (s1[i] != s2[i]) break ;
	}
	return(
		((s1[i]=='\0') && (i>= l))? 1: 0
	) ;
}

static void tcmd_syntax()
{
	int i ;
	static char *tcsl[]= {
		"ttyterm commands:",
		"",
		"commands are:",
		"",
		"\thelp\t\tprints this list",
		"\tput f1 [f2]\tsend local file f1 to remote file f2",
		"\ttake f1 [f2]\treceive remote file f1 to local file f2",
		"\tputdir\t\tsend all files in current local directory",
		"\ttakedir\t\treceive all files from current remote dir",
		"\tcd d\t\tchange local directory to d",
		"\t!cmd\t\tsend 'cmd' to the system",
		"\tquit\t\tquit terminal mode",
		"\tset x y [z]\tset internal parameters:",
		"\t  x:xfer y:{xmodem|text} z:[text|binary]",
		"\t  x:stay y:{on|off}",
		""
	} ;
#define	NTCS	(sizeof(tcsl)/sizeof(tcsl[0]))

	fprintf(stderr, "\n") ;
	for (i= 0; i< NTCS; i++) {
		fprintf(stderr, "-- %s\n", tcsl[i]) ;
	}
}
