#include		"pfkey.h"

#define			CP		(*cp)
#define			MP		(*mp)
#define			BEEP		putch((char)7)

void scoputch() ;
void putch(), redraw();
void draw(), glhome();
char *dequeue();
void queuesav();
int strlen();

static char nuls[] = { '\0' } ;
static char negs[] = { '-', '\0' } ;

int doesc(s,cp,mp)
char *s;
int *cp, *mp ;
{
int n, j, idx, cnt, dir ;
char *q ;

s[MP] = '\0';
n = sscanf(s,"%d %d",&idx,&cnt);		/* get index, count args   */
if (n<1 || (n>1 && cnt<0)) {  			/* if invalid, ZAP    line */
   redraw(nuls,MP); CP=MP=0;
   }
else {						/* else process		   */
   queuesav(1);
   if (dir=(idx<0)) idx=-idx;			/* dir := 1 if negative    */
   for (j=0; j<idx; j++) q=dequeue(dir);	/* count backwards for idx */
   if (n>1) {					/* "count" specified: list */
/*    redraw(s,MP); 	*/
      putch('\r'); putch('\n');
      if (idx<=cnt) cnt -= idx;	
      for (n=0; n<cnt; n++) printf("%s%2d. %s\n", 
		dir?negs:nuls, idx-n, q=dequeue(!dir));
      newhome();
      }	
   strcpy(s,q); redraw(s,MP); CP=(MP=strlen(s));
   queuesav(0);
   }
return(0);
}
