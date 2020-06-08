#include		"ucc.h"
#include		"dkeys.h"

char *winfswl(), *negcfnd(), *cfnd();
int getch(), kbhit();

static int kbrd(cp)
int *cp;
{
register int c;
if (!(c=getch())) c= -getch();
return((*cp=c)==27);
}

static void newscrl(wh,sp,rows,flp,fpp,indent,lines,p0)
int wh, rows, indent, lines;
char **sp, **flp, **fpp, *p0;
{
register int i;
register char *p;
struct uccoord po;

winscrl(wh, lines);
if (lines>0) { 
   po.y= (unsigned char)(rows-1); po.x=0;
   (*sp)=cfnd(*sp); (*flp)=cfnd(*flp); (*fpp)=cfnd(p=(*fpp)); 
   }
else { 					/* assert lines<0 */
   po.y=po.x=0;
   (*flp)=*sp; (*sp)=p=negcfnd(*sp,p0); (*fpp)= negcfnd(*fpp,p0); 
   }
winloc(wh, &po); 
winfswl(wh,p,indent,0);
}

static void newsc(wh,s,rows,flp,fpp,indent)
int wh, rows, indent;
char *s, **flp, **fpp;
{
register int i;
register char *p;
struct uccoord po;
p=s; po.y=po.x=0;
for (i=0; i<rows; i++) {
    winloc(wh, &po);
    p= winfswl(wh,p,indent);
    if (i==0) *flp=p;
    po.y++;	
    }	
*fpp=p;
}

static char *negcfnd(s,f)
char *s, *f;
{
register char *p;
if (*(p=s)) {
   if (*(--p)=='\n') p--;
   while(p>f && *p!='\n') p--;
   if (p>f) p++;
   }
return(p);
}

static char *cfnd(s)
char *s;
{
register char *p;
for (p=s; *p && *p!='\n'; p++);
if (*p) p++;
return(p);
}

void dispit(wh,s,len)
int wh, len;
char *s;
{
register upd;
char *fwdp, *fwdl, *p1;
int i, cmd, indent, scrl, rows;
struct uccoord wsiz;

upd=1; scrl=indent=0; fwdp=fwdl=p1=s; 
winqset(wh,1,&wsiz); rows= (int)wsiz.y;
for (s[len]=0;;) {
   if (upd) { newsc(wh,p1,rows,&fwdl,&fwdp,indent); upd=0; }
   else if (scrl) { newscrl(wh,&p1,rows,&fwdl,&fwdp,indent,scrl,s); scrl=0; }
   if (kbrd(&cmd)) break;
   switch(cmd) {
	case KEYUP:	scrl=-1;
		break;
	case KEYDN:	scrl=1;
		break;
        case KEYLF:     if (indent>0) { indent--; upd=1; } else beep();
		break;
        case KEYRT:	if (indent<255) { indent++; upd=1; } else beep();
		break;
        case KEYPGUP:	for (i=rows;i>0;i--) p1= negcfnd(p1,s); upd=1;
		break;
        case KEYPGDN:	p1=fwdp; upd=1;
		break;
        case KEYHOME:	indent=0; upd=1;
		break;
        case KEYCHOME:  p1=s; indent=0; upd=1;
		break;
	default:	beep();
		break;
	}
   }
}
