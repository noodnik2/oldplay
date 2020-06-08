#include <stdio.h>

#define		WIDTH	35

int main()
{
	int i, x ; 
	short unsigned ch ;
	long unsigned sum ;

	i= 0 ; 
	sum= (long unsigned)0 ;
	while((x= getchar()) != EOF) { 
		ch= (short unsigned)x ;
		sum+= (long unsigned)((i&1)? ch: (ch*256)) ;
		printf("%02x", x) ; 
		if (++i==WIDTH) {
			printf("\n") ; 
			i= 0 ; 
		}
	}
	printf("\n\n") ; 
	sum= ((sum & 0xFFFF) + (sum >> 16) & 0xFFFF);
	printf("%02x", (int)(sum>>8));
	printf("%02x", (int)(sum&255));
	printf("\n") ;		/* mdr: 24.12.92, for pipe() */
	return(0) ; 
}
