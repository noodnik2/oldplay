#include	"compiler.h"
#include	"tty.h"

int ttymode= 1 ;

void ttynop()
{

}

void ttymetr(uy, ux, sflag, sarea)
int uy, ux, sflag ;
char *sarea ;
{
	char buf[80] ; 

	sprintf(buf,"open       %d, %d (%d,0x%x)\n", uy, ux, sflag, sarea) ; 
	pdwrstty(buf) ; 
}

void ttyclos(sflag, sarea)
int sflag ;
char *sarea ;
{
	char buf[80] ; 

	sprintf(buf, "close (%d,0x%x)\n", sflag, sarea) ; 
	pdwrstty(buf) ; 
}

void ttypt(y,x,style)
int y, x, style ;
{
	char buf[80] ; 

	sprintf(buf,"pt         %d %d , %d\n",style,y,x) ; 
	pdwrstty(buf) ; 
}
