#include		<dos.h>

void scrollf(tr, tc, br, bc, a)
unsigned tr, tc, br, bc ;
char a;
{
union REGS regs;

regs.x.ax = 0x0601 ;
regs.h.ch = (char)tr ;
regs.h.cl = (char)tc ;
regs.h.dh = (char)br ;
regs.h.dl = (char)bc ;
regs.h.bh =     a ;
int86(0x10, &regs, &regs);
}
