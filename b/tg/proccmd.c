void proccmd(s)
char *s;
{
int rc;

			/* printf("you said \"%s\"\n", buffer); */

rc = system(s);
if (rc) printf("R(%d); from '%s'\n", rc, s);

}
