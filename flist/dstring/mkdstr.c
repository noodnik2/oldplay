/*


   mkdstr.c			Make Data String File	
	   			Copyright (C) 1989-1992, by Marty Ross


   Accepted restrictions:

	1.) total size of the data string file can't exceed 32K.
	2.) length of a string can't exceed 1020 bytes.


   Reads file of form:

	--- top of file ---

	<magic>					decimal file magic number

	"first string of first array",		first string array data
	..
	"last string of first array"
	"first string of next array",
	..
	"last string of next array"
	..
	"last string of first array"
	"first string of last array",
	..
	"last string of last array"



   Writes file of form:

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


   Input strings must be enclosed in double quotation marks.  To
   represent double quotes, use \" in the string.  The strings
   may include normal C-like escape sequences and are continuned
   (as in C) with a trailing backslash.  Arrays are ended when
   there is no trailing comma after the closing quote.  

*/

#include	<stdio.h>
#include	<fcntl.h>

#include	"../compiler.h"
#include	"mkdstr.h"

struct mkdopt mkdo ;
struct mkddta mkdd ;

static void idata() ;
static void wr(), wri() ;
static int wrdstrs();
static int getstr();
static int rddstrs();

extern void bswap() ;

extern char *gettok() ;
extern void ungettok() ;
extern void tokerr() ;

extern char *malloc() ;

#define	SIZE_IN		1023		/* max. size of input token	*/
#define	SIZE_OUT	1023		/* max. size of processed token */

void main(argc, argv)
int argc;
char *argv[];
{
	int rc ;
							/* Default options */
	mkdo.debug= 0;
	mkdo.help= 0;
							/* Set options */
	mkdo.help= (argc> 1); 

							/* Print help */
	if (mkdo.help) {
		tokerr(1, "no arguments allowed");
	}
							/* Compile file: */
	idata() ;					/* initialize    */
	if ((rc= rddstrs()) == 0) {			/* read strings  */
		rc= wrdstrs() ;				/* write strings */
	} 

	if (rc) tokerr(rc, "final");
	exit(0) ;
}

/*
	void idata() ;

	initialize the program's global variables.

*/	
static void idata()
{
	int i, j ;

	if ((mkdd.heap= (unsigned char *)malloc(MAXA_SIZE))
		== (unsigned char *)NULL) {
		tokerr(1100, "not enough memory for heap");
	}
	
	mkdd.free= mkdd.heap ;
	for (i= 0; i< MAXN_ARRAY; i++) {
		for (j= 0; j< MAXA_DIM; j++) {
			mkdd.data[i][j]= (unsigned char *)NULL ;
		}
		mkdd.dim[i]= 0 ;
	}
}

/*
	rc= rddstrs();

		read the string array(s) from the input stream.

*/
static int rddstrs()
{
	char inbuf[SIZE_IN+1], outbuf[SIZE_OUT+1], *tok ;
	int ai, ad, oo, sl, rc ;
	int cont ;

	if ((tok= gettok()) == (char *)NULL) {
		tokerr(900, "can't read magic number") ;
	}

	if (sscanf(tok, "%d", &ai)!=1) {
		tokerr(901, "can't read magic number") ;
	}

	mkdd.magic= (unsigned short)ai ;
	rc= 1000 ;
	ai= ad= 0 ;

	while(getstr(inbuf, &cont)) {		/* get a string */
		oo= dsparse(inbuf, outbuf) ;
		if (((mkdd.free-mkdd.heap)+oo+1)>= MAXA_SIZE) {
			tokerr(700, "heap overflow") ;
		}

		memcpy(mkdd.free, outbuf, oo) ;
		mkdd.free[oo]= '\0';

		mkdd.data[ai][ad++]= mkdd.free ;
		mkdd.free+= (oo+1) ;

		if (!cont) {
			mkdd.dim[ai++]= ad ;
			rc= 0 ;				/* array complete */
			ad= 0 ;				/* set new array */
		}

	}

	mkdd.num= (rc==0)? ai: 0 ;
	return(rc) ;
}

/*
	rc= wrdstrs();

		write the string array(s) to the output stream.

*/
static int wrdstrs()
{
	register int i, j ;
	unsigned short s_val ;
	char *d_str ;

	if (mkdo.debug) {
		fprintf(stderr, "output: num=%d\n", (int)mkdd.num);
		for (i= 0; i< (int)mkdd.num; i++) {
			for (j= 0; j< (int)mkdd.dim[i]; j++) {
				fprintf(stderr,
					"%3d) \"%s\"\n",
					i+1, mkdd.data[i][j]
				);
			}
		}
	}

	wri() ;					/* open output file */

	s_val= (unsigned short) mkdd.magic ;
#if	BYTEORDER!=HILO
	bswap(&s_val, sizeof(s_val)) ;
#endif
	wr(&s_val, sizeof(s_val)) ;			/* write magic */

	s_val= (unsigned short) BYTEORDER ;
#if	BYTEORDER!=HILO
	bswap(&s_val, sizeof(s_val)) ;
#endif
	wr(&s_val, sizeof(s_val)) ;			/* write byteorder */
	
							/* write # of arrays */
	s_val= (short unsigned)mkdd.num ;
	wr(&s_val, sizeof(s_val)) ;

	for (i= 0; i< (int)mkdd.num; i++) {		/* write dimensions */
		s_val= (unsigned short) mkdd.dim[i] ;
		wr(&s_val, sizeof(s_val)) ;
	}

	s_val= (unsigned short) 0 ;			/* write offsets */
	wr(&s_val, sizeof(s_val)) ;
	for (i= 0; i< (int)mkdd.num; i++) {
		for (j= 0; j< (int)mkdd.dim[i]; j++) {
			s_val+= (unsigned short) strlen(mkdd.data[i][j]) ;
			wr(&s_val, sizeof(s_val)) ;
		}
	}

	for (i= 0; i< (int)mkdd.num; i++) {		/* write data */
		for (j= 0; j< (int)mkdd.dim[i]; j++) {
			d_str= (char *)mkdd.data[i][j] ;
			wr(d_str, strlen(d_str)) ;
		}
	}

	if (mkdd.num==0) {
		tokerr(102, "no data");
	}

	return(0);
}

/*
	char *s;
	int more;

	noteof= getstr(s, &more);

	returns FALSE value when end-of-stream is encountered.

	returns TRUE value if string read into "s". "more" is set
	to TRUE iff the next string belongs to the current array
	(there is a trailing comma).

*/
static int getstr(s, mp)
char *s;
int *mp;
{
	int m, l ;
	char *tok ;

	if ((tok= gettok()) == (char *)NULL) {
		(*mp)= 0;
		return(0);
	}

	m= 0;
	if (tok[0]!='"') strcpy(s, tok);
	else {
		strcpy(s, tok+1);
		if ((l= strlen(s))> 0) s[l-1]= '\0' ;
	}

	if ((tok= gettok()) != (char *)NULL) {
		m= (strcmp(tok, ",")==0) ;
		if  (!m) ungettok(tok);
	}

	(*mp)= m ;
	return(1);

}

static void wri()
{
	mkdd.ofh= 1 ;				/* use stdout for output */
#if	OPSYS==OPMSDOS
	setmode(mkdd.ofh, O_BINARY) ;
#endif
}

static void wr(s, l)
unsigned char *s;
int l;
{
	if (write(mkdd.ofh, s, l) != l) {
		fprintf(stderr, "EOF on output!\n");
		exit(1001);
	}
}
