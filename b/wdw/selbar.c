#include		"ucc.h"
#include		"mkeys.h"

char *winfsw(), *lineup(), *linedn();
unsigned char xattr();
void subline();

int selbar(text)
char *text;
{
int exit, upd, wh, barpos, barofs, mp;
struct uccoord wsiz, wcolor, bpos, blen;
char *top, *more;

if (wh=winqcur()) {
   winqset(wh,1,&wsiz); winqset(wh,3,&wcolor); 
   blen.y=1; blen.x=wsiz.x; bpos.y=bpos.x=0;
   top=text; upd=1; mp=(int)wsiz.y-1;
   for (exit=barpos=barofs=0;!exit;) {
       if (upd) { more=winfsw(wh,top); upd=0; }
       bpos.y= (unsigned char)barpos;
       winattr(wh, &bpos, &blen, xattr(wcolor.x));
       switch(getkey()) {
	   case KEYUP:		if (barpos>0) barpos--;     
		       		else if (barofs>0) {
		       		   top=lineup(1,top,text);
		       		   barofs--;
				   upd=1;	
		       		   }
		       		break;
	   case KEYDN: 		if (barpos<mp) barpos++;    
		       		else if (*more) {
		       		   top=linedn(1,top);
		       		   barofs++;
				   upd=1;
		       		   }
		       		break;
	   case KEYCHOME:	barofs=barpos=0; top=text; upd=1;
		       		break;
	   case KEYHOME:	barpos=0; upd=1;
		       		break;
	   case KEYRET:		exit=1;
		       		break;
	   case KEYESC:		return(-1);		/* Abort */
		       		break;
           }	
       winattr(wh, &bpos, &blen, wcolor.x);
       }
   }
return(barofs+barpos);
}

static int getkey()
{
register int c;
if ((c=getch())==0) c=-getch();
return(c);
}

/*
	This subroutine takes an attribute byte and exchanges the foreground
	and background colors, returning this value. 

	The attribute byte is in the format specified by the IBM CGA and MDA 
	display adapters.
*/
static unsigned char xattr(attr)
unsigned char attr;
{
return( ((attr>>4)&15) | ((attr&15)<<4) );
}

