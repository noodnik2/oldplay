/*

	SCreen Output routines:		SCO--
				
			init		Initialize subsystem--call first
			scp  		Set "cursor" position
			rcp  		Read "cursor" position
 			puts		Draw string, update "cursor" position.

	globals:
			scolines	indicates the number of lines the
					"cursor" has moved down the screen
					since the last setting of the cursor.

*/

static unsigned cbseg;				/* location of phys. scn. */
static unsigned cbnc, cbnr;			/* length of phys. screen */
static unsigned cblr;				/* length of screen "rec" */
static unsigned cbp;				/* "cursor" buffer offset */
static unsigned cbpm;				/* "cursor" max. offset   */
static unsigned ccseg;

unsigned char scolines ;

void scf();

#define			BLEN(l)		(l<<1)

void scoinit(addr,nrows,ncols)
unsigned addr, nrows, ncols; 
{
int crow, ccol ;
cbseg = addr;  ccseg = dataseg(); 
cbnr  = nrows; cbnc  = ncols;
cblr  = BLEN(ncols);
cbp   = 0; 
cbpm  = (cbnr*cblr) ;
}

void scoscp(row, col)
unsigned row, col ;
{
if (row>=cbnr || col>=cbnc) return;
cbp = (row*cblr) + BLEN(col);
scolines = 0;
}

void scorcp(rowp, colp)
unsigned *rowp, *colp;
{
(*rowp) = cbp / cblr ; 
(*colp) = ( cbp % cblr ) >> 1 ;
}

void scoputs(s)
char *s;
{
unsigned x, l;
char cc;

for (l=strlen(s); l!=0; l-=x) {
   x = imin(l,BLEN(cbpm-cbp));
   movedata(ccseg,(unsigned)s,cbseg,cbp,BLEN(x));  /* ThiS IS WRONG */
   s   += x;
   cbp += BLEN(x);
   if ((cbp%cblr)==0) scolines++;
   if (cbp>=cbpm) {
      movedata(cbseg,cbp-1,ccseg,(unsigned)&cc,1);
      scf(cc);
      cbp -= cblr;
      }
   } 
}

static void scf(attr)
char attr ;
{
unsigned r, c;
scrollf(0, 0, cbnr-1, cbnc-1, attr);
rpcur(&r, &c);
if (r>0) spcur(--r, c);
}

static void imin(x,y)
int x, y;
{
return(x<y?x:y) ;
}
