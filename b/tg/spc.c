/*

	Physical Cursor subroutines

	spcur		Set Physical Cursor
	rpcur		Read Physical Cursor

*/

#include		<dos.h>


void spcur(row, col)
unsigned row, col ;
{
union REGS regs ;
regs.h.ah=2; regs.h.bh=0;
regs.h.dh = (char)(row);
regs.h.dl = (char)(col);
int86(0x10, &regs, &regs);
}

void rpcur(rowp, colp)
unsigned *rowp, *colp ;
{
union REGS regs ;
regs.h.ah = 3;
regs.h.bh = 0;
int86(0x10, &regs, &regs);
(*rowp) = (unsigned)(regs.h.dh);
(*colp) = (unsigned)(regs.h.dl);
}
