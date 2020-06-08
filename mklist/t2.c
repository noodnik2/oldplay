#include	<stdlib.h>

void main()
{
	long l ;

	while(1) {
		l= rand() ;
		if (l == (RAND_MAX - 1)) break ;
	}
}
