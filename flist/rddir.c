#include	<stdio.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#include	"compiler.h"

#if	OPSYS==OPUNIX


#if		COMPILER==NCRTOWER
#include	"rddir.h"

#define		DEBUG		1
#define		TEST_RDDIR	0

#if		DEBUG==1
#include	"flist.h"

extern struct fl_opt flo ;
#endif

void rddir() ;

DIR *opendir() ;
struct direct *readdir() ;
void closedir() ;

#if		TEST_RDDIR==1

void main(argc, argv)
int argc;
char *argv[];
{
	if (argc< 2) exit(1) ;

	rddir(argv[1]) ;
	exit(0) ;
}

static void rddir(dname)
char *dname ;
{
	DIR *dfp ;
	struct direct *dire ;

	if ((dfp= opendir(dname)) == (DIR *) NULL) {
		printf("can't open: %s\n", dname) ;
	}

	while((dire= readdir(dfp)) != NULL) {
		printf("dir='%s'\n", dire->d_name) ;
	}
	closedir(dfp) ;
}

#endif

DIR *opendir(name)
char *name ;
{
	int fh ;
	DIR *dfile ;
	struct stat fs ;

	if ((fh= open(name, O_RDONLY))< 0) {
		return((DIR *)NULL) ;
	}
	if (fstat(fh, &fs)!=0) {
		return((DIR *)NULL) ;
	}
	if (!(fs.st_mode & S_IFDIR)) { 
		return((DIR *)NULL) ;
	}
	if ((dfile= (DIR *)malloc(sizeof(DIR))) == (DIR *)NULL) {
		printf(stderr, "malloc failure in opendir()\n") ;
		return((DIR *)NULL) ;
	}
	(dfile->fh)= fh ;
	return(dfile) ;
}

void closedir(dfile)
DIR *dfile ;
{
	close(dfile->fh) ;
	free(dfile) ;
}

struct direct *readdir(dfp)
DIR *dfp ;
{
	static struct direct dstruc ;
	int len ;

	do {
		len= read(dfp->fh,(char *)&dstruc,sizeof(dstruc)) ;
		if (len != sizeof(dstruc)) {
			if (len != 0) {
				fprintf(stderr, "can't read dir in readdir()\n") ;
			}
			return((struct direct *)NULL) ;
		}
	} while(dstruc.d_ino==0) ; /* I THINK inode==0 impossible for file */
#if	DEBUG==1
	if (flo.o_debug) {
		printf("ino=%d fn='%s'\n", dstruc.d_ino, dstruc.d_name) ;
	}
#endif
	return(&dstruc) ;
}

#endif
#endif
