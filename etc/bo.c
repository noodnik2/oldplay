#include		<stdio.h>

void main();
void stccpy();

static char *gnlgets() ;

void main()
{
	char buf[10240], cfn[129];
	FILE *of ;
	int l ; 

	for (of= (FILE *)NULL; gnlgets(buf) != (char *)NULL; ) {
		if (memcmp(buf, "#tshdski end ", 13)==0) {
			if (of) {
				if (strcmp(buf+13, cfn) != 0) {
					fprintf(
						stderr, 
						"nonmatch: begin %s end %s\n", 
						buf+13,
						cfn
					);
				}
				fclose(of) ;
				of= 0 ;
			}
			else {
				fprintf(
					stderr, 
					"note: end without begin: %s",
					buf+13
				);
			}
		}

		if (memcmp(buf, "#tshdski begin ", 15)==0) {
			if (of) {
				fprintf(
					stderr, 
					"note: begin %s before end %s",
					buf+15,
					cfn
				);
				fclose(of);
				of= 0;
			}

			stccpy(cfn, buf+15, sizeof(cfn));
			if (of= fopen(cfn, "w")) {
				printf("Saving %s\n", cfn);
			}
			else printf("Can't open `%s'\n", cfn);
		}
		else {
			if (of) {
				fputs(buf, of) ;
				fputs("\n", of) ;
			}
		}

	}
}

static void stccpy(d, s, n)
char *d, *s;
int n;
{
	int i;

	for (i= 1; (*s)!='\0' && (i<n); i++) {
		*d++= *s++;
	}
	*d++= '\0';
}

static char *gnlgets(s)
char *s ;
{
	register char *p ;
	int c ;

	for (p= s; (c= getchar()) != EOF;) {
		if (c == '\r') continue ;
		if (c == '\n') break ;
		(*p++)= (char) c ; 
	} 
	(*p)= '\0' ;
	if (c == EOF) return((p==s)? (char *)NULL: s) ;
	return(s) ;
}
