#include		<dos.h>
#include		"ucc.h"

						/* External subroutines */
void scp0cput() ;

void scp0vlin(bordc,y0,y1,x,color)
unsigned char bordc, y0, y1, x, color;
{
register int i, j;
struct uccoord up;
int k;

j= (int)y1; i=(int)y0; 
for (k=(y0<y1)?1:-1;;i+=k) {
    scp0cput( bordc, (unsigned char)i, x, color);
    if (i==j) break;
    }
}

void scp0hlin(bordc,y,x0,x1,color)
unsigned char bordc, y, x0, x1, color;
{
register int i, j;
struct uccoord up;
int k;

i=(int)x0; j= (int)x1;
for (k=(x0<x1)?1:-1;;i+=k) {
    scp0cput( bordc, y, (unsigned char)i, color);
    if (i==j) break;
    }
}
