/*



*/

#include	<stdio.h>
#include	"../compiler.h"

void wrstty() ;

void main(argc, argv)
int argc;
char *argv[];
{
	register char *s ;
	int rc, sa, si ;
	char buf[2048] ;
	char cont[3];

	if (argc != 3) {
		printf("give both numbers, sa and si.\n");
		exit(1);
	}

	if (sscanf(argv[1], "%d", &sa) != 1) {
		printf("sa is bad\n");
		exit(2);
	}

	if (sscanf(argv[2], "%d", &si) != 1) {
		printf("si is bad\n");
		exit(2);
	}

	if (rc= rddstri("info.dat", 2017)) {
		printf("R(%d); rddstri\n", rc);
		exit(3);
	}

	if (rc= rddstr(sa, si, buf, sizeof(buf))) {
		printf("R(%d); rddstr(%d,%d,buf,sizeof(buf));\n", rc, sa, si);
		exit(4);
	}

	wrstty(buf);

}

void wrstty(s)
char *s;
{
	while(*s) putchar(*s++) ;
}
