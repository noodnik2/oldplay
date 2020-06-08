//	Revision:      		3

#ifndef	COMPDEFS_H_INC
#define	COMPDEFS_H_INC

/*

	compdefs.h:		qad-ARC21 Compiler Setup: Definitions
				Copyright (C) 1991-1995, Contahal Ltd.

	Macro sets defined:

		OPSYS_*		"generic name" of operating system
		OPSLVL_*	"minumum level" of operating system
		UINTFC_* 	User-Interface
		COMPILER_* 	specific compiler identification
		MACHINE_*   	Specific machine or architechture
		BYTEORDER_*   	Byte ordering within words


	NOTE:	all definitions preserved;
		obsolete definitions are commented out.

*/

/*
	software platform: operating systems/environments	(OPSYS_*)
*/
#define		OPSYS_UNIX	1		/* UNIX		*/
#define		OPSYS_MSDOS	2		/* MS-DOS	*/
#define		OPSYS_MSWIN	3		/* MS-Windows	*/

/*
	operating system levels: OPSYS==OPSYS_UNIX		(OPSLVL_*)
*/
#define		OPSLVL_SYSV	101		/* AT&T UnixV	*/
//#define	OPSLVL_BSD	102		/* Berkeley	*/
//#define	OPSLVL_XENIX	103		/* Xenix	*/
//#define	OPSLVL_SUNOS	104		/* Sun OS	*/

/*
	operating system levels:	OPSYS==OPSYS_MSDOS	(OPSLVL_*)
*/
//#define	OPSLVL_MSD21	201		/* MS-DOS 2.1	*/
//#define	OPSLVL_MSD31	202		/* MS-DOS 3.1	*/
//#define	OPSLVL_MSD41	203		/* MS-DOS 4.1	*/
//#define	OPSLVL_MSD51	204		/* MS-DOS 5.1	*/
//#define	OPSLVL_MSD60	205		/* MS-DOS 6.0	*/
//#define	OPSLVL_MSD62	206		/* MS-DOS 6.2	*/

/*
	operating system levels:	OPSYS==OPSYS_MSWIN	(OPSLVL_*)
*/
//#define	OPSLVL_MSW30	301		/* MS-WIN 3.0	*/
//#define	OPSLVL_MSW31	302		/* MS-WIN 3.1	*/
//#define	OPSLVL_MSW311	303		/* MS-WIN 3.11 (WFWG) */

/*
	software platforms: user-interfaces			(UINTFC_*)
*/
//#define	UINTFC_MSDTTY	1		/* MS-DOS TTY	*/
//#define	UINTFC_UNIXTTY	2		/* Unix	TTY	*/
#define		UINTFC_MSW 	3		/* MS-Windows	*/
#define		UINTFC_XW  	4		/* X Window	*/

/*
	software platforms:	specific compilers		(COMPILER_*)
*/
//#define	COMPILER_MS4	1		/* microsoft C 5.xx	    */
//#define	COMPILER_MS5	2		/* microsoft C 5.xx	    */
//#define	COMPILER_MS6	3		/* microsoft C 6.xx	    */
//#define	COMPILER_MS7	4		/* microsoft C 7.xx	    */
#define		COMPILER_MS8	5		/* microsoft C 8.xx	    */
#define		COMPILER_UXV	6		/* standard Unix V compiler */
//#define	COMPILER_BSD	7		/* standard BSD4.x compiler */
//#define	COMPILER_XENIX	8		/* standard Xenix compiler  */
//#define	COMPILER_SUNOS	9		/* sun-OS compiler	    */

/*
	hardware platforms: machines/architechtures		(MACHINE_*)
*/
#define		MACHINE_NCR3000	1		/* NCR 3000	*/
//#define		MACHINE_SUN360	2		/* SUN 3/60	*/
//#define		MACHINE_HP9000	3		/* HP 9000	*/
//#define		MACHINE_AVIION	4		/* DG AVIION	*/
//#define		MACHINE_NCRTWR	5		/* NCR TOWER	*/
#define		MACHINE_IBMPC	6		/* IBM PC	*/
//#define		MACHINE_SPARC	7		/* Sun SparcStn	*/

/*
	hardware platforms:	byte/word ordering		(BYTEORDER_*)
*/
#define		BYTEORDER_HILO	0
#define		BYTEORDER_LOHI	1

/*
	end of definitions
*/

// #ifndef COMPDEFS_H_INC
#endif

// end of compdefs.h
