#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	<sys/stat.h>
#include	<unistd.h>

char szBuf[1024] ;
static void getname(char *pInput, char *pszName) ;
static void getvar(char *pInput, char *pszVar) ;
static long getsize(char *pszFileName) ;

main()
{
	char *p ;
	char szName[128] ;
	long ls ;

	while(gets(szBuf)) {
		for (p= szBuf; p[0]; p++) {
			if (!isspace(p[0])) break ;
		}
		if (p[0] != ';') {
			if (p[0] == '[') {
				getname(p, szName) ;
				ls= getsize(szName) ;
				printf("%s\n", p) ;
				if (ls>= 0) {
					printf("filesize=%ld\n", ls) ;
				}
				continue ;
			}
			getvar(p, szName) ;
			if (strcmp(szName, "filesize") == 0) {
				continue ;
			}
		}
		printf("%s\n", szBuf) ;
	}
}

static void getname(char *pInput, char *pszName)
{
	char *p ;
	int iLen ;

	pszName[0]= '\0' ;
	p= strchr(pInput, ']') ;
	if (!p) return ;
	iLen= (int) (p - pInput) - 1 ;
	memcpy(pszName, pInput + 1, iLen) ;
	pszName[iLen]= '\0' ;
}

static void getvar(char *pInput, char *pszVar)
{
	char *p ;
	int iLen ;

	pszVar[0]= '\0' ;
	p= strchr(pInput, '=') ;
	if (!p) return ;
	iLen= (int) (p - pInput) ;
	memcpy(pszVar, pszVar, iLen) ;
	pszVar[iLen]= '\0' ;
}

static long getsize(char *pszFileName)
{
	struct stat statbuf ;

	if (stat(pszFileName, &statbuf)< 0) {
		perror(pszFileName) ;
		return(-1L) ;
	}
	return(statbuf.st_size) ;
}

/* end */
