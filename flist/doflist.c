/*

	doflist				Perform the flist function
					Copyright (C) 1990, by ==C=R=C==


*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"flist.h"

extern struct fl_arg fla ;
extern struct fl_scn fls ;

static void wrapup() ;
static int process();
void absmove();
void useline(), saveline(), wrapup() ;
char *fl_gcmd() ;

void doflist()
{
	int i, c ;
	char cmdbuf[CMDSIZE+1] ;

	CY= CX= RX= 0;
	memset(cmdbuf, '\0', CMDSIZE) ;
	regmove(100, CY, CX);

	setupscn(0);
	redraw(-1);
	updscn();

	while((c= inmackey()) != EOF) {

		switch(c) {

			case 27:
				cmdbuf[0]= '\0' ;
				saveline(cmdbuf);
				useline(cmdbuf);
				updscn();
				break;

			case A_CTL('A'):
				saveline(cmdbuf);
				regmove(0, 1, 0);
				clrtoeol();
				addstr("$ "); /* CX= 2; */
				if (getcmd(cmdbuf)> 0) {
					if (cmdbuf[0]=='\003') return ;
					docmd(cmdbuf,CURLINE,0,1);
					setupscn(TOPLINE) ;
					redraw(-1);
				}
				useline(cmdbuf) ;
				updscn() ;
				break;

			case A_CTL('I'):		/* hack */
				if (RX==0 || RX==CX) {
					beep();
					break;
				}
				cmdbuf[CX]= '\0' ;
				RX= CX ;
				clrtoeol() ;
				updscn();
				break;

			case A_CTL('F'):
				if (RX==0 || RX==CX) {
					beep();
					break;
				}
				for (i= CX; i< RX; i++) {
					cmdbuf[i]= cmdbuf[i+1] ;
				}
				RX--;
				delch();
				updscn();
				break;

			case A_CTL('R'):
				flserr(0, NULL, 0);
				saveline(cmdbuf);
				redraw(-1) ;
				useline(cmdbuf) ;
				updscn() ;
				break;

			case A_CTL('X'):
				flserr(0, NULL, 1);
				saveline(cmdbuf);
				execute(CURFLP);
				cmdbuf[0]= '\0' ;
				saveline(cmdbuf);
				useline(cmdbuf);
				updscn();
				break;

			case '\r':
				if (process(cmdbuf)) {
					wrapup();
					return;
				}
				break;

			case '\n':
				saveline(cmdbuf);
				if ((CY+1)>= NUMFILE) CY= 0;
				else CY++;
				regmove(100, CY, CX);
				useline(cmdbuf);
				updscn();
				break;

			case '\b':
				if (CX==0) { beep(); break; }
				regmove(100, CY, --CX);
				updscn();
				break;

			case A_CTL('K'):
				saveline(cmdbuf);
				if ((CY-1)< 0) CY= NUMFILE;
				regmove(100, --CY, CX);
				useline(cmdbuf);
				updscn();
				break;

			case A_CTL('L'):
				if (CX< RX) {
					regmove(100, CY, ++CX) ;
					updscn();
					break;
				}
				beep();
				break;

			case A_CTL('U'):
				if (TOPLINE> 0) {
					flserr(0, NULL, 0); 
					saveline(cmdbuf);
					if (TOPLINE> F_LINES) {
						CX= CY= 0;
						setupscn(TOPLINE-F_LINES);
						redraw(-1);
					}
					else if (TOPLINE> 0) {
						CX= CY= 0;
						setupscn(0);
						redraw(-1);
					}
					useline(cmdbuf);
					updscn();
					break;
				}
				beep() ;
				flserr(0, "TOP:", 2);
				break;

			case A_CTL('D'):
				if (NXTPAGE< TOTFILE) {
					flserr(0, NULL, 0);
					saveline(cmdbuf);
					CX= CY= 0;
					setupscn(NXTPAGE);
					redraw(-1);
					useline(cmdbuf);
					updscn();
					break;
				}
				beep();
				flserr(0, "BOT:", 2);
				break;

			case A_CTL('C'):	
				wrapup();
				return;		
				break;

			default:
				if ((CX+1)>= R_MGN) {
					beep();
					break;
				}
				cmdbuf[CX++]= (char)c ;
				if (CX> RX) RX= CX ;
				addch(c);
				updscn();
				break;
		}
	}
}

static int process(cmdbuf)
char *cmdbuf ;
{
	int rc ;

	flserr(0, NULL, 1);
	saveline(cmdbuf);
	rc= procscn();
	useline(cmdbuf);
	updscn() ;
	return(rc) ;
}

static void wrapup()
{
	absmove(S_LENGTH-1, 0);
	clrtoeol() ;
	updscn();
}
