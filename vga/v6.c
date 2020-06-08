#include	<sys/kd.h>
#include	<stdio.h>
#include	<fcntl.h>

#if 0
struct consolefontdesc {
} ;
#endif

int main()
{
	FILE *fp ;
	int i, l, fd ;
	struct consolefontdesc cfd ;
	char buffer[10240] ;

	fd= open("/dev/console", O_RDWR) ;
	if (fd< 0) {
		fprintf(stderr, "can't open\n") ;
		return 1 ;
	}

	memset(&cfd, '\0', sizeof(struct consolefontdesc)) ;
	cfd.chardata= buffer ;
	cfd.charcount= sizeof(buffer) ;
	if (ioctl(fd, GIO_FONTX, &cfd)< 0) {
		fprintf(stderr, "can't ioctl\n") ;
		return 2 ;
	}

	printf(
		"ok. charcount=%d, charheight=%d\n",
		cfd.charcount,
		cfd.charheight
	) ;
	fp= fopen("v6.out", "w") ;
	if (!fp) {
		fprintf(stderr, "can't dump\n") ;
		return 3 ;
	}

	l= cfd.charcount ;
	l*= cfd.charheight ; 
	for (i= 0; i< l; i++) {
		fputc(buffer[i], fp) ;
	}
	fclose(fp) ;
	
	close(fd) ;
	return 0 ;
}
