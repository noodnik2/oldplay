#include		"ucc.h"
#include		<fcntl.h>

void pause(), sput();
void usewin0();
void nowopen2(), nowopen3(), nowopen4();
int wininit(), winopen() ; 
char *winfsw() ;

struct wpos {
	struct uccoord p;
	struct uccoord s;
	struct uccoord c[2];
	} ;

static struct wpos w[] = {
		{ {0,0}, {25,80}, { {0,15}, {0, 1} } },
		{ {10,20}, {10,40}, { {0,15}, {0, 4} } },
		{ {7,10}, {6,55},   { {0,15}, {0, 3} } },
		{ {6,17}, {3,44},   { {0,15}, {0, 7} } },
		{ {4,8}, {16,37},  { {0,15}, {0, 6} } }
	} ;

static int mt;

void main(argc, argv)
int argc;
char *argv[];
{
int wm, wh;

mt = 0;
if (argc>1) {
   if (strcmp(argv[1],"cga")==0) mt=1;
   else if (strcmp(argv[1],"mono")==0) mt=0;
   else exit(syntax());
   }

wininit(mt);
if ((wm=winopen(&w[0].p,&w[0].s,&w[0].c[mt]))<=0) {
   printf("Cannot open main window.\n");
   exit(10);
   }

wintitle(wm, 0, "This is the main window");
wintitle(wm, 1, "CENTER");
wintitle(wm, 2, "RIGHT");
wintitle(wm, 3, "BOTL");
wintitle(wm, 4, "BOTCENT");
wintitle(wm, 5, "BOTRIGHT");

if ((wh=winopen(&w[1].p,&w[1].s,&w[1].c[mt]))<=0) {
   printf("Cannot open first sub-window.\n");
   exit(11);
   }

usewin0(wh);
winclose(wh);
winclose(wm);
}

static void usewin0(wh)
int wh;
{
char msgbuf[200];

wintitle(wh, 0, "This is the first window");
winactiv(wh);

sprintf(msgbuf,"Window %d opened.", wh); sput(wh, msgbuf); pause(); 
if (mt) wincolor(wh, 0, 1);
sput(wh, " This will be a rather hefty test of the thing which I am testing now.\n");
sput(wh, "That is about all that I want to say at this time, until you press the key.\n");
pause();

if (mt) wincolor(wh, 0, 2);
sput(wh, "Now that you have pressed the key, I will continue to print so much");
sput(wh, "in the box that it will be forced to scroll so that I can test it.\n");
pause();

if (mt) wincolor(wh, 0, 3);
sput(wh, "Now that you have pressed the key, I will continue to print so much");
sput(wh, "in the box that it will be forced to scroll so that I can test it.\n");
pause();

if (mt) wincolor(wh, 0, 4);
sput(wh, "Now that you have pressed the key, I will continue to print so much");
sput(wh, "in the box that it will be forced to scroll so that I can test it.\n");
pause();

nowopen2(wh);

}

static void nowopen2(owh)
int owh;
{
int wh, x;
char secbuf[200];

sput(owh, "Opening another window...");

if ((wh=winopen(&w[2].p, &w[2].s, &w[2].c[mt]))<=0) {
   sput(owh, "\nSecond window could not be opened.\n");
   }
else {
   winactiv(wh);
   sput(wh, "This is an example of using the second window for\n");
   sput(wh, "intense scrolling activity.  Press a key when ok:\n\n");
   pause();
   for (x=0; x<200; x++) {
       sprintf(secbuf, "%d\n", x);
       sput(wh, secbuf);	
       }
   sput(wh, "\nThat's all there is to it!");
   pause();	
   
   nowopen3(wh);

   winclose(wh); winactiv(owh);
   sprintf(secbuf,"Alt. window #%d closed.\n", wh); sput(owh, secbuf);
   pause() ;
   }
}


static void nowopen3(owh)
int owh;
{
int wh, x;
char tertbuf[200], t2b[2];

sput(owh, "Opening yet another window...");

if ((wh=winopen(&w[3].p, &w[3].s, &w[3].c[mt]))<=0) {
   sput(owh, "\nThird window could not be opened.\n");
   }
else {
   winactiv(wh);
   sprintf(t2b,"%c",219);
   sput(wh, "Colors: ");
   for (x=0;x<32;x++) {
       wincolor(wh, 0, x);	
       sput(wh, t2b);
       }
   wincolor(wh, w[3].c[mt].y, w[3].c[mt].x) ;
   pause();
   
   nowopen4(wh);

   winclose(wh); winactiv(owh);
   sprintf(tertbuf,"Tert. window #%d closed.\n", wh); sput(owh, tertbuf);
   pause() ;
   }
}

static void nowopen4(owh)
int owh;
{
int fh;
int wh, x;
char *p, quadbuf[1024] ;

sput(owh, "Opening still another window...");

if ((wh=winopen(&w[4].p, &w[4].s, &w[4].c[mt]))<=0) {
   sput(owh, "\nFourth window could not be opened.\n");
   }
else {
   winactiv(wh);
   if ((fh=open("tw.dat", O_RDONLY|O_BINARY))<=0) {
      sput(wh, "No data file.");
      }
   else {
      x = read(fh,quadbuf,sizeof(quadbuf)-1);
      quadbuf[x] = '\0';
      for (p=quadbuf; *p; ) {
	  p = winfsw(wh, p, 1);
          pause();
	  }
      close(fh);
      }
   pause(); winclose(wh); winactiv(owh);
   sprintf(quadbuf,"Quad. window #%d closed.\n", wh); sput(owh, quadbuf);
   }
pause() ;
}

static void pause()
{
while(kbhit()) getch();
while(!kbhit());
getch();
}

static void sput(wh, s)
int wh ;
char *s;
{
while(*s) {
    wincput(wh, *s++);
    }
}

static int syntax()
{
printf("\"cga\" or \"mono\" required.\n");
return(1000);
}
