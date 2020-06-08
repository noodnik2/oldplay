#include		<dos.h>

void getcpos(rowp, colp)
int *rowp, *colp;
{
union REGS regs ;

regs.h.ah = 3;				/* READ CURSOR POSITION */
regs.h.bh = 0;				/* PAGE ZERO 		*/
int86(0x10,&regs,&regs);
(*rowp) = (int)(regs.h.dh) ;
(*colp) = (int)(regs.h.dl) ;
}

void putcpos(row, col)
int row, col;
{
union REGS regs ;

regs.h.ah = 2;				/* SET CURSOR POSITION 	*/
regs.h.bh = 0;				/* PAGE ZERO 	       	*/
regs.h.dh = (char)(row);
regs.h.dl = (char)(col);
int86(0x10,&regs,&regs);
}
