#include	<signal.h>

void catcher() ;

void main()
{

	signal(SIGCLD, catcher) ;
	if (fork() == 0) {
		exit() ;
	}
	pause() ;
}

void catcher(signo)
int signo ;
{
	printf("parent caught sig\n") ;
	signal(SIGCLD, catcher) ;
}

