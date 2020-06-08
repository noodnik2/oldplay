/*

	pw_proc.h	

*/

#define		MAXPROCS	200

#define		PWPL_NAME	32
#define		PWPL_USER	10

typedef struct {
	int free ;
	long pid ;
	long secs ;
	char user[PWPL_USER] ;
	char name[PWPL_NAME] ;
} pwproc_t ;

typedef struct {
	int n ;
	pwproc_t pp_a[MAXPROCS] ;
} pwlist_t ;

