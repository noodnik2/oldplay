#include		<dos.h>

/*
	procedure that determines monitor type:

		-1	- Unknown
		0	- Monochrome Display Adapter 	(MDA)
		1	- Color Graphics Adapter	(CGA)
*/


int montype()
{
register int m;
union REGS regs;

regs.h.ah=15;
int86(0x10,&regs,&regs);
m=(int)regs.h.al;
return(m==7? 0: (m==2 || m==3 || m==6)? 1: -1);
}
