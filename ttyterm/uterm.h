/*

	uterm.h


*/

#define		UT_CONSOLE	"/dev/tty"

#define		UTOO_NONE	0
#define		UTOO_DEBUG	1

#define		UT_IBUFSIZE	1024

struct uterm {
	char name0[34] ;
	char name1[34] ;
	int inp_fh, inp_baud ;
	int out_fh, out_baud ;
	int inblen, inbptr ;
	unsigned char inbuf[UT_IBUFSIZE] ;
} ;

struct utcopt {
	int echo ;
	int icanon ;
	int cread ;
	int local ;
	int ixon ;
	int ixoff ;
	int irts ;
	int opost ;
	int brkint ;
	int isig ;
	int icrnl ;
} ;
