
#define	SUBVAL_MAXLEN	65

struct sub_item {
	char *search_str ;
	char *format_str ;
	void *subval ;
	void (*subproc)() ;
} ;
