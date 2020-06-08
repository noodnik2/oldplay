/*
	print.c

	done to prevent I/O's from happening in middle 
	of data gathering procedures, possibly influencing
	the results.

*/

#include	<stdio.h>

#include	"compiler.h"

static char print_buf[10240] ;

void print_clear()
{
	print_buf[0]= '\0' ;
}

void print_add(s)
char *s ;
{
	strcat(print_buf, s) ;
}

void print_dump()
{
	printf("%s", print_buf) ;
}
