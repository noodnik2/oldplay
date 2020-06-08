#include	<stdio.h>
#include	<fcntl.h>

#include	<sys/types.h>
#include	<sys/signal.h>
#include	<sys/fault.h>
#include	<sys/syscall.h>
#include	<sys/procfs.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"pd.h"

extern struct opts o ;
extern struct parms p ;

void pd_diag(pid)
long pid ;
{
	pdd_args pda ;

	pda.pid= pid ;
	sprintf(pda.pidfn, "%s%05ld", p.procdirpfx, pda.pid) ;
	if ((pda.pidfh= open(pda.pidfn, O_RDONLY))< 0) {
		pdfnerror(&pda, "can't open") ;
		return ;
	}

	print_clear() ;

	sprintf(pda.msgbuf, "----- pid=%ld -----\n", (long) pda.pid) ;
	print_add(pda.msgbuf) ;

	if (o.diagpsinfo) pdinfo(&pda) ;
	if (o.diagstate) pdstat(&pda) ;
	if (o.diagmem) pdmem(&pda) ;

	print_dump() ;

	close(pda.pidfh) ;
}

pdfnerror(pdap, msg)
pdd_args *pdap ;
char *msg ;
{
	sprintf(pdap->msgbuf, "%s: %s", msg, pdap->pidfn) ;
	clerror(pdap->msgbuf) ;
}
