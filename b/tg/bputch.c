#include		<dos.h>

typedef unsigned char UCHAR ; 

void bputch(c)
int c;
{
union REGS regs;

regs.h.ah = (UCHAR)14;
regs.h.al = (UCHAR)(c);
regs.x.bx = 7;
int86(0x10, &regs, &regs);
}
