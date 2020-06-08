#include	<stdio.h>
#include	"compiler.h"

void scan() ;
void mkfext() ;
char *strim() ;
char *dequote() ;

void main(argc, argv)
int argc;
char *argv[];
{
	int i ;

	for (i= 1; i< argc; i++) {
		scan(argv[i]) ;
	}
}

static void scan(fn)
char *fn;
{
	FILE *fp ;
	char rec[1025], *s ;
	char ofn[250] ;

	strcpy(ofn, fn) ;
	mkfext(ofn, "o") ;
	printf("%s:\t", ofn) ;
	
	if ((fp= fopen(fn, "r")) == (FILE *)NULL) {
		printf("(can't open)\n") ;
		return ;
	}

	while(fgets(rec, sizeof(rec), fp) != NULL) {
		s= strim(rec) ;	
		if (s[0]=='#') {
			s= strim(s+1) ;
			if (memcmp(s, "include", 7)==0) {
				s= strim(s+7);
				if (s[0]=='"') {
					printf("%s ", dequote(s)) ;
				}
			}
		}
	}
	
	fclose(fp) ;
	printf("\n\n") ;

}

char *strim(s)
char *s;
{
	register int i;

	while((s[0]==' ') || (s[0]=='\t')) s++;
	for (i= strlen(s); i> 0; i--) {
		if ((s[i-1]!=' ') && (s[i-1]!='\t')) break;
	}
	s[i]= '\0';
	return(s);
}

char *dequote(s)
char *s;
{
	int i ;

	if (s[0]=='"') {
		for (i= 1; s[i] != '\0'; i++) {
			if (s[i]=='"') {
				s[i]= '\0' ;
				break;
			}	
		}
		return(s+1) ;
	}
	return(s) ;
}

void mkfext(fn, ext)
char *fn, *ext ;
{
	register int i ;

	for (i= 0; fn[i] != '\0'; i++) {
		if (fn[i]=='.') break;
	}
	fn[i++]= '.' ;
	strcpy(fn+i, ext) ;
}
