
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

static void pdstat_flags() ;
static void pdstat_whystopped() ;

extern struct opts o ;

void pdstat(pdap)
pdd_args *pdap ;
{
	prstatus_t pstat ;

	if (ioctl(pdap->pidfh, PIOCSTATUS, &pstat)< 0) {
		pdfnerror(pdap, "can't PIOCSTATUS") ;
		return ;
	}
	print_add("stat") ;
	pdstat_flags("st flags", pstat.pr_flags) ;
	if (pstat.pr_flags & PR_STOPPED) {
		pdstat_whystopped(pstat.pr_why, pstat.pr_what) ;
	}
	if (o.detail) {
		pdpr_timestruc("usr time", &pstat.pr_utime) ;
		pdpr_timestruc("sys time", &pstat.pr_stime) ;
		pdpr_2str("schedule", pstat.pr_clname, 8, NULL, 0) ;
	}
}

struct {
	long flag ;
	char *name ;
} pds_flags[]= {
	{ PR_STOPPED, 	"stopped"	},
	{ PR_ISTOP,	"istop"		},
	{ PR_DSTOP,	"dstop"		},
	{ PR_ASLEEP,	"asleep"	},
	{ PR_FORK,	"fork"		},
	{ PR_RLC,	"rlc"		},
	{ PR_PTRACE,	"ptrace"	},
	{ PR_PCINVAL,	"pcinval"	},
	{ PR_ISSYS,	"issys"		},
} ;
#define	N_PDS_FLAGS	(sizeof(pds_flags)/sizeof(pds_flags[0]))

static void pdstat_flags(msg, flags)
char *msg ;
long flags ;
{
	int i ;

	pdpr_ttl(msg) ;
	for (i= 0; i< N_PDS_FLAGS; i++) {
		if (flags & pds_flags[i].flag) {
			print_add(" ") ;
			print_add(pds_flags[i].name) ;
		}
	}
	print_add("\n") ;
}

struct {
	short why ;
	char *name ;
	int what_valid ;
} why_reasons[]= {
	{ PR_REQUESTED,	"requested", 0	},
	{ PR_SIGNALLED,	"signalled", 1	},
	{ PR_FAULTED,	"faulted", 1	},
	{ PR_SYSENTRY,	"entry", 1	},
	{ PR_SYSEXIT,	"exit", 1	},
	{ PR_JOBCONTROL,"jobcontrol", 1	},
} ;
#define	N_WHY_REASONS	(sizeof(why_reasons)/sizeof(why_reasons[0]))

static void pdstat_whystopped(why, what)
short why, what ;
{
	int i ;
	char nmsg[65] ;

	pdpr_ttl("reason stopped") ;
	for (i= 0; i< N_WHY_REASONS; i++) {
		if (why == why_reasons[i].why) {
			print_add(" ") ;
			print_add(why_reasons[i].name) ;
			if (why_reasons[i].what_valid) {
				sprintf(nmsg, "(%d)", what) ;
				print_add(nmsg) ;
			}
		}
	}
	print_add("\n") ;	
}
