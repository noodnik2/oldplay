#include	<stdio.h>

static char *lp_dev= "/dev/term/s02" ;
static char *title= (char *)NULL ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	int i, rc ;
	char cmdbuf[5120] ;

	for (i= 1; i< argc; i++) {
		if (argv[i][0] != '-') break ;
		switch(argv[i][1]) {

			case 'd': lp_dev= argv[i] + 2 ;		break ;
			case 't': title=  argv[i] + 2 ;		break ;

			default:
				fprintf(
					stderr,
					"invalid option: %s\n",
					argv[i]
				) ;
				exit(1) ;
				break ;
		}
	}

	if (i != argc) {
		fprintf(stderr, "no filename(s) allowed; reads stdin\n") ;
		exit(2) ;
	}

	if (title != (char *)NULL) {
		sprintf(cmdbuf, "(echo %s; cat) > %s", title, lp_dev) ;
	}
	else {
		sprintf(cmdbuf, "cat > %s", lp_dev) ;
	}

	rc= system(cmdbuf) ;
	
	printf("rc from '%s' is %d\n", cmdbuf, rc) ;
	exit(0) ;
}
