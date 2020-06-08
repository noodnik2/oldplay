/*
	WINPoinT	- Update the real cursor to the position of the
			  cursor of the specified window.
*/

#include		"ucc.h"
#include		"win0.h"

extern struct win0 winframe[] ;
void srcrsr();

void winpt(wh)
int wh;
{
register struct win0 *wfp;
struct uccoord rcur;
wfp= winframe+(wh-1);
rcur.y= wfp->cursor.y+1; rcur.x= wfp->cursor.x+1; 
srcrsr(&wfp->pos,&rcur);
}
