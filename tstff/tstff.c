#include	<stdio.h>
#include	<errno.h>
#include	<io.h>

static int once(char *filespec)
{
	struct _finddata_t fd ;
	long handle ;

	handle= _findfirst(filespec, &fd) ;
	if (handle == -1L) return(errno) ;

	do {
		printf("file='%s', size=%d\n", fd.name, fd.size) ;
	} while(_findnext(handle, &fd) == 0) ;

	_findclose(handle) ;
	return(0) ;
}

int main(int argc, char **argv)
{
	int i, ntimes ;
	char *filespec ;
	int rc ;

	if (argc> 1) {
		filespec= argv[1] ;
		if (argc> 2) {
			ntimes= atoi(argv[1]) ;
		}
		else {
			ntimes= 1 ;
		}
	}
	else {
		fprintf(stderr, "syntax:\n\t%s \"<pattern>\" [count]\n", argv[0]) ;
		exit(1) ;
	}

	fprintf(stderr, "doing it %d time(s)\n", ntimes) ;
	for (i= 0; i< ntimes; i++) {
		rc= once(filespec) ;
		if (rc != 0) {
			fprintf(
				stderr,
				"can't find '%s' (errno=%d)\n",
				filespec,
				rc
			) ;
			exit(1) ;
		}
	}

	exit(0) ;
}

