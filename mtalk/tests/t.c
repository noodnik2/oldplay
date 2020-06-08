#include	<fcntl.h>

void main()
{
	int i, j, k, l, fdin, fdout ;
	char buf[128] ;

	fdin= open("jp.in", O_RDONLY) ;
	if (fdin<= 0) exit(1) ;

	fdout= open("jp.out", O_WRONLY) ;
	if (fdout<= 0) exit(1) ;

	for (i= 0; i< 10000; i++) {
		l= read(fdin, buf, 128) ;
		printf("l=%d\n", l) ;
		if (l< 0) break ;
		if (l == 0) continue ;
		buf[l]= '\0' ;
		printf("s='%s'\n", buf) ;
		sprintf(buf, "ok, whaddya want from me, anyways # %d\n", i) ;
		l= strlen(buf) ;
		k= write(fdout, buf, l) ;
		if (k != l) {
			printf("%d != %d\n", k, l) ;
		}
	}
	close(fdin) ;
	close(fdout) ;
}
