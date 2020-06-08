#include		<malloc.h>

#define		MAXQ		200

static char *sa[MAXQ] ;
static int   sii = 0;	/* initial index into sa[] before full 		  */
static int   sci = 0;	/* first/last index of sa[] when full (sii==MAXQ) */

void initqueue()
{
int i;

for (i=0; i<MAXQ; i++) {
    sa[i] = 0;
    }
}

void queue(s)
char *s;
{
char *p;
int i;
						/* Get appropriate index     */
if (sii<MAXQ)   { i = (sci=sii++);	       		}
else  	        { if ((i=sci++)>=MAXQ) i=(sci=0);	}

if (sa[i]) {					/* Free if already allocated */
   free(sa[i]);
   sa[i] = 0;
   }

if (p=malloc(strlen(s)+sizeof(char))) {		/* allocate space, copy line */
   strcpy(sa[i]=p, s);
   }

}

char *dequeue(r)
int r;
{
int i;
char *q;
static char nlc = '\0';

if (sii) {
   i = sci + ( r ? 1 : -1 ) ;
   if (i>=sii) i = 0;
   else if (i<0) i = sii-1;
   q = sa[ ((r) ? i : sci) ];
   sci=i;
   }
else q = &nlc;
return(q);
}

void queuelst(n,c)
int n, c ;
{
int i, j, k, l;

k = sci;
for (i=j=l=0; i<sii; i++) {
    if (--k<0) k=sii-1;
    if (++j>=n) {
       if (++l>=c) break;
       cprintf("%3d %s\n", j, sa[k]);
       }	
    }
}

void queuesav(save)
int save;
{
static int ssci, ssii;
if (save) { ssci=sci; ssii=sii; }			/* save status */
else 	  { sci=ssci; sii=ssii; }			/*   restore   */
}
