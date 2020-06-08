/*
	note: there are hard-coded line length constant(s) in this pgm!
*/

#include		"pfkey.h"

#define			CP		(*cp)
#define			MP		(*mp)
#define			BEEP		putch((char)7)

int strlen();

void scoputch();
void putch(), dupc(), redraw();
void draw(), fast(), zip(), glhome();
char *dequeue();

char insmode = 0;

int pfkey(key,s,cp,mp)
int key, *cp, *mp ;
char *s ;
{
unsigned l;
char *p;

p=s+CP; l=(MP-CP); s[MP] = '\0';

switch(key) {

    case _PFEND:	draw(p,l); CP=MP; 			break;
    case _PFHOME: 	glhome(); CP=0;		 		break;
    case _PFINS:       cursor((int)(insmode=(char)(!insmode))); break;
    case _PFRIGHT:	if (CP<MP) { scoputch(s[CP++]); }	break;
    case _PFLEFT:	if (CP>0) { scoputch('\b'); CP--; }	break;
    case _PFCTLRI:	fast(1,s,cp,mp);			break;
    case _PFCTLLE:	fast(-1,s,cp,mp);			break;
    case _PFUP:		zip(-80,s,cp,mp);  /* ! */		break;
    case _PFDOWN:	zip(80,s,cp,mp);			break;

    case _PFDEL:					/* Delete Key */
	if ( (MP) && (l) ) {
	   scopcp(0); memcpy(p, p+1, l); MP--;
	   draw(p, l); scoputch(' '); scopcp(1);
	   }				
	break;

    case _PFPGUP:					/* Page Up key */
	strcpy(s,dequeue(0)); redraw(s,MP); CP=(MP=strlen(s));
	break;

    case _PFPGDN:					/* Page Down key */
	strcpy(s,dequeue(1)); redraw(s,MP); CP=(MP=strlen(s));
	break;

    case _PFCTLEN:					/* Control-End key */
	s[CP]='\0'; scopcp(0); dupc(l,' '); scopcp(1); MP=CP ;
	break;

    default:	BEEP;	break;				/* Unknown key	*/
    }	

return(0);
}

static void fast(d,s,cp,mp)
register char *s;
int d, *cp, *mp;
{
register int i;
int ocp;
char c;

ocp = CP;
if (d>0) {   							/* Forwards */
   if ((i=CP+1)>MP) return;
   while(i<MP) { c=s[i];  if (c==' ' || c=='\t') break; i++; }
   while(i<MP) { c=s[i];  if (c!=' ' && c!='\t') break; i++; }
   }
else {								/* Backwards */
   if ((i=CP-1)<0) return;
   while(i>0) { c=s[i];   if (c!=' ' && c!='\t') break; i--; }
   while(i>0) { c=s[--i]; if (c==' ' || c=='\t') { i++; break; }}
   }
if (ocp<i) draw(s+ocp, i-ocp);
else       dupc(ocp-i, '\b');
CP = i;
}

static void zip(n,s,cp,mp)
char *s;
int n, *cp, *mp;
{
int i, ocp;

i = (ocp=CP) + n; 
if (i>=0 && i<=MP) {
   if (ocp<i) 	draw(s+ocp, i-ocp);  
   else 	dupc(ocp-i, '\b');
   CP = i;
   }
}
