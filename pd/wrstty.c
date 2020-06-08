#include	<stdio.h>
#include	"compiler.h"

wrstty(s)
char *s ;
{
	fprintf(stderr, "%s\n", s) ;
}
