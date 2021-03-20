/*	Revision:	7
*/
/*

	list			file list management
				Copyright (C) 1990-1992, by Marty Ross


	sloppy; uses malloc(3) now.  should use own memory allocator
	with garbage collection since makes use of small fragments.

		struct fl_line *makelist(dn, &n, &s, &rc) ;	expand dir
		char *dn ;
		int n ;

		struct fl_line *getlist(nrv, rv, &n, &s, &rc) ;	get list
		int nrv ;
		char *rv[] ;

		struct fl_line *fl_lofs(index) ;		get list item
		int index ;

		void fl_scmd(flp, value) ;			set file cmd

		char *fl_gcmd(flp) ;				get file cmd

*/

#include	<stdio.h>
#include	<fcntl.h>

#include	"compiler.h"
#include	"flist.h"
#include	"dirlist.h"

extern struct fl_arg fla ;
extern struct fl_opt flo ;

static int toomany();
static int notadir();
static struct fl_line *i_xpandir();
static int xpand();

struct fl_line *i_xpandir();

extern char *malloc() ;

struct fl_line *makelist(dn, np, tsp, rcp)
char *dn ;
int *np, *rcp ;
long *tsp ;
{
	struct fl_line *flp ;

	flp= i_xpandir(dn, np, tsp, rcp);
	return(flp);
}

struct fl_line *getlist(nrv, rv, np, tsp, rcp)
int nrv, *np, *rcp ;
char *rv[] ;
long *tsp ;
{
	int i, n ;
	struct fl_line *flist, *elist, *lp ;
	long totsize ;

	n= 0 ;
	totsize= 0L ;
	flist= elist= (struct fl_line *)NULL ;
	(*rcp)= 0 ;

	for (i= 0; i< nrv; i++) {

		if (
			(lp= (struct fl_line *)malloc(sizeof(struct fl_line)))
			== NULL
		) {
			(*rcp)= toomany() ;
			break ;
		}

		if (flist == (struct fl_line *)NULL) {
			flist= elist= lp;
		}
		else {
			(elist->next)= lp;
			elist= lp ;
		}

		(elist->line)= rv[i] ;
		(elist->next)= (struct fl_line *) NULL ;
		(elist->cmd)= (struct fl_cmd *) NULL ;
		n++ ;

		fl_diag(elist) ;

		if (!(elist->stat.isdir)) {
			totsize+= (elist->stat.size) ;
			continue ;
		}
		
		if ((*rcp)= xpand(&elist, &n, &totsize)) break ;
	}
	
	(*tsp)= totsize ;
	(*np)= n ;

	return(flist);
}

struct fl_line *fl_lofs(topline)
int topline;
{
	register struct fl_line *flp ;
	register int i;

	flp= FL_LIST ;
	for (i= 0; (flp != (struct fl_line *)NULL) && (i< topline); i++) {
		flp= (flp->next) ;
	}
	return(flp) ;
}

void fl_scmd(flp, buf)
struct fl_line *flp ;
char *buf;
{
	struct fl_cmd *fcp ;

	if (flp != NULL) {
		if ((fcp= (flp->cmd)) != NULL) {
			free((char *)fcp) ;
			flp->cmd= NULL ;
		}
		while((buf[0] == ' ') || (buf[0] == '\t')) buf++ ;
		if (buf[0] != '\0') {
			fcp= (struct fl_cmd *)malloc(sizeof(struct fl_cmd)) ;
			if (fcp == NULL) {
				flserr(19, "can't get memory for command", 2) ;
				return ;
			}
			fcp->rc= (-9999) ;
			strcpy(fcp->cmdline, buf) ;
			flp->cmd= fcp ;
		}
	}
}

char *fl_gcmd(flp)
struct fl_line *flp ;
{
	if (flp != NULL) {
		if ((flp->cmd) != NULL) {
			return((flp->cmd)->cmdline) ;
		}
	}
	return((char *)NULL) ;
}

int fl_diag(flp)
struct fl_line *flp ;
{
	int rc ;

	if (rc= statfile(flp->line, &flp->stat)) {
		flp->stat.size= 0L ;
		flp->stat.date= 0L ;
		flp->stat.isdir= 0 ;
	}
	return(rc) ;
}

/*
	static subroutines
*/

static struct fl_line *i_xpandir(dn, np, tsp, rcp)
char *dn ;
int *np, *rcp ;
long *tsp ;
{
	int i, b, n, pp ;
	struct fl_line *flist, *elist, *rlist, *lp ;
	DIRLIST_T *dl_p ;
	DIRENTRY_T *de_p ;
	char fname[FL_FNSIZE+1] ;
	unsigned char *p ;
	long totsize ;

	n= 0 ;
	(*rcp)= 0 ;
	elist= flist= (struct fl_line *)NULL ;
	totsize= 0L ;
	pp= (strcmp(dn, ".") != 0) ;

	if (dirlist_init(dn, &dl_p)) {
		(*rcp)= notadir(dn) ;
		(*tsp)= totsize ;
		(*np)= n ;
		return(flist) ;
	}

	for (i= 1; dirlist_read(dl_p, &de_p) == 0; i++) {

		if (!flo.o_all) {
		/*
			UNIX:	omit all files beginning with '.'
			MSDOS:	omit files: "." and ".."
		*/
			if (de_p->d_name[0] == '.') continue ;
		}

		if (de_p->d_name[0] == PATHSEP_CH) { /* absolute: leave alone */
			strcpy(fname, de_p->d_name) ;
		}
		else {
			if (pp) {			/* if not dir==. */
				strcpy(fname, dn) ;
				strcat(fname, PATHSEP_CHA) ;
				strcat(fname, de_p->d_name) ;
			}
			else {
				strcpy(fname, de_p->d_name) ;
			}
		}

		b= sizeof(struct fl_line) ;
		b+= (((strlen(fname)+1)+3) & ~3) ;

		if ((p= (unsigned char *)malloc(b)) == NULL) {
			(*rcp)= toomany() ;
			break ;
		}

		memcpy(	p + sizeof(struct fl_line),
			fname,
			b - sizeof(struct fl_line)
		);

		lp= (struct fl_line *) p ;

		lp->line= (char *)(p + sizeof(struct fl_line)) ;
		lp->next= (struct fl_line *) NULL ;
		lp->cmd= (struct fl_cmd *) NULL ;
		if (fl_diag(lp)) {
			free((char *)lp) ;
			continue ;
		}

		if (flist == (struct fl_line *)NULL) flist= lp ;
		else (elist->next)= lp ;

		elist= lp ;
		n++ ;

		if (!(elist->stat.isdir)) {
			totsize+= (elist->stat.size) ;
			continue ;
		}

		if ((*rcp)= xpand(&elist, &n, &totsize)) break ;
	}

	dirlist_fini(dl_p) ;

	(*tsp)= totsize ;
	(*np)= n ;

	return(flist) ;
}

static int xpand(listpp, nnp, tsp)
struct fl_line **listpp ;
int *nnp ;
long *tsp ;
{
	struct fl_line *elist, *rlist ;
	long ns ;
	int rc, nn ;

	if (!flo.o_recur) return(0) ;

	elist= (*listpp) ;
	rlist= i_xpandir(elist->line, &nn, &ns, &rc) ;
	if ((rlist == NULL) || (rc != 0)) return(rc) ;

	(elist->next)= rlist ;
	while((rlist->next) != NULL) {
		rlist= (rlist->next) ;
	}
	(*listpp)= rlist ;
	(*tsp)+= ns ;
	(*nnp)+= nn ;

	return(0) ;
}

static int notadir(dn)
char *dn ;
{
	char emsg[FL_FNSIZE+25] ;

	sprintf(emsg, "not a directory: %s", dn) ;
	clerror(emsg) ;
	return(1) ;
}

static int toomany()
{
	clerror("too many files") ;
	return(2) ;
}			
