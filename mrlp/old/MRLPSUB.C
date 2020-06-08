/*

	mrlpsub.c:		Marty's Remote Line Printer: Subroutines
				Copyright (C) 1992, Contahal Ltd.


*/

#include	<stdio.h>
#include	<errno.h>
#include	<netdb.h>
#include	<sys/time.h>

#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"mrlpsub.h"

int mrlp_getnp(sock_in_p, servicename, protoname)
struct sockaddr_in *sock_in_p ;
char *servicename, *protoname ;
{
	struct servent *sent ;
	int portno ;

	if (sscanf(servicename, "%d", &portno) == 1) {
		sock_in_p->sin_family= AF_INET ;
		sock_in_p->sin_port= htons((short) portno) ;
		return(0) ;
	}

	if (    (sent= getservbyname(servicename, protoname))
		== (struct servent *)NULL
	) {
		return(2) ;
	}

	sock_in_p->sin_family= AF_INET ;
	sock_in_p->sin_port= (sent->s_port) ;
	return(0) ;
}

int mrlp_getnh(sock_in_p, hostname)
struct sockaddr_in *sock_in_p ;
char *hostname ;
{
	struct hostent *hent ;
	u_long inet ;

	if ((inet= inet_network(hostname)) != (u_long)-1) {
		sock_in_p->sin_addr.s_addr= htonl(inet) ;
		return(0) ;
	}

	if (    (hent= gethostbyname(hostname))
		== (struct hostent *)NULL
	) {
		return(1) ;
	}
	if (hent->h_addrtype != AF_INET) {
		return(2) ;
	}
	memcpy(
		(char *)&sock_in_p->sin_addr,
		(char *)hent->h_addr,
		hent->h_length
	) ;
	return(0) ;
}

int mrlp_getline(sock, buf, maxlen)
int sock, maxlen ;
char *buf ;
{
	int i ;
	char c ;

	maxlen-- ;
	for (i= 0; i< maxlen; i++) {
		if (read(sock, &c, 1)<= 0) {
			buf[i]= '\0' ;
			return(1) ;
		}
		if (c == '\n') break ;
		buf[i]= c ;
	}
	buf[i]= '\0' ;
	return(0) ;
}

/*
	message subroutines
*/

static char msgid[MRLP_IDBSIZE+1]= "" ;
static char msgname[MRLP_NAMEBSIZE+1]= "" ;
static char msgbuf[MRLP_MSGBSIZE+1] ;

static void _apply_pfx(xmsgbuf)
char *xmsgbuf ;
{
	if ((msgname[0] == '\0') && (msgid[0] == '\0')) {
		strcpy(xmsgbuf, msgbuf) ;
	}
	else if (msgid[0] == '\0') {
		sprintf(xmsgbuf, "%s: %s", msgname, msgbuf) ;
	}
	else {
		sprintf(xmsgbuf, "%s[%s]: %s", msgname, msgid, msgbuf) ;
	}
}

static void _std_msg()
{
	char xmsgbuf[MRLP_MSGBSIZE+1] ;

	_apply_pfx(xmsgbuf) ;
	fprintf(STDTTY, "%s\n", xmsgbuf) ;
}

static void _perr_msg()
{
	char xmsgbuf[MRLP_MSGBSIZE+1] ;

	_apply_pfx(xmsgbuf) ;
	perror(xmsgbuf) ;
}

void mrlp_setname(name)
char *name ;
{
	strncpy(msgname, name, MRLP_NAMEBSIZE) ;
}

void mrlp_setid(id)
long id ;
{
	sprintf(msgid, "%ld", id) ;
}

void mrlp_msg(msg)
char *msg ;
{
	strncpy(msgbuf, msg, MRLP_MSGBSIZE) ;
	_std_msg() ;
}

void mrlp_nmsg(msg, ntype, n)
char *msg, *ntype ;
int n ;
{
	sprintf(msgbuf, "%s: %s=%d", msg, ntype, n) ;
	_std_msg() ;
}

void mrlp_smsg(msg, str)
char *msg, *str ;
{
	sprintf(msgbuf, "%s: %s", msg, str) ;
	_std_msg() ;
}

void mrlp_err(msg)
char *msg ;
{
	sprintf(msgbuf, "error: %s", msg) ;
	_std_msg() ;
}

void mrlp_serr(msg, str)
char *msg, *str ;
{
	sprintf(msgbuf, "error: %s: %s", msg, str) ;
	_std_msg() ;
}

void mrlp_nerr(msg, n)
char *msg ;
int n ;
{
	sprintf(msgbuf, "error: %s, code=%d", msg, n) ;
	_std_msg() ;
}

void mrlp_perr(msg)
char *msg ;
{
	sprintf(msgbuf, "%s: (errno=%d)", msg, errno) ;
	_perr_msg() ;
}

void mrlp_abort(msg)
char *msg ;
{
	sprintf(msgbuf, "aborted: %s", msg) ;
	_std_msg() ;
	exit(1) ;
}

char *mrlp_inetaddr_str(addr_p)
struct sockaddr_in *addr_p ;
{
	int i ;
	unsigned char FAR *s ;
	static char buf[65] ;

#if 0
	s= inet_ntoa(&addr_p->sin_addr) ;
	for (i= 0; s[i] != '\0'; i++) {
		buf[i]= s[i] ;
	}
	sprintf(buf+i, "!%u", (unsigned)(ntohs(addr_p->sin_port))) ;
#else
	s= (unsigned char *)&(addr_p->sin_addr) ;
	buf[0]= '\0' ;
	for (i= 0; i< sizeof(addr_p->sin_addr); i++) {
		sprintf(buf+strlen(buf), "%u", (unsigned)(s[i])) ;
		if (i< (sizeof(addr_p->sin_addr)-1)) {
			strcat(buf, ".") ;
		}
	}
	sprintf(buf+strlen(buf), "!%u", (unsigned)(ntohs(addr_p->sin_port))) ;
#endif
	return(buf) ;
}

int mrlp_iscmdmsg(buf, cmd, argp_p)
char *buf, *cmd, **argp_p ;
{
	int clen ;

	if (memcmp(buf, cmd, clen= strlen(cmd)) == 0) {
		(*argp_p)= buf + clen ;
		return(1) ;
	}
	return(0) ;
}

int mrlp_wsock(sock, buf, len, desc)
int sock, len ;
char *buf, *desc ;
{
	int l ;

	if ((l= write(sock, buf, len))< len) {
		if (l< 0) mrlp_perr(desc) ;
		else      mrlp_err(desc) ;
		return(1) ;
	}	
	return(0) ;
}

#define		USE_SELECT_INPUT	1

int mrlp_rsock(sock, buf, bufsize, to_secs)
int sock, bufsize, to_secs ;
char *buf ;
{

#if	USE_SELECT_INPUT

	fd_set read_template ;
	struct timeval wait ;
	int nb ;
	
	FD_ZERO(&read_template) ;
	FD_SET(sock, &read_template) ;
	memset((char *)&wait, '\0', sizeof(wait)) ;
	wait.tv_sec= to_secs ;

	nb= select(
		FD_SETSIZE,
		&read_template,
		(fd_set *)NULL,
		(fd_set *)NULL,
		(to_secs< 0)? (struct timeval *)NULL: &wait
	) ;

	if (nb<= 0) {
		return(-1) ;
	}

	if (!FD_ISSET(sock, &read_template)) {
		mrlp_err("rsock failed: bit not set") ;
		return(-2) ;
	}

#if 0
	if (ioctl(sock, I_NREAD, &nb)< 0) {
		mrlp_perr("rsock: ioctl") ;
		return(-3) ;
	}

	bufsize= (nb> bufsize)? bufsize: nb ;
#endif

#endif

	return(read(sock, buf, bufsize)) ;
}
