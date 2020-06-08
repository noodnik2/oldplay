/*

	docmd.c			Perform system command while in flist
	       			Copyright (C) 1990-1992, by Marty Ross


*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"flist.h"

extern struct fl_arg fla ;
extern struct fl_scn fls ;
extern struct fl_opt flo ;

int execute(flp)
struct fl_line *flp ;
{
	int rc, nap ;		/* nap: "no append" flag */
	char *cmd, *ecmd;

	rc= 0;
	cmd= fl_gcmd(flp);
	ecmd= cmd ;

	if (cmd!=NULL) {
		if (nap= ((cmd[0]=='\\') && (cmd[1]=='\\'))) cmd+= 2;
		if (e_iscmd(cmd)) {
			rc= e_docmd(cmd, ecmd, flp, !nap, 2);
			setupscn(TOPLINE) ;
			if (flo.o_redraw) redraw(-1);
		}
	}
	return(rc) ;
}

int docmd(cmd, flp, ap, disp)
char *cmd;
struct fl_line *flp ;
int ap, disp;
{
	int rc, len, ns ;
	char cmdbuf[CMDSIZE+1] ;

	fls.tl[1][0]= '\0';		/* kill the error message */
	if ((len= sparse(cmd, cmdbuf, flp->line, &ns))< 0) return(-6);
	cmdbuf[len]= '\0' ;
	if (ap!=0 && ns==0) {		/* if no arguments were substituted */
		strcat(cmdbuf, " ");		/* tack on the screen file  */
		strcat(cmdbuf, flp->line);
	}

	if (disp!=0) clear();

	if (disp!=3 && flo.o_cmddisp) {
		addstr(cmdbuf);
		addch('\n');
	}

	updscn(); scnmode(0); rc= system(cmdbuf); scnmode(1);

	if (disp>0) {
		if ((disp==1 && flo.o_more) || (rc!=0)) domore() ;
	}
	if (rc!=0) flserr(rc, cmdbuf, 0);

	return(rc);
}

int e_iscmd(cmd)
char *cmd;
{
	while(cmd[0]==' ' || cmd[0]=='\t') cmd++;
	if (cmd[0]=='*' || cmd[0]=='^') return(0);
	return(1);
}

int e_docmd(cmd,ecmd,flp,ad,mf)
char *cmd, *ecmd ;
struct fl_line *flp ;
int ad, mf;
{
	int rc;

	if (!e_iscmd(cmd)) return(-1);
	rc= docmd(cmd, flp, ad, mf) ;
	if (ecmd!=NULL) ecmd[0]= (char) (rc==0)? '*': '^' ;
	return(rc);
}

/*
	enter a new level of flist
	on the specified directory.
*/
int e_enter(cmd, ecmd, flp, ad, mf)
char *cmd, *ecmd ;
struct fl_line *flp ;
int ad, mf;
{
	char icmd[CMDSIZE+1] ;

	sprintf(icmd, "flist %cT \"(nested)\" %s %cd %s %s",
		OPTPFX_CH,
		OPTSTR,
		OPTPFX_CH,
		flp->line,
		cmd
	) ;
	return(e_docmd(icmd, ecmd, flp, ad, mf)) ;
}
