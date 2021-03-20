/*


   rddstr.c				Read Data String File	
					Copyright (C) 1989, by ==C=R=C==	


	rc= rddstri(fname, magicno);		Opens the string data file.

	rc= rddstr(sa, si, buf, buflen);	Reads string from open file.

	rc= rddstrf();				Closes the string data file.



   Reads file of form:

	--- top of file ---

	<magic>					HILO short file magic number
	<byteorder>				HILO short byte order indicator
	<n_arrays>				short number of arrays
	<dim_a1>				short first array dimension
	..
	<dim_a<n_arrays>>			short last array dimension
	<string_a1.loc0>			short rel. seek pos. of strings
	..
	<string_a1.loc<dim_a1>>
	..
	<string_a<n_arrays>.loc0>
	..
	<string_a<n_arrays>.loc<dim_a1>>

	--- start of string data ---
	
	<string data>				containing only ASCII text


*/

#include	<stdio.h>
#include	<fcntl.h>

#include	"compiler.h"
#include	"rddstr.h"


#if	OPSYS!=OPMSDOS
#define	O_BINARY	0
#endif

static struct rdstrdta rdd= { 0 } ;

static int rd(), rdi() ;
static int rdf();
static unsigned short rdpos() ;
static int rdseek();
static int rdstbl();
static int i_rddstri();

extern long lseek() ;
extern void bswap() ;

int rddstri(fn, magic)
char *fn;
short unsigned magic;
{
	int rc ;
						
	if (rdi(fn)) return(-1);		/* can't open */

	if (rdd.nopen==1) {				
		if (rc= i_rddstri(magic)) {
			rdf() ;
			return(rc) ;		/* can't read */
		}
	}

	return(0);
}

int rddstr(sa, si, buf, bufsize)
int sa, si ;
char *buf;
int bufsize ;
{
	int spos, pos, len, idx ;

						/* out of range */
	if (sa>= (int)rdd.num)				return(-11);
	if (si>= (int)rdd.dim[sa])			return(-12);

	idx= rdstbl(sa) + si ;
	spos= (int)rdd.spos ;

	pos= (int)rdd.data[idx] ;
	len= (int)rdd.data[idx+1] - pos ;

	    					/* msg too large */
	if (len>= bufsize)				return(-13);

						/* can't seek */
	if (rdseek((short unsigned)(pos+spos)))		return(-14);

	if (len< 0) {
		fprintf(stderr, "pos=%d+%d, len=%d\n", spos, pos, len);
	}

						/* can't read */
	if (rd(buf, len))				return(-15);

	buf[len]= '\0' ;
	return(0) ;
}

int rddstrf()
{
	return(rdf()) ;
}

/*

	static subroutines 

*/

static int i_rddstri(magic)
short unsigned magic ;
{
	register int i, j ;
	int k ;
	short unsigned s_val ;

				/* can't read */
	if (rd(&rdd.magic, sizeof(rdd.magic)))		return(-2);
	if (rd(&rdd.border, sizeof(rdd.border)))	return(-3);
	if (rd(&rdd.num, sizeof(rdd.num)))		return(-4);

#if	BYTEORDER!=HILO
	bswap(&rdd.magic, sizeof(rdd.magic)) ;
	bswap(&rdd.border, sizeof(rdd.border)) ;
#endif

				/* bad magic */
	if (magic != rdd.magic)				return(-5);

	if (rdd.swap= (rdd.border != (short unsigned)BYTEORDER)) {
		bswap(&rdd.num, sizeof(rdd.num)) ;
	}

				/* file too large */
	if (rdd.num> N_AMAX)				return(-6);

	i= 0;
	while(i< (int)rdd.num) {			/* can't read */
		if (rd(&s_val, sizeof(s_val))) return(-7) ;
		if (rdd.swap) bswap(&s_val, sizeof(s_val)) ;
		rdd.dim[i++]= s_val ;
	}

	k= 0;

	for (i= 0; i< (int)rdd.num; i++) {
		for (j= 0; j< (int)rdd.dim[i]; j++) {
			if (rd(&s_val, sizeof(s_val)))	return(-7) ;
			if (rdd.swap) bswap(&s_val, sizeof(s_val)) ;
			rdd.data[k++]= s_val ;
		}
	}

	if (rd(&s_val, sizeof(s_val)))			return(-7) ;
	if (rdd.swap) bswap(&s_val, sizeof(s_val)) ;
	rdd.data[k++]= s_val ;

	rdd.spos= rdpos() ;
	return(0);
}

static int rdi(fn)
char *fn ;
{
	int fh ;

	if (rdd.nopen==0) {
		if ((fh= open(fn, O_RDONLY|O_BINARY))< 0) return(-1);
		rdd.ifh= fh ;
	}
	rdd.nopen++ ;
	return(0);
}

static int rdf()
{
	if (rdd.nopen!=0) {
		if (rdd.nopen==1) {
			close(rdd.ifh) ;
			rdd.ifh= 0 ;
		}
		rdd.nopen-- ;
		return(0) ;
	}
	return(-1);
}

static int rd(s, l)
unsigned char *s;
int l;
{
	if (l<0) return(-1);
	if (l>0) {
		if (read(rdd.ifh, s, l) != l) {
			return(-1);
		}
	}
	return(0);
}

static unsigned short rdpos()
{
	return((unsigned short)lseek(rdd.ifh, 0L, 1)) ;
}

static int rdseek(pos)
short unsigned pos ;
{
	long lpos ;

	lpos= (long) pos ;
	if (lseek(rdd.ifh, lpos, 0) == -1L) {
		fprintf(stderr, "lseek(%d, %ld, -1) error\n", rdd.ifh, lpos);
		return(-1);
	}
	return(0);
}

static int rdstbl(sa)
int sa;
{
	register int i, k;

	k= 0;
	for (i= 0; i< sa; i++) {
		k+= rdd.dim[i];
	}
	return(k);
}
