#include		<dos.h>

extern int montype ;

static unsigned char cs[][2][2] = {
     { { 12, 13 }, { 6, 7 } },				/* Regular     */
     { { 6, 13 }, { 4, 7 } }				/* Insert mode */
     } ;

void cursor(style)
int style;
{
union REGS regs ;

regs.h.ah = 1;
regs.h.ch = cs[style][montype][0];
regs.h.cl = cs[style][montype][1];
int86(0x10, &regs, &regs);
}
