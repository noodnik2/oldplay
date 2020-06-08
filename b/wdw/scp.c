#include		"ucc.h"
#include		"scp.h"

				/* internal forward routines */
void scp0attr(), scp0xfer();

				/* external user routines    */
void scp0hlin(), scp0vlin(), scp0cput(), scp0scrl();
void ucccpy(), uccsum(), uccsub() ;
unsigned dataseg() ;
void uswap() ;

				/* external library routines */
void movedata();

/*

rc =  scpinit(montype);			    		*

ul =  scpisize(size);			       		*
      scpxfer(dir,buf,pos,size);	      		*
      scpbord(weight,pos,size,color);	       		*
      scpscrl(n,pos,size,color);			*
      scpattr(pos,size,attr);				*

*/

static struct mtype mt[] = {
		{ 0xb000, 2, 25, 80 },	/* Monochrome */
		{ 0xb800, 2, 25, 80 }	/* CGA        */
	}; 

struct mtype *scpcmtp = mt ;		/* "Default" device is Monochrome   */
unsigned scpdseg = 0;			/* Zero indicates unitialized state */

#define		NMONS		(sizeof(mt)/sizeof(mt[0]))

int scpinit(montype)
int montype;
{
if (montype<0 || montype>=NMONS) return(-1);
scpcmtp= mt+montype;
scpdseg= dataseg();
return(0);
}

/*
	use this routine to find out how large of a buffer
	is required to save an image of a specified size.
*/
unsigned scpisize(size)
struct uccoord *size;
{
register unsigned result ;
result =  (unsigned)(size->y);
result *= (unsigned)(size->x);
result *= (unsigned)(scpcmtp->sochar);
return(result);
}

/*
	static unsigned char scplbar[3][2] = {
		{' ',' '}, {'-','|'}, {'=','!'}
	};	
	static unsigned char scpcrnr[3][4] = {
		{' ',' ',' ',' '}, {',','.','\'','`' }, {'/','\\','/','\\'}
	};
*/

static unsigned char scplbar[3][2] = {
		{32,32}, {196,179}, {205,186}
	};			         
static unsigned char scpcrnr[3][4] = {
		{32,32,32,32}, {218,191,217,192}, {201,187,188,200}
	};
  					    
/*					    

	draws a border around the window of the specified weight:

	weight 0 indicates a blank border is to be drawn,
	weight 1 indicates a single line border,
	weight 2 indicates a double line border.

	weights which are negative or greater than 2 are not drawn

*/
void scpbord(lines, pos, size, color)
int lines;
struct uccoord *pos, *size;
unsigned char color;
{
struct uccoord p0, p1, p2, p3;

if (lines<0 || lines>2) return;

p0.y=pos->y; p0.x=pos->x; 
p2.y=p0.y+size->y-1; p2.x=p0.x+size->x-1;
p1.y=p0.y; p1.x=p2.x; p3.y=p2.y; p3.x=p0.x;
scp0hlin(scplbar[lines][0], p0.y, p0.x+1, p1.x-1, color);
scp0cput(scpcrnr[lines][1], p1.y, p1.x, color);
scp0vlin(scplbar[lines][1], p1.y+1, p2.y-1, p1.x, color);
scp0cput(scpcrnr[lines][2], p2.y, p2.x, color);
scp0hlin(scplbar[lines][0], p2.y, p2.x-1, p3.x+1, color);
scp0cput(scpcrnr[lines][3], p3.y, p3.x, color);
scp0vlin(scplbar[lines][1], p3.y-1, p1.y+1, p3.x, color);
scp0cput(scpcrnr[lines][0], p0.y, p0.x, color);
}

void scpscrl(n,pos,size,color)
int n;
struct uccoord *pos, *size;
unsigned char color;
{
scp0scrl(n,pos,size,color);
}

void scpattr(pos,size,color)
struct uccoord *pos, *size;
unsigned char color;
{
register int i, ly;
struct uccoord up;
int lx;

up.y= pos->y;  	  up.x= pos->x;
ly= (int)size->y; lx= (int)size->x;
for (i=0; i<ly; i++) { scp0attr(lx,&up,color); up.y++; }
}

/*
	This routine transfers the contents of a window from the screen
	to memory, or from memory to the screen.  The caller must supply
	the buffer (of size at least scpisize(size)).
*/
void scpxfer(dir, addr, pos, size)
int dir;
unsigned char *addr;
struct uccoord *pos, *size;
{
register char *p;
register int ofs;
int soc, ly, lx, od, oa;

soc = (int)scpcmtp->sochar; 		ly  = (int)size->y;
od  = (soc*(int)scpcmtp->ncols); 	oa  = (soc*(lx=(int)size->x)); 
ofs = (od*(int)pos->y + soc*(int)pos->x);
for (p=addr; ly-->0; p+=oa, ofs+=od) scp0xfer(dir,p,ofs,lx);
}
