/*

	ebcdic.c			EBCDIC translation facility
					Copyright (C) 1989, by ==C=R=C==


	rc= ebcinit(table);		module function: call first
	flag= ebctrans(s, l);		module function: translate a buffer
	a= ebc2asc(e);			conversion macro: returns ASCII text
	flag= ebcistxt(e);		indicator macro: is 'e' EBCDIC text?
	flag= ebcisok(e);		indicator macro: is 'e' valid index?

*/

#define		TABLE_OWNER	1

#include	<stdio.h>
#include	"../compiler/compiler.h"
#include	"../ebcdic/ebcdic.h"

chau ebcatca[TABLE_SIZE] ;

static char def_tfn[]= {
	"/usr/local/bin/ebcdic.T"
} ;

/*

	int ebcinit(table);

	Initializes (loads) the ebcdic translation table.

	If return is 0, subsequent calls to ebc2asc(), 
	ebcistxt(), or ebctrans() may be made.

	(-1) 	indicates file could not be opened
		other values indicate parsing errors

*/	
int ebcinit(table)
char *table;
{
	FILE *fp;
	int i, n, val, index ;
	char item[10+1] ;
	char file[128+2] ;

	for (i= 0; i< TABLE_SIZE; i++) {
		TABLEW(i, A_MISSING);
	}

	strcpy(file, (table!=(char *)NULL)? table: def_tfn);

	if ((fp= fopen(file, "r")) == (FILE *)NULL) {
		return(-1);
	}

	while(  (n= fscanf(fp, "%d %s", &index, item)) == 2 ) {
		if ( (index< 0) || (index>= TABLE_SIZE) ) {
			fclose(fp);
			return(-2);
		}
		if ((val= mkval(item))< 0) {
			fclose(fp);
			return(-3);
		}
		TABLEW(index, val);
	}
	fclose(fp);

	return( 0 );
}

/*

	flag= ebctrans(buf, len);

	Returns 0 if all characters in buffer were translated
	successfully (were located in the table).  Returns 1
	if SOME characters were translated, and returns (-1)
	if NO characters were translated.

*/
int ebctrans(buf, len)
chau *buf;
int len;
{
	register int i;
	int e, a ;
	int some, all ;

	some= 0; all= 1;
	for (i= 0; i< len; i++) {
		e= (int)(buf[i]) ;
		if ((a= TABLER(e)) == A_MISSING) {
			all= 0;
#if 0
		fprintf(stderr, "%02x not in EBCDIC table\n", e);
#endif
		}
		else {
			some= 1;
			buf[i]= (chau)a;
		}
	}	
	return( all? 0: some? 1: -1 ) ;
}

static int mkval(item)
char *item;
{
	int e;

							/* get e */
	if (strlen(item)==1) e= (int)item[0];
	else if (sscanf(item, "%d", &e)!=1) return(-1);

							/* check e */
	if (ebcisok(e)) return(e);
	return(-1);
}
