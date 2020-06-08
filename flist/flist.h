/*

	flist.h			header file: artifacts of VM
				Copyright (C) 1990-1992, by Marty Ross


*/

#define		T_LINES		2
#define		F_LINES		20
#define		B_LINES		2

#define		S_LENGTH	(T_LINES+F_LINES+B_LINES)

#define		R_MGN		80

#if	OPSYS==OPUNIX
#define		S_WIDTH		256
#define		FL_FNSIZE	256
#define		OPTSIZE		1024
#define		CMDSIZE		8192
#define		OPTPFX_CH	'-'
#define		PATHSEP_CH	'/'
#define		PATHSEP_CHA	"/"
#define		FLMAC_EXT	".flmac"
#endif

#if	OPSYS==OPMSDOS
#define		S_WIDTH		80
#define		FL_FNSIZE	80
#define		OPTSIZE		256
#define		CMDSIZE		512
#define		OPTPFX_CH	'/'
#define		PATHSEP_CH	'\\'
#define		PATHSEP_CHA	"\\"
#define		FLMAC_EXT	".flm"
#endif

#define		FL_MACMAGIC	2017

#define		A_CTL(c)	(((c)-'A'+1))

#define		FLA		(fla)
#define		FLS		(fls)
#define		FLO		(flo)

#define		DIRFN		(FLA.dirfile)
#define		MACFN		(FLA.macfile)
#define		HELPCMD		(FLA.helpcmd)
#define		OPTSTR		(FLA.optstr)
#define		TITLE		(FLA.title)
#define		ICMD		(FLA.icmd)
#define		TOTFILE		(FLA.totfiles)
#define		TOTSIZE		(FLA.totsizes)
#define		CX		(FLA.fx)
#define		CY		(FLA.fy)
#define		RX		(FLA.fxr)
#define		FL_LIST		(FLA.flfp)
#define		FL_CLP		(FLA.clfp)

#define		MAXWID		(FLS.maxwid)
#define		NUMFILE		(FLS.numfiles)
#define		TOPLINE		(FLS.topline)

#define		CURLINE		(TOPLINE+CY)
#define		NXTPAGE		(TOPLINE+NUMFILE)

#define		TOPFLP		(fl_lofs(TOPLINE))
#define		CURFLP		(fl_lofs(CURLINE))

struct fl_line *fl_lofs() ;
char *fl_gcmd() ;

/*
	the screen command queue structures 
*/

struct fq_line {
	char *line, *ecmd ;
	struct fl_line *flp ;
	int jval ;
} ;

struct fq_queue {
	int hp, tp ;
} ;

#define	SCQFQL	FLA.fql
#define	SCQFQQ	FLA.fqq

/*
	the file list structures
*/

struct fl_scn {
	int numfiles ;
	int maxwid ;
	int topline ;
	char tl[T_LINES][S_WIDTH+1] ;
	char fl[F_LINES][S_WIDTH+1] ;
	char bl[B_LINES][S_WIDTH+1] ;
} ;

struct fl_stat {
	long size ;
	long date ;
	short unsigned isdir ;
} ;

struct fl_cmd {
	char cmdline[S_WIDTH+1] ;
	int rc ;
} ;

struct fl_line {
	struct fl_stat stat ;
	struct fl_line *next ;
	struct fl_cmd *cmd ;
	short unsigned id ;
	char *line ;
} ;

struct fl_arg {
	struct fl_line *flfp ;
	struct fl_line *clfp ;
	struct fq_line fql[F_LINES+1] ;
	struct fq_queue fqq ;
	int totfiles ;
	int cy, cx ;
	int fy, fx, fxr ;
	int usemac, macptr ;
	long totsizes ;
	char dirfile[FL_FNSIZE+1] ;
	char macfile[FL_FNSIZE+1] ;
	char macbuf[CMDSIZE+1] ;
	char optstr[OPTSIZE+1] ;
	char icmd[S_WIDTH+1] ;
	char title[S_WIDTH+1] ;
	char helpcmd[81] ;			/* why 81? */
} ;

struct fl_opt {
	int o_all ;
	int o_more ;
	int o_debug ;
	int o_right ;
	int o_cmddisp ;
	int o_redraw ;
	int o_dsize ;
	int o_stout ;
	int o_pdate ;
	int o_help ;
	int o_recur ;
} ;
