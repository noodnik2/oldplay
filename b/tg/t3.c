#define			ESC		27

char *str[] = { 
	"Hello there this is a test string which I am using. ",
	"                                                    "
	} ;

void bputch();

void main()
{
int d, y, x, i, q, key, row, col;

for (d=q=0;;/* q=!q, */ d++) {
   spcur(0,0);
   for (x=0;x<5;x++) {
       for (i=d;str[q][i];i++) {
           bputch(str[q][i]);
           }
      } 
   while((key=getkey())==0) ;
   if (key==ESC) { putch('\r'); putch('\n'); break; }
   } 
}
