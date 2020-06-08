#include	<stdio.h>

static void process() ;

void main()
{
	char record[1024] ;

	while(gets(record)) {
		process(record) ;
	}
}

static void process(rec)
char *rec ;
{
	char record[1024] ;
	int i ;

	memcpy(record, rec+40, sizeof(record)) ;
	record[sizeof(record)-1]= '/0' ;

	for (i= strlen(record); i> 0; i--) {
		if (record[i-1] == '/') break ;
	}
	if (i> 1) {
		write(1, record, i-1) ;
		write(1, "\n", 1) ;
	}
}	
