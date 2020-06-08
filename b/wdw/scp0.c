#include		"ucc.h"
#include		"scp.h"

extern struct mtype *scpcmtp;
extern unsigned scpdseg ;

void movedata();
unsigned getvoff();
void uswap();

/*
	physical screen-access routines
	subroutines for scp routines.

	scp0cput		write a single character
	scp0wrs			write a string of characters

	scp0attr		change the attributes of screen characters

	scp0xfer		transfer an area of the screen <=> memory
	
*/
void scp0cput(c, uy, ux, co)
unsigned char c, uy, ux, co;
{
struct uccoord up;
unsigned char b[2];

up.y=uy; up.x=ux; b[0]=c; b[1]=co; 
movedata(scpdseg, b, scpcmtp->dmaseg, getvoff(&up), sizeof(b));
}

void scp0wrs(s,len,pos,co)
unsigned char *s;
int len;
struct uccoord *pos;
unsigned char co;
{
register char *p;
register unsigned voff ;
unsigned soc;
unsigned char b[2];

p=s; b[1]=co; soc=(unsigned)(scpcmtp->sochar);
for (voff=getvoff(pos); (b[0]=*p++) && len-->0; voff+=soc) {
   movedata(scpdseg, b, scpcmtp->dmaseg, voff, sizeof(b));
   }
}

void scp0attr(len,pos,co)
int len;
struct uccoord *pos;
unsigned char co;
{
register unsigned voff ;
register unsigned soc;
unsigned char a;

a=co; soc=(unsigned)(scpcmtp->sochar);
for (voff=getvoff(pos)+1; len-->0; voff+=soc) {
   movedata(scpdseg, &a, scpcmtp->dmaseg, voff, sizeof(a));
   }
}

void scp0xfer(dir, addr, o, lx)
int dir;
unsigned char *addr; 
int o, lx;
{
unsigned sseg, soff, dseg, doff, nbytes;
sseg= scpdseg; soff=(unsigned)addr;
dseg= scpcmtp->dmaseg; 	doff=(unsigned)o;
nbytes= ( (unsigned)lx * (unsigned)scpcmtp->sochar );
if (dir) { uswap(&sseg,&dseg); uswap(&soff,&doff); }
movedata(sseg, soff, dseg, doff, nbytes);
}

/*
	begin static subroutines
*/	

static unsigned getvoff(pos)
struct uccoord *pos;
{
register unsigned voff;
register unsigned soc ; 

soc= (unsigned)(scpcmtp->sochar);
voff= (unsigned)(scpcmtp->ncols);
voff*= (soc * (unsigned)(pos->y));
voff+= (soc * (unsigned)(pos->x));
return(voff);
}
