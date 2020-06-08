#if	OPSYS==OPMSDOS

#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#define		INBFMODE	(O_RDONLY|O_BINARY)
#define		OUTBFMODE	(O_WRONLY|O_BINARY|O_TRUNC|O_CREAT)
#define		OUTBFPROT	(S_IREAD|S_IWRITE)

#define		INTFMODE	(O_RDONLY)
#define		OUTTFMODE	(O_WRONLY|O_TRUNC|O_CREAT)
#define		OUTTFPROT	(S_IREAD|S_IWRITE)

#define	FNSIZE		64
#define	FESIZE		3
#define	PSCHAR		'\\'
#define	OPCHAR		'/'
#endif

#if	OPSYS==OPUNIX

#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#define		INBFMODE	(O_RDONLY)
#define		OUTBFMODE	(O_WRONLY|O_TRUNC|O_CREAT)
#define		OUTBFPROT	(S_IREAD|S_IWRITE)

#define		INTFMODE	INBFMODE
#define		OUTTFMODE	OUTBFMODE
#define		OUTTFPROT	OUTBFPROT

#define	FNSIZE		72
#define	FESIZE		32
#define	PSCHAR		'/'
#define	OPCHAR		'-'
#endif

struct file_id {
	int fh;			/* handle: integer */
	char fn[FNSIZE+2];	/* name: 1 byte for '\0', 1 byte for slop */
} ;
