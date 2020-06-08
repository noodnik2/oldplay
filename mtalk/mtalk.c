#define	MSRC_Revision		47

/*

*/

#include	"compiler.h"

#include	<stdio.h>
#include	<errno.h>
#include	<signal.h>
#include	<time.h>
#include	<sys/time.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<fcntl.h>
#include	<unistd.h>

#include	"clgetopt.h"
#include	"mtalk.h"


/*

*/

#ifndef	NFDS
#define	NFDS(x)		(x)
#endif


/*
	internal data
*/

struct mtalk_parms mtp ;


/*
	internal function declarations
*/

static void init(int argc, char *argv[]) ;
static MTALK_RC getListenSock() ;
static MTALK_RC waitAndTakeAction() ;
static MTALK_RC acceptCall() ;
static MTALK_RC doTk(
	MTALK_SOCK mtSockIn,
	MTALK_SOCK mtSockOut,
	MTALK_BOOL bIsNVT
) ;
static MTALK_RC writeToCaller(struct mtalk_caller *pCaller, char *pszMessage) ;
static MTALK_RC readByteFromCaller(struct mtalk_caller *pCaller, int *piChar) ;
static MTALK_RC readRawFromCaller(
	struct mtalk_caller *pCaller,
	MTALK_BOOL *pbLineComplete
) ;
static MTALK_RC welcomeRecipient(struct mtalk_caller *pCallerRecipient) ;
static MTALK_RC notifyRecipient(struct mtalk_caller *pCallerSender) ;
static struct mtalk_caller *pCreateCallerStruc(
	MTALK_SOCK mtSockIn,
	MTALK_SOCK mtSockOut,
	MTALK_BOOL bIsNVT
) ;
static void freeCallerStruc(struct mtalk_caller *pCaller) ;
static MTALK_RC addCaller(struct mtalk_caller *pCaller) ;
static MTALK_RC muxCaller(MTALK_CALLERIDX ciTalker) ;
static MTALK_RC muxCallerLine(MTALK_CALLERIDX ciTalker) ;
static MTALK_BOOL addCline(struct mtalk_caller *pCaller, int ch) ;
static MTALK_RC whosOn(MTALK_CALLERIDX ciTalker, MTALK_ROLE mtRole) ;
static MTALK_RC exitCaller(MTALK_CALLERIDX ciExiter) ;
static MTALK_RC writeToAllCallers(
	char *pszToWrite,
	MTALK_ROLE mtRoleListener
) ;
static void fatalArgs(char *pszMessage) ;
static void syntax() ;
static void signon() ;
static void setsigcatch(int signo) ;
static void sigcatch(int signo) ;
static void dumpstatus() ;
static void dumpstat0() ;
static char *pt() ;


/*
	main function
*/

void main(int argc, char *argv[])
{
	init(argc, argv) ;
	/*
		change to userid "nobody" (which currently happens
		to be uid=65535).  We should accept this as a parameter
		and, if specified, look the name up in "/etc/passwd",
		and use the returned value.  IT SHOULDN'T BE HARD CODED
		AS IT CURRENTLY IS!
	*/
	if (setuid((uid_t) 65535)< 0) {
		perror("setuid") ;
		exit(1) ;
	}

	if (getListenSock() != MTALKRC_SUCCESS) {
		exit(1) ;
	}
	setsigcatch(SIGPIPE) ;
	setsigcatch(SIGUSR1) ;
	if (mtp.iDebugLevel> 0) {
		printf("%s: waiting...\n", pt()) ;
		fflush(stdout) ;
	}
	while(/*CONSTCOND*/ 1) {
		if (waitAndTakeAction() != MTALKRC_SUCCESS) {
			clerror("aborted due to error") ;
			exit(2) ;
		}
	}
	exit(0) ;
}


/*
	internal functions
*/

static void setsigcatch(int signo)
{
	signal(signo, sigcatch) ;
}

static void sigcatch(int signo)
{
	setsigcatch(signo) ;
	fprintf(
		stderr,
		"mtalk: %s: signal caught: signo=%d\n",
		pt(),
		signo
	) ;
	if (signo == SIGUSR1) {
		dumpstatus() ;
	}
}

static void dumpstatus()
{
	static MTALK_BOOL bInUse= 0 ;
	if (bInUse) {
		fprintf(
			stderr,
			"mtalk: dumpstatus: %s: recusion ignored\n",
			pt()
		) ;
		return ;
	}
	bInUse= 1 ;
	dumpstat0() ;
	bInUse= 0 ;
}

static void dumpstat0()
{
	MTALK_CALLERIDX ci ;
	struct mtalk_caller *pCaller ;
	FILE *fp ;

	fp= fopen("/tmp/mtalk.stat", "w") ;
	if (!fp) {
		fprintf(
			stderr,
			"mtalk: dumpstatus: %s: can't open\n",
			pt()
		) ;
		return ;
	}

	for (ci= 0; ci< MTALKMAX_NUMCALLERS; ci++) {
		pCaller= mtp.pmtca[ci] ;
		if (!pCaller) continue ;
		if (pCaller->mtState != MTALKSTATE_CHAT) {
			continue ;
		}
		fprintf(fp, pCaller->szName) ;
		fprintf(fp, "\n") ;
	}

	fclose(fp) ;
}

static void init(int argc, char *argv[])
{
	int i ;

	memset(&mtp, '\0', sizeof(mtp)) ;

	clinit(argv) ;
	while((i= clgetopt(argc, argv, "hqz"))> 0) {

		switch(i) {

			case	'h':
				mtp.bHelp= MTALKBOOL_TRUE ;
				break ;

			case	'q':
				mtp.bQuiet= MTALKBOOL_TRUE ;
				break ;

			case	'z':
				mtp.iDebugLevel++ ;
				break ;

			default:
				fatalArgs(0) ;
				break ;

		}
	}

	if (!mtp.bQuiet) {
		signon() ;
	}

	if (mtp.bHelp) {
		syntax() ;
		exit(MTALKE_HELP) ;
		/*NOTREACHED*/
	}
	
}

#define	MTALKREV_MAJOR(x)	((x) / 100)
#define	MTALKREV_MINOR(x)	((x) % 100)
static void signon()
{
	FILE *fp ;

	fp= stderr ;

	fprintf(
		fp,
		"mtalk %d.%d - Marty's Talker\n", 
		MTALKREV_MAJOR(MSRC_Revision),
		MTALKREV_MINOR(MSRC_Revision)
	) ;
	fprintf(fp, "Copyright (C) 1996, Marty Ross\n") ;
	fprintf(fp, "\n") ;
}

static void syntax()
{
	static char *pText[]= {
		"syntax:",
		"\tmtalk [option(s)]",
		"",
		"options:",
		"\th\tPrint this help text",
		"\tq\tQuiet operation.",
		"\tz\tIncrease general debug level",
		"",
	} ;
#define	N_TEXT	(sizeof(pText) / sizeof(pText[0]))
	int i ;

	for (i= 0; i< N_TEXT; i++) {
		printf("%s\n", pText[i]) ;
	}
}

static void fatalArgs(char *pszMessage)
{
	if (pszMessage) clerror(pszMessage) ;
	clerror("(use 'h' option for help)") ;
	exit(MTALKE_BADARGS) ;
	/*NOTREACHED*/
}

static MTALK_RC getListenSock()
{
	MTALK_SOCK sSock ;
	struct sockaddr_in sAddr ;
	int iYes ;

	sSock= socket(AF_INET, SOCK_STREAM, 0) ;
	iYes= 1 ;
	if (
		setsockopt(
			sSock,
			SOL_SOCKET,
			SO_REUSEADDR,
			&iYes,
			sizeof(iYes)
		)< 0
	) {
		perror("setsockopt") ;
		return(MTALKRC_FAILURE) ;
	}
	
	memset(&sAddr, '\0', sizeof(sAddr)) ;
	sAddr.sin_family= AF_INET ;
	sAddr.sin_port= htons(5004) ;
	sAddr.sin_addr.s_addr= INADDR_ANY ;

	if (
		bind(
			sSock,
			(struct sockaddr *) &sAddr,
			sizeof(sAddr)
		)< 0
	) {
		perror("bind") ;
		return(MTALKRC_FAILURE) ;
	}

	if (listen(sSock, 5)< 0) {
		perror("listen") ;
		close(sSock) ;
		return(MTALKRC_FAILURE) ;
	}	

	mtp.mtSockListen= sSock ;
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC waitAndTakeAction()
{
	fd_set rdfds ;
	int numfds ;
	int fdSock, rc ;
	MTALK_CALLERIDX ci ;

	FD_ZERO(&rdfds) ;

	numfds= 0 ;
	for (ci= 0; ci< MTALKMAX_NUMCALLERS; ci++) {
		if (mtp.pmtca[ci]) {
			fdSock= mtp.pmtca[ci]->mtSockIn ;
			FD_SET(fdSock, &rdfds) ;
			if (numfds< fdSock) numfds= fdSock ;
		}
	}

	FD_SET(mtp.mtSockListen, &rdfds) ;
	if (numfds< mtp.mtSockListen) {
		numfds= mtp.mtSockListen ;
	}

	if (mtp.iDebugLevel> 3) {
		printf("%s: entering select\n", pt()) ;
		fflush(stdout) ;
	}

	while(/*CONSTCOND*/1) {
		rc= select(
			NFDS((numfds + 1)),
			&rdfds,
			(fd_set *) NULL,
			(fd_set *) NULL,
			(struct timeval *) NULL
		) ;
		if (mtp.iDebugLevel> 3) {
			printf("%s: select returns %d\n", pt(), rc) ;
			fflush(stdout) ;
		}
		if (rc< 0) {
			printf("%s: (errno=%d)\n", pt(), errno) ;
			fflush(stdout) ;
			if (errno == EINTR) {
				/* interrupted system call; do again	*/
				continue ;
			}
			perror("select") ;
			return(MTALKRC_FAILURE) ;
		}
		break ;
	}

	if (FD_ISSET(mtp.mtSockListen, &rdfds)) {
		return(acceptCall()) ;
	}

	for (ci= 0; ci< MTALKMAX_NUMCALLERS; ci++) {
		if (!mtp.pmtca[ci]) continue ;
		fdSock= mtp.pmtca[ci]->mtSockIn ;
		if (!FD_ISSET(fdSock, &rdfds)) continue ;
		if (muxCaller(ci) != MTALKRC_SUCCESS) {
			return(MTALKRC_FAILURE) ;
		}
	}

	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC deleteCaller(MTALK_CALLERIDX ci)
{
	struct mtalk_caller *pCaller ;

	pCaller= mtp.pmtca[ci] ;
	close(pCaller->mtSockIn) ;
	if (pCaller->mtSockOut != pCaller->mtSockIn) {
		close(pCaller->mtSockOut) ;
	}
	freeCallerStruc(pCaller) ;
	mtp.pmtca[ci]= 0 ;
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC muxCaller(MTALK_CALLERIDX ciTalker)
{
	struct mtalk_caller *pCallerTalker ;
	MTALK_BOOL bLineComplete ;

	pCallerTalker= mtp.pmtca[ciTalker] ;
	if (
		readRawFromCaller(pCallerTalker, &bLineComplete)
	     != MTALKRC_SUCCESS
	) {
		clerror("error reading talking caller; deleting") ;
		return(deleteCaller(ciTalker)) ;
	}

	if (bLineComplete) {
		return(muxCallerLine(ciTalker)) ;
	}

	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC handleSpecialCmd(
	MTALK_CALLERIDX ciTalker,
	char *pszCLine,
	MTALK_BOOL *pbDone
) {
	(*pbDone)= MTALKBOOL_TRUE ;
	if (strcmp(pszCLine, "/who") == 0) {
		return(whosOn(ciTalker, MTALKROLE_UNKNOWN)) ;
	}
	if (strcmp(pszCLine, "/whor") == 0) {
		return(whosOn(ciTalker, MTALKROLE_RECIPIENT)) ;
	}
	if (strcmp(pszCLine, "/whos") == 0) {
		return(whosOn(ciTalker, MTALKROLE_SENDER)) ;
	}
	if (strcmp(pszCLine, "/exit") == 0) {
		return(exitCaller(ciTalker)) ;
	}
	(*pbDone)= MTALKBOOL_FALSE ;
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC processInitialLine(MTALK_CALLERIDX ciTalker)
{
	struct mtalk_caller *pCallerTalker ;
	MTALK_BOOL bDone ;

	pCallerTalker= mtp.pmtca[ciTalker] ;

	if (
		handleSpecialCmd(
			ciTalker,
			pCallerTalker->szCLine,
			&bDone
		) != MTALKRC_SUCCESS
	) {
		return(MTALKRC_FAILURE) ;
	}

	if (bDone) return(MTALKRC_SUCCESS) ;

	strncpy(
		pCallerTalker->szName,
		pCallerTalker->szCLine,
		MTALKMAX_CALLERNAMELEN
	) ;
	pCallerTalker->mtState= MTALKSTATE_WAIT ;
	if (strcmp(pCallerTalker->szName, "Marty8717") == 0) {
		pCallerTalker->mtRole= MTALKROLE_RECIPIENT ;
		strcpy(pCallerTalker->szName, "Marty") ;
		if (welcomeRecipient(pCallerTalker) != MTALKRC_SUCCESS) {
			writeToCaller(
				pCallerTalker,
				"Can't notify recipient\n"
			) ;
		}
	}
	else {
		pCallerTalker->mtRole= MTALKROLE_SENDER ;
		if (notifyRecipient(pCallerTalker) != MTALKRC_SUCCESS) {
			writeToCaller(pCallerTalker, "Can't notify recipient\n") ;
		}
		else {
			writeToCaller(pCallerTalker, "Recipient notified\n") ;
			writeToCaller(pCallerTalker, "Please wait...\n") ;
		}
	}

	if (!mtp.bQuiet) {
		printf("%s: caller='%s'\n", pt(), pCallerTalker->szName) ;
		fflush(stdout) ;
	}

	writeToCaller(
		pCallerTalker,
		"\n(use \"/exit\" to leave at any time)\n"
	) ;

	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC notifyOfExitingCaller(struct mtalk_caller *pCallerExiting)
{
	MTALK_ROLE mtRoleListener ;
	MTALK_CALLERIDX ciListener ;
	static char szToWrite[MTALKMAX_CLINESIZE+MTALKMAX_CALLERNAMELEN+17] ;
	struct mtalk_caller *pCallerListener ;

	mtRoleListener= (
		((pCallerExiting->mtRole) == MTALKROLE_RECIPIENT)
		?	MTALKROLE_SENDER
		:	MTALKROLE_RECIPIENT
	) ;
	sprintf(szToWrite, "\n(%s disconnected)\n", pCallerExiting->szName) ;
	writeToAllCallers(szToWrite, mtRoleListener) ;
	for (ciListener= 0; ciListener< MTALKMAX_NUMCALLERS; ciListener++) {
		pCallerListener= mtp.pmtca[ciListener] ;
		if (!pCallerListener) continue ;
		if (	
			(pCallerListener->mtRole != mtRoleListener)
		     || (pCallerListener->mtState != MTALKSTATE_CHAT)
		) {
			continue ;
		}
		if ((--pCallerListener->nChat) == 0) {
			writeToCaller(pCallerListener, "(nobody here)\n") ;
			pCallerListener->mtState= MTALKSTATE_WAIT ;
		}
	}
	writeToCaller(pCallerExiting, "Byebye!\n") ;
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC exitCaller(MTALK_CALLERIDX ciTalker)
{
	struct mtalk_caller *pCallerTalker ;

	pCallerTalker= mtp.pmtca[ciTalker] ;
	if (pCallerTalker->mtState == MTALKSTATE_CHAT) {
		notifyOfExitingCaller(pCallerTalker) ;
	}
	return(deleteCaller(ciTalker)) ;
}

static MTALK_RC whosOn(MTALK_CALLERIDX ciTalker, MTALK_ROLE mtRole)
{
	static char *pszRoleA[]= {		/* MTALKROLE_*	*/
		"unknown",
		"sender",
		"recipient"
	} ;
	static char *pszStateA[]= {		/* MTALKSTATE_*	*/
		"initial",
		"wait",
		"chat",
		"final"
	} ;
	char szPBuf[128] ;
	struct mtalk_caller *pCallerTalker ;
	struct mtalk_caller *pCallerListener ;
	MTALK_CALLERIDX ciListener ;

	pCallerTalker= mtp.pmtca[ciTalker] ;
	for (ciListener= 0; ciListener< MTALKMAX_NUMCALLERS; ciListener++) {
		pCallerListener= mtp.pmtca[ciListener] ;
		if (!pCallerListener) continue ;
		if (mtRole != MTALKROLE_UNKNOWN) {
			if (pCallerListener->mtRole != mtRole) continue ;
		}
		writeToCaller(pCallerTalker, pCallerListener->szName) ;
		writeToCaller(pCallerTalker, ": ") ;
		sprintf(
			szPBuf,
			"%s %s\n", 
			pszRoleA[pCallerListener->mtRole],
			pszStateA[pCallerListener->mtState]
		) ;
		writeToCaller(pCallerTalker, szPBuf) ;
	}

	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC writeToAllCallers(
	char *pszToWrite,
	MTALK_ROLE mtRoleListener
) {
	MTALK_CALLERIDX ciListener ;
	struct mtalk_caller *pCallerListener ;

	for (ciListener= 0; ciListener< MTALKMAX_NUMCALLERS; ciListener++) {
		pCallerListener= mtp.pmtca[ciListener] ;
		if (!pCallerListener) continue ;
		if (pCallerListener->mtRole != mtRoleListener) continue ;
		if (pCallerListener->mtState != MTALKSTATE_CHAT) continue ;
		writeToCaller(pCallerListener, pszToWrite) ;
	}
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC muxCallerLine(MTALK_CALLERIDX ciTalker)
{
	struct mtalk_caller *pCallerTalker ;
	MTALK_ROLE mtRoleListener ;
	MTALK_BOOL bDone ;
	static char szToWrite[MTALKMAX_CLINESIZE+MTALKMAX_CALLERNAMELEN+17] ;

	pCallerTalker= mtp.pmtca[ciTalker] ;

	if (pCallerTalker->mtState == MTALKSTATE_INITIAL) {
		return(processInitialLine(ciTalker)) ;
	}

	if (mtp.iDebugLevel> 0) {
		printf(
			"%s says: '%s'\n",
			pCallerTalker->szName,
			pCallerTalker->szCLine
		) ;
		fflush(stdout) ;
	}

	if (
		handleSpecialCmd(
			ciTalker,
			pCallerTalker->szCLine,
			&bDone
		) != MTALKRC_SUCCESS
	) {
		return(MTALKRC_FAILURE) ;
	}

	if (bDone) return(MTALKRC_SUCCESS) ;

	mtRoleListener= (
		((pCallerTalker->mtRole) == MTALKROLE_RECIPIENT)
		?	MTALKROLE_SENDER
		:	MTALKROLE_RECIPIENT
	) ;

	sprintf(
		szToWrite,
		"%s: %s\n",
		pCallerTalker->szName,
		pCallerTalker->szCLine
	) ;
	if (writeToAllCallers(szToWrite, mtRoleListener) != MTALKRC_SUCCESS) {
		return(MTALKRC_FAILURE) ;
	}

	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC acceptCall()
{
	int iaLen ;
	struct sockaddr_in sAddrPeer ;
	MTALK_SOCK sSockTk ;	

	iaLen= sizeof(sAddrPeer) ;
	sSockTk= accept(
		(int) mtp.mtSockListen,
		(struct sockaddr *) &sAddrPeer,
		&iaLen
	) ;
	if (sSockTk< 0) {
		perror("accept") ;
		return(MTALKRC_FAILURE) ;
	}

	if (fcntl(sSockTk, F_SETFL, O_NONBLOCK)< 0) {
		clerror("can't set socket flags") ;
		return(MTALKRC_FAILURE) ;
	}

	return(doTk(sSockTk, sSockTk, MTALKBOOL_TRUE)) ;
}

#define	NVTC_WILL		251
#define	NVTC_WONT		252
#define	NVTC_DO			253
#define	NVTC_DONT		254
#define	NVT_IAC			255

#define	NVTCO_ECHO		1
#define	NVTCO_SGA		3
#define	NVTCO_BM		19
#define	NVTCO_EOR		25

static MTALK_RC doTk(
	MTALK_SOCK mtSockIn,
	MTALK_SOCK mtSockOut,
	MTALK_BOOL bIsNVT
) {
	static char szNVTInitSeq[]= {

		NVT_IAC,
		NVTC_DONT,
		NVTCO_ECHO,

		NVT_IAC,
		NVTC_DONT,
		NVTCO_EOR,

		NVT_IAC,
		NVTC_DONT,
		NVTCO_BM,

		NVT_IAC,
		NVTC_WILL,
		NVTCO_ECHO,

		NVT_IAC,
		NVTC_WONT,
		NVTCO_EOR,

		NVT_IAC,
		NVTC_WONT,
		NVTCO_BM,

		0
	} ;
	struct mtalk_caller *pCaller ;

	pCaller= pCreateCallerStruc(mtSockIn, mtSockOut, bIsNVT) ;
	if (!pCaller) {
		clerror("can't create caller") ;
		return(MTALKRC_FAILURE) ;
	}

	if (addCaller(pCaller) != MTALKRC_SUCCESS) {
		clerror("can't add caller") ;
		freeCallerStruc(pCaller) ;
		return(MTALKRC_FAILURE) ;
	}

	if (bIsNVT) {
		writeToCaller(pCaller, szNVTInitSeq) ;
		writeToCaller(pCaller, "Enter your name: ") ;
	}

	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC	welcomeRecipient(struct mtalk_caller *pCallerRecipient)
{
	struct mtalk_caller *pCallerSender ;
	MTALK_CALLERIDX ci ;

	writeToCaller(pCallerRecipient, "Welcome, ") ;
	writeToCaller(pCallerRecipient, pCallerRecipient->szName) ;
	writeToCaller(pCallerRecipient, "\n") ;

	for (ci= 0; ci< MTALKMAX_NUMCALLERS; ci++) {
		pCallerSender= mtp.pmtca[ci] ;
		if (!pCallerSender) continue ;
		if (pCallerSender->mtRole != MTALKROLE_SENDER) continue ;
		if (pCallerSender->mtState == MTALKSTATE_WAIT) {
			writeToCaller(pCallerRecipient, "waiting: ") ;
			writeToCaller(pCallerRecipient, pCallerSender->szName) ;
			writeToCaller(pCallerRecipient, "\n") ;
			pCallerSender->mtState= MTALKSTATE_CHAT ;
			pCallerSender->nChat++ ;
		}
		else if (pCallerSender->mtState == MTALKSTATE_CHAT) {
			/* THIS SHOULD NEVER HAPPEN, SO WHY PUT IN IN? */
			writeToCaller(pCallerRecipient, "chatting: ") ;
			writeToCaller(pCallerRecipient, pCallerSender->szName) ;
			writeToCaller(pCallerRecipient, "\n") ;
			pCallerSender->nChat++ ;
		}
		pCallerRecipient->nChat++ ;
		writeToCaller(pCallerSender, pCallerRecipient->szName) ;
		writeToCaller(pCallerSender, " has arrived\n") ;
	}

	pCallerRecipient->mtState= MTALKSTATE_CHAT ;
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC	joinCurrentRecipient(
	struct mtalk_caller *pCallerSender,
	struct mtalk_caller **ppCallerRecipient
) {
	struct mtalk_caller *pCaller ;
	struct mtalk_caller *pCallerRecipient ;
	MTALK_CALLERIDX ci ;

	pCallerRecipient= (struct mtalk_caller *) 0 ;
	for (ci= 0; ci< MTALKMAX_NUMCALLERS; ci++) {
		pCaller= mtp.pmtca[ci] ;
		if (!pCaller) continue ;
		if (pCaller->mtRole != MTALKROLE_RECIPIENT) continue ;
		if (
			(pCaller->mtState == MTALKSTATE_CHAT)
		     || (pCaller->mtState == MTALKSTATE_WAIT)
		) {
			pCallerRecipient= pCaller ;
			break ;	
		}
	}
	(*ppCallerRecipient)= pCallerRecipient ;
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC notifyRecipient(struct mtalk_caller *pCallerSender)
{
	char *pszCallerName ;
	char szNotifyCommand[MTALKMAX_NOTIFYCMDLEN+1] ;
	int src ;
	struct mtalk_caller *pCallerRecipient ;

	pszCallerName= pCallerSender->szName ;
	if (mtp.iDebugLevel> 2) {
		printf(
			"%s: notifying recipient of call from '%s'\n",
			pt(),
			pszCallerName
		) ;
		fflush(stdout) ;
	}

	if (
		joinCurrentRecipient(
			pCallerSender,
			&pCallerRecipient
		) != MTALKRC_SUCCESS
	) {
		return(MTALKRC_FAILURE) ;
	}

	if (pCallerRecipient) {
		writeToCaller(pCallerRecipient, pszCallerName) ;
		writeToCaller(pCallerRecipient, " has joined\n") ;
		writeToCaller(pCallerSender, pCallerRecipient->szName) ;
		writeToCaller(pCallerSender, " is here\n") ;
		pCallerSender->mtState= MTALKSTATE_CHAT ;
		pCallerRecipient->mtState= MTALKSTATE_CHAT ;
		if (mtp.iDebugLevel> 1) {
			printf(
				"%s: %s joined to existing recipient: %s\n",
				pt(),
				pCallerSender->szName,
				pCallerRecipient->szName
			) ;
			fflush(stdout) ;
		}
		return(MTALKRC_SUCCESS) ;
	}

	sprintf(szNotifyCommand, "mtnotify marty \"%s\"", pszCallerName) ;
	src= system(szNotifyCommand) ;
	if (src != 0) {
		if (mtp.iDebugLevel> 0) {
			printf(
				"%s: system(%s) returns %d\n",
				pt(),
				szNotifyCommand,
				src
			) ;
		}
		return(MTALKRC_FAILURE) ;
	}
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC writeRawByteToCaller(struct mtalk_caller *pCaller, int iByte)
{
	int i ;
	char cByte ;

	if ((iByte< 0) || (iByte> 255)) return(MTALKRC_OOB) ;
	cByte= (char) iByte ;
	i= write(pCaller->mtSockOut, &cByte, 1) ;
	if (i<= 0) return(MTALKRC_IOERROR) ;
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC writeByteToCaller(struct mtalk_caller *pCaller, int iByte)
{
	if ((iByte == '\r') && (pCaller->bCrToNl)) {
		iByte= '\n' ;
	}
	if ((iByte == '\n') && (pCaller->bNlToCrNl)) {
		if (writeRawByteToCaller(pCaller, '\r') != MTALKRC_SUCCESS) {
			return(MTALKRC_FAILURE) ;
		}
	}
	if (writeRawByteToCaller(pCaller, iByte) != MTALKRC_SUCCESS) {
		return(MTALKRC_FAILURE) ;
	}
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC writeToCaller(struct mtalk_caller *pCaller, char *pszMessage)
{
	int i, iByte ;

	for (i= 0; pszMessage[i]; i++) {
		iByte= (int) ((unsigned char *) pszMessage)[i] ;
		if (writeByteToCaller(pCaller, iByte) != MTALKRC_SUCCESS) {
			return(MTALKRC_FAILURE) ;
		}
	}
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC readByteFromCaller(struct mtalk_caller *pCaller, int *piChar)
{
	int l ;

	if ((pCaller->iNVTBlockPtr) == (pCaller->iNVTBlockLen)) {
		l= read(
			pCaller->mtSockIn,
			pCaller->caNVTBlock,
			MTALKMAX_NVTBLOCKSIZE
		) ;
		if (l< 0) {
			if (errno == EAGAIN) return(MTALKRC_EOF) ;	
			perror("read") ;
			return(MTALKRC_FAILURE) ;
		}
		if (l == 0) return(MTALKRC_EOF) ;
		pCaller->iNVTBlockPtr= 0 ;
		pCaller->iNVTBlockLen= l ;
	}
	(*piChar)= (int) ((unsigned char *) pCaller->caNVTBlock)[
		pCaller->iNVTBlockPtr++
	] ;
	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC echoIt(struct mtalk_caller *pCaller, int iByte)
{
	if (!(pCaller->bEcho)) return ;
	return(writeByteToCaller(pCaller, iByte)) ;
}

static MTALK_BOOL addCline(struct mtalk_caller *pCaller, int ch)
{
	if (ch< 32) {
		if (ch == 8) {
			if (pCaller->iLenCLine> 0) {
				pCaller->iLenCLine-- ;
				echoIt(pCaller, ch) ;
			}
		}
		if ((ch == '\n') || (ch == '\r')) {
			echoIt(pCaller, ch) ;
			return(MTALKBOOL_TRUE) ;
		}
		return(MTALKBOOL_FALSE) ;
	}
	if (pCaller->iLenCLine< MTALKMAX_CLINESIZE) {
		pCaller->szCLine[pCaller->iLenCLine++]= ch ;
		echoIt(pCaller, ch) ;
	}
	return(MTALKBOOL_FALSE) ;
}

static MTALK_RC readRawFromCaller(
	struct mtalk_caller *pCaller,
	MTALK_BOOL *pbLineComplete
) {
	int ch, ch2, ch3 ;
	MTALK_RC mtrc ;

#define		MTRB_GETBYTE(ch)					\
	mtrc= readByteFromCaller(pCaller, &ch) ;			\
	if (mtrc != MTALKRC_SUCCESS) {					\
		if (mtrc == MTALKRC_EOF) return(MTALKRC_SUCCESS) ;	\
		return(MTALKRC_FAILURE) ;				\
	}

	(*pbLineComplete)= MTALKBOOL_FALSE ;
	while(/*CONSTCOND*/1) {
		MTRB_GETBYTE(ch) ;
		if (ch != NVT_IAC) {
			if (addCline(pCaller, ch)) break ;
			continue ;
		}
		MTRB_GETBYTE(ch2) ;
		if (mtp.iDebugLevel> 2) {
			printf("%s: nvt: %d %d\n", pt(), ch, ch2) ;
			fflush(stdout) ;
		}
		if (ch2 == NVT_IAC) {
			if (addCline(pCaller, ch2)) break ;
			continue ;
		}
		if (
			(ch2 == NVTC_DO)
		     ||	(ch2 == NVTC_DONT)
		     ||	(ch2 == NVTC_WILL)
		     ||	(ch2 == NVTC_WONT)
		) {
			MTRB_GETBYTE(ch3) ;
			if (mtp.iDebugLevel> 2) {
				printf("%s: nvto: %d\n", pt(), ch3) ;
				fflush(stdout) ;
			}
		}
	}

	pCaller->szCLine[pCaller->iLenCLine]= '\0' ;
	if (pCaller->iLenCLine> 0) {
		pCaller->iLenCLine= 0 ;
		(*pbLineComplete)= MTALKBOOL_TRUE ;
	}

	return(MTALKRC_SUCCESS) ;
}

static MTALK_RC addCaller(struct mtalk_caller *pCaller)
{
	int i ;

	if (!pCaller) {
		return(MTALKRC_FAILURE) ;
	}
	for (i= 0; i< MTALKMAX_NUMCALLERS; i++) {
		if (mtp.pmtca[i] == (struct mtalk_caller *) NULL) {
			mtp.pmtca[i]= pCaller ;
			return(MTALKRC_SUCCESS) ;
		}
	}
	return(MTALKRC_FAILURE) ;
}

static struct mtalk_caller *pCreateCallerStruc(
	MTALK_SOCK mtSockIn,
	MTALK_SOCK mtSockOut,
	MTALK_BOOL bIsNVT
) {
	struct mtalk_caller *pCaller ;

	pCaller= (struct mtalk_caller *) malloc(
		sizeof(struct mtalk_caller)
	) ;
	if (pCaller != (struct mtalk_caller *) NULL) {
		memset(pCaller, '\0', sizeof(struct mtalk_caller)) ;
		pCaller->mtSockIn= mtSockIn ;
		pCaller->mtSockOut= mtSockOut ;
		pCaller->bIsNVT= bIsNVT ;
		pCaller->mtRole= MTALKROLE_UNKNOWN ;
		pCaller->mtState= MTALKSTATE_INITIAL ;
		pCaller->bEcho= MTALKBOOL_TRUE ;
		pCaller->bNlToCrNl= MTALKBOOL_TRUE ;
		pCaller->bCrToNl= MTALKBOOL_TRUE ;
	}
	return(pCaller) ;
}

static void freeCallerStruc(struct mtalk_caller *pCaller)
{
	free(pCaller) ;
}

static char *pt()
{
	time_t t ;
	struct tm *ptm ;
	static char szTimeFormat[32] ;

	t= time(0) ;
	ptm= localtime(&t) ;
	sprintf(
		szTimeFormat,
		"%02u%02u%02u%02u%02u",
		ptm->tm_mon + 1,
		ptm->tm_mday,
		ptm->tm_hour,
		ptm->tm_min,
		ptm->tm_sec
	) ;
	
	return(szTimeFormat) ;
}

/* end of mtalk.c */
