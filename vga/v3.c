#include	<unistd.h>
#include	<sys/mman.h>
#include	<stdio.h>
#include	<fcntl.h>
#include	<errno.h>

int main()
{
	int i, fd ;
	unsigned char lines, cols ;
	unsigned char *pScreen ;

	fd= open("/dev/vcsa0", O_RDWR) ;
	if (fd< 0) {
		fprintf(stderr, "can't open virtual console\n") ;
		exit(1) ;
	}

	read(fd, &lines, 1) ;
	read(fd, &cols, 1) ;

	printf("lines=%d, cols=%d\n", (int) lines, (int) cols) ;

	pScreen= (unsigned char *) mmap(
		NULL,
		(int) cols * (int) lines,
		PROT_WRITE | PROT_READ,
		MAP_SHARED,
		fd,
		0
	) ;

	printf("pScreen=0x%lX\n", (long) pScreen) ;

	if (pScreen == MAP_FAILED) {
		fprintf(stderr, "mmap failed; errno=%d\n", errno) ;
		exit(2) ;
	}

	for (i= 0; i< 64; i++) {
		pScreen[4+(i*2)]= (unsigned char) ('A' + i) ;
	}

	sleep(10) ;

	close(fd) ;
	return(0) ;
}

