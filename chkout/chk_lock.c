/*

	chk_lock.c				File Lock Control


	To update a file, first lock it:

		chk_lock_mv(fn, lockfn) ;

	Then, create a new file under the filename returned by:

		chk_lock_temp(tempfn) ;

	Then, replace the original file with the temporary file by:

		chk_lock_mv(tempfn, fn) ;
		chk_lock_rm(lockfn) ;

	To restore the original file, simply do:

		chk_lock_rm(tempfn) ;
		chk_lock_mv(lockfn, fn) ;

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chkout.h"


/*
	(int)rc= chk_lock_mv(fn, lockfn) ;

	Locks the specified file by renaming it to a "locked name".

	During the lock period, it is impossible for another process to
	update the file (because it doesn't exist under the original name),
	and also impossible for users of chk_lock_mv() to lock a second
	copy (for the same reason).

	Use chk_lock_mv(lockfn, fn) to un_lock a previously locked file.

*/
int chk_lock_mv(fn, lockfn)
char *fn, *lockfn ;
{
	if (link(fn, lockfn) != 0) {
		return(-1) ;
	}	
	if (unlink(fn) != 0) {
		return(1) ;
	}
	return(0) ;
}

/*
	(int)rc= chk_lock_rm(fn) ;

	Removes specified file.
*/
int chk_lock_rm(fn)
char *fn ;
{
	if (unlink(fn)) {
		return(1) ;
	}
	return(0) ;
}

/*
	(int)rc= chk_lock_temp(tempfn) ;

	Creates a temporary file in tempfn[].

*/
int chk_lock_temp(tempfn)
char *tempfn ;
{
	strcpy(tempfn, TFN_TEMPLATE) ;
	mktemp(tempfn) ;
	if (tempfn[0] == '\0') return(1) ;
	return(0) ;
}
