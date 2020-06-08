/*
	pw_gp_1.c

	"Specific" process read
	Works on AT&T SYSV Unices

*/

#include	<stdio.h>
#include	<fcntl.h>
#include	<dirent.h>
#include	<pwd.h>
#include	<sys/types.h>
#include	<sys/syscall.h>
#include	<sys/procfs.h>

#include	"compiler.h"
#include	"pw.h"
#include	"pw_proc.h"
#include	"pw_scan.h"

extern struct opts o ;

#define		PFN_SIZE	65

static char proc_dir[]= "/proc" ;
static char proc_fmt[]= "%05ld" ;

static void loadstr() ;
static void loadcname() ;
static void rtrim() ;

pwscan_t *procscan_open_0()
{
	DIR *dp ;
	static pwscan_t psh ;

	if ((dp= opendir(proc_dir)) == (DIR *)NULL) {
		return((pwscan_t *)NULL) ;
	}
	psh.dp= (void *)dp ;
	return(&psh) ;
}

int procscan_read_0(psh_p, pp_p)
pwscan_t *psh_p ;
pwproc_t *pp_p ;
{
	long pid, mm, ss ;
	struct dirent *de_p ;

	do {
		de_p= readdir((DIR *)psh_p->dp) ;
		if (de_p == (struct dirent *)NULL) {
			closedir((DIR *)psh_p->dp) ;
			return(1) ;
		}
	} while(sscanf(de_p->d_name, proc_fmt, &pid) != 1) ;
	memset((char *)pp_p, '\0', sizeof(pwproc_t)) ;

	if (o.debug) fprintf(stderr, "pw1: pid=%ld\n", pid) ;
	pp_p->pid= pid ;
	return(0) ;
}

int procscan_close_0(psh_p, pp_a, nprocs)
pwscan_t *psh_p ;
pwproc_t *pp_a ;
int nprocs ;
{
	int i ;

	for (i= 0; i< nprocs; i++) {
		if (gpinfo(pp_a[i].pid, &pp_a[i])) {	/* can't get inf? */
			pp_a[i].pid= 0L ;		/* "delete" entry */
		}
		if (o.debug) fprintf(stderr, "pw2: pid=%ld\n", pp_a[i].pid) ;
	}
	
	return(0) ;
}

/*
	begin static (internal) subroutines
*/

static int gpinfo(pid, pp_p)
long pid ;
pwproc_t *pp_p ;
{
	int procfh ;
	prpsinfo_t pinfo ;
	struct passwd *pw_e ;

	char procfn[PFN_SIZE+1] ;

	strcpy(procfn, proc_dir) ;
	strcat(procfn, "/") ;
	sprintf(procfn + strlen(procfn), proc_fmt, pid) ;
	if ((procfh= open(procfn, O_RDONLY))< 0) {
		if (o.debug) fprintf(stderr, "pw: can't open: %s\n", procfn) ;
		return(1) ;
	}
	if (ioctl(procfh, PIOCPSINFO, &pinfo)< 0) {
		fprintf(stderr, "pw: can't ioctl proc=%ld\n", pid) ;
		return(2) ;
	}
	close(procfh) ;

	pp_p->pid= pid ;
	pp_p->secs= pinfo.pr_time.tv_sec ;

	loadcname(pp_p->name, PWPL_NAME, pinfo.pr_psargs, PRARGSZ) ;

	if ((pw_e= getpwuid(pinfo.pr_uid)) != (struct passwd *)NULL) {
		loadstr(pp_p->user, pw_e->pw_name, PWPL_USER) ;
	}
	else {
		sprintf(pp_p->user, "u%ld", (long) pinfo.pr_uid) ;
	}

	return(0) ;
}

static void loadstr(to, from, maxlen)
char *to, *from ;
int maxlen ;
{
	int i, j ;

	maxlen-- ;
	for (i= 0; i< maxlen; i++) {
		if (from[i] != ' ') break ;
	}
	for (j= 0; i< maxlen; i++, j++) {
		if (from[i] == ' ') break ;
		to[j]= from[i] ;
	}
	to[j]= '\0' ;
}

static void loadcname(to, maxlen, args, l_args)
char *to, *args ;
int maxlen, l_args ;
{
	int i ;
	char *p ;

	if (maxlen< l_args) l_args= maxlen ;
	for (p= to, i= 0; i< l_args; i++) p[i]= args[i] ;
	p[i]= '\0' ;
	rtrim(p) ;
}

static void rtrim(s)
char *s ;
{
	char *p ;

	for (p= s + strlen(s); p> s; p--) {
		if ((*(p-1)) != ' ') break ;
	}
	(*p)= '\0' ;
}
