#include	<stdio.h>
#include	<ctype.h>

void main()
{
	int c ;

	while((c= getchar()) != EOF) {
		putchar(tolower(c)) ;
	}
}
	
