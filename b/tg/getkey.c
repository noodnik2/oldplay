int getkey()
{
int keyval ;

if (kbhit()) {
  if ((keyval=(int)(getch()))==0) {
     keyval = -((int)(getch())) ;
     }
  return(keyval);
  }
return(0);
}
