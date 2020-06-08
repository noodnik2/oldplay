#include		"ucc.h"
#include		"win0.h"

					/* External library routines */
char *malloc() ;
void free() ;
int strlen() ; 

					/* External user subroutines */
void ucccpy(), uccsum(), uccsub() ;
unsigned scpisize() ;
int scpinit();
void scpxfer(), scpbord(), scpscrl(), scpcput();
void win0sttl(), win0dttl() ;
char *win0fttl();

					/* Forward static subroutines */
int nxtwin();
void win0bord() ;
void win0untt(), win0uptt() ;

struct win0 winframe[MAXWINS];
int winidx = 0;

static struct uccoord uccvzer = { 0, 0 } ;
static struct uccoord uccvone = { 1, 1 } ;
static struct uccoord uccvtwo = { 2, 2 } ;

/*

	Window-Initialize.  
	Must be called before any other entry point.

*/
int wininit(mt)
int mt;
{
return(scpinit(mt));
}

/*
	wh = winopen(pos,size,color)
 
	opens window

	first finds a free windowslot, 
	
	then allocates enough memory to save previous screen contents,
	then draws a single border around the currently active window, if any,
	then copies the screen image under window to the obtained buffer,	
	then clears the screen image with the background color, 
	then places the cursor at the home position of the window {1,1}.
	returns positive integer if sucessful: "window handle", which is
	the index to the new, currently active window.

*/
int winopen(pos,size,color)
struct uccoord *pos, *size, *color ;
{
int who ;
unsigned char *buf ;
register struct win0 *wfp ;

if ((who=nxtwin())<0) return(who);
wfp = winframe+who;
if ((buf=(unsigned char *)malloc(scpisize(size)))==0) return(-2);
if (winidx) win0bord(1,winidx);
scpxfer(1,buf,pos,size);
ucccpy(&wfp->pos,pos); ucccpy(&wfp->size,size); 
ucccpy(&wfp->color,color); 
wfp->lbuf=buf; 
scpscrl((int)size->y,pos,size,color->x);  /* clear the window by scrolling */
ucccpy(&wfp->cursor,&uccvzer);		  /* home cursor */
return(winidx=who+1);
}

/*
	winactiv(wh)
 
	"activates" window

	draws a single border around the currently active window,
	then draws a double border around the specified window,
	returns 0 iff successful

*/
int winactiv(wh)
int wh;
{
register struct win0 *wfp ;

wfp = winframe+(wh-1);
if (wfp->lbuf==0) return(-1);
if (winidx) win0bord(1,winidx);
win0bord(2,wh);
return(winidx=wh);
}


/*
	Assigns a new title to a window
	window must be "activated" before the new title appears
*/
void wintitle(wh, loc, s)
int wh, loc;
char *s;
{
win0sttl(wh-1, loc, s);
}

/*
	winclose(wh)
 
	closes an open window

	first re-draws the original image over the window,
	then de-allocates the windowsave buffer and windowslot,
	then de-allocates the titles of the window.

*/
int winclose(wh)
int wh ;
{
unsigned char *buf ;
register struct win0 *wfp ;

wfp = winframe+(wh-1);
if ((buf=wfp->lbuf)==0) return(-1);
scpxfer(0,buf,&wfp->pos,&wfp->size);
free(buf); wfp->lbuf=0;
if (winidx==wh) winidx=0;
win0untt(wh-1);
return(0);
}

/*
	wincolor(wh,b,f)
 
	changes the color of a window--
	future text output only.

*/
void wincolor(wh,b,f)
int wh, b, f;
{
register struct win0 *wfp ;
wfp = winframe+(wh-1);
if (b>=0) wfp->color.y=(unsigned char)b; 
if (f>=0) wfp->color.x=(unsigned char)f;
}

/*
	winloc(wh,pos)
 
	moves the cursor to a window-relative position.

*/
void winloc(wh,wo)
int wh ;
struct uccoord *wo ;
{
register struct win0 *wfp ;
wfp = winframe+(wh-1);
ucccpy(&wfp->cursor,wo);
}

/*
	Clears an open window.  
	Border is left alone.
*/
int winclear(wh)
int wh;
{
register struct win0 *wfp ;
struct uccoord hpos, len;
wfp = winframe+(wh-1);
if (wfp->lbuf==0) return(-1);
uccsum(&hpos,&wfp->pos,&uccvone);
uccsub(&len,&wfp->size,&uccvtwo);
scpscrl((int)len.y,&hpos,&len,wfp->color.x);
return(0);
}


/*
	Scrolls an open window.  
	Border is left alone.
*/
int winscrl(wh,dir)
int wh, dir;
{
register struct win0 *wfp ;
struct uccoord hpos, len;
wfp = winframe+(wh-1);
if (wfp->lbuf==0) return(-1);
uccsum(&hpos,&wfp->pos,&uccvone);
uccsub(&len,&wfp->size,&uccvtwo);
scpscrl(dir,&hpos,&len,wfp->color.x);
return(0);
}

/*

	static subroutines

*/

/*
	returns -1 if all windowslots used, or index of first free slot.
*/
static int nxtwin()
{
int i;
for (i=0; i<MAXWINS; i++) {
    if (winframe[i].lbuf==0) return(i);
    }
return(-1);
}

/*
       Places a title into the window border area
*/
static void win0title(wfp, loc, s)
struct win0 *wfp;
int loc;
char *s;
{
struct uccoord op ;
unsigned char ul;

ucccpy(&op, &uccvzer);
ul  = (unsigned char)strlen(s);
if      (loc>=3) op.y+= wfp->size.y-1;				/* Up/Down  */
if      (loc==0 || loc==3) op.x+= 2;				/* Left     */
else if (loc==1 || loc==4) op.x+= (wfp->size.x-ul-2)/2;		/* Center   */
else if (loc==2 || loc==5) op.x+= wfp->size.x-ul-2;		/* Right    */
while(*s) scpcput(*s++, &wfp->pos, &wfp->size, wfp->color.y, &op);
}

/*
        Updates all the titles on a window
	Calls win0title
*/
static void win0uptt(who)
int who;
{
register struct win0 *wfp;
register int i;
char *s;

wfp= winframe+who;
for (i=0; i<6; i++) {
    if (s=win0fttl(who,i)) {
       win0title(wfp, i, s);
       }
    }
}

/*
        Deletes all the titles on a window
*/
static void win0untt(who)
int who;
{
register int i;
for (i=0; i<6; i++) {
    win0dttl(who, i);
    }
}

/*
	draws a border around the window
	Calls win0uptt()
*/
static void win0bord(nlines, wh)
int nlines, wh;
{
register struct win0 *wfp;
if (wh--) {
   wfp= winframe+wh;
   scpbord(nlines,&wfp->pos,&wfp->size,wfp->color.x);
   win0uptt(wh);
   }
}
