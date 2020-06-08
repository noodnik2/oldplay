#include	"compiler.h"
#include	"flist.h"

#define		TESTCOMP	0
#define		MAXSIZE		(CMDSIZE)

char *memchr() ;
static int procesc(cp, il, ip);

int cescstr(cl, il, ml)
char *cl;
int *il, ml;
{
	int i;
	char c;

	i= 0;
	while( i < ml ) {
		switch(c= *(cl++)) {
			case '\0':  return(i);			break;
			case '\\':  cl+= procesc(cl, il, &i);	break;
			default:    il[i++]= c;			break;
		}
	}
	return(-1);
}

static char slec[]= { '\\', 'n', 'r', 'b', 't', '~', '=', '!', '@', '#' } ;
static int slcc[]= { '\\', '\n', '\r', '\b', '\t', -1, -2, -3, -4, -5   } ;

static int procesc(cp, il, ip)
char *cp;
int *il, *ip;
{
	char *ccp;
	register int i;
	int j;

	if ( (ccp= memchr(slec, *cp, sizeof(slec)))!=NULL ) {
		i= (ccp-slec) ;
		il[(*ip)++]= slcc[i];
		return(1);
	}

	if (cp[0]=='0' && cp[1]=='x') {
		if (sscanf(cp+2, "%2x", &j)==1) {
			il[(*ip)++]= j;
			return(4);
		}
	}	

	if (cp[0]>='A' && cp[0]<='Z')  {
		il[(*ip)++]= (int)(cp[0]-'A') + 1 ;
		return(1);
	}

	for (i= 0; (i<3) && (cp[i]>='0' && cp[i]<='7'); i++);
	if (i==3) {
		if (sscanf(cp, "%3o", &j)==1) {
			il[(*ip)++]= j;
			return(3);
		}
	}

	return(0);
}

#if	TESTCOMP==1

void doit_to();

void main(argc, argv)
int argc;
char *argv[];
{
	int i;

	for (i= 1; i< argc; i++) {
		doit_to(argv[i]);
	}
}

static void doit_to(s)
char *s;
{
	int i, rc, il[MAXSIZE];
	char os[MAXSIZE+1];
	
	printf("parseing: '%s'\n", s);
	rc= cescstr(s, il, MAXSIZE);
	printf("Done: rc= %d\n", rc);
	if (rc>=0) {
		for (i= 0; i<rc; i++) {
			printf("%d: %d\n", i+1, il[i]);
			os[i]= (char)(il[i]>=0)? il[i]: '.' ;
		}
		os[i]= '\0';
		printf("========\n%s\n", os);
	}
	printf("========\n\n");
}

#endif
