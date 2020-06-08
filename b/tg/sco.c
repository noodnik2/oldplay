/*

	SCreen Output routines:		SCO--
				
			init		Initialize subsystem--call first
			scp  		Set "cursor" position
			rcp  		Read "cursor" position
 			putch		Draw char, update "cursor" position.
			ucp		Update BIOS cursor position with ours
			pcp		push/pop "cursor", cursor position

	globals:
			scorow		indicates the row number of the   
					"cursor" before it was last moved.

			sco_ucm		flag which causes scoputch() to update
					the display cursor (physical) to the 
					location of the "cursor" (internal) 
					after displaying each character.

			sco_om		flag which causes scoputch() to use
					BIOS instead of memory to write chars.

*/

			/* Static "constants" */
static unsigned cbseg;				/* location of phys. scn. */
static unsigned cbnc, cbnr;			/* length of phys. screen */
static unsigned cblr;				/* length of screen "rec" */
static unsigned cbpm;				/* "cursor" max. offset   */
static unsigned ccseg;

			/* Static "variables" */
static unsigned cbp;				/* "cursor" buffer offset */

			/* Global (public) variables */
unsigned char scorow ;			/* row number of top of inputarea */
char sco_ucm = 1;			/* Update cursor immediate mode ? */
char sco_om = 0; 			/* output mode: 0=memory, 1=bios  */

void scu1l(), uc(), guard(), cantprint() ;
unsigned dataseg();

#define		NLINES		(cbp/cblr)
#define		NCOLS		((cbp%cblr)>>1)
#define		ROWO(r)		((r)*cblr) 
#define		BLEN(c)		((c)<<1)
#define		NEWCBP(r,c)	((ROWO((r)))+BLEN((c)))
#define		GETCPRC(r,c)	(r)=NLINES;(c)=NCOLS

void scoinit(addr,nrows,ncols)
unsigned addr, nrows, ncols; 
{
int crow, ccol ;
cbseg = addr;  ccseg = dataseg(); 
cbnr  = nrows; cbnc  = ncols;
cblr  = BLEN(ncols);
cbpm  = (cbnr*cblr) ;
cbp   = 0; 
}

void scoscp(row, col)
unsigned row, col ;
{
if ( (row>=cbnr) || (col>=cbnc) ) return;
cbp = NEWCBP(row, col);
scorow = (unsigned char)(row);
}

void scorcp(rowp, colp) 
unsigned *rowp, *colp; 
{ 
GETCPRC( (*rowp), (*colp) );
}

/*
	SCOPushorpoptheCursorPosition(m): m=0 for "push", m=1 for "pop"

		this routine saves the cursor position, row-relative to scorow,
		and later restores it by again relating to scorow (this insures
		that if a scroll happens, the "pop" will be adjusted correctly).

*/	
void scopcp(m)
int m;
{
static unsigned drow, dcol;
unsigned row;

if (m) { 					    	/* pop  */
   row = (unsigned)(scorow);
   row += drow;
   cbp = NEWCBP(row, dcol);
   if (sco_ucm) spcur(row, dcol);
   }
else { 							/* push */
   GETCPRC(drow,dcol);
   drow -= (unsigned)(scorow);
   }
}

void scoputch(c)
int c;
{
char cc;
unsigned row, col ;

if (c==0 || (c>=32 && c<=127)) {	/* "printable" character?  */
   if (sco_ucm || sco_om) { GETCPRC(row,col); }
   if (cc=(char)c) {
      if (!sco_om) movedata(ccseg,(unsigned)&cc,cbseg,cbp,1);
      else biosputc(row,col,c);
      if ((++col)>=cbnc) col = 0;	
      cbp += 2;
      guard();
      }
   }
else cantprint(c);
if (sco_ucm) uc();			/* update cursor if required */
}

static void cantprint(c)
int c;
{
if (c=='\b' && cbp>2) { cbp -= 2; 	    }
else if (c=='\r') { cbp = NEWCBP(NLINES,0); }
else if (c=='\n') { cbp += cblr; guard();   }
}


/*
	guard() should be called whenever the possibility that the "cursor"
	could go below the bottom the physical screen arises.  The routine
	insures that when cursor is updated to the "cursor", everything 
	will work (there will be enough space on the screen).

	the external 'scorow' is updated to reflect its new physical
	location should scrolling take place.

*/
static void guard()
{
unsigned x;
char cc;
for (x=0; cbp>=cbpm; x++) {			/* do while need to scroll */
    movedata(cbseg,cbp-1,ccseg,(unsigned)&cc,1);/* ust attr. of last char  */
    scu1l(cc);			       		/* Scroll screen upward 1  */
    cbp -= cblr ; 		       		/* Pull back cursor 1 line */
    }
scorow -= (unsigned char)(x);
}

static void scu1l(attr)			/* Scroll upward one line	  */
char attr ;
{
scrollf(0, 0, cbnr-1, cbnc-1, attr);
}

static void uc()			/* Update cursor with "cursor"    */
{
unsigned r, c;
scorcp(&r, &c);
spcur(r,c);
}
