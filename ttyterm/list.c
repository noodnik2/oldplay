/*

	terminal driver - file transfer

*/

#include	<stdio.h>
#include	<fcntl.h>
#include	<signal.h>

#include	"compiler.h"

#define		USE_FS		0

#if		USE_FS
#include	<sys/dir.h>
#else
#include	<dirent.h>
#endif

void main()
{
#if	OPSYS==OPUNIX
#if	OPSYSLVL==OPLSYSV
	int fh, len ;

#if	USE_FS
	struct direct dirent ;
	
	fprintf(stderr, "\n") ;
	if ((fh= open(".", O_RDONLY))<= 0) {
		fprintf(stderr, "--Can't open directory\n") ;
		return ;
	}

	for (;;) {
		len= read(fh, (char *)&dirent, sizeof(dirent)) ;
		if (len != sizeof(dirent)) {
			if (len==0) break ;
			fprintf(stderr, "--Can't read directory\n") ;
			break ;
		}
		if (dirent.d_ino==0) continue ;
		if (strcmp(dirent.d_name, ".")==0) continue ;
		if (strcmp(dirent.d_name, "..")==0) continue ;
		printf("n='%s'\n", dirent.d_name) ;
	}

	close(fh) ;
#else
	DIR *fdp ;
	struct dirent *dep ;
	
	fprintf(stderr, "\n") ;
	if ((fdp= opendir(".")) == (DIR *)NULL) {
		fprintf(stderr, "--Can't open directory\n") ;
		return ;
	}

	for (;;) {
		if ((dep= readdir(fdp)) == (struct dirent *)NULL) {
			break ;
		}
		if (dep->d_ino==0) continue ;
		if (strcmp(dep->d_name, ".")==0) continue ;
		if (strcmp(dep->d_name, "..")==0) continue ;
		printf("n='%s'\n", dep->d_name) ;
	}

	closedir(fdp) ;
#endif

#endif
#endif
}
