#include		"ucc.h"

/*
      scpcput(c,pos,size,color,cursor);
*/

void scp0cput(), scp0scrl();

/*
	This routine places the character specified at the current cursor
	location.  Also, it guards against out-of-window cursor movement.
	Many cursor control characters are interpreted here as well.

	The cursor structure is updated to the position of the NEXT
	character to be written.

*/
void scpcput(c, pos, size, color, cursor)
int c;
struct uccoord *pos, *size, *cursor;
unsigned char color;
{
struct uccoord up;
unsigned char ui;	    

up.y= pos->y+cursor->y;  up.x= pos->x+cursor->x;

if (cursor->x >= size->x) { 
   up.x= pos->x; up.y++; cursor->y++;
   }

if (cursor->y >= size->y) { 
   ui= (unsigned char)1 + cursor->y - size->y ;
   scp0scrl((int)ui, pos, size, color); 
   up.y -= ui;
   }
if (c>=' ') scp0cput((unsigned char)c, up.y, up.x++, color);
else { 
   switch(c) {
   	case '\n': ++up.y; up.x= pos->x;	break;	/* L.F. */
        case '\r': up.x= pos->x;		break;	/* C.R. */
        case 7:    beep();	     		break;	/* Bell */
        case 8:    if (up.x>pos->x) up.x--; 
		   else if (up.y>pos->y) {
			up.y--; up.x=pos->x+size->x-1;
			}	     		break;	/* B.S. */
        case 12:   scp0scrl((int)size->y,pos,size,color); /* F.F. */
        case 11:   up.y=pos->y; up.x=pos->x;	break;	/* Home */
        case 28:   up.x++;			break;	/* Rght */
        case 29:   if (up.x>pos->x) up.x--;	break;	/* Left */
        case 30:   up.y++;			break;	/* Up   */
        case 31:   if (up.y>pos->y) up.y--;	break;	/* Down */
	default:   scp0cput((unsigned char)c, up.y, up.x++, color);
  	}
   }
cursor->y = up.y - pos->y; cursor->x = up.x - pos->x;
}				
