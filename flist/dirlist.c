/*

	dirlist.c


*/

#include	<stdio.h>
#include	"compiler.h"
#include	"dirlist.h"

#if	OPSYS==OPMSDOS
#include	"flist.h"

extern struct fl_opt flo ;
#endif

int dirlist_init(fn, dlp_p)
char *fn ;
DIRLIST_T **dlp_p ;
{
	DIRLIST_T *dl_p ;

#if	OPSYS==OPUNIX
	if ((dl_p= opendir(fn)) == (DIRLIST_T *)NULL) return(1) ;
#endif

#if	OPSYS==OPMSDOS
	char dname[132] ;
	unsigned attr ;

	dl_p= (DIRLIST_T *)malloc(sizeof(DIRLIST_T)) ;
	if (dl_p == (DIRLIST_T *)NULL) return(-1) ;

	strcpy(dname, fn) ;
	strcat(dname, "\\*.*") ;
	attr= flo.o_all?
		_A_NORMAL | _A_RDONLY | _A_ARCH | _A_SUBDIR | _A_HIDDEN:
		_A_NORMAL |                       _A_SUBDIR
	;
	if (_dos_findfirst(dname, attr, (struct find_t *)dl_p) != 0) {
		return(1) ;
	}
#endif

	(*dlp_p)= dl_p ;
	return(0) ;
}

int dirlist_read(dl_p, dep_p)
DIRLIST_T *dl_p ;
DIRENTRY_T **dep_p ;
{
	DIRENTRY_T *de_p ;

#if	OPSYS==OPUNIX
	if ((de_p= readdir(dl_p)) == (DIRENTRY_T *)NULL) {
		return(1) ;
	}
	memcpy((char *)de_p, (char *)de_p, sizeof(DIRENTRY_T)) ;
#endif

#if	OPSYS==OPMSDOS
	static DIRENTRY_T de ;

	de_p= &de ;
	if (dl_p->name[0] == '\0') return(1) ;
	strncpy(de_p->d_name, dl_p->name, SFN_SIZE+1) ;
	if (_dos_findnext(dl_p) != 0) dl_p->name[0]= '\0' ;
#endif

	(*dep_p)= de_p ;
	return(0) ;
}

int dirlist_fini(dl_p)
DIRLIST_T *dl_p ;
{
#if	OPSYS==OPUNIX
	closedir(dl_p) ;
#endif
#if	OPSYS==OPMSDOS
	free((char *)dl_p) ;
#endif
}
