#include		<dos.h>

#define			BELL		1

void beep()
{
#if	BELL==1
union REGS regs ;
regs.x.ax=0x0E07 ;
int86(0x10, &regs, &regs);
#endif
}
