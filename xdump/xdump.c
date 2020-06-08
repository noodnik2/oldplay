#include <stdio.h>


static int eflg = 0 ; 
static int fadr = 0 ; 
static int ladr = 0xfffffff ;

static void getadr() ; 
static void syntax() ; 
static void initout() ; 
static void send() ; 
static void clout() ; 



main(argc,argv)
int argc ; 
char *argv[] ; 
{ 
	int	a ; 
	int	i ; 
	static FILE *fp ; 

	for (i=1;i<argc;i++) { 
		if (argv[i][0]!='-') break ; 
		switch(argv[i][1]) { 
			case 'E':	eflg = 1 ; break ; 
			case 'f':	getadr(&fadr,argv[++i]) ; break ; 
			case 'l':	getadr(&ladr,argv[++i]) ; break ;
			default:	syntax() ; break ; 
		}
	}
	if (i<argc) { 
		if (i+1 != argc) syntax() ; 
		if ((fp=fopen(argv[i],"r"))==NULL) { 
			fprintf(stderr,"xdump:  file %s not found\n",argv[i]) ;
			exit(1) ; 
			/*NOTREACHED*/
		}
	}
	else	fp = stdin ; 
		
	if (fadr>=ladr) return(0) ; 
	for (a=0;a<fadr;a++) { 
		if (getc(fp) == EOF) return(0) ; 
	}
	for (initout(fadr); (i=getc(fp))!=(EOF) && a<ladr;a++) send(i) ; 
	clout() ; 
	return(0) ; 
}

static void syntax()
{
	fprintf(stderr,"\
xdump:  Usage:  xdump [-E -f # -l #] [fn]\n\
		-E	use ebcdic encoding\n\
		-f #	first address to dump\n\
		-l #	last address to dump\n\n\
	write hexadecimal dump of fn (or stdin) to stdout.\n\
	address must be in hex and divisible by 16 (end in 0).\n\
") ; 
	exit(1) ; 
}

static void getadr(da,s)
int *da ; 
char *s ; 
{
	if (sscanf(s,"%x",da)!=1) { 
		fprintf(stderr,"xdump:  invalid option address '%s'\n",s) ; 
		exit(1) ; 
		/*NOTREACHED*/
	}
	if (div16(*da)*16 != *da) { 
		fprintf(stderr,"xdump:  option address not 0mod16\n") ; 
		exit(1) ; 
		/*NOTREACHED*/
	}
}
	
		

static int div16(i)
int i ; 
{
	return(i/16) ; 
}

/*
An output line will appear:  
adr      0    2    4    6    8    a    c    e  
aaaaaaa xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx   1234567890abcdef\n
0123456789012345678901234567890123456789012345678901234567890123456
	  1         2         3         4         5         6
%07x
*/

#define LINELEN 66
#define ADRCOL	0
#define ADRFMT	"%07x"
int bytecol[16] = {
	8,10, 13,15, 18,20, 23,25, 28,30, 33,35, 38,40, 43,45
		} ; 
#define CCOL0	50


char oline[80] ; 
char loline[80] ; 

static int 	adr = 0 ; 
static int	offset = 0 ; 
static int 	laststar = 0 ; 

static void iline() ; 
static void prline() ; 
static void initline() ; 

static void initout(first)
int first ; 
{
	adr = first ; 
	iline(oline) ; 
	initline() ; 
	loline[bytecol[0]] = '\0' ; 
}

static void clout()
{
	if (offset) printf("%s\n",oline) ; 
}

static void send(c)
int c ; 
{
	int 	cc ; 

	sprintf(&(oline[bytecol[offset]]),"%02x",c) ; 
	oline[bytecol[offset]+2] = ' ' ; 
	if (eflg) { 
		cc = seb2asc(c) ; 
		if (cc==c) cc = 0 ; 
	}
	else	cc = c ;
	if (cc<32 || cc>127) cc = '.' ; 
	oline[CCOL0+offset] = cc ; 
	if (++offset == 16) { 
		prline() ; 
		offset = 0 ; adr += 16 ; 
		initline() ; 
	}
}

static void initline()
{
	strcpy(loline,oline) ; 
	iline(oline) ; 
	sprintf(oline,"%07x",adr) ; 
	oline[7] = ' ' ; 
}


static void iline(s)
char *s ; 
{
	int i ; 

	for (i=0;i<LINELEN;i++) s[i] = ' ' ; 
	s[i] = '\0' ; 
}


static void prline()
{
	if (strcmp(&oline[bytecol[0]],&loline[bytecol[0]])) {
		printf("%s\n",oline) ; 
		laststar = 0 ; 
	}
	else if (laststar==0) { 
		printf("*\n") ; 
		laststar = 1 ; 
	}
}
