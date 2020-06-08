/* 
	flag= pdmdinit() 
		One-time initialization code. 
		Return 0 if successful. 

	(void) pdmdclos() 
		One-time closeout code. 
		pdmdclos() will not be called unless pdmdinit() succeeded.

	Copyright (c) 1990 by ==C=R=C== 
*/

#include	"compiler.h" 
#include	"pd.h"

int pdmdinit() 
{
	if (pddbugflg) pderrmsg("dbg: mdinit") ;
	return(0) ; 
} 

void pdmdclos() 
{
	if (pddbugflg) pderrmsg("dbg: mdclos") ;
	return ; 
}
