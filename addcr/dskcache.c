/*

	dskcache.c:			routines for buffered file I/O

	read section:

	 (void)	dskrsti(fh, bbuf, bsize)	resets input stream for fh
          (int)	len= dskread(buf, len)		reads into buf for len
	  (int) ch= dskrdc()			reads character from file,
						returns EOF (-1) if past end
	 (void) ch= dskurdc(chr)		un-reads character from file
						NOTE: file must have been read
	  (int)	rc= dskrstif()			closes input stream; returns 0
						only if EOF (-1) encountered

	write section:

	 (void)	dskrsto(fh, bbuf, bsize)	resets output stream for fh
	  (int)	len= dskwrite(buf, len)		writes from buf for len
	  (int)	rc= dskwrc(chr)			writes character to file
	  (int)	rc= dskrstof()			closes output stream
						NOTE: rc is nonzero if error
*/

#include	"compiler.h"
#include	<stdio.h>

struct dskcache {
	int fh ;
	unsigned char *bbuf ;
	int bsize ;
	int eof, bptr, blen ;
} ;
	
static struct dskcache dskr ;
static struct dskcache dskw ;


static int dskwrblk() ;
static int dskrblk() ;

/*
	void dskrsti(fh, buf, size)

	note:	if (fh< 0), then one-time read of characters in (buf),
		for (size) will be done, instead of reading from file.

*/
void dskrsti(fh, buf, size)
int fh, size ;
char *buf ;
{
	dskr.fh= fh ;
	dskr.bbuf= (unsigned char *)buf ;
	dskr.bsize= size ;
	dskr.bptr= 0 ;
	dskr.blen= (fh< 0)? size: 0 ;
	dskr.eof= 0 ;
}

void dskurdc(chr)
char chr ;
{
	dskr.bbuf[--dskr.bptr]= (unsigned char) chr ;
}

int dskrdc()
{
	if (dskr.bptr == dskr.blen) {
		if (dskrblk()) return(-1) ;
	}
	return((int)(dskr.bbuf[dskr.bptr++])) ;
}

#if 0
int dskread(buf, len)
char *buf ;
int len ;
{
	register int outlen ;
	int ch ;

	for (outlen= 0; outlen< len; outlen++) {
		if ((ch= dskrdc())< 0) break ;
		buf[outlen]= (char)ch ;
	}
	return(outlen) ;
}
#endif

int dskrstif()
{
	if (dskr.fh>= 0) {
		close(dskr.fh) ;
		dskr.fh= -1 ;
	}
	return(dskr.eof? 0: 1) ;
}

void dskrsto(fh, buf, size)
int fh, size ;
char *buf ;
{
	dskw.fh= fh ;
	dskw.bbuf= (unsigned char *)buf ;
	dskw.bsize= size ;
	dskw.bptr= 0 ;
/*	dskw.blen= 0 ;		*/		/* unused */
	dskw.eof= 0 ;
}

#if 0
int dskwrite(buf, len)
char *buf ;
int len ;
{
	register int outlen ;

	for (outlen= 0; outlen< len; outlen++) {
		if (dskwrc(buf[outlen]) != 0) break ;
	}
	return(outlen) ;
}
#endif

int dskwrc(chr)
char chr ;
{
	if (dskw.bptr==dskw.bsize) {
		if (dskwrblk()) return(-1) ;		/* error flushing */
	}
	dskw.bbuf[dskw.bptr++]= (unsigned char) chr ;
	return(0) ;
}

int dskrstof()
{
	int rc ;

	rc= dskwrblk() ;
	if (dskw.fh>= 0) {
		close(dskw.fh) ;
		dskw.fh= -1 ;
	}
	return(rc) ;
}

/*
	end of entry points
	begin static routines
*/

static int dskrblk()
{
	if (dskr.fh>= 0) {
		if ((dskr.blen= read(dskr.fh, dskr.bbuf, dskr.bsize))> 0) {
			dskr.bptr= 0 ;
			return(0) ;
		}
	}
	dskr.eof= 1 ;
	return(-1) ;
}

static int dskwrblk()
{
	if (dskw.fh> 0) {
		if (dskw.bptr> 0) {
			if (write(dskw.fh, dskw.bbuf, dskw.bptr) != dskw.bptr) {
				dskw.eof= 1 ;
				return(-1) ;	/* error flushing */
			}
			dskw.bptr= 0 ;
		}
		return(0) ;			/* flushed ok     */
	}
	return(1) ;				/* no output file */
}
