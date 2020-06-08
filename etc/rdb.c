#include	<stdio.h>
#include	<signal.h>

static int signo= 0 ;
static void sigfun() ;

main()
{
	char buf[1024] ;
	int l, i ;

	signal(SIGINT, sigfun) ;
	system("stty -icanon -echo -echoe") ;
	i= sizeof(buf) ;
	while(signo == 0) {
		if ((l= read(0, buf, i))> 0) {
			write(1, buf, l) ;
		}
	}
	system("stty icanon echo echoe") ;
}

static void sigfun(sig)
int sig ;
{
	signal(SIGINT, sigfun) ;
	signo= sig ;
}
