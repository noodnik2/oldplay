#include		"ucc.h"
#include		"win0.h"

#define		FSWREQD	1	/* set to 0 if winfsw() not required */

extern struct win0 winframe[];
void scpcput(), scpscrl(), scpattr();
int scpsput();

/*

       this routine writes a specified portion of 'text' in "full-screen" mode

       write as much of text as will fit into current line,
       then blank out the rest (rightmost part) of line.
       Return address of end of string, regard
       (Newlines are processed, and if on last line, cause return)

*/
char *winfswl(wh,text,in)
int wh, in;
char *text;
{
register struct win0 *wfp ;
register int i;
struct uccoord hpos, len, cs;

wfp = winframe+(wh-1);
hpos.y= wfp->pos.y+wfp->cursor.y+1; hpos.x= wfp->pos.x+wfp->cursor.x+1;
len.y= wfp->size.y-2; len.x= wfp->size.x-2;
cs.y=cs.x=0; i= scpsput(text, &hpos, &len, wfp->color.x, &cs, in);
if (len.x>cs.x) { 
   len.x-= cs.x; len.y= (unsigned char)1; hpos.x+= cs.x;
   scpscrl(1, &hpos, &len, &wfp->color); 
   }
if (*(text+=i)=='\n') text++;	/* Advance past \n if was present */
return(text);
}

#if		FSWREQD==1

/*

	this routine writes a full screen from 'text'

	Clear the specified window, then write as much of text
	as will fit into window.  
       	Return address of next-2-b-displayed char.
      	(Newlines are processed, and if on last line, cause return)

*/
char *winfsw(wh,text)
int wh;
char *text;
{
register char *p;
register struct win0 *wfp ;
struct uccoord hpos, len, cs, ml;

wfp = winframe+(wh-1);
hpos.y= wfp->pos.y+1; hpos.x= wfp->pos.x+1;
len.y= wfp->size.y-2; len.x= wfp->size.x-2;
ml.y= len.y-1; ml.x= len.x-1; cs.y=cs.x= 0;
scpscrl((int)len.y, &hpos, &len, &wfp->color);
if (*(p=text)) do scpcput(*p++, &hpos, &len, wfp->color.x, &cs);
                  while(*p && cs.y<=ml.y);
return(p);
}

#endif

/*
	wincput(wh,c)
 
	puts out a character to the designated window

*/
int wincput(wh,c)
int wh ;
unsigned char c;
{
register struct win0 *wfp ;
struct uccoord hpos, len;
wfp = winframe+(wh-1);
hpos.y= wfp->pos.y+1; hpos.x= wfp->pos.x+1;
len.y= wfp->size.y-2; len.x= wfp->size.x-2;
scpcput(c,&hpos,&len,wfp->color.x,&wfp->cursor);
return(0);
}

/*
	winattr(wh,pos,size,attr)

	changes the attribute of a specified area of a window
*/
void winattr(wh, loc, size, attr)
int wh;
struct uccoord *loc, *size;
unsigned char attr;
{
register struct win0 *wfp;
register int i;
struct uccoord up, si;
int l, x;

wfp= winframe+(wh-1);
if ((loc->x)>=wfp->size.x || (loc->y)>=wfp->size.y) return;
si.y= (((loc->y)+(size->y))>(wfp->size.y))? (wfp->size.y)-(loc->y): (size->y);
si.x= (((loc->x)+(size->x))>(wfp->size.x))? (wfp->size.x)-(loc->x): (size->x);
up.y= wfp->pos.y+loc->y+1; 
up.x= wfp->pos.x+loc->x+1;
scpattr(&up,&si,attr);
}
