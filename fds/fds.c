#include	<stdio.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<unistd.h>

static int getfds(const char* pszFilename, char* pszfds) ;

int main(int argc, const char *argv[])
{
	const char *pszFilename ;
	char szfds[25] ;
	int i ;

	for (i= 1; i< argc; i++) {
		pszFilename= argv[i] ;
		if (!getfds(pszFilename, szfds)) {
			fprintf(stderr, "can't get fds(%s)\n", pszFilename) ;
			continue ;
		}
		printf("%s\n%s\n", pszFilename, szfds) ;
	}
	return(0) ;
}

static int getfds(const char* pszFilename, char* pszfds)
{
	struct stat fs ;
	struct tm *timestat ;
	time_t t0 ;

	if (stat(pszFilename, &fs)< 0) {
		return(0) ;
	}

	t0= fs.st_mtime ;
	t0-= 3600 * 7 ;
	timestat= gmtime(&t0) ;
	if (!timestat) {
		return(0) ;
	}

	sprintf(
		pszfds,
		"%02u%02u%02u%02u%02u%02u",
		(unsigned) (timestat->tm_year % 100),
		(unsigned) (timestat->tm_mon + 1),
		(unsigned) timestat->tm_mday,
		(unsigned) timestat->tm_hour,
		(unsigned) timestat->tm_min,
		(unsigned) timestat->tm_sec
	) ;

	return(1) ;
}

/* end of file */
