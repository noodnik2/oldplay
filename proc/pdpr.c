#include	<stdio.h>
#include	<fcntl.h>

#include	<sys/types.h>
#include	<sys/signal.h>
#include	<sys/fault.h>
#include	<sys/syscall.h>
#include	<sys/procfs.h>

#include	"compiler.h"
#include	"clgetopt.h"

void pdpr_ttl(ttlmsg)
char *ttlmsg ;
{
	print_add("\t") ;
	print_add(ttlmsg) ;
	print_add(": ") ;
}

void pdpr_line(ttl, line)
char *ttl, *line ;
{
	pdpr_ttl(ttl) ;
	print_add(line) ;
	print_add("\n") ;
}

void pdpr_timestruc(whatmsg, ts_p)
char *whatmsg ;
timestruc_t *ts_p ;
{
	char timestr[100] ;

	timefmt_timestruc(timestr, ts_p) ;
	pdpr_line(whatmsg, timestr) ;
}

void pdpr_2str(msg, cmd, l_cmd, args, l_args)
char *msg, *cmd, *args ;
int l_cmd, l_args ;
{
	int i ;
	char *p, result[1024] ;

	for (p= result, i= 0; i< l_cmd; i++) p[i]= cmd[i] ;
	p[i]= '\0' ;
	rtrim(p) ;
	strcat(p, " ") ;
	i= strlen(p) ;

	for (p+= i, i= 0; i< l_args; i++) p[i]= args[i] ;
	p[i]= '\0' ;
	rtrim(p) ;

	pdpr_line(msg, result) ;
}

void pdpr_longdec(msg, lval)
char *msg ;
long lval ;
{
	char decmsg[65] ;

	sprintf(decmsg, "%ld", lval) ;
	pdpr_line(msg, decmsg) ;
}

void pdpr_longhex(msg, lval)
char *msg ;
long lval ;
{
	char hexmsg[65] ;

	sprintf(hexmsg, "0x%lX", lval) ;
	pdpr_line(msg, hexmsg) ;
}
