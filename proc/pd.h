
struct parms {
	char *procdirpfx ;
	int nargs ;
	char **arglist ;
} ;

struct opts {
	int help ;
	int debug ;
	int diagpsinfo ;
	int diagstate ;
	int diagmem ;
	int detail ;
} ;

typedef struct {
	char msgbuf[1024] ;
	char pidfn[100] ;
	int pidfh ;
	int pid ;
} pdd_args ;	

