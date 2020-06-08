void main()
{
	int i ;

	for (i= 0; i< 1000; i++) {
		if (fork() == 0) {
			sleep(5) ;
			exit(0) ;
		}
	}
	exit(0) ;
}
