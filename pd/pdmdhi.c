/*
	
	pdmdhi.c				PD machine dependent

	0= pdmdhello()  
	0= pdmdbye() 

	When RTM comes active, it sends a hello.  

	When PD sees a hello, it initializes QADTTY.

	Always returns 0.

*/

#include	"compiler.h"

int pdmdhello()
{
	wrstty("Hello") ;
	return(0) ; 
}

int pdmdbye() 
{
	wrstty("Goodbye") ;
	return(0) ; 
}
