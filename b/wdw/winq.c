/*

	Window-Query:			Reports window status

	winqcur()	returns the index of the currently active window
	winqset()	returns attributes of a currently open window

	0 	Window location 
	1 	Window size
	2 	Window cursor
	3 	Window colors

*/

#include		"ucc.h"
#include		"win0.h"

extern struct win0 winframe[MAXWINS];
extern int winidx;

int winqcur() { return(winidx); }

void winqset(wh, id, rp)
int wh, id;
struct uccoord *rp;
{
register struct win0 *wfp;
register struct uccoord *result;

wfp= winframe+(wh-1);
result= rp;
switch(id) {
     case 0:   	result->y=wfp->pos.y+1;  result->x=wfp->pos.x+1; 	break;
     case 1:   	result->y=wfp->size.y-2; result->x=wfp->size.x-2;	break;
     case 2:   	result->y=wfp->cursor.y; result->x=wfp->cursor.x;	break;
     case 3:   	result->y=wfp->color.y;  result->x=wfp->color.x;	break;
     }	
}
