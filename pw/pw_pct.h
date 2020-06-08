/*

	pw_pct.h	

*/

typedef struct {
	int free ;
	long pid ;
	long stime ;
	long ttime ;
	char user[PWPL_USER] ;
	char name[PWPL_NAME] ;
} pwpct_t ;

typedef struct {
	int n ;
	pwpct_t pc_a[MAXPROCS] ;
} pwpctl_t ;

