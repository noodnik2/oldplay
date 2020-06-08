#include		"ucc.h"
#define			MAXWTTL	20

					/* External library routines */
int strlen() ; 

					/* Forward static subroutines */
int nxtttl(), findttl() ;

struct win0ttl {
	unsigned char wid;		/* Window-ID: 0..255 only     */
	unsigned char wtp;		/* Window-Title pos. 0..5     */
	char *ttl;			/* Pointer to title string    */
	} ;

static int ttlidx = 0;
static struct win0ttl wttl[MAXWTTL];


/*
	set a title
*/
void win0sttl(wid, pos, title)
int wid, pos;
char *title; 
{
int fi;
register struct win0ttl *wtp;

if ((fi=findttl(wid, pos))<0) {
   if ((fi=nxtttl())<0) return;
   }
wtp = wttl+fi;
wtp->wid = (unsigned char)wid;
wtp->wtp = (unsigned char)pos;
wtp->ttl = title;
}

/*
	delete a title 
*/
void win0dttl(wid, pos)
int wid, pos;
{
register int fi;
if ((fi=findttl(wid, pos))<0) return;
wttl[fi].ttl= 0;
}

/*
	find a title 
*/
char *win0fttl(wid, pos)
int wid, pos;
{
register int fi;
return( ((fi=findttl(wid,pos))>=0)? wttl[fi].ttl : 0 ) ;
}

/*
	Start of static subroutines
*/

static int nxtttl()
{
register int i;

for (i=0; i<MAXWTTL; i++) {
    if (wttl[i].ttl==0) return(i);
    }
return(-1);
}

static int findttl(wid, pos)
int wid, pos;
{
register int i ;
unsigned char fw, pw ;

fw= (unsigned char)wid; pw= (unsigned char)pos;
for (i=0; i<MAXWTTL; i++) {
    if (wttl[i].wid==fw && wttl[i].wtp==pw) return(i);
    }
return(-1);
}
