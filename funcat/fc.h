/*	Revision:	1
*/
/*

	fc.h			Funcat Header File
				Copyright (C) 1993, Contahal Ltd.

*/

#define		FCB_MAX_SIZE	32000
#define		FCE_SUCCESS	0
#define		FCE_BADPARMS	1
#define		FCE_FNOTFOUND	28
#define		FCE_BUFOVFL	36
#define		FCE_ABORTED	99

#define		FCB_DFLT_SIZE	16384

#define		FCF_GETC()	getc(fcfs.ifp)
#define		FCF_PUTC(c)	putc(c, fcfs.ofp)

#define		FCB_SIZE	fcgs.buf_size
#define		FCB_IDX		fcgs.buf_idx
#define		FCB_BUF		fcgs.buf_ptr

#define		FCP_NAME	fcgs.progname

#define		FCF_NAME	fcgs.fnlist[fcgs.cur_fno]
#define		FCF_LINE	fcfs.line_no
#define		FCF_FNLINE	fcfs.csf_line

#define		FCS_FNB_LVL	fcfs.fnb_level
#define		FCS_FLP_LVL	fcfs.flp_level
#define		FCS_CMT_LVL	fcfs.cmt_level
#define		FCS_IN_CMT	fcfs.in_comment
#define		FCS_IN_BQ	fcfs.in_bquote
#define		FCS_IN_DQ	fcfs.in_dquote
#define		FCS_IN_SQ	fcfs.in_squote
#define		FCS_IN_FPARM	fcfs.in_fparm
#define		FCS_IN_FDECL	fcfs.in_fdecl
#define		FCS_IN_FDECP	fcfs.in_fdecp
#define		FCS_IN_NL	fcfs.in_nl
#define		FCS_IN_CDL	fcfs.in_cdl
#define		FCS_IN_LDATA	fcfs.in_ldata
#define		FCS_IN_FN	fcfs.in_fn
#define		FCS_POS_FN	fcfs.pos_fndef
#define		FCS_POS_FLP	fcfs.pos_fleftparen

#define		FCO_SHOWCMT	fcopts.show_cmt
#define		FCO_SHOWSTATIC	fcopts.show_static
#define		FCO_PREFIX	fcopts.prefix
#define		FCO_INDENT	fcopts.indent
#define		FCO_HELP	fcopts.help
#define		FCO_LINES	fcopts.lines

typedef struct {
	int show_cmt ;			/* show function commnts also */
	int show_static ;		/* don't show static functns  */
	int indent ;			/* indent output lines        */
	int prefix ;			/* prefix output with filename*/
	int help ;			/* display syntax diagram     */
	int lines ;			/* show source lines	      */
} fc_opts ;

typedef struct {
	FILE *ifp ;			/* current input file pointer */
	FILE *ofp ;			/* current output file pointr */
	int line_no ;			/* current parser line number */
	int csf_line ;			/* current start of fn line   */
	int cmt_level ;			/* comment nesting level      */
	int fnb_level ;			/* function bracket level     */
	int flp_level ;			/* function parentheses level */
	int in_comment ;		/* inside comment             */
	int in_dquote ;			/* double quoted literal strg */
	int in_squote ;			/* single quoted literal char */
	int in_bquote ;			/* backslash literal quote    */
	int in_fparm ;			/* entered possible fn decl   */
	int in_fdecl ;			/* after fn decl right paren  */
	int in_fdecp ;			/* saw fn params - ';' ok now */
	int in_nl ;			/* in series of new-lines     */
	int in_cdl ;			/* in compiler directive line */
	int in_ldata ;			/* data was seen in line      */
	int in_fn ;			/* in (possible) function?    */
	int pos_fndef ;			/* position of fn definition  */
	int pos_fleftparen ;		/* position of fn left paren  */
} fc_fstate ;

typedef struct {
	char *progname ;
	char **fnlist ;
	int cur_fno ;
	int num_files ;
	int rc ;
	int buf_idx ;
	int buf_size ;
	char *buf_ptr ;
} fc_gstate ;

/* end of fc.h */
