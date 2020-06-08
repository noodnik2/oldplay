#include	<stdio.h>
#include	<signal.h>

static void do_loop() ;
static void break_int() ;

static int break_flag ;

void main()
{
	signal(SIGINT, break_int) ;
	do_loop() ;
	signal(SIGINT, SIG_DFL) ;
}

static void do_loop()
{
	char buf[513] ;

	buf[0]= '\0' ;
	printf("(enter null line to leave)\n") ;
	do {
		break_flag= 0 ;
		gets(buf) ;
		if (!break_flag) printf("line='%s'\n", buf) ;
	} while(break_flag || buf[0] != '\0') ;
	printf("bye\n") ;
}

static void break_int(signo)
int signo ;
{
	signal(SIGINT, break_int) ;
	printf("break: sig=%d\n", signo) ;
	break_flag= 1 ;
}

