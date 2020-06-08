/*

	rebin:			Reconstruct Binary version of file
				converted to ASCII by 'debin'.

				Copyright (C) 1989, by ==C=R=C==

		The file is a series of lines containing hexadecimal
		byte values.  Each line ends with a line-feed or a
		carriage-return/line-feed.  The file data ends when a
		null line is encountered, and if a checksum is present,
		it follows the null line.  Any data after the checksum
		is ignored.

*/

#include	<stdio.h>
#include	<ctype.h>

short unsigned xnib() ;

void main()
{
	int col, i, in_cksum ; 
	short unsigned ch, chkval ;
	long sum ;

	in_cksum= 0;				/* not reading checksum */
	sum= (long) 0;

	for (col= 0; (i= getchar()) != EOF;) {
		if (i == '\n') {			/* if end of line */
			if (col==0) in_cksum= 1;	/* if null line   */
			col= 0;
			continue ;
		}
		if (!isxdigit(i)) continue;		/* skip otherstuf */
		ch= (xnib(i)<<4) | xnib(getchar()) ;	/* read hex char  */
		if (in_cksum==1) {			/* 1st byte of CS */
			chkval= ch * (short unsigned)256 ;
			in_cksum++;
			continue;
		}
		if (in_cksum>1) {			/* 2nd byte of CS */
			chkval+= ch ;
			sum= (((sum & 0xFFFF) + (sum >> 16)) & 0xFFFF);
			if (((short unsigned)sum) != chkval) {
				fprintf(stderr, "error: checksum failed.\n");
				fprintf(stderr, "c=0x%04x r=0x%04x\n",
				  (unsigned)sum, (unsigned)chkval);
				exit(1);
			}
			exit(0);
		}
		putchar((int)ch);		/* output char    */
		sum+= (long)((col&1)? ch: ch*256) ;
		col++ ;
	}
	if (in_cksum<2) {
		fprintf(stderr, "warning: no checksum\n");
	}
	if (col!=0) {
		fprintf(stderr, "error: file ended mid-line\n");
		exit(1);
	}
}

static short unsigned xnib(x)
int x;
{
	if ((x>='A') && (x<='F')) return((short unsigned)(x-'A'+10));
	if ((x>='a') && (x<='f')) return((short unsigned)(x-'a'+10));
	return((short unsigned)(x-'0'));
}
