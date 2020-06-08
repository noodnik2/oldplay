#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/streamio.h>
#include	<sys/stropts.h>

flush_line()
{
	char buf[1024] ;
	int l, i ;

	while(1) {
		ioctl(0, FIONREAD, &i) ;
		if (i==0) break ;
		if (i> sizeof(buf)) i= sizeof(buf) ;
		if ((l= read(0, buf, i))> 0) {
			write(1, buf, l) ;
		}
	}
}
