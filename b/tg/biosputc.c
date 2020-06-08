#include		<dos.h>

typedef unsigned char UCHAR ; 

void biosputc(row,col,c)
int row, col, c;
{
union REGS regs;

regs.h.ah = (UCHAR)2;				/* Set Cursor */
regs.h.bh = (UCHAR)0;
regs.h.dh = (UCHAR)row;
regs.h.dl = (UCHAR)col;
int86(0x10, &regs, &regs);

regs.h.ah = (UCHAR)10;				/* Write character */
regs.h.bh = (UCHAR)0;
regs.h.al = (UCHAR)(c);
regs.x.cx = 1;
int86(0x10, &regs, &regs);
}
