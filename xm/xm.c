/* %W% %H% %T% ( %G% %U% ) */
/*
 *  XM - a UNIX-to-CP/M file transfer shell
 *      by Richard Conn
 *
 *  XM is based on UC version 1.2, which in turn was based on UMODEM 3.5,
 *  originally written by Lauren Weinstein, and mutated by Richard Conn,
 *  Bennett Marks, Michael Rubenstein, Ben Goldfarb, David Hinnant, and
 *  Lauren Weinstein. XM differs from UC in that it offers only a basic
 *  UNIX-to-CP/M file transfer facility using the Christensen XMODEM protocol
 *  in checksum mode.
*/

#define		DEBUG	1

/*  Library Utilities  */
#include        <stdio.h>
#include        <sys/types.h>
#include        <sys/stat.h>
#include        <signal.h>
#include        <ctype.h>
#include        <setjmp.h>

/* Local Environment Definitions */
#include	"compiler.h"

/* Program Definitions */
#define versmaj 1       /* Major Version */
#define versmin 0       /* Minor Version */
			/*  Basics  */
#define FALSE   0
#define TRUE    ~FALSE
			/*  ASCII Characters  */
#define SOH     001
#define STX     002
#define ETX     003
#define EOT     004
#define ENQ     005
#define ACK     006
#define LF      012
#define CR      015
#define NAK     025
#define SYN     026
#define CAN     030
#define CTRLZ   032
#define ESC     033
				/*  XM Constants  */
#define TIMEOUT -1              /* Timeout Flag */
#define BLOCKSZ 128             /* Size of transmission block */
#define CREATE  0644            /* Mode for New Files */

#define ERRMAX  7               /* Max errors tolerated */
#define DELAY   3               /* Basic delay for transmission */

#define	AB_ARE		0
#define	AB_USER		1
#define	AB_TIMEOUT	2
#define	AB_CKSUM	3
#define	AB_RETRY	4
#define	AB_IOERR	5
#define	AB_SYNC		6
#define	AB_DUPBLOCK	7

static char *abr[]= {
	"because file already exists",
	"by user",
	"due to timeout",
	"due to checksum errors",
	"due to excessive retries",
	"due to file I/O errors",
	"due to synchronization error",
	"due to excessive duplicate blocks",
} ;
		
jmp_buf rbenv;			/*  Setjmp Environment */

main(argc, argv)
int argc;
char *argv[];
{
        int sendflg, recvflg, statflg;  /* major functions */
        char filetype;                  /* tranfer type */
        FILE *fopen();                  /* forward ref */
        int index;      /* index for arg parsing loop */
        char opt;       /* current option character */
        char *getenv(); /* getenv function defn */

        				/* Print Banner */
        fprintf(stderr, "XM Version %d.%d - Christensen XMODEM Protocol\n"
                ,versmaj,versmin);
        fprintf(stderr, "File Transfer Tool\n");
        /* Check for Help Request */
        if (argc == 1) {
                help();
                exit(0);
        }
        				/* Init for Option Parsing */
        sendflg= FALSE;
        recvflg= FALSE;
        statflg= FALSE;

        				/* Process Options */
        index= 0;
        while((opt = argv[1][index++]) != '\0') switch (opt) {
                case '-' :              /* skip dash */
                        break;
                case 'F' :
                case 'f' :
                        statflg = TRUE; /* set file stat mode */
                        break;
                case 'R' :
                        recvflg = TRUE; /* set file recv mode */
                        filetype = 'b';
                        break;
                case 'r' :
                        recvflg = TRUE; /* set file recv mode */
                        filetype = 't';
                        break;
                case 'S' :
                        sendflg = TRUE; /* set file send mode */
                        filetype = 'b';
                        break;
                case 's' :
                        sendflg = TRUE; /* set file send mode */
                        filetype = 't';
                        break;
                default :
                        fprintf(stderr, "xm: Invalid Option %c\n", opt);
                        break;
        }

        /* Select and Execute Major Mode */
        if (statflg) {          /* File Status Display */
                if (argc < 3) {
                        fprintf(stderr, "xm: no filename specified\n");
                        exit(0);
                }
                fxstat(filetype,argv[2]);
                exit(0);
        }

        if (sendflg) {          /* Send File */
                if (argc < 3) {
                        fprintf(stderr, "xm: no filename specified\n");
                        exit(0);
                }
                sendfile(filetype, argv[2]);
                exit(0);
        }

        if (recvflg) {          /* Receive File */
                if (argc < 3) {
                        fprintf(stderr, "xm: no filename specified\n");
                        exit(0);
                }
                recv(filetype, argv[2]);
                exit(0);
        }

        fprintf(stderr, "xm: no mode selected\n");
        help();
        exit(0);
}

/* Print Help */
help()
{
        fprintf(stderr, "Usage:  XM c filename\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "where 'c' MUST be One of the Following Commands --\n");
        fprintf(stderr, "\tR -- Receive Binary File\n");
        fprintf(stderr, "\tr -- Receive Text File\n");
        fprintf(stderr, "\tS -- Send Binary File\n");
        fprintf(stderr, "\ts -- Send Text File\n");
        fprintf(stderr, "\tf or F -- Show File Status\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "Examples:\n");
        fprintf(stderr, "\tXM S myfile <-- Send Binary File \"myfile\"\n");
        fprintf(stderr, "\tXM s mytext <-- Send Text File \"mytext\"\n");
}


int abortmsg() ;

/* Send File */
int sendfile(filetype, filename)
char filetype, *filename;
{
        FILE *fd, *fopen();
        int blocknum;                   /* Current Block Number */
        int nlflg;                      /* New Line for File Convert */
        int sending;                    /* Xmit In-Progress Flag */
        int tries;                      /* Attempt Count */
        int bufctr;                     /* Counter for Buffer Build */
        int c;                          /* Temp Char */
	int i ;
        int rcode;                      /* Return Code */
	int aborted; 			/* was process aborted? */
	int srblocks ;			/* number of sent blocks */
        char buf[BLOCKSZ];              /* Buffer for Transfer */

				        /* Print Banner */
        fprintf(stderr, "XM Sending %s File: %s\n",
                (filetype == 't') ? "Text" : "Binary",
                filename);

        /* Open File for Input and Print Opening Messages */
        if ((fd= fopen(filename, "r")) == 0) {
                fprintf(stderr, "Can`t Open File %s for Send\n", filename);
                return;
        }
        fxstat(filetype,filename);   /* Print File Status Info */
        fprintf(stderr, "Ready to Send File\n");
        binary(TRUE, TRUE);      /* Open Binary Communications */

        /* Init Parameters */
        nlflg= FALSE ;
        sending= TRUE ;
	srblocks= 0 ;
        blocknum= 1 ;

	/*
		synchronize with the other side:

		reads characters from other side
		until either:

			(1)	receives NAK (done)
			(2)	512 bytes received (error)
			(3)	timeout (10 seconds)
	*/
	for (i= 0; i< 512; i++) {
		c= recvbyte(10) ;
		if (c==NAK || c==TIMEOUT) break ;
	}
	if (c != NAK) {
		fclose(fd) ;
		binary(FALSE, TRUE) ;
		abortmsg(c==TIMEOUT? AB_TIMEOUT: AB_SYNC) ;
		return ;
	}

        /* Main Transmission Loop */
	aborted= 0 ;
        while(sending) {

                				/* Build Next Block into buf */
                for (bufctr= 0; bufctr< BLOCKSZ;) {
                        if (nlflg) {			/* New Line */
                                buf[bufctr++]= LF;     	/* Store LF */
                                nlflg= FALSE;
                        }
                        if (bufctr == BLOCKSZ) break;   /* Leave for Loop */
                        c= getc(fd);		/* Get Next Byte from File */
                        if (c == EOF) {
                                sending= FALSE;		/* Done */
                                if (!bufctr) break;	/* Avoid Extra Block */
                                for(;bufctr< BLOCKSZ; bufctr++) {
                                        buf[bufctr]= (filetype == 't')?
                                                CTRLZ: '\0' ;
				}
                                continue;       /* Exit for Loop */
                        }
                        if ((c==LF) && (filetype=='t')) {  /* NL? */
                              	buf[bufctr++]= CR;	/* Insert CR */
                               	nlflg= TRUE;		/* New Line */
                        }
                        else buf[bufctr++]= c;		/* Store Char */
                }

                					/* Send Block */
                if (bufctr) {
			for (tries= 0; tries< ERRMAX; tries++) {
                       		putblock(filetype, buf, blocknum);
				if ((i= ackwait(DELAY, 512))==0) break ;
#if	DEBUG
				fprintf(stderr, "resend: %d: ack=%d\n",
					blocknum, i) ;
#endif
                	}
			blocknum++ ;
                	blocknum&= 0xFF ;
			srblocks++ ;
                	if (tries == ERRMAX) {
				sending= FALSE;  /* Error Abort */
				aborted= AB_RETRY ;
			}
		}
        }
						/* Cleanup After Send */
        fclose(fd);				/* Close File */

        					/* end transmission */
	for (tries= 0; tries< ERRMAX; tries++) {
        	sendbyte(EOT, 1);
		if (ackwait(DELAY, 512)==0) break ;
	}

#if 0	/* was working with 1 but otherside timeout on multiple */
        while(recvbyte(1) != TIMEOUT);
#endif

        binary(FALSE, TRUE);		/* Leave Binary Mode */
        fprintf(stderr, "\n");

	if (tries==ERRMAX) {
		fprintf(stderr, "xm: warning: EOT unacknowledged\n") ;
	}

	if (aborted) {
		abortmsg(aborted) ;
		return ; 
	}

	fprintf(stderr, "xm: sent %s; %d blocks.\n", filename, srblocks) ;

}
					/* Send Buffer to Receiver */
int putblock(filetype, buf, blocknum)
char filetype;
unsigned char *buf;
int blocknum;
{
        int i, j, checksum;

        sendbyte(SOH, 0);			/* Send Start of Header */
        sendbyte(blocknum & 0xff, 0);     	/* Send Block Number */
        sendbyte((-blocknum-1) & 0xff, 0);	/* Send Block Complement */
        checksum = 0;
        for (i= 0; i< BLOCKSZ; i++) {
                sendbyte((*buf) & 0xff, 0);		/* Send Byte */
                checksum= (checksum + *buf++) & 0xff;
        }
        sendbyte(checksum & 0xff, 1);             /* Checksum */
}

/* Receive File */
int recv(filetype, filename)
char filetype, *filename;
{
        int fh;                 /* file descriptor: handle */
        int blocknum;           /* next block to receive */
        int rbcnt;              /* total number of received blocks */
        int errorcnt;           /* number of errors on current block */
        int receiving;          /* continuation flag */
        int char1;              /* first char received in block */
        int rcode;              /* received block code */
	int aborted;		/* was reception aborted? */

        if (!access(filename,2)) {
                if (!getyn("\nxm: file already exists: Delete? ")) {
			abortmsg(AB_ARE) ;
                        return;
                }
        }

        unlink(filename);       /* delete old file, if any */
        if ((fh= creat(filename, CREATE)) == -1) {     /* can't create */
                fprintf(stderr, "\nxm: can't create %s\n", filename);
                return;
        }

        /* We Have a GO */
        fprintf(stderr, "XM Receiving %s File: %s\n",
                (filetype == 't') ? "Text" : "Binary",
                filename);
        fprintf(stderr, "Ready to Receive\n");

        				/* Init Counters et al */
        rbcnt= 0;
        errorcnt= 0;
        blocknum= 1;
        receiving= TRUE;

        				/* Establish Binary Communications */
        binary(TRUE, TRUE);
        					/* Synchronize with Sender */
        sendbyte(NAK, 1);

        					/* Receive Next Packet */
	aborted= 0 ;
        while(receiving) {

                do {
                        char1= recvbyte(DELAY);
                } while((char1!=SOH) && (char1!=EOT) && (char1!=TIMEOUT));

                switch (char1) {
                case TIMEOUT :       /* Timeout */
                        if ((++errorcnt) == ERRMAX) {
                                receiving= FALSE;
				aborted= AB_TIMEOUT ;
                        }
                        sendbyte(NAK, 1);
                        break;

                case EOT :      /* End of Transmission */
                        sendbyte(ACK, 1) ;
                        receiving= FALSE;
                        break;

                case SOH :      /* New or Old Block */
                        rcode= getblock(filetype, fh, blocknum) ;

                        switch (rcode) {

                        case 0 :        /* OK */
				blocknum++ ;
                                blocknum&= 0xff;
                                rbcnt++;
					/* pass thru */

                        case AB_DUPBLOCK:	/* OK, but Duplicate Block */
                                errorcnt= 0;
                                sendbyte(ACK, 1);
                                break;

			case AB_TIMEOUT:
			case AB_RETRY:
                        case AB_CKSUM :        /* Xmit Error, Non-Fatal */
                                if ((++errorcnt)< ERRMAX) {
                                        sendbyte(NAK, 1);
                                        break;
                                }
					/* pass thru */

                        default :       /* Xmit Error, Fatal */
                                sendbyte(CAN, 1);
                                receiving= FALSE;
				aborted= rcode ;
                                break;
                        }
                        break;
                }
        }
        close(fh);

	binary(FALSE, TRUE) ;

	fprintf(stderr, "\n") ;
	if (aborted) {
		abortmsg(aborted) ;
		return ;
	}

	fprintf(stderr, "xm: received %s; %d blocks.\n", filename, rbcnt) ;

}

					/* Get Block from Sender */
int getblock(filetype, fh, blocknum)
char filetype;
int fh, blocknum;
{
        int curblock, cmpblock;
        int recdone, checksum, inchecksum, byte, bufcnt, c;
        int startstx, endetx, endenq;
        char buff[BLOCKSZ];
        int j;

        curblock= recvbyte(DELAY);
        if (curblock == TIMEOUT) return(AB_TIMEOUT) ;

        cmpblock= recvbyte(DELAY);
        if (cmpblock == TIMEOUT) return(AB_TIMEOUT) ;

        if ((curblock + cmpblock) != 0xff) {
                while(recvbyte(DELAY) != TIMEOUT);  /* Flush */
                return(AB_RETRY) ;
        }

        checksum= 0;           /* Init Checksum */
        byte= 0;               /* Init Buff Ptr */
        recdone = FALSE;        /* File Receive NOT Done */
        for (bufcnt= 0; bufcnt< BLOCKSZ; bufcnt++) {
                c= recvbyte(DELAY);
                if (c == TIMEOUT) return(AB_TIMEOUT) ;
                buff[byte] = c;
                checksum = (checksum + c) & 0xff;
                if (filetype != 't') {
                        byte++;         /* binary xfer, so advance */
                        continue;
                }
                if (c == CR) continue;  /* skip CR */
                if (c == CTRLZ) {       /* done */
                        recdone = TRUE;
                        continue;
                }
                if (!recdone) byte++;           /* continue */
        }

        inchecksum= recvbyte(DELAY);
        if (inchecksum == TIMEOUT) return(AB_TIMEOUT);

        if (checksum != inchecksum) return(AB_CKSUM) ;

        if (curblock != blocknum) {
                if (((curblock+1) & 0xff) == blocknum) {
                	return(AB_DUPBLOCK) ;
                }
		return(AB_SYNC) ;
        }

        if (write(fh, buff, byte) != byte) {
                return(AB_IOERR) ;
        }

        return(0);

}

/*
	ackwait()		wait for ACK character

	reads characters from remote side, until
	TIMEOUT, ACK, NAK, or 'max' characters received.
	return zero only if ACK was read.
*/
static int ackwait(secs, max)
int secs, max ;
{
	int i, c ;

	for (i= 0; i< max; i++) {
		c= recvbyte(secs) ;
		if (c==ACK || c==NAK || c==TIMEOUT) break ;
	}
	return((c==NAK)? -1: (c==ACK)? 0: 1) ;	
}

/* File Status Display */
fxstat(filetype,filename)
char filetype;
char *filename;
{
        struct stat fsi;        /* file status info */
        if (stat(filename, &fsi) == -1) {      /* get file status info */
                fprintf(stderr, "File %s Not Found\n", filename);
                return;
        }
        fprintf(stderr, "File Size is %ldK, %ld Blocks\n",
        (fsi.st_size%1024) ? (fsi.st_size/1024)+1 : fsi.st_size/1024,
        (fsi.st_size%128) ? (fsi.st_size/128)+1 : fsi.st_size/128);
}

					/*  SUPPORT ROUTINES  */
					/* get yes or no from user */
int getyn(ques)
char *ques ;
{
#if 0
        int c, true ;

	fprintf(stderr, "%s", ques) ;
	fflush(stderr) ;
	binary(TRUE, FALSE) ;
        c= getchar();    /* get char */
	binary(FALSE, FALSE) ;
	fprintf(stderr, (true= (c=='y'||c=='Y'))? "Yes\n": "No\n") ;
	return(true) ;
#else
	return(FALSE) ;
#endif
}

static char sendbuf[(BLOCKSZ*2)+1] ;
static int sendlen= 0 ;
					/* send byte to receiver */
sendbyte(data, now)
char data ;
int now ;
{
	sendbuf[sendlen++]= data ;
	if (now || (sendlen>= sizeof(sendbuf))) sendflush() ;
}

sendflush()
{
	if (sendlen> 0) {
		write(1, sendbuf, sendlen) ;
		sendlen= 0 ;
	}
}

char recvbuf[(BLOCKSZ*2)+1] ;
int recvptr= 0 ;
int recvlen= 0 ;

void alarmfunc();	                /* forward declaration */

					/* receive a byte from sender */
recvbyte(seconds)
unsigned seconds;
{
        char c;

	if (recvptr< recvlen) {
		return((recvbuf[recvptr++]) & 0xff) ;
	}

        signal(SIGALRM, alarmfunc);    			/* catch alarm */
        if (setjmp(rbenv)< 0) return(TIMEOUT);		/* alarm jump  */
        alarm(seconds); 		                /* set clock */

	recvlen= read(0, recvbuf, sizeof(recvbuf)) ;
	recvptr= 0 ;

        alarm(0); 		                      /* clear clock */
	return((recvbuf[recvptr++]) & 0xff) ;

}

					/* dummy alarm function */
void alarmfunc()
{
        longjmp(rbenv, TIMEOUT);	/* jump to recvbyte */
}

static int abortmsg(why)
int why ;
{
	fprintf(stderr, "\nxm: file transfer aborted %s\n", abr[why]) ;
}

#if	OPSYS==OPUNIX

#if	OPSLVL==OPLBSD42
						/* set and clear binary mode */
#include        <sgtty.h>

binary(setflg,scope)
int setflg, scope;
{
        static struct sgttyb ttys, ttysold;
        if (setflg) {   /* set binary */
                if (gtty (0, &ttys) < 0) return(FALSE); /* failed */
                ttysold.sg_ispeed = ttys.sg_ispeed;     /* save old values */
                ttysold.sg_ospeed = ttys.sg_ospeed;
                ttysold.sg_erase = ttys.sg_erase;
                ttysold.sg_kill = ttys.sg_kill;
                ttysold.sg_flags = ttys.sg_flags;
                ttys.sg_flags |= RAW;           /* set for RAW Mode */
                ttys.sg_flags &= ~ECHO;         /* set no ECHO */
                if (scope) {            /* cover all values? */
                        ttys.sg_flags &= ~XTABS;        /* set no tab exp */
                        ttys.sg_flags &= ~LCASE;        /* set no case xlate */
                        ttys.sg_flags |= ANYP;          /* set any parity */
                        ttys.sg_flags &= ~NL3;          /* no delays on nl */
                        ttys.sg_flags &= ~TAB0;         /* no tab delays */
                        ttys.sg_flags &= ~TAB1;
                        ttys.sg_flags &= ~CR3;          /* no CR delay */
                        ttys.sg_flags &= ~FF1;          /* no FF delay */
                        ttys.sg_flags &= ~BS1;          /* no BS delay */
                }
                if (stty (0, &ttys) < 0) return(FALSE); /* failed */
#ifdef  MESG
                if (scope) system("mesg n >/dev/null"); /* turn off messages */
#endif
                return(TRUE);
        }
							/* clear binary */
        if (stty (0, &ttysold) < 0) return (FALSE);
#ifdef  MESG
        if (scope) system("mesg y >/dev/null"); /* turn on messages */
#endif
        return(TRUE);   /* OK */
}

#endif					/* UNIX BSD 4.2 */

#if OPSLVL==OPLUXV

#include	<termio.h>

static struct termio oldterm ;
void intrst() ;

binary(setflg,scope)
int setflg, scope;
{
	struct termio term ;	

        if (setflg) {					/* set binary */

		if (ioctl(0, TCGETA, (char *)&term) == (-1)) {
			return(FALSE) ; 		/* failed */
		}
							/* save old values */
		memcpy((char *)&oldterm,
			(char *)&term,
			sizeof(oldterm)) ;

		term.c_cflag&= ~(CSIZE|PARENB|CSTOPB) ;	/* no parity   */
		term.c_cflag|= (CS8|CREAD) ;		/* use 8 bits  */

		term.c_lflag&= ~(ICANON|ECHO|ISIG) ;	/* line disc.  */
		term.c_lflag&= ~(ECHOE|ECHOK|ECHONL|XCASE) ;

		term.c_iflag= (BRKINT|IGNPAR) ;		/* input disc. */
		/* term.c_iflag&= IRTS ; */		/* input disc. */
#if 0
		term.c_cc[VINTR]=  (char)0 ;		/* cntrl codes */
		term.c_cc[VQUIT]=  (char)0 ;
		term.c_cc[VERASE]= (char)0 ;
		term.c_cc[VKILL]=  (char)0 ;
		term.c_cc[VSWTCH]= (char)0 ;
#endif
		term.c_cc[VEOF]=   (char)1 ;		/* MIN # CHARS */
		term.c_cc[VEOL]=   (char)0 ;		/* TIME/.1 sec */

                if (scope) { 				/* cover all values? */
			term.c_oflag= 0;		/* no output control */
			term.c_lflag&= ~XCASE;		/* set no case xlate */
                }
		else {
			term.c_oflag&= ~(ONLCR|OCRNL|ONOCR|
					TAB3|NLDLY|CRDLY|
					TAB1|TAB2|BSDLY|
					VTDLY|FFDLY) ;
		}

		if (ioctl(0, TCSETAW, (char *)&term) == (-1)) {	   /* modify */
			return(FALSE) ;				   /* failed */
		}

		signal(SIGINT, intrst) ;

#ifdef  MESG
                if (scope) system("mesg n >/dev/null"); /* turn off messages */
#endif
                return(TRUE);
        }
							/* clear binary */
	if (ioctl(0, TCSETAW, &oldterm) == (-1)) {	/* replace stat */
		return(FALSE) ;			/* failed */
	}

#ifdef  MESG
        if (scope) system("mesg y >/dev/null"); /* turn on messages */
#endif

	signal(SIGINT, SIG_DFL) ;

        return(TRUE); 				/* OK */
}

static void intrst()
{
	ioctl(0, TCSETAW, &oldterm);		/* replace stat */
	ioctl(0, TCFLSH, 2) ;			/* flush queues */
	abortmsg(AB_USER);
	exit(1) ;
}

#endif					/* UNIX SYSTEM V */

#endif					/* UNIX SYSTEM */
