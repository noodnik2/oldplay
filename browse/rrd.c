/*

	rrd.c				Read records from a file
	     				Copyright (C) 1990-1992, by Marty Ross

	rc= rrdi(fn);
	eof= rrd(recno, recbuf, sizeof(recbuf), &len);
	void rrdf() ;

*/

#include	<stdio.h>
#include	<fcntl.h>

#include	"compiler.h"
#include	"browse.h"

extern struct br_arg bra ;
extern struct br_opt bro ;
extern struct br_scn brs ;

static int rrd_fh ;			/* current file handle   */
static long rrd_crn ;			/* current record number */
static long rrd_cp ;			/* current file position */

long lseek() ;

#if	OPSYS==OPMSDOS
#define		RDO_MODE	(O_RDONLY | O_BINARY)
#else
#define		RDO_MODE	(O_RDONLY)
#endif

int rrdi(fn)
char *fn;
{
	int fh ;

	if ((fh= open(fn, RDO_MODE))<= 0) return(1) ;
	rrd_crn= 0L ;
	rrd_cp= 0L ;
	rrd_fh= fh ;
	return(0) ;
}

int rrd(recno, recbuf, recsize, lp)
long recno ;
int recsize, *lp ;
char *recbuf ;
{
	int fl ;

	if (bro.o_debug) {
		fprintf(stderr, "rrd wants: %ld\n", recno) ;
	}

	if (recno == rrd_crn) return(rdbuf(recbuf, recsize, lp)) ;

	if (recno == 0L) {
		rrd_cp= 0L ;
		rrd_crn= 0L ;
		return(rdbuf(recbuf, recsize, lp)) ;
	}

	while(recno> rrd_crn) if (rdskip(1, recbuf, recsize)) break ;
	while(recno< rrd_crn) if (rdskip(-1, recbuf, recsize)) break ;

	if (bro.o_debug) {
		fprintf(stderr, "rrd has: %ld\n", rrd_crn) ;
	}

	return(rdbuf(recbuf, recsize, lp)) ;
}

void rrdf()
{
	if (rrd_fh != 0) {
		close(rrd_fh) ;
		rrd_fh= 0 ;
	}
}

/*
	static routines
*/

int rdbuf(recbuf, recsize, rlp)
char *recbuf ;
int recsize, *rlp ;
{
	int rc, rl, len, fl ;

	if (rc= rfrec(recbuf, recsize, &rl, &fl, &len)) return(rc) ;
#if	OPSYS==OPMSDOS
	if (!BINMODE) {
		if (rl> 0) {
			if ((recbuf[rl] == '\n') && (recbuf[rl-1] == '\r')) {
				rl-- ;
			}
		}
	}
#endif
	recbuf[rl]= '\0' ;
	(*rlp)= rl ;
	if (len == 0) return(1) ;		 	/* eof */
	rrd_cp+= (long) fl ;				/* next rec */
	rrd_crn++ ;					/* advance  */
	return(0) ;
}
/*

	rc= rdskip(n, recbuf, recsize);

	this routine skips forward or backward 1 record (according
	to the sign of n) in the open file, reading the record into
	recbuf, for a maximum size of recsize.

	only two valid values for n: -1 and 1.

	returns 0 if worked properly

*/
static int rdskip(n, recbuf, recsize)
int n, recsize;
char *recbuf ;
{
	long newpos ;
	int rc, i, j, len ;

	switch(n) {

		case 1:
			if (rc= rfrec(recbuf, recsize, &j, &i, &len)) {
				return(rc) ;
			}
#if 0
			if (i == 0) return(1) ;		/* eof */
#endif
			if (len == 0) return(1) ;	/* eof */
			rrd_cp+= (long) i ;	
			rrd_crn++ ;
			break;

		case -1:
			if (rrd_crn == 0) return(1) ;	/* tof */

			newpos= rrd_cp ;
			newpos-= (long) (j= recsize) ;
			if (newpos< 0L) {
				j+= (int) newpos ;
				newpos= 0L ;
			}	
			if (lseek(rrd_fh, newpos, 0)< 0L) return(-1) ;
			if (!BINMODE) {
				len= read(rrd_fh, recbuf, j) ;
				for (i= len-1; i> 0; i--) {	
					if (recbuf[i-1] == '\n') break;
				}
				rrd_cp= newpos ;
				rrd_cp+= (long) i ;
			}
			else {
				rrd_cp= newpos ;				
			}
			rrd_crn-- ;
			if (bro.o_debug) {
				fprintf(
					stderr,
					"-1: now%ld, at%ld\n",
					rrd_crn,
					rrd_cp
				) ;
			}
			break;

		default:
			return(-2);

	}
	return(0) ;
}

static int rfrec(recbuf, recsize, rlp, flp, xp)
char *recbuf ;
int recsize, *rlp, *flp, *xp ;
{
	int i, j, len ;

	if (lseek(rrd_fh, rrd_cp, 0)< 0L) return(-1) ;
	i= j= len= read(rrd_fh, recbuf, recsize) ;
	if (!BINMODE) {
#if 0
		for (i= 0; i< len;) {
			if (recbuf[i++] == '\n') break;
		}
#else
		for (i= j= 0; i< len; i++) {
			if (recbuf[j++] == '\n') break;
		}
		
#endif
	}

	(*rlp)= i ;
	(*flp)= j ;					/* ret len */
	(*xp)= len ;

	return(0);
}
