#include		"ucc.h"
#include		<dos.h>

/*

      scp0scrl(n,pos,size,co);

      This routine scrolls the window 
      specified using the background color.

*/
void scp0scrl(nl,pos,size,co)
int nl;
struct uccoord *pos, *size;
unsigned char co;
{
union REGS regs;

regs.h.ah= 6;				/* Scroll up:   BIOS subfunction 6 */
if (nl<0) { nl=-nl; regs.h.ah++; }	/* Scroll down: BIOS subfunction 7 */
regs.h.al= (unsigned char)nl;
regs.h.ch= (unsigned char)pos->y; 
regs.h.cl= (unsigned char)pos->x; 
regs.h.dh= (unsigned char)(pos->y+size->y-1); 
regs.h.dl= (unsigned char)(pos->x+size->x-1);
regs.h.bh= co;
int86(0x10, &regs, &regs);
}
