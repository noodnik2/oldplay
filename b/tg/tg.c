#include	<stdio.h>
#define		toupper(c)	(((c)>='a'&&(c)<='z')?((c)&223):(c))

void proccmd() ;

extern int montype ;
extern char sco_om ;

char *getline(char *,int);

void main(argc, argv)
int argc;
char *argv[];
{
char buffer[2000], *p, c;
int i;

for (i=1; i<argc; i++) {			/* Parse, process arguments */
   c=(*argv[i]); c=toupper(c);
   if (c=='C') montype = 1;
   else if (c=='M') montype = 0;
   else if (c=='B') sco_om = 1;
   else if (c=='N') sco_om = 0;
   }

do {						/* Read commands 'till exit */
   putch('.'); putch(' ');
   p = getline(buffer,sizeof(buffer)); printf("\n");
   if (!strcmp(p,"help")) printf("type \"exit\" to quit.\n");
   else if (*p) proccmd(p);
   } while(strcmp(p,"exit"));

cursor(0);

printf("Bye.\n");

}
