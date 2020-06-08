/*	Revision:      		9
*/

#ifndef	A21FLOG0_H_INC
#define	A21FLOG0_H_INC

/*

	a21flog0.h			qad-ARC21 A21FLOG Internal Defs
					Copyright (C) 1995, Contahal Ltd.

*/

/*
	sub-#includes
*/

#include	<stdio.h>

/*
	#defines
*/
#define	A21FLMAX_FILENAME	81	/* max. size of log file name	*/

/*
	macros
*/

#define	ha21flog2pa21flog(ha21flog)  ((PA21FLOG) &a21flog_a[(int)(ha21flog)])
#define	pa21flog2ha21flog(pa21flog)  ((HA21LOG) ((pa21flog) - &a21flog_a[0]))

#define	ISBADHA21FLOG(ha21flog)		       			\
	(	(((int) (ha21flog))< 0)				\
	     || (((int) (ha21flog))>= A21LDV_MAXA21LOGS)	)

/*
	types
*/

typedef struct a21flog_t_tag {
	M_SBOOL busy ;			/* file structure entry in use	*/
	FILE *fp ;			/* task-specific file handle	*/
	M_SULONG ult0 ;			/* initial time			*/
	int fnum ;			/* system wide "file number"	*/
	int column ;			/* current output column	*/
	int flags ;			/* log file open mode flags	*/
	int lmgn ;			/* user-set left margin		*/
	char szFileName[A21FLMAX_FILENAME+1] ;		/* log filename	*/
} a21flog_t, A21FLOG, *PA21FLOG, M_MNEAR *NPA21FLOG, M_MFAR *LPA21FLOG ;

// #ifndef A21FLOG0_H_INC
#endif

/* end of a21flog0.h */
