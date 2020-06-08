/*
	split.c						by Marty Ross


		split infile outfile1 outfile2 nbytes

		splits infile into two parts: outfile1 contains first "nbytes"
		of bytes from infile, outfile2 contains rest of infile.
*/

#include	"compiler.h"
#include	"files.h"

#define		BUFSIZE		0x4000		/* 16K buffer */

static int ifn, ofn1, ofn2 ;

void finis(), output() ;

void main(argc, argv)
int argc;
char *argv[];
{
	char c;
	int last, count, bufsize, req;
	long lbufsize, length, offset, lwork;
	char buffer[BUFSIZE+1];

	if (argc != 5) {
		printf("syntax:      split infile[.ext] out1file[.ext] out2file[.ext] nbytes\n");
		exit(1);
	}

	if (sscanf(argv[4],"%ld",&length)!=1 || length<=0L) {
		printf("Fourth parameter must be positive integer\n");
		exit(2);
	}

	if ((ifn= open(argv[1], INBFMODE))< 0 ) {
		printf("Can't open '%s'\n", argv[1]);
		exit(3);
	}

	if ((ofn1= open(argv[2], OUTBFMODE, OUTBFPROT))< 0 ) {
		printf("Can't open '%s'\n", argv[2]);
		exit(4);
	}

	if ((ofn2= open(argv[3], OUTBFMODE, OUTBFPROT))< 0 ) {
		printf("Can't open '%s'\n", argv[3]);
		exit(5);
	}

	offset= 0L;
	last= 0;
	bufsize= sizeof(buffer);
	lbufsize= (long)bufsize;
	while(ifn) {
		if (lbufsize+offset>length && !last) {
			req= (int)(length-offset);
			last= -1;
		}
		else req= bufsize;
		if (req>0) {
			count= read(ifn, buffer, req);
			if (last<=0) output(ofn1, buffer, count, 1, 98);
			else output(ofn2, buffer, count, 2, 99);
		}
		if (count != req) {
			if (last<=0) {
				printf("NOTE: EOF occurred during first part\n");
			}
			else if (count==0) {
				printf("NOTE: No bytes written to second file\n");
			}
			finis(0);
		}
		else {
			if (last<0) last= 1;
			offset+= (long) count;
		}
	}
}

/*
	subroutines 
*/

static void finis(rc)
int rc;
{
	if (ofn1) close(ofn1);
	if (ofn2) close(ofn2);
	if (ifn) close(ifn);
	ifn= ofn1= ofn2= 0;
}

static void output(fn, buffer, count, num, rc)
int fn;
char *buffer;
int count, num, rc;
{
	if (write(fn,buffer,count) != count) {
		printf("EOF on output file %d!\n", num);
		finis(rc);
	}
}
