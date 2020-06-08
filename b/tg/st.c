extern unsigned char scolines ;

void outstring();

char *outmsg[] = { 
       "Hello there this is a test of the new and the improved thing which I am testing at this time--that is all",
       "                                                                                                         " 
	} ;

void main()
{
unsigned x, row, col;
int z, q;

scoinit(0xb000,25,80);

spcur(24,0);
scoscp(24,0);

for (z=q=0;;z++ /* ,q=!q */ ) {
   for (x=0; x<6; x++) {
        outstring(&outmsg[q][z-q]);
	}
   if (!again()) break;
   scorcp(&row, &col);
   x = row - (unsigned)(scolines) ;	
   spcur(x,0);
   scoscp(x,0);
   } 

scorcp(&row, &col);
spcur(row, col);
printf("\nDone.  scolines=%d.\n",(unsigned)scolines);
}

static void outstring(s)
char *s;
{
while(*s) {
     scoputch((unsigned)(*s++));
     }
}

static int again()		/* False ONLY if user hits ESC key */
{
int x ;
while(kbhit()) getch();
while(!kbhit());
return(getch()!=27);
}
