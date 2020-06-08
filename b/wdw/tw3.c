#include		"ucc.h"
#include		<fcntl.h>

int 	strcmp(), wininit(), 
	winopen(), montype(), syntax() ;

void 	doit();

struct wpos {
	struct uccoord p;
	struct uccoord s;
	struct uccoord c[2];
	} ;

static struct wpos w[] = {
		{ {0,0},   {25,80},  	{ {0x0F, 0x07}, {0x0F, 0x07} } },
		{ {7,15},  {8,12},    	{ {0x70, 0x0F}, {0x70, 0x0F} } },
	} ;				

static int mt;

void main(argc, argv)
int argc;
char *argv[];
{
char *a;
int k, wm, wh, argst;

mt = montype();
for (argst=1;argst<argc;argst++) {
   a=argv[argst];
   if (strcmp(a,"cga")==0) mt=1;
   else if (strcmp(a,"mono")==0) mt=0;
   else exit(syntax());
   }

wininit(mt);
if ((wm=winopen(&w[0].p,&w[0].s,&w[0].c[mt]))<=0) {
   printf("Cannot open main window.\n");
   exit(10);
   }

wintitle(wm, 1, " This is test program: TW3 ");
wintitle(wm, 3, mt? "CGA":"mono");

if ((wh=winopen(&w[1].p,&w[1].s,&w[1].c[mt]))<=0) {
   printf("Cannot open subwindow.\n");
   exit(11);
   }

doit(wh);

winclose(wh);
winclose(wm);
}

static int syntax()
{
printf("\"cga\" or \"mono\" required,\n");
printf("\"sub\" optional.\n");
return(1000);
}
