/*

	dirlist.h


*/

#if	OPSYS==OPUNIX

#define		SFN_SIZE	80

#if	COMPILER==HP9000
#include	<ndir.h>
#define	DIRENTRY_T	struct direct
#endif

#if	COMPILER==NCRTOWER
#include	<sys/types.h>
#include	"rddir.h"
#define	DIRENTRY_T	struct direct
#endif

#if	COMPILER==SUNC
#include	<sys/types.h>
#include	<sys/dir.h>
#define	DIRENTRY_T	struct direct
#endif

#if	COMPILER==CCUXV
#include	<dirent.h>
#define	DIRENTRY_T	struct dirent
#endif

#if	COMPILER==NCRNST
#include	<dirent.h>
#define	DIRENTRY_T	struct dirent
#endif

#define DIRLIST_T	DIR
#endif

#if	OPSYS==OPMSDOS
#define		SFN_SIZE	14
#include	<dos.h>

#define	DIRLIST_T	struct find_t

typedef struct {
	char d_name[SFN_SIZE+1] ;
} DIRENTRY_T ;

#endif

