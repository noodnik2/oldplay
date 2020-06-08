#include		"tkeys.h"

int getch();
void wincput();
      
void doit(wh)
int wh;
{
int c;
winactiv(wh);
for (winpt(wh);;) {
   switch(c=getkey()) {
      case KEYESC:		return;	
				break;
      default: 			wincput(wh, c);
				winpt(wh);
				break;
      }
   }
}

static int getkey()
{
register int c;
if ((c=getch())<0) c=getch();
return(c);
}
