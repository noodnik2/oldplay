/*
	msff.c			Porting from Windows to Linux
				Emulation of Microsoft _find* functions

		See:

			_findfirst
			_findnext
			_findclose 

		functions in Microsoft MSDN for reference.

*/

#include	<sys/stat.h>
#include	<glob.h>
#include	<errno.h>

#include	"msff.h"


/*
	internal structures
*/

typedef struct _msffhandle_tag {
	glob_t glob ;
	int idx ;
} _msffhandle_t ;


/*
	internal function declarations
*/

static int _msemffloadfileinfo(
	_msffhandle_t *pfh,
	struct _finddata_t *fileinfo
) ;


/*
	entry points
*/

long _findfirst(char *filespec, struct _finddata_t *fileinfo)
{
	_msffhandle_t *pfh ;

	pfh= (_msffhandle_t *) malloc(sizeof(_msffhandle_t)) ;
	memset(pfh, '\0', sizeof(_msffhandle_t)) ;

	if (glob(filespec, GLOB_NOSORT, NULL, &pfh->glob) != 0) {
		/* error expanding pattern; simply say: "not found" */
		errno= ENOENT ;
		free(pfh) ;
		return(-1L) ;
	}

	if (_msemffloadfileinfo(pfh, fileinfo)< 0) {
		globfree(&pfh->glob) ;
		free(pfh) ;
		return(-1L) ;
	}

	/* portability WARNING: casting ptr to long !! */
	return((long) pfh) ;
}

int _findnext(long handle, struct _finddata_t *fileinfo)
{
	if (handle == -1L) {
		errno= ENOENT ;
		return(-1) ;
	}

	/* portability WARNING: casting long to ptr !! */
	if (_msemffloadfileinfo((_msffhandle_t *) handle, fileinfo)< 0) {
		/* errno already set */
		return(-1) ;
	}

	return(0) ;
}

int _findclose(long handle)
{
	_msffhandle_t *pfh ;

	if (handle == -1L) {
		errno= ENOENT ;
		return(-1) ;
	}

	/* portability WARNING: casting long to ptr !! */
	pfh= (_msffhandle_t *) handle ;
	if (!pfh) {
		errno= EINVAL ;
		return(-1) ;
	}

	globfree(&pfh->glob) ;
	free(pfh) ;

	return(0) ;
}


/*
	internal functions
*/

static int _msemffloadfileinfo(
	_msffhandle_t *pfh,
	struct _finddata_t *fileinfo
) {
	struct stat st ;

	if (!pfh) {
		errno= EINVAL ;
		return(-1) ;
	}

	memset(fileinfo, '\0', sizeof(struct _finddata_t)) ;

	if ((pfh->idx)>= (pfh->glob.gl_pathc)) {
		errno= ENOENT ;
		return(-1) ;
	}

	strncpy(
		fileinfo->name,
		pfh->glob.gl_pathv[pfh->idx],
		sizeof(fileinfo->name)
	) ;

	if (stat(fileinfo->name, &st)< 0) {
		/* errno already set by "stat"	*/
		/* NOTE: this may be nonstandard */
		return(-1) ;
	}

	fileinfo->time_create= (time_t) -1 ; 	/* not supported */
	fileinfo->time_access= st.st_atime ;
	fileinfo->time_write= st.st_mtime ;
	fileinfo->size= st.st_size ;

	pfh->idx++ ;
	return(0) ;
}

/* end of msff.c */
