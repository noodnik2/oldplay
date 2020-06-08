#define		BEEP		putch(7)

#ifndef		MONTYPE		
#define		MONTYPE		1 	/* CGA is default */
#endif

void scoputch(), scoinit(), scorcp(), scoscp() ;
void rpcur(), spcur(), queue(), glhome(), dobs(), newhome() ;
int doesc(), pfkey(), getkey();
char *malloc();

extern unsigned char scorow ; 
extern char          sco_ucm ; 
extern char 	     insmode ; 		/* Insert mode flag */

static int row0, col0;			/* "home" cursor pos */

int montype = MONTYPE ;			/* Monitor type     */


/*	
	local variables in next routine:

	cp=  		Cursor Position
	mp=  		Max. cur. Position
 	ch=  		Character from KBD
 	row, col= 	Current cursor pos

*/
char *getline(buffer, maxlen)
char *buffer;
int maxlen;
{
int cp, mp, ch, row, col ;

mp = cp = 0;						/* Initialize    */
scoinit(montype ? 0xb800 : 0xb000, 25, 80);

newhome();						/* get "home"    */

do {	 						/* Loop on KEYBD */

   if (ch=getkey()) {					/* Get, test key */
      if (ch=='\r') break;				/* End line w/CR */
      else if (ch=='\n') continue;			/* Ignore LFs    */
      else if (ch=='\033') {  				/* Do ESC        */
	 if (doesc(buffer,&cp,&mp)) break;
	 }
      else if (ch=='\b') {				/* Do BackSpaces */
	 dobs(buffer,&cp,&mp);	
	 }
      else if (ch<0) { 					/* And PFkeys    */
         if (pfkey(-ch,buffer,&cp,&mp)) break;
	 }
      else {		     				/* Regular Chars */
	 if (mp>(maxlen-2)) { BEEP; continue; }
	 if ( insmode && (cp<mp) ) {
	    memcpy((buffer+cp)+1,(buffer+cp),mp-cp+1);	/* insert here   */
  	    buffer[cp] = (char)(ch);			/* write in buff */
	    scoputch(ch); scopcp(0);
	    cp++,mp++; draw(buffer+cp,mp-cp);		/* new rightline */
	    scopcp(1);
	    }
	 else {						/* No insert:    */
  	    buffer[cp++] = (char)(ch);			/* write in buff */
	    scoputch(ch);     				/* Echo to TTY   */
	    }
	 }
      if (cp>mp) { 					/* insure no overflow */
	 if (cp<maxlen) mp = cp; 
	 else { BEEP; cp=mp; }
	 }
      }
   } while(1);

buffer[mp]='\0';
glhome();
draw(buffer,mp);
queue(buffer);						/* Save as _Last */
return(buffer);
}

void glhome()
{
unsigned row, col;
row0 = (unsigned)(scorow);
if (sco_ucm) spcur(row0,col0);
scoscp(row0,col0);
}

void newhome() { rpcur(&row0,&col0); scoscp(row0, col0); }

