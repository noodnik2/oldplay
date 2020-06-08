/*	Revision:	1
*/
/*

	fcmain.c
				Copyright (C) 1992-1993, Contahal Ltd.
*/

#include	<stdio.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"fc.h"

char *malloc() ;

fc_opts fcopts ;
fc_fstate fcfs ;
fc_gstate fcgs ;

static void initvars() ;
static void initargs() ;
static void initproc() ;
static void procfiles() ;
static void finiproc() ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	initvars() ;
	initargs(argc, argv) ;
	initproc() ;
	procfiles() ;
	finiproc() ;
	exit(fcgs.rc) ;
}

static void initvars()
{
	memset((char *)&fcgs, '\0', sizeof(fcgs)) ;
	memset((char *)&fcopts, '\0', sizeof(fcopts)) ;
	fcgs.buf_size= FCB_DFLT_SIZE ;
}

static void initargs(argc, argv)
int argc ;
char *argv[] ;
{
	static char *stdlist[]= { "-" } ;
	int i ;

	clinit(argv) ;

	while((i= clgetopt(argc, argv, "b:chilps"))> 0) {
		switch(i) {
			case 'b': fcgs.buf_size= gonum(cloptarg) ;	break ;
			case 'c': fcopts.show_cmt= 1 ;			break ;
			case 'h': fcopts.help= 1 ;			break ;
			case 'i': fcopts.indent= 1 ;			break ;
			case 'l': fcopts.lines= fcopts.indent= 1 ;	break ;
			case 'p': fcopts.prefix= 1 ;			break ;
			case 's': fcopts.show_static= 1 ;		break ;
			default:  clerror("use 'h' option for help") ;
				exit(FCE_BADPARMS) ;			break ;
		}
	}

	fcgs.progname=  argv[0] ;
	fcgs.fnlist= 	argv + cloptind ;
	fcgs.num_files= argc - cloptind ;

	if (fcgs.num_files == 0) {
		fcgs.fnlist= stdlist ;
		fcgs.num_files= 1 ;
	}

	if (FCO_HELP) {
		fcsyntax() ;
		exit(FCE_SUCCESS) ;
	}

}

static int gonum(str)
char *str ;
{
	int i, badflag ;
	char emsg[256] ;

	if (!(badflag= (sscanf(str, "%d", &i) != 1))) {
		badflag= (i< 0) ;
	}

	if (badflag) {
		sprintf(emsg, "bad value: %s", str) ;
		clerror(emsg) ;
		exit(FCE_BADPARMS) ;
	}

	return(i) ;
}

static void initproc()
{
	char *p ;
	char emsg[256] ;

	if ((p= malloc(fcgs.buf_size)) == (char *)NULL) {
		sprintf(
			emsg,
			"can't allocate collection buffer - size=%d",
			fcgs.buf_size
		) ;
		fcabort(emsg) ;
		/*NOTREACHED*/
	}
	fcgs.buf_ptr= p ;
}

static void procfiles()
{
	int i ;

	for (i= 0; i< fcgs.num_files; i++) {
		fcgs.cur_fno= i ;
		fcgs.rc= proc_cur() ;
		if (fcgs.rc != 0) break ;
	}
}

static void finiproc()
{
	free(fcgs.buf_ptr) ;
}

static int proc_cur()
{
	FILE *fp ;
	int pcrc ;

	memset((char *)&fcfs, '\0', sizeof(fcfs)) ;

	if (strcmp(FCF_NAME, "-") == 0) {
		fcfs.ifp= stdin ;
	}
	else {
		if ((fp= fopen(FCF_NAME, "r")) == (FILE *)NULL) {
			fcfnmsg("can't open") ;
			return(FCE_FNOTFOUND) ;
		}
		fcfs.ifp= fp ;
	}
	fcfs.ofp= stdout ;

	if (FCO_PREFIX) {
		if ((fcfs.ofp == stdout) && (fcfs.ifp != stdin)) {
			printf("%s:\n", FCF_NAME) ;
		}
	}

	pcrc= proc_read() ;

	if (fcfs.ifp != stdin) fclose(fcfs.ifp) ;
	if (fcfs.ofp != stdout) fclose(fcfs.ofp) ;

	return(pcrc) ;
}

/* end of fcmain.c */
