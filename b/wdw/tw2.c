#include		"ucc.h"
#include		<fcntl.h>

int 	strlen(), selbar(), open(), 
	read(), strcmp(), wininit(), 
	winopen(), montype(), syntax() ;

char 	*strcpy(), *malloc(), *subline(),
	*usewin1(), *winfsw() ;

void 	readit(), dispit(), 
	sput(), usewin0() ;

struct wpos {
	struct uccoord p;
	struct uccoord s;
	struct uccoord c[2];
	} ;

static struct wpos w[] = {
		{ {0,0},   {25,80},  	{ {0x0F, 0x07}, {0x0F, 0x07} } },
		{ {7,15},  {8,12},    	{ {0x70, 0x0F}, {0x70, 0x0F} } },
		{ {10,20}, {10,40},   	{ {0x0C, 0x0C}, {0x0C, 0x0C} } },
		{ {5,7},   {4,64},     	{ {0x0E, 0x0E}, {0x0E, 0x0E} } },
		{ {7,51},  {9,25},     	{ {0x0F, 0x0F}, {0x0F, 0x0F} } }
	} ;				

static int mt;

#define		NDWDW	3			/* Number of "subwin"s   */

void main(argc, argv)
int argc;
char *argv[];
{
char *a;
int k, wm, wh, argst, subwin, scrlwin;
int wca[NDWDW];

scrlwin = subwin = 0;
mt = montype();
for (argst=1;argst<argc;argst++) {
   a=argv[argst];
   if (strcmp(a,"cga")==0) mt=1;
   else if (strcmp(a,"mono")==0) mt=0;
   else if (strcmp(a,"sub")==0) subwin=1;
   else if (strcmp(a,"scrl")==0) scrlwin=1;
   else exit(syntax());
   }

wininit(mt);
if ((wm=winopen(&w[0].p,&w[0].s,&w[0].c[mt]))<=0) {
   printf("Cannot open main window.\n");
   exit(10);
   }

wintitle(wm, 1, " This is test program: TW2 ");
wintitle(wm, 3, mt? "CGA":"mono");
usewin0(wm);

if (subwin) {
   for (k=0; k<NDWDW; k++) {
      if ((wh=winopen(&w[k+2].p,&w[k+2].s,&(w[k+2].c[mt])))<=0) {
         printf("Cannot open subwin #%d.\n",k);
         exit(11);
	 }
      usewin0(wca[k]=wh);
      }
   while(k>0) {
      winclose(wca[--k]);
      if (k>0) { winactiv(wca[k-1]); getch(); }
      }
   winactiv(wm);
   getch();
   }
if (scrlwin) {
   char buf[25];
   winclear(wm); 
   do {
      sput(wm,"Answer: \"");    winpt(wm);
      if ((wh=winopen(&w[1].p,&w[1].s,&(w[1].c[mt])))<=0) {
         printf("Cannot open scrlwin.\n");
         exit(13);
         }
      a=usewin1(wh,buf,sizeof(buf)); winclose(wh);
      winactiv(wm); sput(wm,a?a:"***EXIT***"); sput(wm,"\"\n");
      } while(a);	
   getch();
   }
winclose(wm);
}

/*
	
*/
static void usewin0(wh)
int wh;
{
char msgbuf[20];
wintitle(wh,0,strcpy(malloc(strlen(sprintf(msgbuf," Window #%d ", wh))+1),msgbuf));
winactiv(wh);
readit(wh, "tw2.dat");
}

static char *usewin1(wh,output,outlen)
int wh, outlen ;
char output[];
{
char msgbuf[20], *sellist;
int sel;
wintitle(wh,0,strcpy(malloc(strlen(sprintf(msgbuf,"W#%d/Scrl", wh))+1),msgbuf));
wintitle(wh,4,strcpy(malloc(strlen(sprintf(msgbuf,"\030,\031,CR,ESC", wh))+1),msgbuf));
winactiv(wh);
sellist="SELECT\nDESELECT\nREPORT\nFUNNEL\nTEST\nRANDOM\nMIXTURE\n\nEXIT";
if ((sel=selbar(sellist))<0) return(0);
return(subline(sel,sellist,output,outlen));
}

static void sput(wh, s)
int wh ;
char *s;
{
while(*s) wincput(wh, *s++);
}

static int syntax()
{
printf("\"cga\" or \"mono\" required,\n");
printf("\"sub\" optional.\n");
return(1000);
}

/*
		
*/
static void readit(wh, fn)
int wh;
char *fn;
{
int fh;
char *p, quadbuf[8192] ;

winactiv(wh);
if ((fh=open(fn, O_RDONLY|O_BINARY))<=0) sput(wh, "No data file.");
else {
   dispit(wh,quadbuf,read(fh,quadbuf,sizeof(quadbuf)-1));
   close(fh);
   }
}
