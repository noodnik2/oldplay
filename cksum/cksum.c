#include	<stdio.h>
#include	"compiler.h"

long bufcksum() ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	char buf[4096] ;
	long cck ;
	int l; 

	cck= 0L ;
	while(gets(buf) != (char *) NULL) {
		l= strlen(buf) ;
		cck+= bufcksum(buf, l) ;
	}

	printf("%d\n", mkint(cck)) ;

}

/*

        Do a one's complement checksum

 */
long bufcksum(buffer, len)
chau *buffer;
int len;
{
        register int i ;
        short unsigned wrck ;
        long x ;

        x= 0L ;
        for (i= 0; i< len; i+= 2) {
                if (wrck= (short unsigned)buffer[i]) wrck*= 256 ;
                if ((i+1)< len) wrck+= (short unsigned)buffer[i+1] ;
                x+= (long) wrck ;
        }
	return(x) ;
}

static int mkint(x)
long x;
{
        x= (x & 0xFFFF) + (x >> 16) ;
        return((int) (x & 0xFFFF)) ;
}
