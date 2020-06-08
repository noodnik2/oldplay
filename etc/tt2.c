#include	<stdio.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<errno.h>

#define		N_FILES		250

void main()
{
	int i, nf ;
	int fh[N_FILES] ;
	char buf[200] ;
	char fn[128] ;

	printf("creating %d files: file#.jnk using file handles\n", N_FILES) ;
	for (i= 0; i< N_FILES; i++) {
		sprintf(fn, "file%d.jnk", i) ;
		fh[i]= open(fn, O_WRONLY|O_CREAT, S_IREAD|S_IWRITE) ;
		if (fh[i]< 0) {
			printf("errno=%d\n", errno) ;
			break ;
		}
	}
	nf= i ;
 
	printf("%d files created\n", nf) ;
	if (nf != N_FILES) {
		printf("NOTE:	ran out of files!\n") ;
	}

	printf("writing one line to files\n") ;
	for (i= 0; i< nf; i++) {
		sprintf(buf, "File %d\n", i) ;
		write(fh[i], buf, strlen(buf)) ;
	}

	printf("closing files\n") ;
	for (i= 0; i< nf; i++) {
		close(fh[i]) ;
	}

}
