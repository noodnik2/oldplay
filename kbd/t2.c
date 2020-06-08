
int main()
{
	int i ;
	unsigned char a[100]= { 2 } ;

	for (i= 0 ; i< sizeof(a) / sizeof(a[0]) ; i++) {
		printf("a[%d]=%d\n", i, a[i]) ;
	}
}
