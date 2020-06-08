
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

void pdinfo(pdap)
pdd_args *pdap ;
{
	prpsinfo_t pinfo ;

	if (ioctl(pdap->pidfh, PIOCPSINFO, &pinfo)< 0) {
		pdfnerror(pdap, "can't PIOCPSINFO") ;
		return ;
	}
	print_add("psinfo") ;
#if 0
	pdpr_2str("name", pinfo.pr_fname, 16, pinfo.pr_psargs, PRARGSZ) ;
#else
	pdpr_2str("name", pinfo.pr_psargs, PRARGSZ, NULL, 0) ;
#endif
	pdpr_timestruc("usr+sys time", &pinfo.pr_time) ;
	pdpr_longdec("process image", pinfo.pr_size) ;
	pdpr_longdec("resident set", pinfo.pr_rssize) ;
	pdpr_longhex("ps flags", pinfo.pr_flag) ;
}
