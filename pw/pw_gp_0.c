/*
	pw_gp_p0.c

	"Generic" process read
*/

#include	<stdio.h>

#include	"compiler.h"
#include	"pw.h"
#include	"pw_proc.h"
#include	"pw_scan.h"

#define		TFN_SIZE	128

extern struct opts o ;
extern struct parms p ;

static void loadstr() ;

#define		PSLINE_SIZE	132
static char ps_tempfn[TFN_SIZE+1] ;
pwscan_t *procscan_open_0()
{
	char pscmd[20+TFN_SIZE+1] ;
	char psline[PSLINE_SIZE+1] ;
	FILE *fp ;
	static pwscan_t psh ;

	sprintf(ps_tempfn, "/tmp/pw%ld", p.mypid) ;
	sprintf(pscmd, "ps -ef > %s", ps_tempfn) ;
	system(pscmd) ;
	if ((fp= fopen(ps_tempfn, "r")) == (FILE *)NULL) {
		return((pwscan_t *)NULL) ;
	}
	/* strip the header line */
	if (fgets(psline, PSLINE_SIZE, fp) == (char *)NULL) {
		fclose(fp) ;
		unlink(ps_tempfn) ;
		return((pwscan_t *)NULL) ;
	}
	psh.dp= (void *)fp ;
	return(&psh) ;
}

int procscan_read_0(psh_p, pp_p)
pwscan_t *psh_p ;
pwproc_t *pp_p ;
{
	char psline[PSLINE_SIZE+1] ;
	long pid, mm, ss ;
	int pslen ;

	if (fgets(psline, PSLINE_SIZE, (FILE *)psh_p->dp) == (char *)NULL) {
		fclose((FILE *)psh_p->dp) ;
		unlink(ps_tempfn) ;
		return(1) ;
	}
	pslen= strlen(psline) - 1 ;
	psline[pslen]= '\0' ;

	memset((char *)pp_p, '\0', sizeof(pwproc_t)) ;

	if (pslen> 0) {
		loadstr(pp_p->user, psline, PWPL_USER) ;
	}
	if (pslen> 47) {
		loadstr(pp_p->name, psline+47, PWPL_NAME) ;
	}
	if (pslen> 9) {
		if (sscanf(psline+9, "%5ld", &pid) == 1) {
			pp_p->pid= pid ;
		}
	}
	if (pslen> 39) {
		if (sscanf(psline+39, "%ld:%ld", &mm, &ss) == 2) {
			pp_p->secs= (mm * 60L) + ss ;
		}
	}
	return(0) ;
}

int procscan_close_0(psh_p, pp_p, nprocs)
pwscan_t *psh_p ;
pwproc_t *pp_p ;
int nprocs ;
{
	return(0) ;
}

/*
	begin static (internal) routines
*/

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
