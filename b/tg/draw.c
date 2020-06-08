/*


	functions defined here`

		redraw(s,maxl)
		draw(s,maxl)
		dupc(n,c)

*/

void dupc();
void dc();

extern char sco_ucm ;

/*

  OLD LOGIC:
	This routine redraws the line by writing 'maxl' spaces from the 
	start of the line (wiping out the previous line), then writing
	the string 's' (the cursor is left wherever 's' ends).

  NEW LOGIC: 
	This routine redraws the line by first writing the new line over the
	old line.  If the old line was longer, as many blanks as are necessary
	to wipe out the remaining old text are written, but first the initial
	cursor position is saved using the scopcp() stack facility.  After
	the trailer blanks are written, if any, the cursor is restored to
	just past the end of the new string.

	NOTE:  As of this writing, the scopcp() stack facility is only
	       one level deep, so MY caller cannot use scopcp() !

*/
void redraw(s,maxl)
register char *s;
int maxl ;
{
int i, r, c;
char omode ;

omode=sco_ucm; sco_ucm=0;
glhome(); for (i=0;*s;i++) scoputch(*s++);		/* Draw the new line */
if (i<maxl) {
   scopcp(0); dc(maxl-i,' '); scopcp(1);		/* Clear out the old */
   }
sco_ucm=omode; scoputch(0);
}

void draw(s,n)
register char *s;
int n;
{
char omode ;
omode=sco_ucm; sco_ucm=0; while(n-->0 && *s) scoputch(*s++); 
sco_ucm=omode; scoputch(0);
}
		/*
			NOTE:  dc() is the internal version of the
			       more general entry point, dupc().	
		*/
static void dc(n,c) int n,c; { while(n-->0) scoputch(c); }

void dupc(n,c)
int n, c;
{
char omode ;
omode=sco_ucm; sco_ucm=0; dc(n,c); sco_ucm=omode; scoputch(0);
}
