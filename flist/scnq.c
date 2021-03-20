/*

	scnq.c				Queue Slash commands from screen
					Copyright (C) 1990, by ==C=R=C==


*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"flist.h"

extern struct fl_arg fla ;

static int adv_q();

int queueslc(cmd, ecmd, flp, j)
char *cmd, *ecmd;
struct fl_line *flp;
int j;
{
	struct fq_queue *fqqp ;
	struct fq_line *fqlp ;
	int ni ;

	fqqp= &SCQFQQ ;
	ni= adv_q(fqqp->tp) ;
	if (ni == (fqqp->hp)) {
		flserr(-1, "slc q overflow", 2);
		return(-1);
	}

	fqlp= &SCQFQL[fqqp->tp] ;
	fqlp->flp= flp ;
	fqlp->jval= j ;
	fqlp->line= cmd ;
	fqlp->ecmd= ecmd ;
	(fqqp->tp)= ni ;

	return(0);	
}

int unqueueslc(cmdp, ecmdp, flpp, jp)
char **cmdp, **ecmdp ;
struct fl_line **flpp ;
int *jp  ;
{
	struct fq_queue *fqqp ;
	struct fq_line *fqlp ;

	fqqp= &SCQFQQ ;
	fqlp= &SCQFQL[fqqp->hp] ;

	if ((fqqp->hp) != (fqqp->tp)) {
		(*cmdp)= fqlp->line ;		/* return values */
		(*ecmdp)= fqlp->ecmd ;
		(*flpp)= fqlp->flp ;
		(*jp)= fqlp->jval ;
		(fqqp->hp)= adv_q(fqqp->hp);
		return(0);			/* indicate ok */
	}

	return(-1);
}

static int adv_q(i)
int i;
{
	if ((++i)> F_LINES) return(0);
	return(i);
}

