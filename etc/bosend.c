#include	<stdio.h>

static void sendit() ;

void main(argc, argv)
int argc;
char *argv[];
{
	int i;

	for (i= 1; i< argc; i++) {
		sendit(argv[i]);
	}
}

unsigned char START_STR[]= { "#tshdski begin " } ;
unsigned char END_STR[]= { "#tshdski end " } ;

static void sendit(s)
char *s;
{
	char buffer[4096] ;
	FILE *ifp ;

	if ((ifp= fopen(s, "r"))==(FILE *)NULL) {
		fprintf(stderr, "Can't open: %s\n", s);
		return;
	}
	write(1, START_STR, strlen(START_STR));
	write(1, s, strlen(s));
	write(1, "\n", 1);
	while(fgets(buffer, sizeof(buffer), ifp) != (char *)NULL) {
		write(1, buffer, strlen(buffer));
	}
	fclose(ifp);
	write(1, END_STR, strlen(END_STR));
	write(1, s, strlen(s));
	write(1, "\n", 1);
}
