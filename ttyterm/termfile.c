/*

	terminal driver - file transfer

*/

#include	<stdio.h>
#include	<fcntl.h>
#include	<signal.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"uterm.h"
#include	"utset.h"
#include	"term.h"
#include	"ttyio.h"

#define		USE_FSDIR		0

#if	USE_FSDIR
#include	<sys/dir.h>		/* using sysV FS- file system */
#else
#include	<dirent.h>		/* using file system indep rtn */
#endif

extern int stopflag ;
extern struct termopts term_o ;

#if 0
void do_sfile()
{
	char lfnbuf[257], rfnbuf[257]; 

	fprintf(stderr, "\n") ;
	fprintf(stderr, "--Send local %s to remote system:\n",
		term_o.xfmode) ;
	if (getxfn(lfnbuf, rfnbuf)) return ; 
	std_sendfile(lfnbuf, rfnbuf) ;
}

void do_rfile()
{
	char lfnbuf[257], rfnbuf[257]; 

	fprintf(stderr, "\n") ;
	fprintf(stderr, "--Receive %s from remote system:\n",
		term_o.xfmode) ;
	if (getxfn(lfnbuf, rfnbuf)) return ;
	std_rcvfile(rfnbuf, lfnbuf) ;
}
#endif

void do_upldir()
{
#if	OPSYS==OPUNIX
#if	OPSYSLVL==OPLSYSV
#if	USE_FSDIR
	int fh, len ;
	struct direct dirent ;

	fprintf(stderr, "\n") ;

	if ((fh= open(".", O_RDONLY))<= 0) {
		fprintf(stderr, "--Can't open directory\n") ;
		return ;
	}

	for (;;) {
		len= read(fh, (char *)&dirent, sizeof(dirent)) ;
		if (len != sizeof(dirent)) {
			if (len==0) break ;
			fprintf(stderr, "--Can't read directory\n") ;
			break ;
		}
		if (dirent.d_ino==0) continue ;
		if (strcmp(dirent.d_name, ".")==0) continue ;
		if (strcmp(dirent.d_name, "..")==0) continue ;
		if (std_sendfile(dirent.d_name, dirent.d_name)) break ;
	}

	close(fh) ;

#else

	DIR *fdp ;
	struct dirent *dep ;
	
	fprintf(stderr, "\n") ;
	if ((fdp= opendir(".")) == (DIR *)NULL) {
		fprintf(stderr, "--Can't open directory\n") ;
		return ;
	}

	for (;;) {
		if ((dep= readdir(fdp)) == (struct dirent *)NULL) {
			break ;
		}
		if (dep->d_ino==0) continue ;
		if (strcmp(dep->d_name, ".")==0) continue ;
		if (strcmp(dep->d_name, "..")==0) continue ;
		if (std_sendfile(dep->d_name, dep->d_name)) break ;
	}

	closedir(fdp) ;
#endif
#endif
#endif
}

void do_dnldir()
{
#if	OPSYS==OPUNIX
#if	OPSYSLVL==OPLSYSV
	register int i ;
	char *lp, list[10240] ;
	char xffn[257] ;
	int nfiles, fno ;

	fprintf(stderr, "\n") ;
	fprintf(stderr, "--Obtaining file list...\n") ;
	if ((nfiles= getlist(list, sizeof(list)))< 0) {
		fprintf(stderr, "--list aborted--\n") ;
		return ; 
	}
	
	fprintf(stderr, "--Transferring %d files...\n", nfiles) ;
	for (fno= 1, lp= list; (*lp) != '\0'; fno++) {
		for (i= 0; i< (sizeof(xffn)-1);) {
			xffn[i++]= (*lp++) ;
			if ((*lp)=='\n') {
				lp++ ;
				break ;
			}
		}	
		xffn[i]= '\0' ;
		fprintf(stderr,"--Remote file %d/%d: %s\n",fno,nfiles,xffn) ;
		if (std_rcvfile(xffn, xffn)) break ;
		if (stopflag) break ;
	}
	if (stopflag) {
		fprintf(stderr, "--Transfer aborted at #%d: %s\n", fno, xffn) ;
	}

#endif
#endif
}

static int gettc_to_flag ;

static void gettc_to(i)
int i ;
{
	gettc_to_flag= 1 ;
}

static int gettc()
{
	int c ;

	gettc_to_flag= 0 ;
	signal(SIGALRM, gettc_to) ;
	alarm(3) ;
	while(gettc_to_flag==0) {
		if ((c= t_inc()) != (-1)) {
			alarm(0) ;
			return(c) ;
		}
	}
	return(-1) ;
}

static void putts(s)
char *s ;
{
	t_outs(s, strlen(s)) ;
}

static int getlist(lp, ml)
char *lp ;
int ml ;
{
	char ls_cmd[256] ;
	int i, ch, lastch, intext, done, nfiles ;

	STD_TERM_ON() ;

	putts("(ls; echo '>')\n") ;
		
	i= 0;
	nfiles= 0 ;
	lastch= -1 ;
	done= intext= 0 ;

	while(stopflag==0) {
		if ((ch= gettc()) == (-1)) break ;
		if (done) continue ;
		if (!intext) {
			if ((ch=='\n') || (ch=='\r')) {
				intext= 1 ;
			}
			continue ;
		}
		if (ch=='>') {
			done= 1 ;
			continue ;
		}
		if ((i+1)>= ml) {
			fprintf(stderr,
				"--filename list too long\n") ;
			nfiles= -1 ;
			break ;
		}
		switch(ch) {
			case '@':
			case '\t': case ' ':
			case '\r': case '*':
				ch= '\n' ;
				break ;
		}
		if (ch=='\n' && (lastch==ch || lastch==(-1))) {
			continue ;
		}
		else if (ch=='\n' && lastch=='/') {
			i-- ;
		}
		if (ch=='\n') nfiles++ ;
		lp[i++]= (char) ch ;
		lastch= ch ;
	}
	lp[i]= '\0' ;

	if (stopflag) {
		stopflag= 0 ;
		return(-1) ;
	}

	STD_TERM_OFF() ;
	return(nfiles) ;
}
