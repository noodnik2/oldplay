/*

	term.h:		UNIX terminal standard definitions

*/

#define		ESC_CHAR	'~'

#define		XFER_TEXT	0
#define		XFER_XMT	1
#define		XFER_XMB	2

struct termopts {
	int help ;
	int debug ;
	int esc ;
	int baud ;
	int xfer ;
	int openopts ;
	char *xfmode ;		/* NOTE: bound to .xfer: if change, update! */
	char *ttyline0 ;
	char *ttyline1 ;
} ;

#define		STD_TERM_ON()		t_start(T_CH,TM_TANDEM,TM_OPOST)
#define		STD_TERM_OFF()		t_stop(T_CH)

