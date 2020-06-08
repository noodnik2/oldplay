/*




*/

#include	<stdio.h>
#include	<signal.h>

#include	"compiler.h"
#include	"chkout.h"

int chk_trap(restore_routine)
void (* restore_routine)() ;
{
	void (* rtn)() ;

	rtn= (restore_routine != (void (*)())NULL)? 
		restore_routine: (void (*)())SIG_DFL ;
	signal(SIGINT, rtn) ;
	signal(SIGHUP, rtn) ;
	signal(SIGKILL, rtn) ;
	signal(SIGTERM, rtn) ;
	signal(SIGQUIT, rtn) ;
	return(0) ;
}
