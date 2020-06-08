/*
	msff.h			Porting from Windows to Linux
				Emulation of Microsoft _find* functions

*/

#ifndef	__MSFF_H__
#define	__MSFF_H__


/*
	sub-#includes
*/

#include	<time.h>

/*
	#defines
*/

/*
	_finddata_t.attrib values:
*/
#define	_A_NORMAL	0x0000
#define	_A_RDONLY	0x0001
#define	_A_HIDDEN	0x0002
#define	_A_SYSTEM	0x0004
#define	_A_SUBDIR	0x0010
#define	_A_ARCH		0x0020


/*
	types
*/

typedef unsigned long _fsize_t ;


/*
	structs
*/

struct _finddata_t {
	unsigned short attrib ;
	time_t time_create ;
	time_t time_access ;
	time_t time_write ;
	_fsize_t size ;
	char name[260] ;
} ;


/*
	entry point declarations
*/

long _findfirst(char *filespec, struct _finddata_t *fileinfo) ;
int _findnext(long handle, struct _finddata_t *fileinfo) ;
int _findclose(long handle);


/* #ifndef __MSFF_H__ */
#endif

/* end of msff.h */
