#include	<sys/socket.h>
#include	<sys/un.h>

void main()
{
	int i, l, k ;
	int sin, sout ;
	struct sockaddr_un sain, saout ;
	char buf[128] ;

	sin= socket(AF_UNIX, SOCK_STREAM, 0) ;
	if (sin< 0) {
		perror("in socket") ;
		exit(1) ;
	}

	sout= socket(AF_UNIX, SOCK_STREAM, 0) ;
	if (sout< 0) {
		perror("out socket") ;
		exit(2) ;
	}

	memset(&sain, '\0', sizeof(sain)) ;
	sain.sun_family= AF_UNIX ;
	strcpy(sain.sun_path, "mtalk.so") ;

	memset(&saout, '\0', sizeof(saout)) ;
	saout.sun_family= AF_UNIX ;
	strcpy(saout.sun_path, "mtalk.si") ;

	if (bind(sin, (struct sockaddr *) &sain, sizeof(sain))< 0) {
		perror("bind: input") ;
		exit(4) ;
	}

	if (bind(sout, (struct sockaddr *) &saout, sizeof(saout))< 0) {
		perror("bind: output") ;
		exit(5) ;
	}

	for (i= 0; i< 10000; i++) {
		sprintf(buf, "from t3 # %d\n", i) ;
		k= strlen(buf) ;
		l= write(sout, buf, k) ;
		if (l != k) {
			printf("%d != %d\n", l, k) ;
		}
		l= read(sin, buf, sizeof(buf)) ;
		printf("l=%d\n", l) ;
		if (l< 0) {
			perror("read") ;
			continue ;
		}
		if (l == 0) continue ;
		buf[l]= '\0' ;
		printf("s='%s'\n", buf) ;
	}

	close(sin) ;
	close(sout) ;
}
