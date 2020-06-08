/*


*/

#include	<stdio.h>
#include	<termio.h>

#include	"compiler.h"

#include	"pw.h"
#include	"pw_inter.h"

void pw_inter_init()
{
	pw_tio_init() ;
}

void pw_inter_fini()
{
	pw_tio_fini() ;
}

int pw_inter_getcmd(to_sec)
int to_sec ;
{
	switch(pw_tio_getch(to_sec)) {
		case 'r':
		case 'R':	return(PWIC_RPT) ;
				break ;

		case 'q':
		case 'Q':	return(PWIC_EXIT) ;
				break ;
	}
	return(PWIC_NONE) ;
}
