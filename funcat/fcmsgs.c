/*

	fcmsg.c

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"fc.h"

extern fc_opts fcopts ;
extern fc_fstate fcfs ;
extern fc_gstate fcgs ;

void fcfnmsg(msg)
char *msg ;
{
	char emsg[256] ; 

	sprintf(emsg, "%s: %s\n", FCF_NAME, msg) ;
	clerror(emsg) ;
}

void fcabort(msg)
char *msg ;
{
	clerror(msg) ;
	exit(FCE_ABORTED) ;
}

/* end of fcmsg.c */
