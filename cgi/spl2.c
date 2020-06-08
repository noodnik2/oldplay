#include	<stdio.h>

void main()
{
	FILE *fp ;
	char *fmt ;
	char buf[256] ;
	int i ;

	for (i= 0;; i++) {
		if (!gets(buf)) break ;
		if (i & 1) { fp= stderr ; fmt= "%s\n" ; }
		else	   { fp= stdout ; fmt= "%s " ; }
		fprintf(fp, fmt, buf) ;
	}
	exit(0) ;
	/*NOTREACHED*/
}

