#include	<stdio.h>

#include	"compiler.h"
#include	"pw.h"
#include	"pw_proc.h"

void pw_entry(ename, new_pp_p, old_pp_p)
char *ename ;
pwproc_t *new_pp_p, *old_pp_p ;
{
	char *chgstr ;
	char chgbuf[25] ;

	if (old_pp_p == (pwproc_t *)NULL) {
		chgstr= ename ;
	}
	else {
		sprintf(chgbuf, "(+%ld)", (new_pp_p->secs - old_pp_p->secs)) ;
		chgstr= chgbuf ;
	}

	printf(
		"%-10s %-10s p%-5ld s%-5ld %s\n",
		chgstr,
		new_pp_p->user,
		new_pp_p->pid,
		new_pp_p->secs,
		new_pp_p->name
	) ;
}
