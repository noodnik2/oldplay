/*


*/

#include	<stdio.h>
#include	<sys/types.h>
#include	<time.h>

#include	"compiler.h"
#include	"chkout.h"

char *ctime() ;

int chk_time(cur_time)
char *cur_time ;
{
	time_t ct ;

	if ((ct= time((time_t *)NULL)) == (time_t)(-1)) {
		return(1) ;
	}
	strncpy(cur_time, ctime(&ct), TIME_SIZE+1) ;
	return(0) ;
}

