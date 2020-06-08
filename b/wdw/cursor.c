/*
	Set Real CuRSoR
*/

#include		<dos.h>
#include		"ucc.h"

void srcrsr(pos,loc)
struct uccoord *pos, *loc;
{
union REGS regs;
regs.h.ah= 2; regs.h.bh= 0;
regs.h.dh= pos->y+loc->y; regs.h.dl= pos->x+loc->x;
int86(0x10, &regs, &regs);
}
