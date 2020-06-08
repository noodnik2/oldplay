/*	Revision:		1
*/

#ifndef	BORDER_H_INC
#define	BORDER_H_INC

/*

	border.h		Byte-order conversions
				Copyright (C) 1996, Pointofsale Ltd.

*/


/*
	sub-#includes
*/
#include	"compiler.h"

/*
	#defines
*/

#if	CD_BYTEORDER == CD_BYTEORDER_HILO
/*
	machine architecture is HILO	(e.g. Motorola)
*/
#define	BORDER_NATIVE2HILOSHORT(ps)		/*nothing to do*/
#define	BORDER_NATIVE2HILOLONG(pl)		/*nothing to do*/
#define	BORDER_NATIVE2LOHISHORT(ps)	border_flips((short *) ps)
#define	BORDER_NATIVE2LOHILONG(pl)	border_flipl((long *) pl)
#define	BORDER_HILO2NATIVESHORT(ps)		/*nothing to do*/
#define	BORDER_HILO2NATIVELONG(pl)		/*nothing to do*/
#define	BORDER_LOHI2NATIVESHORT(ps)	border_flips((short *) ps)
#define	BORDER_LOHI2NATIVELONG(pl)	border_flipl((long *) pl)
#endif

#if	CD_BYTEORDER == CD_BYTEORDER_LOHI
/*
	machine architecture is LOHI	(e.g. Intel)
*/
#define	BORDER_NATIVE2HILOSHORT(ps)	border_flips((short *) ps)
#define	BORDER_NATIVE2HILOLONG(pl)	border_flipl((long *) pl)
#define	BORDER_NATIVE2LOHISHORT(ps)		/*nothing to do*/
#define	BORDER_NATIVE2LOHILONG(pl)		/*nothing to do*/
#define	BORDER_HILO2NATIVESHORT(ps)	border_flips((short *) ps)
#define	BORDER_HILO2NATIVELONG(pl)	border_flipl((long *) pl)
#define	BORDER_LOHI2NATIVESHORT(ps)		/*nothing to do*/
#define	BORDER_LOHI2NATIVELONG(pl)		/*nothing to do*/
#endif


/*
	API function templates
*/

void border_flips(short *) ;
void border_flipl(long *) ;

/* #ifndef BORDER_H_INC */
#endif

/* end of border.h */
