#include	<stdio.h>

#include	"compiler.h"
#include	"pd.h"

main()
{
	int rc ;

	if (pdinit()) {
		fprintf(stderr, "can't PDINIT\n") ;
		exit(1) ;
	}
	rc= pdterm() ;

	pdfini() ;
	exit(rc) ;
}
