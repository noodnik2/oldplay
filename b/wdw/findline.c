char *lineup(n,s,s0)
int n;
char *s, *s0;
{
register char *p;

for (p=s;n-->0 &&  s>s0; s=p) {
   if (*(--p)=='\n') { if (*(--p)=='\r') p--; }
   while(p>s0) { if (*p=='\n') { p++; break; } p--; }
   }
return(s);
}

char *linedn(n,s)
int n;
char *s;
{
register char *p;
for (p=s; *p && n-->0; s=p) { while(*p) if (*p++=='\n') break; }
return(*p?p:s);
}
