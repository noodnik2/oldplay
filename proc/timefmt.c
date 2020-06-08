#include	<stdio.h>

#include	<sys/types.h>
#include	<sys/signal.h>
#include	<sys/fault.h>
#include	<sys/syscall.h>
#include	<sys/procfs.h>

#include	"compiler.h"

void timefmt_timestruc(s, ts_p)
char *s ;
timestruc_t *ts_p ;
{
	char *p ;
	sprintf(
		s,
		"%ld.%09ld",
		(long)ts_p->tv_sec,
		(long)ts_p->tv_nsec
	) ;
	for (p= s + strlen(s); p> s; p--) {
		if ((*(p-1)) != '0') break ;
	}
	(*p)= '\0' ;
	if ((*(p-1)) == '.') strcat(s, "0") ;
	strcat(s, " sec") ;
}
