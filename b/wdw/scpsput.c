/*

      scpsput(s,pos,size,color,cursor,in);

*/

#include		"ucc.h"
#include		"scp.h"

void movedata();
void scp0wrs();
void xpand();
int vlofs();

extern unsigned scpdseg ;
extern struct mtype *scpcmtp;

/*

*/
int scpsput(s, pos, size, color, cursor, in)
char *s;
struct uccoord *pos, *size, *cursor;
unsigned char color;
int in;
{
register int i, j;
struct uccoord up;
char linebuf[256];
int k;

xpand(s,linebuf,sizeof(linebuf),&k);
if (i=vlofs(linebuf,in)) {		/* get i: bytes to output, k: buffer pos */
   j= (int)(size->x)-(int)(cursor->x);  /* get j: bytes left in window */
   if (i>j) i=j; 			/* truncate to within window  */
   up.y=pos->y+cursor->y; up.x=pos->x+cursor->x;
   scp0wrs(linebuf+in,i,&up,color);
   cursor->x+= (unsigned char)i;
   } 
return(k);
}				

static int vlofs(s,in)
char *s;
int in;
{
register char *p;
register int i;
for (p=s, i=0; *p; p++) { if (in) in--; else i++; }
return(i);
}

static void xpand(input,output,ml,kp)
char *input, *output;
int ml, *kp;
{
register int i;
register char *p;
int j;

for (--ml, p=input, i=0; *p && i<ml; p++) {
    if (*p>=' ') output[i++]=*p;	
    else {
       if (*p=='\t') for (j=8-(i&7);j>0 && i<ml;j--) output[i++]=' ';
       else if (*p=='\n') break;
       }	
    }
*kp=(int)(p-input);
output[i]=0;
}
