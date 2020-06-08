void main()
{

#if 1
	signed short x ;

	x= 0xFF01 ;
	printf("%d\n", x) ;
#else
	static unsigned char str[]= "\033xy" ;
	int i ;

	for (i= 0; str[i] != '\0'; i++) {
		printf("%d\n", (int)str[i]) ;
	}
#endif
}

