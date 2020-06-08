char *linedn();
char *strncpy();

char *subline(i,s,b,ml)
int i, ml;
char *s, *b;
{
register int mp;
register char *more;

s=linedn(i,s); more=linedn(1,s); 
if (mp=(int)(more-s)) {
   if (s[--mp]=='\n') {
      if (mp) { 
	 if (s[--mp]=='\r') { 
	    if (mp) mp--; 
	    } 
	 else mp++;
	 }
      }
   else mp++;
   if (mp>=ml) mp=ml-1;
   }
strncpy(b,s,mp+1);
b[mp]=0;
return(b);
}
