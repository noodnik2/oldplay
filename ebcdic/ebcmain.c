/*

	ebcmain.c			EBCDIC conversion utility
					Copyright (C) 1989, by ==C=R=C==


*/

#include	<stdio.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#include	"../compiler/compiler.h"
#include	"../getopt/clgetopt.h"
#include	"../ebcdic/ebcdic0.h"
#include	"../ebcdic/ebcdic.h"

#if	OPSYS==OPUNIX
#define	RMODE	(O_RDONLY)
#define	WMODE	(O_WRONLY|O_TRUNC|O_CREAT)
#define	WATTR	(S_IREAD|S_IWRITE)
#endif

#if	OPSYS==OPMSDOS
#define	RMODE	(O_RDONLY|O_BINARY)
#define	WMODE	(O_WRONLY|O_TRUNC|O_CREAT|O_BINARY)
#define	WATTR	(S_IREAD|S_IWRITE)
#endif

struct ebcopt ebco ;

void main(argc, argv)
int argc;
char *argv[];
{
	int type ;
	int i, e;
	int fi1, fi2 ;

	clinit(argv);
	ebco.etp= (char *)NULL;
	ebco.quiet= ebco.debug= ebco.help= 0;

	while( (i= clgetopt(argc, argv, "hqzE:")) != -1 ) {
		switch(i) {
			case 'h':	ebco.help= 1;			break;
			case 'q':	ebco.quiet= 1;			break;
			case 'z':	ebco.debug= 1;			break;
			case 'E':	ebco.etp= cloptarg;		break;
			default:	exit(12);			break;
		}
	}

	if (ebco.help) {
		ebchelp();
		exit(1);
	}

	if (ebcinit(ebco.etp)) {
		fprintf(stderr, "can't open ebcdic table\n");
		exit(28);
	}

	if (cloptind != argc) {
		if (cloptind==(argc-2)) {
			fi1= cloptind;
			fi2= cloptind+1;
			if (ebcopy(argv[fi1], argv[fi2])) {
				exit(24);
			}
			exit(0);
		}
		clerror("zero, or two filenames are required");
		exit(12);	
	}

	type= 0;
	while( (e= getchar()) != EOF ) {
		if (ebcistxt(e)) {
			e= ebc2asc(e);
		}
		else {
			type= 1;
		}
		putchar( e ) ;
	}
	
	if (type) {
		if (!ebco.quiet) clerror("contains nontext");
		exit(1);
	}

	exit(0);
}

static int ebcopy(from, to)
char *from, *to;
{
	char buf[32760];
	char mesg[256];
	int len, type ;
	int ifh, ofh;

	if ((ifh= open(from, RMODE))< 0) {
		return(cantopen(from));
	}

	if ((ofh= open(to, WMODE, WATTR))< 0) {
		close(ifh);
		return(cantopen(to));
	}
	
	type= 0;
	while( (len= read(ifh, buf, sizeof(buf)))> 0 ) {
		if (len> 0) {
			if (ebctrans(buf, len)) type= 1;
			if (write(ofh, buf, len)!=len) {
				close(ifh); close(ofh);
				if (!ebco.quiet) clerror("EOF on output");
				return(-3);
			}
		}
	}

	close(ifh);
	close(ofh);

	if (type) {
		sprintf(mesg, "contains nontext: %s", from);
		if (!ebco.quiet) clerror(mesg);
		return(1);
	}

	return(0);
}
		
static int cantopen(fn)
char *fn;
{
	char mesg[256];

	sprintf(mesg, "can't open: %s", fn);
	clerror(mesg);
	return(-1);
}
