/*

	chdate		CHange DATE			DOS Utility

	chdate filename mm/dd/yy [hh:mm[:ss]][a|p]

	Compiled under Microsoft C Compiler v4.00

	Requires modification under Lattice because 
	of file-handle table.  Use 'dopen()' instead
	of open (Lattice v3.00 or later).
*/

#include		<fcntl.h>
#include		<dos.h>

void readft(), writeft();

void main(argc, argv)
int argc;
char *argv[];
{
int fh, nd, nt ; 
unsigned oldd, oldt, newd, newt ;
 
if (argc<3) exit(syntax());

nd = nt = 0;
if (argc>2) { nd=1; if (prsdate(argv[2],&newd)) exit(badarg("date",argv[2])); }
if (argc>3) { nt=1; if (prstime(argv[3],&newt)) exit(badarg("time",argv[3])); }

if (nd || nt) {
   if ((fh=open(argv[1],O_RDWR))<=0) exit(nofile(argv[1]));
   readft(fh, &oldd, &oldt);
   writeft(fh, nd? newd:oldd, nt? newt:oldt);
   close(fh);
   printf("Change made.\n");
   exit(0);
   }

printf("No change made.\n");
exit(1);
}

static int syntax()
{
printf("syntax:		chdate filename mm/dd/yy [hh:mm[:ss]]\n");
return(100);
}

static int nofile(fn)
char *fn;
{
printf("Can't open %s.\n",fn);
return(12);
}

static int badarg(at,av)
char *at, *av;
{
printf("Bad %s argument: %s.\n",at,av);
return(8);
}

/*
		Parse a date into word, 
		DOS directory format.

		See file timestamp structure defined in the
		DOS Technical Reference manual.  This value
		is passed to/from DOS in the DX register.
*/
static int prsdate(s,up)
char *s;
unsigned *up;
{
unsigned mo, dy, yr ;
int i;
char c1, c2, c3;

i = sscanf(s,"%2d%1c%2d%1c%2d%1c", &mo, &c1, &dy, &c2, &yr, &c3) ;
if (i<5 || mo==0 || dy==0 || mo>12 || dy>31 || yr>99 || yr<80 || 
	   (c1!='/' && c1!='-') || (c2!='/' && c2!='-') || (c1!=c2) ) {
/*printf("i=%d, mo=%d, c1=%c, dy=%d, c2=%c, yr=%d\n", i, mo, c1, dy, c2, yr);*/
   return(1);
   }
*up = (dy&31) | ((mo&15)<<5) | (((yr-80)&127)<<9) ;
return(0);
}

/*
		Parse a 24-hr. time-of-day into word, 
		DOS directory format.

		See file timestamp structure defined in the
		DOS Technical Reference manual.  This value
		is passed to/from DOS in the CX register.
*/
static int prstime(s,up)
char *s;
unsigned *up;
{
unsigned mh, hh, mm, ss ;
int i;
char c, c1, c2, c3;

i = sscanf(s, "%2d%1c%2d%1c%2d%1c", &hh, &c1, &mm, &c2, &ss, &c3) ;

if (i==6) c=c3;
else if (i==4) c=c2;
else c=0;

if (i<5) { ss=0; c2=':';  }

mh = c? 12 : 24 ;

if (hh==mh) hh=0;

printf("i=%d, mh=%d, hh=%d, c1=%c, mm=%d, c2=%c, c=%c, ss=%d\n", 
	i,    mh,    hh,    c1,    mm,    c2,    c,    ss);

if ( (i<3) || (hh>=mh) || (c? (c!='a' && c!='p'): 0) ||
	      (mm>59) || (ss>59) || (c1!=':') || (c2!=':')  ) {
/*printf("i=%d, hh=%d, c1=%c, mm=%d, c2=%c, ss=%d\n", i, hh, c1, mm, c2, ss);*/
   return(1);
   }
*up = ((ss/2)&31) | ((mm&63)<<5) | (((hh+(c=='p'?12:0))&31)<<11) ;
return(0);
}

/*
		readft(fh, unsigned *, unsigned *)
		Return timestamp information from file:
		date and time-of-day values, DOS directory format.
*/
static void readft(fh, dp, tp)
int fh;
unsigned *dp, *tp;
{
union REGS regs;
regs.x.ax=0x5700; regs.x.bx=fh;
intdos(&regs,&regs);
(*dp)=regs.x.dx; (*tp)=regs.x.cx;
}

/*
		writeft(fh, unsigned *, unsigned *)

		Set timestamp information of file:
		date and time-of-day values, DOS directory format.
*/
static void writeft(fh,d,t)
int fh;
unsigned d, t;
{
union REGS regs;
regs.x.ax=0x5701; regs.x.bx=fh;
regs.x.dx=d; regs.x.cx=t;
intdos(&regs,&regs);
}
