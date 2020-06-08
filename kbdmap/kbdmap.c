#include	<stdio.h>
#include	<signal.h>
#include	<termio.h>

#define		NFILES		1

#define		MODE_READFILE	0
#define		MODE_WAITUSER	1
#define		MODE_WAITKBD	2

#define		LINEMAX_LENGTH	256

static struct termio tio[NFILES], tio_save[NFILES] ;	

static void abort(int intno) ;
static void save_termio(void) ;
static void restore_termio(void) ;
static void note(char *msg) ;

void main(int argc, char **argv)
{
	char line[LINEMAX_LENGTH+1] ;
	unsigned char c ;
	int mode, i ; 
	FILE *listfp= (FILE *) NULL ;
	FILE *mapfp= (FILE *) NULL ;

	fprintf(stderr, "kbdraw 0.01 - Raw keyboard input/display\n") ;


	if (argc> 1) {
		listfp= fopen(argv[1], "r") ;
		if (listfp == (FILE *) NULL) {
			fprintf(stderr, "file: '%s' not found\n", argv[1]) ;
			exit(1) ;
		}
		fprintf(stderr, "(reading keylist from '%s')\n", argv[1]) ;
	}
	else {
		fprintf(stderr, "(note: no keylist file given as 1st arg)\n") ;
	}
	if (argc> 2) {
		mapfp= fopen(argv[2], "w") ;
		if (mapfp == (FILE *) NULL) {
			fprintf(stderr, "can't create: '%s'\n", argv[2]) ;
			exit(1) ;
		}
		fprintf(stderr, "(writing map to '%s')\n", argv[2]) ;
	}
	else {
		fprintf(stderr, "(note: no output map file given as 2nd arg)\n") ;
	}

	save_termio() ;
	signal(SIGINT, abort) ;

	fprintf(stderr, "<Break> to abort\n") ;
	mode= MODE_READFILE ;
	while(1) {
		if (mode == MODE_READFILE) {
			if (listfp != (FILE *) NULL) {
				if (!fgets(line, LINEMAX_LENGTH, listfp)) {
					break ;
				}
				line[strlen(line)-1]= '\0' ;
				printf("key: '%s'\n", line) ;
				if (mapfp != (FILE *) NULL) {
					fprintf(mapfp, "key: '%s'\n", line) ;
				}
			}
			mode= MODE_WAITUSER ;
		}
		if (read(0, &c, 1)> 0) {
			printf("c=0x%02X\n", (int) c) ;
			if (mapfp != (FILE *) NULL) {
				fprintf(mapfp, "c=0x%02X\n", (int) c) ;
			}
			mode= MODE_WAITKBD ;
			continue ;
		}
		if (mode == MODE_WAITKBD) {
			if (listfp == (FILE *) NULL) {
				note("end of keyseq") ;
			}
			mode= MODE_READFILE ;
		}
	}
	note("end of keylist") ;
	restore_termio() ;

}

static void save_termio()
{
	int i ;

	for (i= 0; i< NFILES; i++) {
		ioctl(i, TCGETA, &tio[i]) ;
		memcpy(&tio_save[i], &tio[i], sizeof(struct termio)) ;
		tio[i].c_lflag&= ~(ICANON | ECHO) ;
		tio[i].c_cc[VMIN]= 0 ;
		tio[i].c_cc[VTIME]= 5 ;
		ioctl(i, TCSETAW, &tio[i]) ;
	}
}

static void restore_termio()
{
	int i ;

	for (i= 0; i< NFILES; i++) {
		ioctl(i, TCSETAW, &tio_save[i]) ;
	}
}

static void abort(int intno)
{
	restore_termio() ;
	printf("\n(user aborted)\n") ;
	exit(1) ;	
}

static void note(char *msg)
{
	printf("-----%s\n", msg) ;
}

/* end of kbdmap.c */
