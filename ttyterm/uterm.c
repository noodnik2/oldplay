/*
	uterm.c				Unix terminal file functions
					Copyright (C) 1990-1993, Contahal Ltd.

	28.4.93:	added 'utermwait()' function; uses 'streams' poll()

*/

#include	<stdio.h>
#include	<fcntl.h>
#include	<errno.h>
#include	<signal.h>
#include	<stropts.h>
#include	<poll.h>

#define		MAXTTYS		2

#include	"compiler.h"
#include	"uterm.h"
#include	"utset.h"

void mod_tlc() ;

static int uterm_open_options= 0 ;

/*
	rc= utermwait(ttylist, n)

	Waits for input to appear on any of the ttys listed in 'ttylist'

	Returns:
		-99	bad input parameters
		-1	unknown fault; signal
		1	tty read error
		0	I/O ready on one of the ttys in 'ttylist'

*/
int utermwait(ttylist, n)
struct uterm *ttylist[] ;
int n ;
{
	struct pollfd pds[MAXTTYS] ;
	int i ;
#define	MP_PRFLAGS	(POLLIN | POLLRDNORM | POLLPRI)
#define	MP_PEFLAGS	(POLLERR | POLLHUP | POLLNVAL)

	if (n> MAXTTYS) return(-99) ;
	for (i= 0; i< n; i++) {
		pds[i].fd= ttylist[i]->inp_fh ;
		pds[i].events= MP_PRFLAGS ;
		pds[i].revents= 0 ;
	}

	while(/*CONSTCOND*/1) {
		if (poll(pds, n, INFTIM)< 0) {
			if (errno != EINTR) break ;
			continue ;
		}
		for (i= 0; i< n; i++) {
			if (pds[i].revents & MP_PRFLAGS) return(0) ;
			if (pds[i].revents & MP_PEFLAGS) return(1) ;
		}
	}
	return(-1) ;

#undef	MP_PRFLAGS
#undef	MP_PEFLAGS
}

/*
	utermo_setopt(opts)	set current uterm open options
*/
void utermo_setopt(opts)
int opts ;
{
	uterm_open_options= opts ;
}

/*
	utermo		open a terminal node *utp from device 'name*'.
			read current characteristics into *uts.

*/
int utermo(name0, name1, utp, uts)
char *name0, *name1 ;
struct uterm *utp ;
struct utset *uts ;
{
	int ifh, ofh ;

	if (strlen(name0)>= sizeof(utp->name0)) return(1) ;

	if (name1 != (char *)NULL) {
		if (strlen(name1)>= sizeof(utp->name1)) return(1) ;
		if ((ifh= open(name0, O_RDONLY|O_NDELAY))< 0) return(2) ;
		if ((ofh= open(name1, O_WRONLY|O_NDELAY))< 0) {
			close(ifh) ;
			return(3) ;
		}
		if (uterm_open_options & UTOO_DEBUG) {
			fprintf(stderr, "uterm: opened twice\n") ;
		}
		strcpy(utp->name0, name0) ;
		strcpy(utp->name1, name1) ;
	}
	else {
		if ((ifh= ofh= open(name0, O_RDWR|O_NDELAY))< 0) return(2) ;
		if (uterm_open_options & UTOO_DEBUG) {
			fprintf(stderr, "uterm: opened once\n") ;
		}
		strcpy(utp->name0, name0) ;
		strcpy(utp->name1, name0) ;
	}

#if 0
/* #if OPSYSLVL==OPLBSD42 */
	r= fcntl(ifh, F_GETFL, 0) ;
	if (!(r & O_NDELAY)) {
		fcntl(ifh, F_SETFL, r|O_NDELAY) ;
	}
/* #endif */
#endif

	utp->inp_fh= ifh ;
	utp->out_fh= ofh ;
	utp->inbptr= 0 ;
	utp->inblen= 0 ;

	utermqs(utp, uts) ;		/* read current port settings */

					/* decipher current baud rate */
	baud2term(&(uts->inp_s), &(utp->inp_baud), 0) ;
	baud2term(&(uts->out_s), &(utp->out_baud), 1) ;

	return(0) ;
}

/*
	utermset:		set terminal node characteristics

	sets terminal node *utp characteristics to value in *uts.

*/
void utermset(utp, uts)
struct uterm *utp ;
struct utset *uts ;
{
	utermss(utp, uts) ;
}

/*
	utermcso:		modify terminal node characteristics

	characteristics are set according to values in *uto.

*/
void utermcso(utp, uto, baud)
struct uterm *utp ;
struct utcopt *uto ;
int baud ;
{
	struct utset cts ;

	utermqs(utp, &cts) ;			/* read current sets */
	mod_tlc(&cts.inp_s, uto, baud) ;	/* modify input settings */
	mod_tlc(&cts.out_s, uto, baud) ;	/* modify output setting */
	utermss(utp, &cts) ;			/* apply settings to tty */
}

/*
	utermin		receive a character from remote terminal node

*/
int utermin(utp)
struct uterm *utp ;
{
	if ((utp->inbptr)>= (utp->inblen)) {
		(utp->inblen)= read(utp->inp_fh, (utp->inbuf), UT_IBUFSIZE) ;
		(utp->inbptr)= 0 ;
		if ((utp->inblen)<= 0) return(-1) ;
	}
	return((int)((utp->inbuf)[(utp->inbptr)++])) ;
}

/*
	utermout		send a character to remote terminal node

*/
int utermout(utp, c)
struct uterm *utp ;
int c;
{
	unsigned char cc ;

	cc= (unsigned char)c ;
	if (write(utp->out_fh, &cc, 1)!=1) return(-1) ;
	return(0) ;
}

/*
	utermouts		send a string to remote terminal node

*/
int utermouts(utp, s, l)
struct uterm *utp ;
char *s ;
int l ;
{
	if (write(utp->out_fh, s, l) != l) return(-1) ;
	return(0) ;
}

/*
	utermc		close the terminal node

*/
int utermc(utp)
struct uterm *utp ;
{
	if ((utp->inp_fh) != 0) close(utp->inp_fh) ;
	if ((utp->out_fh) != 1) close(utp->out_fh) ;
}

/*
	od_tlc:		modify terminal line characteristics structure
			according to settings in *uto.

*/
static void mod_tlc(tcp, uto, baud)
TERMCHAR *tcp ;
struct utcopt *uto ;
int baud ;
{
	int andflag, orflag ;

	term2baud(tcp, baud, 0) ;
	term2baud(tcp, baud, 1) ;

#if OPSYSLVL==OPLBSD42
	andflag= orflag= 0 ;
        orflag|= RAW ;		          	/* set RAWmode */
       	andflag|= ECHO ;	 		/* set no ECHO */
	(tcp->sg_flags)|= orflag ;
	(tcp->sg_flags)&= ~andflag ;
#endif

#if	OPSYSLVL==OPLSYSV
						/* output flags */
	orflag= andflag= 0 ;
	if ((uto->opost)<0) andflag|= OPOST ;
	else if ((uto->opost)>0) orflag|= OPOST ;
	(tcp->c_oflag)|= orflag ;
	(tcp->c_oflag)&= ~andflag ;
						/* line flags */
	orflag= andflag= 0 ;
	if ((uto->echo)<0) andflag|= (ECHO|ECHOE|ECHOK|ECHONL) ;
	else if ((uto->echo)>0) orflag|= (ECHO|ECHOE|ECHOK|ECHONL) ;
	if ((uto->icanon)<0) andflag|= (ICANON) ;
	else if ((uto->icanon)>0) orflag|= (ICANON) ;
	if ((uto->isig)<0) andflag|= (ISIG) ;
	else if ((uto->icanon)>0) orflag|= (ISIG) ;
	(tcp->c_lflag)|= orflag ;
	(tcp->c_lflag)&= ~andflag ;
						/* control flags */
	orflag= andflag= 0 ;
	if ((uto->cread)<0) andflag|= (CREAD) ;
	else if ((uto->cread)>0) orflag|= (CREAD) ;
	if ((uto->local)<0) andflag|= (CLOCAL) ;
	else if ((uto->local)>0) orflag|= (CLOCAL) ;
	(tcp->c_cflag)|= orflag ;
	(tcp->c_cflag)&= ~andflag ;
						/* input flags */
	orflag= andflag= 0 ;
	if ((uto->icrnl)<0) andflag|= (ICRNL) ;
	else if ((uto->icrnl)>0) orflag|= (ICRNL) ;
	if ((uto->brkint)<0) andflag|= (BRKINT) ;
	else if ((uto->brkint)>0) orflag|= (BRKINT) ;
	if ((uto->ixon)<0) {
		andflag|= (IXON) ;
	}
	else if ((uto->ixon)>0) {
		orflag|= (IXON) ;
		andflag|= (IXANY) ;
	}
	if ((uto->ixoff)<0) andflag|= (IXOFF) ;
	else if ((uto->ixoff)>0) orflag|= (IXOFF) ;
#if	COMPILER!=NCRNST
	if ((uto->irts)<0) andflag|= (IRTS) ;
	else if ((uto->irts)>0) orflag|= (IRTS) ;
#endif

	(tcp->c_iflag)|= orflag ;		/* signal interrupt break */
	(tcp->c_iflag)&= ~andflag ;
					/* cleanup: according to settings */
	if (!((tcp->c_lflag) & ICANON)) {
		(tcp->c_cc)[VEOF]= 1 ; 		/* 1 char groups  */
		(tcp->c_cc)[VEOL]= 10 ;		/* .1 sec timeout */
	}

#endif

}

/*
	get_tlc:	get line characteristics structure

*/
static int get_tlc(fh, tcp)
int fh ;
TERMCHAR *tcp ;
{
#if	OPSYSLVL==OPLBSD42
	if (gtty(fh, tcp)< 0) return(1) ;
#endif

#if	OPSYSLVL==OPLSYSV
	if (ioctl(fh, TCGETA, tcp)< 0) return(1) ;
#endif
	return(0) ;
}

/*
	put_tlc:	applies terminal line characteristics from *tcp
			to terminal file fh.

*/
static int put_tlc(fh, tcp)
int fh ;
TERMCHAR *tcp ;
{
#if OPSYSLVL==OPLBSD42
        if (stty(fh, tcp)< 0) return(1);	/* failed */
#endif

#if OPSYSLVL==OPLSYSV
	if (ioctl(fh, TCSETAW, tcp)<0) return(1) ;     /* failed */
#endif

	return(0) ; 
}

/*
	using the devices described by (utp->inp_fh) and (utp->out_fh),
	read the terminal characteristics into *uts.

*/
static int utermqs(utp, uts)
struct uterm *utp ;
struct utset *uts ;
{
	if (get_tlc(utp->inp_fh, &(uts->inp_s))) return(1) ;
	if (get_tlc(utp->out_fh, &(uts->out_s))) return(1) ;
#if 0
	baud2term(&(uts->inp_s), &(utp->inp_baud), 0) ;
	baud2term(&(uts->out_s), &(utp->out_baud), 1) ;
#endif
	return(0) ;
}

/*
	utermss:		set settings

		DOES NOT UPDATE BAUD RATE FROM *utp !
*/
static int utermss(utp, uts)
struct uterm *utp ;
struct utset *uts ;
{
#if 0
	term2baud(&(uts->inp_s), utp->inp_baud, 0) ;
	term2baud(&(uts->out_s), utp->out_baud, 1) ;
#endif
	if (put_tlc(utp->inp_fh, &(uts->inp_s))) return(1) ;
	if (put_tlc(utp->out_fh, &(uts->out_s))) return(1) ;
	return(0) ;
}

#if OPSYSLVL==OPLBSD42

static int term2baud(stp, speed, outflag)
TERMCHAR *stp ;
int speed, outflag ;
{
	int val ;
	char *dp ;

	dp= (outflag? &(stp->sg_ospeed): &(stp->sg_ispeed)) ;
	switch(speed) {
		case	300:		val= B300;	break; 
		case	1200:		val= B1200;	break; 
		case	2400:		val= B2400;	break; 
		case	4800:		val= B4800;	break; 
		case	9600:		val= B9600;	break; 
		case	19200:		val= EXTA;	break; 
		default:		val= -1;	break;
	}
	if (val!=(-1)) {
		(*dp)= (char)val ;
		return(0) ;
	}
	return(-1) ;
}

static int baud2term(stp, utbp, outflag)
TERMCHAR *stp ;
int *utbp, outflag ;
{
	int speed, val ;

	speed= (int) (outflag? stp->sg_ospeed: stp->sg_ispeed) ;
	switch(speed) {
		case	B300:		val= 300;	break; 
		case	B1200:		val= 1200;	break; 
		case	B2400:		val= 2400;	break; 
		case	B4800:		val= 4800;	break; 
		case	B9600:		val= 9600;	break; 
		case	EXTA:		val= 19200;	break; 
		default:		val= -1;	break;
	}
	if (val!=(-1)) {
		(*utbp)= val ;
		return(0) ;
	}
	return(-1) ;
}
#endif

#if OPSYSLVL==OPLSYSV

/*
	term2baud

	set speed variable in *stp to corresponding BAUD
	value for 'speed'.  outflag true iff output file.
*/
static int term2baud(stp, speed, outflag)
TERMCHAR *stp ;
int speed, outflag ;
{
	int val ;
	unsigned short *dp ;

	dp= (unsigned short *)&(stp->c_cflag) ;
	switch(speed) {
		case	300:		val= B300;	break; 
		case	1200:		val= B1200;	break; 
		case	2400:		val= B2400;	break; 
		case	4800:		val= B4800;	break; 
		case	9600:		val= B9600;	break; 
		case	19200:		val= EXTA;	break; 
		default:		val= -1;	break;
	}
	if (val != (-1)) {
		(*dp)= ((*dp) & ~(unsigned short)CBAUD) | (unsigned short)val ;
		return(0) ;
	}
	return(-1) ;
}

/*
	baud2term

		look at the terminal baud rate in *stp,
		translate it to human, then write it to
		*utbp.
*/
static int baud2term(stp, utbp, outflag)
TERMCHAR *stp ;
int *utbp, outflag ;
{
	int speed, val ;

	speed= (int) ((stp->c_cflag) & (unsigned short)CBAUD) ;
	switch(speed) {
		case	B300:		val= 300;	break; 
		case	B1200:		val= 1200;	break; 
		case	B2400:		val= 2400;	break; 
		case	B4800:		val= 4800;	break; 
		case	B9600:		val= 9600;	break; 
		case	EXTA:		val= 19200;	break; 
		default:		val= -1;	break;
	}
	if (val != (-1)) {
		(*utbp)= val ;
		return(0) ;
	}
	return(-1) ;
}
#endif
