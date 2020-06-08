
#include	<stdio.h>
#include	"compiler.h"

void rtrim(s)
char *s ;
{
	char *p ;

	for (p= s + strlen(s); p> s; p--) {
		if ((*(p-1)) != ' ') break ;
	}
	(*p)= '\0' ;
}
