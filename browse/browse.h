/*	Revision:	3
*/
/*

	browse.h			Header for browse utility
					Copyright (C) 1990-1993, by Marty Ross

*/

#define		S_WIDTH		80
#define		S_LENGTH	24

#define		T_LINES		2
#define		F_LINES		22
#define		B_LINES		(S_LENGTH-(T_LINES+F_LINES))

#define		RECSIZEMAX	1024	/* was 512 */

#define		CMDSIZE		S_WIDTH

#define		CMD_YPOS	1
#define		CMD_XPOS	5

#define		CDRAW		(bra.cdp)
#define		NDRAW		(bra.ndp)

#define		RLINE		(CDRAW.rline)
#define		INDENT		(CDRAW.indent)
#define		RECSIZE		(CDRAW.recsize)
#define		TOPLINE		(CDRAW.topline)
#define		BINMODE		(CDRAW.binmode)
#define		EBCMODE		(CDRAW.ebcmode)
#define		HEXMODE		(CDRAW.hexmode)
#define		TABMODE		(CDRAW.tabmode)
#define		NUMLINE		(CDRAW.numline)

#define		NEWRLINE	(NDRAW.rline)
#define		NEWINDENT	(NDRAW.indent)
#define		NEWRECSIZE	(NDRAW.recsize)
#define		NEWTOPLINE	(NDRAW.topline)
#define		NEWBINMODE	(NDRAW.binmode)
#define		NEWEBCMODE	(NDRAW.ebcmode)
#define		NEWHEXMODE	(NDRAW.hexmode)
#define		NEWTABMODE	(NDRAW.tabmode)
#define		NEWNUMLINE	(NDRAW.numline)

#define		CASEMODE	(bra.casemode)
#define		FILENAME	(bra.filename)
#define		EBCDIC_OK	(bra.ebcdic_ok)
#define		HELPCMD		(bra.helpcmd)
#define		FILESIZE	(bra.filesize)
#define		FILEDATE	(bra.filedate)
#define		CY		(bra.cy)
#define		CX		(bra.cx)

#define		SCN_TL(x)	(brs.tl[x])
#define		SCN_FL(x)	(brs.fl[x])
#define		SCN_BL(x)	(brs.bl[x])

#define		A_CTL(x)	((x)-'A'+1)
#define		A_ESC		(27)

#define		LCP		(*lcpp)
#define		KEY		(*keyp)

#define		KEYREDRAW	(A_CTL('R'))
#define		KEYEXIT		(A_CTL('C'))
#define		KEYESC		(27)
#define		KEYLF		(A_CTL('J'))
#define		KEYENTER	(A_CTL('M'))
#define		KEYCMDMODE	':'
#define		KEYHACK		(A_CTL('I'))
#define		KEYUP		(A_CTL('K'))
#define		KEYDN		(A_CTL('J'))
#define		KEYBS		(A_CTL('H'))
#define		KEYFS		(A_CTL('L'))
#define		KEYPGUP		(A_CTL('U'))
#define		KEYPGDN		(A_CTL('D'))
#define		KEYHERE		(A_CTL('F'))
#define		KEYWHERE	(A_CTL('W'))

#define		BCM_RESPECT	0
#define		BCM_IGNORE	1

struct br_opt {
	int o_hex ;
	int o_tab ;
	int o_help ;
	int o_debug ;
	int o_mixed ;
	int o_scale ;
	int o_bin ;
	int o_recsize ;
	int o_ebcdic ;
} ;

struct br_draw {
	long topline ;				/* top line of file	   */
	int numline ;				/* number of lines dsplayd */
	int rline ;				/* reserved line number    */
	int recsize ;				/* max. size of "record"   */
	int indent ;				/* indent # of characters  */
	int hexmode ;				/* hex display mode value  */
	int binmode ;				/* bin display mode value  */
	int tabmode ;				/* tab display mode value  */
	int ebcmode ;				/* translating ebcdic txt? */
} ;

struct br_arg {
	char *filename ;			/* current browse filename */
	char helpcmd[132] ;			/* help cmd: why 132?      */
	long filesize, filedate ;		/* currebt browse filedata */
	int cy, cx ;				/* current cursor position */
	struct br_draw cdp ;			/* current drawing parms   */
	struct br_draw ndp ;			/* new drawing parms       */
	int casemode ;				/* case-sensitive search?  */
	int redraw ;				/* redraw screen flag      */
	int ebcdic_ok ;				/* ok to use ebcdic table? */
} ;

struct br_scn {
	char tl[T_LINES][S_WIDTH+1] ;		/* topmost lines of screen */
	char fl[F_LINES][S_WIDTH+1] ;		/* file display area       */
#if	B_LINES != 0
	char bl[B_LINES][S_WIDTH+1] ;		/* bottom lines of screen  */
#endif
} ;

struct fl_stat {				/* return from filestat()  */
	long size, date ;
	int isdir ;
} ;

