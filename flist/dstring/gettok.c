#if 0
/*


   gettok.c				Parse a token from stdin
					Copyright (C) 1990, by ==C=R=C==	


	char *gettok();

		gets a token from the input stream.  if no token available
		returns (char *)NULL.

	void ungettok(tok);
	
		un-gets a token to the input stream.  the next time gettok()
		is called to get a token, a copy of the supplied token will
		be returned.

	void tokerr(number, message);

		prints error message on stderr, exits with number.


	Skips comments of format /* .. */.
	Gets or ungets space-delimited tokens.  If token starts with
	a quote character ("), all data following until the matching
	close quote is taken as string data.


	Restrictions:

		Token can be no larger than SIZE_IN bytes.


*/
#endif

#include	<stdio.h>
#include	<ctype.h>

#include	"../compiler.h"

#define		SIZE_IN		1023
#define		SIZE_OUT	1023

static char token[SIZE_IN+1] ;
static char saved_token[SIZE_IN+1] ;
static char saved_used= 0 ;
static long inputline= 0L ;

void ungettok(s)
char *s;
{
	if (s == (char *)NULL)	saved_token[0]= '\0';
	else			strcpy(saved_token, s);
	saved_used= 1;
}

char *gettok()
{
	int i, c ;
	int cf, q ;

	if (saved_used) {
		saved_used= 0;
		return(saved_token);
	}

	i= 0;
	cf= q= 0;
	while(i< sizeof(token)) {

		if ((c= gc())< 0) break ;

	again:

		if (isspace(c)) {		/* ignore white space */
			continue;
		}
		
		if (c=='/') {			/* could be a comment */
			if ((c= gc())< 0) break;
			if (c!='*') goto again ;
			cf++;
			continue;
		}

		if (cf>0) {
			if (c=='*') {
				if ((c= gc())< 0) break;
				if (c!='/') goto again ;
				cf--;
			}
			continue;
		}

		token[i++]= (char)c ;
		q= (c=='"' || c=='\'')? c: -1 ;
		while(i< sizeof(token)) {	/* inside string */
			if ((c= gc())< 0) break;
			if ((q==-1) && isspace(c)) break;
			else if (c==q) {
				token[i++]= (char)c ;
				break ;
			}
			else if (c=='\\') {
				if ((c= gc())< 0) break;
				if (c=='\n') continue;
				token[i++]= '\\' ;
			}
			token[i++]= (char)c ;
		}
		token[i++]= '\0';
		break;

	}
	return((c==EOF)? (char *)NULL: token);
}

/*

	int number;
	char *message;
	void tokerr(number, message);

	prints message to stderr.
	exits with number.

*/
void tokerr(number, message)
int number;
char *message;
{
	fprintf(
		stderr,
		"error %d at %d: %s\n",
		number, inputline, message
	);
	exit(number) ;
}

/*
	static subroutines 
*/

/*
	c= gc();

	get and return a character from input stream.
	if EOF, returns -1.

	tracks physical lines in global
	value 'inputline'.
*/
static int gc()
{
	int c ;

	if ((c= getchar()) == EOF) return(-1);
	if (c=='\n') inputline++;
	return(c);
}
