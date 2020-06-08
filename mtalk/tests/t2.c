#include	<errno.h>
#include	<sys/socket.h>
#include	<sys/un.h>

void main()
{
	int i, l, k, lsahim ;
	int sme, shim ;
	struct sockaddr_un same, sahim ;
	char buf[128] ;

	sme= socket(AF_UNIX, SOCK_DGRAM, 0) ;
	if (sme< 0) {
		perror("my socket") ;
		exit(1) ;
	}

	shim= socket(AF_UNIX, SOCK_DGRAM, 0) ;
	if (shim< 0) {
		perror("his socket") ;
		exit(2) ;
	}

	memset(&same, '\0', sizeof(same)) ;
	same.sun_family= AF_UNIX ;
	strcpy(same.sun_path, "mtalk.me") ;
	unlink(same.sun_path) ;
	if (bind(sme, (struct sockaddr *) &same, sizeof(same))< 0) {
		perror("bind") ;
		exit(4) ;
	}

	for (i= 0;; i++) {
		memset(&sahim, '\0', sizeof(sahim)) ;
		sahim.sun_family= AF_UNIX ;
		strcpy(sahim.sun_path, "mtalk.him") ;
		lsahim= sizeof(sahim) ;
		l= recvfrom(
			sme,
			buf,
			sizeof(buf),
			0,
			(struct sockaddr *) &sahim,
			&lsahim
		) ;
		printf("l=%d\n", l) ;
		if (l< 0) {
			printf("errno=%d\n", errno) ;
			perror("recvfrom") ;
			continue ;
		}
		if (l == 0) continue ;
		buf[l]= '\0' ;
		printf("s='%s'\n", buf) ;
		sprintf(buf, "ok, this is enogh # %d\n", i) ;
		k= strlen(buf) ;
		l= sendto(shim, buf, k, 0, (struct sockaddr *) &sahim, sizeof(sahim)) ;
		if (l != k) {
			printf("%d != %d\n", l, k) ;
		}
	}

	close(sme) ;
	close(shim) ;
}
