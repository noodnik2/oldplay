/*

	procscn.c		Process screen commands
		 		Copyright (C) 1990-1992, by Marty Ross


*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"flist.h"

extern struct fl_arg fla ;
extern struct fl_opt flo ;
extern struct fl_scn fls ;

struct fl_line *fl_lofs() ;

static char *strim() ;
static int find_str();

int (*s_name()), (*s_size()), (*s_date()), (*s_ext()) ;

static struct fl_cmd fl_cfc= { "=" } ;

int procscn()
{
	int i, j, l, nj, rc, rv, cs; /* nj: "number of jobs successful" */
	struct fl_line *flp ;
	char msgbuf[S_WIDTH+1] ;
	char *sarg, *ecmd, *cmd ;

	cs= -1 ;			/* so clears screen on first docmd */

					/* go across all file lines on scn */
	for (
		flp= fl_lofs(TOPLINE), j= TOPLINE, i= nj= 0;
		(flp != NULL) && (i< NUMFILE);
		i++, j++, flp= flp->next
	) {

					/* if command exists for file, */
		if (flp->cmd != NULL) {

			/* left and right- trim the command line */

			cmd= strim((flp->cmd)->cmdline);

			if (cmd[0]!='&') ecmd= cmd ;	
			else {
				cmd= strim(cmd+1) ;
				ecmd= NULL;
			}

			if (strcmp(cmd, "=")==0) {	/* use current = */
				strcpy(cmd, fl_cfc.cmdline);
				fl_scmd(flp, cmd);
			}
			else if (cmd[0]=='/') {		/* queue '/...'  */
				nj+= (queueslc(cmd, ecmd, flp, j)>=0) ;
				continue ;
			}
			else if (cmd[0]=='\\' && cmd[1]=='\\') { /* cmd */
				if ((e_docmd(cmd+2, ecmd, flp, 0, cs))!=-1) {
					cs= 0;
					nj++;
				}
				continue ;
			}
			if (e_iscmd(cmd)) {
				strcpy(fl_cfc.cmdline, cmd);     /* prpgt = */
				if (e_docmd(cmd, ecmd, flp, 1, cs)!=-1) {
					cs= 0;
					nj++;
				}
			}
		}	

	}
			
				/* if there were jobs run, pause */
	if (cs==0) {
		if (flo.o_more) domore() ;
	}

				/* if there were "things" to do */
	if (nj> 0) {

				/* do any pending line commands */
		while(unqueueslc(&cmd, &ecmd, &flp, &j) != -1) {

			rc= 0 ;
			msgbuf[0]= '\0' ; 
			sarg= strim(cmd+2) ;

			switch(cmd[1]) {

				case ' ':
				case '\0':
					TOPLINE= j ;
					CY= CX= 0 ;
					break ;

				case 'b':			/* bot */
					l= (TOTFILE - F_LINES) ;
					if (l< 0) {
						beep() ;
						rc= -1 ;
						break ;
					}
					if (CURLINE>= l) {
						beep() ;
						rc= -2 ;
						break ;
					}
					TOPLINE= l ;
					CY= 0; CX= 0 ;
					break ;

				case 'f':			/* find */
					sprintf(msgbuf, "find: %s", sarg) ;
					rc= find_str(sarg) ;
					break ;

				case 't':			/* top */
					TOPLINE= 0 ;
					CY= CX= 0 ;
					break ;

				case 'q':			/* quit */
					return(1) ;
					break ;

				case 'r':			/* redraw */
					break ;

				case 'e':			/* enter */
					rc= e_enter(sarg, NULL, flp, 0, 3) ;
					break ;

				case 'o':
					rc= e_docmd(sarg,NULL,flp,0,-1) ;
					break ;

				case 'h':			/* help */
					clear(); updscn();
					scnmode(0) ;
					syntax() ;	/* domore was here */
					scnmode(1) ;
					domore() ;
					break;

				case 's':

					strcpy(msgbuf, "sort order is: ");
	
					if (cmd[3]=='r') rv= 1;	/* reverse */
					else if (	cmd[3]==' ' ||
							cmd[3]=='\0') {
						rv= 0;
					}
					else {
						rc= -3 ;
						break;
					}

					switch(cmd[2]) {

						case 'd':	/* date */
							TOPLINE= 0;
							CX= CY= 0;
							sortby(s_date, rv);
							strcat(msgbuf, "date");
							break;

						case 't':	/* type */
						case 'e':	/* .ext */
							TOPLINE= 0;
							CX= CY= 0;
							sortby(s_ext, rv);
							strcat(msgbuf, "type");
							break;

						case 'n':	/* name */
						case 'p':	/* path */
						case 'f':	/* file */
							TOPLINE= 0;
							CX= CY= 0;
							sortby(s_name, rv);
							strcat(msgbuf, "file");
							break;

						case 'b':	/* blks */
						case 's':	/* size */
							TOPLINE= 0;
							CX= CY= 0;
							sortby(s_size, rv);
							strcat(msgbuf, "size");
							break;

						default:
							rc= -2;
							break;
					}

					if (rc==0) {
						if (rv) {
							strcat(
								msgbuf,
								", reverse"
							);
							}
					}
					break;

				default:
					if (sscanf(cmd+1, "%d", &i)==1) {
						if (i< 1 || (i>TOTFILE)) {
							rc= -5 ;
							break ;
						}
						TOPLINE= (i-1);
						CY= CX= 0;
						break;
					}
					rc= -1;
					break;

			}

			if (rc != 0) {
				flserr(rc, cmd, 0);
				if (ecmd!=NULL) ecmd[0]= '^';
			}
			else {
				flserr(0, msgbuf, 0);
				if (ecmd!=NULL) ecmd[0]= '*' ;
			}

		}
				/* setup the (possibly new) screen */
		setupscn(TOPLINE) ;
		if (flo.o_redraw) {
			redraw(-1);	/* redraw, if nec. */
			updscn();
		}
	}
	return(0);
}

static char *strim(cmd)
char *cmd;
{
	register int i;

	while(cmd[0]==' ') cmd++;
	for (i= strlen(cmd); i> 0; i--) {
		if (cmd[i-1]!=' ') break;
	}
	cmd[i]= '\0';
	return(cmd);
}

static int find_str(s)
char *s;
{
	register struct fl_line *flp ;
	char *p ;
	int i ;

	i= CURLINE; flp= CURFLP ;
	for (i++, flp= flp-> next; flp != NULL; i++, flp= flp->next) {
		if ((p= flp->line) != NULL) {
			if (index(p, s)>= 0) {
				TOPLINE= i ;
				CY= 0; CX= 0;
				return(0);
			}
		}
	}
	return(-1);
}
		
static int index(haystack, needle)
char *haystack, *needle ;
{
	int i, l, h, w ;

	l= strlen(needle) ;
	h= strlen(haystack) ;
	w= (h-l+1) ;
	for (i= 0; i< w; i++) {
		if (memcmp(haystack+i, needle, l)==0) return(i) ;
	}
	return(-1);
}
