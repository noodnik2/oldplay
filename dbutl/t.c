#include	<stdio.h>
#include	<ctype.h>

char szBuf[1024] ;

main()
{
	int bSaw ;
	char *p ;

	bSaw= 1 ;
	while(gets(szBuf)) {
		for (p= szBuf; p[0]; p++) {
			if (!isspace(p[0])) break ;
		}
		if (p[0] == '\0') continue ;
		if (p[0] != ';') {
			if (p[0] == '[') {
				if (!bSaw) {
					printf("nokeyword=1\n") ;
				}
				printf("\n") ;
				bSaw= 0 ;
			}
			else {
				bSaw= 1 ;
			}
		}
		printf("%s\n", szBuf) ;
	}
}

