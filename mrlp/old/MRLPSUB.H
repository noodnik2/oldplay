/*

	mrlpsub.h:		Marty's Remote Line Printer: Subroutines
				Copyright (C) 1992, Contahal Ltd.


*/

#if	OPSYS==OPMSDOS
#define		FAR	far
#endif

#if	OPSYS==OPUNIX
#define		FAR
#endif

extern void mrlp_setid() ;
extern void mrlp_msg() ;
extern void mrlp_nmsg() ;
extern void mrlp_err() ;
extern void mrlp_nerr() ;
extern void mrlp_perr() ;
extern void mrlp_abort() ;

extern char *mrlp_inetaddr_str() ;
extern int mrlp_getline() ;

#define		MRLPD_VER		"0.14"

#define		MRLPD_EVLPHOST		"MRLPD_HOST"
#define		MRLPD_EVLPCMD		"MRLPD_LPCMD"
#define		MRLPD_EVLPARGS		"MRLPD_LPARGS"
#define		MRLPD_EVLPOPTS		"MRLPD_LPOPTS"
#define		MRLPD_EVLPSERV		"MRLPD_SERV"
#define		MRLPD_EVLPDSERV		"MRLPD_DSERV"
#define		MRLPD_EVXHELPFN		"MRLPD_XHELP"
#define		MRLP_EVXHELPFN		"MRLP_XHELP"

#define		MRLPD_SCMD		"mrlpcmd"
#define		MRLPD_SDATA		"mrlpdata"

#define		MRLPD_PCMD		"tcp"
#define		MRLPD_PDATA		"tcp"

#define		MRLP_XHELPFN		"/usr/local/bin/mrlp.hlp"
#define		MRLPD_XHELPFN		"/usr/local/bin/mrlpd.hlp"

#define		MRLPCMD_PRINT		"print:"
#define		MRLPCMD_TESTHOST	"testhost:"
#define		MRLPCMD_EXIT		"exit:"
#define		MRLPCMD_VERCHK		"verchk:"
#define		MRLPCMD_JOBCHK		"jobchk:"
#define		MRLPCMD_SETDPORT	"setdport:"
#define		MRLPCMD_LPCMD		"lpcmd:"
#define		MRLPCMD_LPOPT		"lpopt:"

#define		MRLPCMDR_OK		"ok:"
#define		MRLPCMDR_ABORT		"abort:"

#define		SOCKA_T			struct sockaddr_in
#define		SOCKA_SADDR(x)		(x).sin_addr.s_addr
#define		SOCKA_SPORT(x)		(x).sin_port

#define		MRLP_FNBSIZE		256
#define		MRLP_MSGBSIZE		256
#define		MRLP_CMDBSIZE		133
#define		MRLP_IOBSIZE		512

#define		MRLP_HNBSIZE		12	/* host name buffer size */
#define		MRLP_SNBSIZE		12	/* service name buf size */
#define		MRLP_PNBSIZE		12	/* protocol name buf siz */

#define		MRLP_LECBSIZE		133	/* line printer "exec"   */
#define		MRLP_LEABSIZE		512	/* line printer args     */
#define		MRLP_LEOBSIZE		256	/* line printer opts     */
#define		MRLP_LENBSIZE		256	/* line printer filename */
#define		MRLP_NEARGS		64	/* max # lp arguments    */

#define		MRLP_IDBSIZE		32	/* max size of prog "id" */
#define		MRLP_NAMEBSIZE		64	/* max size of prog name */

