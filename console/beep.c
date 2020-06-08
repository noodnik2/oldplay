/*

	Beep.c				Windows Beep() emulation for Linux

*/

#ifdef	_WINDOWS_
#include	<windows.h>
#include	<stdio.h>
#else
#include	<sys/kd.h>
#include	<fcntl.h>
#include	<errno.h>
#endif

#ifndef	_WINDOWS_
typedef unsigned long DWORD ;
typedef int BOOL ;
#define	TRUE		1
#define	FALSE		0
BOOL Beep(DWORD dwFreq, DWORD dwDuration) ;
#endif

int main(int argc, char *argv[])
{
	DWORD dwFreq, dwDuration ;

	if (argc< 3) return(1) ;
	if (sscanf(argv[1], "%lu", &dwFreq) != 1) return(2) ;
	if (sscanf(argv[2], "%lu", &dwDuration) != 1) return(3) ;

	if (!Beep(dwFreq, dwDuration)) {
		printf("errno=%d\n", errno) ;
		return(4) ;
	}

	printf("ok\n") ;
	return(0) ;
}

#ifndef	_WINDOWS_
BOOL Beep(DWORD dwFreq, DWORD dwDuration)
{
	DWORD dwTone, dwIoctlArg ;
	BOOL bOk ;
	int fdc ;

	dwTone= 1190000UL ;
	dwTone/= dwFreq ;
	dwIoctlArg= (
		((dwDuration & 0xFFFFUL) <<16)
	      | (dwTone & 0xFFFFUL)
	) ;

	fdc= open("/dev/console", O_RDWR) ;
	if (fdc< 0) return(FALSE) ;
	bOk= TRUE ;
	if (ioctl(fdc, KDMKTONE, dwIoctlArg)< 0) {
		bOk= FALSE ;
	}
	close(fdc) ;
	return(bOk) ;
}
#endif


/* end of beep.c */
