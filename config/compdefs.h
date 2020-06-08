/*	Revision:		2
*/

#ifndef	COMPDEFS_H_INC
#define	COMPDEFS_H_INC

/*

	compdefs.h:			Compiler Definitions
					Copyright (C) 1995, Point Of Sale Ltd.

*/

/*
	Operating systems supported:		(CD_OPSYS)
*/
#define	CD_OPSYS_UNIX		1		/* UNIX			*/
#define	CD_OPSYS_MSDOS		2		/* MS/PC-DOS		*/

/*
	UNIX Operating system flavors:		(CD_UXFLAVOR)
*/
#define	CD_UXFLAVOR_ATTV	1		/* AT&T UnixV		*/
#define	CD_UXFLAVOR_BSD		2		/* Berkeley BSD		*/
#define	CD_UXFLAVOR_AIX		3		/* IBM AIX		*/
#define	CD_UXFLAVOR_LINUX	4		/* Linux		*/

/*
	MSDOS Operating system versions:	(CD_MSDVER)
*/
#define	CD_MSDVER_21		210		/* MS-DOS 2.1		*/
#define	CD_MSDVER_31		310		/* MS-DOS 3.1		*/
#define	CD_MSDVER_50		500		/* MS-DOS 5.0		*/
#define	CD_MSDVER_60		600		/* MS-DOS 6.0		*/
#define	CD_MSDVER_70		700		/* MS-DOS 7.0		*/

/*
	Compilers supported:			(CD_COMPILER)
*/
#define	CD_COMPILER_STD		1		/* std. C compiler	*/
#define	CD_COMPILER_GNU		2		/* GNU C/C++		*/
#define	CD_COMPILER_MSC		3		/* Microsoft		*/
#define	CD_COMPILER_MSVC	4		/* MSVisual C/C++	*/

/*
	Compiler versions:			(CD_COMPILERVER)
*/
/*	just use the three-decimal digit version #, as in CD_MSDVER	*/

/*
	Byte-orderings for words:		(CD_BYTEORDER)
*/
#define	CD_BYTEORDER_HILO	0		/* e.g. motorola 680x0	*/
#define	CD_BYTEORDER_LOHI	1		/* e.g. Intel 80x86	*/

/* #ifndef COMPDEFS_H_INC */
#endif

/* end of compdefs.h */
