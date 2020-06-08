#define			ESC		27

void bputch();

void main()
{
int key;

do {
   if ((key=getkey())>0) {
      bputch(key);
      }
   } while(key!=ESC) ;
}
