/*	Revision:		3
*/

#ifndef	MTALK_H_INC
#define	MTALK_H_INC

/*

*/


/*
	#defines
*/

#define	MTALK_CALLERIDX		int
#define	MTALK_SOCK		int
#define	MTALK_BOOL		int
#define	MTALK_E			int
#define	MTALK_RC		int
#define	MTALK_STATE		int
#define	MTALK_ROLE		int

#define	MTALKBOOL_FALSE		0
#define	MTALKBOOL_TRUE		(!MTALKBOOL_FALSE)

#define	MTALKE_SUCCESS		0
#define	MTALKE_HELP		1
#define	MTALKE_BADARGS		12

#define	MTALKRC_SUCCESS		0
#define	MTALKRC_FAILURE		1
#define	MTALKRC_EOF		-1
#define	MTALKRC_NOMEMORY	2
#define	MTALKRC_IOERROR		3
#define	MTALKRC_OOB		4

#define	MTALKSTATE_INITIAL	0
#define	MTALKSTATE_WAIT		1
#define	MTALKSTATE_CHAT		2
#define	MTALKSTATE_FINAL	3

#define	MTALKROLE_UNKNOWN	0
#define	MTALKROLE_SENDER	1
#define	MTALKROLE_RECIPIENT	2

#define	MTALKMAX_NUMCALLERS	10
#define	MTALKMAX_CALLERNAMELEN	64
#define	MTALKMAX_NOTIFYCMDLEN	255
#define	MTALKMAX_CLINESIZE	255
#define	MTALKMAX_NVTBLOCKSIZE	16


/*
	structs
*/

struct mtalk_caller {
	char szName[MTALKMAX_CALLERNAMELEN+1] ;
	char caNVTBlock[MTALKMAX_NVTBLOCKSIZE] ;
	char szCLine[MTALKMAX_CLINESIZE+1] ;
	int iLenCLine ;
	int iNVTBlockLen ;
	int iNVTBlockPtr ;
	MTALK_SOCK mtSockIn ;
	MTALK_SOCK mtSockOut ;
	MTALK_STATE mtState ;
	MTALK_ROLE mtRole ;
	int nChat ;
	MTALK_BOOL bEcho ;
	MTALK_BOOL bNlToCrNl ;
	MTALK_BOOL bCrToNl ;
	MTALK_BOOL bIsNVT ;
} ;

struct mtalk_parms {
	MTALK_SOCK mtSockListen ;
	struct mtalk_caller *pmtca[MTALKMAX_NUMCALLERS] ;
	int iDebugLevel ;
	MTALK_BOOL bHelp ;
	MTALK_BOOL bQuiet ;
} ;

/* #ifndef MTALK_H_INC */
#endif

/* end of mtalk.h */
