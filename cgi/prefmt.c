#include <stdio.h>

static char szBuf[4096] ;

void main()
{
#if 0
	while(gets(szBuf)) {
		printf("<pre>%s</pre>\n", szBuf) ;
	}
#else
	printf("<pre>\n") ;
	while(gets(szBuf)) {
		printf("%s\n", szBuf) ;
	}
	printf("</pre>\n") ;
#endif
}
