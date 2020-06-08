/*	Revision:      		3
*/

#ifndef	A21LOGT_H_INC
#define	A21LOGT_H_INC

/*

	a21logt.h		qad-ARC21 A21LOG.DLL Utility Header: Types
				Copyright (C) 1994-1995, Contahal Ltd.

*/

#include	"musrdef.h"

/*
	system-wide log file numbers	(#defined for external use only)
*/
#define	A21LFID_XYB 	0	/* all A21xyB modules use this		*/
#define	A21LFID_GUI 	1	/* all A21 GUI interface modules	*/
#define	A21LFID_APP 	2	/* all A21 application modules  	*/
#define	A21LFID_REP 	3	/* all A21 repository mgment modules	*/
#define	A21LFID_USER	0x1000	/* user-defined modules start here	*/

/*
	defined values
*/

#define	A21LDV_MAXA21LOGS	8	/* max. # open files   		*/
#define	A21LDV_MAXMODNAMELEN	256	/* max. size of module name	*/

#define	A21LDV_OBUFSIZE		512	/* output blocking size		*/
#define	A21LDV_PRINTFBUFSIZE	1024	/* max. size of printf 		*/
#define	A21LDV_PFXLEN		32	/* max. size of prefix 		*/
#define	A21LDV_TSLEN		15	/* max. size of time stamp	*/

#define	A21LDV_MAXFOATTEMPTS	8	/* max. # open attempts		*/
#define	A21LDV_FNSIZE		128	/* filename max. size  		*/
#define	A21LDV_LFNSIZE		128	/* filename local max. size  	*/

#define	A21LF_NONE		0	/* use default behaviour	*/
#define	A21LF_APPEND		1	/* Open file in append mode	*/
#define	A21LF_REOPEN		2	/* Close/Reopen file each line	*/
#define	A21LF_TSTAMP		4	/* Timestamp each log line?	*/
#define	A21LF_NM_TITLE		8	/* Suppress open/close titles	*/
#define	A21LF_FLUSH		16	/* Flush file each line		*/

/*
	types
*/

typedef int HA21LOG ;
#define	HA21LOG_ERROR		((HA21LOG) (-1))

typedef struct a21lmid_t_tag {
	int fnum ;
	char prefix[A21LDV_PFXLEN+1] ;
} a21lmid_t, A21LMID, *PA21LMID, M_MNEAR *NPA21LMID, M_MFAR *LPA21LMID ;

/* #ifndef A21LOGT_H_INC */
#endif

/* end of a21logt.h */
