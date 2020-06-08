#if 0
/*

   bswap.c				Swap bytes in binary value.
					Copyright (C) 1989, by ==C=R=C==	


	void bswap(s, l);

	string "s" is reversed so that all the lowest-order bytes
	occupy the highest-order positions, and vice-versa.

	This function qualifies to swap binary integers between
	HILO and LOHI representations, e.g:


	  on a HILO machine, the C test program,

		typedef word int ;	/* int, or whatever */
		test(hilo_word)
		word hilo_word;
		{
			word lohi_word= hilo_word ;
			bswap(&lohi_word, sizeof(lohi_word));
			printf("0x%x, 0x%x", hilo_word, lohi_word);
		}
		
	  when invoked as "test(0x1234)", would print:
			
		0x1234, 0x34120000		(unix machine)
	 or:
		0x1234, 0x3412			(MS-DOS machine)


*/
#endif

#include	"compiler.h"

void bswap(ca, l)
unsigned char *ca ;
int l;
{
	register int i ;
	unsigned char tc ;
	int l2 ;

	l2= (l/2) ;
	for (i= 0; i< l2; i++) {
		tc= ca[i] ;
		ca[i]= ca[l-i-1] ;
		ca[l-i-1]= tc ;
	}
}
