#include        <stdio.h>
#include        <fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#include	"compiler.h"
#include	"clgetopt.h"

static char tty_line[256] ;
static char err_file[256] ;
static char xm_prog[256] ;
static int debug, help, efile ;

void syntax() ;
void makeline() ;

#define		MAXARGS		10

void main(argc, argv)
int argc ;
char *argv[];
{
        int i, j, rc ;
	char *args[MAXARGS] ;

	help= 0 ;
	debug= 0 ;
	efile= 0 ;
	strcpy(xm_prog, "xm") ;
	strcpy(tty_line, "/dev/tty") ;

	clinit(argv) ;
	while((i= clgetopt(argc, argv, "e:hl:p:z")) != (-1)) {
		switch(i) {
			case 'e':
				strcpy(err_file, cloptarg) ;
				efile= 1 ;
				break ;
			case 'h':
				help= 1 ;
				break ;
			case 'l':
				strcpy(tty_line, cloptarg);
				break ;
			case 'p':
				strcpy(xm_prog, cloptarg);
				break ;
			case 'z':
				debug= 1 ;
				break ;
			default:
				exit(1) ;
				break ;
		}
	}

	if (help || (cloptind == argc)) {
		syntax() ;
		exit(0) ;
	}

	args[0]= xm_prog ;
	for (i= 1, j= cloptind; j< argc; i++, j++) {
		if (i>= (MAXARGS-1)) {
			clerror("too many arguments") ;
			exit(1) ;
		}
		args[i]= argv[j] ;
	}
	args[i]= (char *)NULL ;

	if (debug) {
		fprintf(stderr,
			"--invoking %s on line: %s\n", xm_prog, tty_line) ;
	}

	makeline() ;
        rc= execvp(xm_prog, args) ;

	/*
		we don't return to here
		if xm process started ok.
	*/

        fprintf(stderr, "R(%d) from exec\n", rc) ;

}       

static void makeline()
{
	int ifh, ofh ; 

        close(0) ;
        if ((ifh= open(tty_line, O_RDONLY))< 0) {
		clerror("can't open line for input") ;
		exit(1) ;
	}
	if (ifh != 0) {
		clerror("can't make line stdin") ;
		exit(1) ;
	}

        close(1) ;
        if ((ofh= open(tty_line, O_WRONLY))< 0) {
		clerror("can't open line for output") ;
		exit(1) ;
	}
	if (ofh != 1) {
		clerror("can't make line stdout") ;
		exit(1) ;
	}

	if (efile) {
        	close(2) ;
		unlink(err_file) ;
        	if ((ofh= open(err_file,
			O_WRONLY|O_CREAT|O_TRUNC,
			S_IREAD|S_IWRITE)
		)< 0) {
			open("/dev/tty", O_WRONLY) ;	/* make new stderr */
			clerror("can't open error file") ;
			exit(1) ;
		}
		if (ofh != 2) {
			clerror("can't make error file stderr") ;
			exit(1) ;
		}
	}
}

static void syntax() 
{
	int i ;
	static char *hl[]= {
		"rxm: remote XMODEM invocation",
		"",
		"rxm [-option(s)] - <xm arguments>",
		"",
		"options:",
		"	-h		print this message",
		"	-l dev		use \"dev\" as tty line",
		"	-p prog		use \"prog\" as xmodem process",
		"	-e file		use \"file\" for stderr",
		"	-z		enable DEBUG messages",
		"",
		"rxm invokes the local program \"xm\" (or other, if the",
		"option -p is used) and directs its input and output to",
		"the terminal line specified with the -l option.  This",
		"is useful if a remote process is waiting for an XMODEM",
		"connection on a tty line other than the current terminal."
	} ;
#define	NHL	(sizeof(hl)/sizeof(hl[0])) 

	fprintf(stderr, "\n") ;
	for (i= 0; i< NHL; i++) {
		fprintf(stderr, "%s\n", hl[i]) ;
	}
	fprintf(stderr, "\n") ;
}
