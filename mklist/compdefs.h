/*	Revision:      		4
*/

#ifndef	COMPDEFS_H_INC
#define	COMPDEFS_H_INC

/*

	compdefs.h:		Compiler Setup: Definitions

	Macro sets defined:

		CDOPSYS_*	"generic name" of operating system
		CDOPSLVL_*	"minumum level" of operating system
		CDUI_* 		User-Interface
		CDCOMPILER_* 	specific compiler identification
		CDMACHINE_*   	Specific machine or architechture
		CDBYTEORDER_*  	Byte ordering within words

*/

/*
	software platform: operating systems/environments	(CDOPSYS_*)
*/
#define		CDOPSYS_UNIX		1		/* UNIX		*/
#define		CDOPSYS_MSDOS  		2		/* MS-DOS	*/
#define		CDOPSYS_MSWIN  		3		/* MS-Windows	*/

/*
	operating system levels: CDOPSYS==CDOPSYS_UNIX		(CDOPSLVL_*)
*/
#define		CDOPSLVL_SYSV  		101		/* AT&T UnixV	*/
#define		CDOPSLVL_BSD   		102		/* Berkeley	*/
#define		CDOPSLVL_XENIX 		103		/* Xenix	*/
#define		CDOPSLVL_SUNOS 		104		/* Sun OS	*/

/*
	operating system levels:	CDOPSYS==CDOPSYS_MSDOS	(CDOPSLVL_*)
*/
#define		CDOPSLVL_MSD21 		201		/* MS-DOS 2.1	*/
#define		CDOPSLVL_MSD31 		202		/* MS-DOS 3.1	*/
#define		CDOPSLVL_MSD41 		203		/* MS-DOS 4.1	*/
#define		CDOPSLVL_MSD51 		204		/* MS-DOS 5.1	*/
#define		CDOPSLVL_MSD60 		205		/* MS-DOS 6.0	*/
#define		CDOPSLVL_MSD62 		206		/* MS-DOS 6.2	*/

/*
	operating system levels:	CDOPSYS==CDOPSYS_MSWIN	(CDOPSLVL_*)
*/
#define		CDOPSLVL_MSW30 		301		/* MS-WIN 3.0	*/
#define		CDOPSLVL_MSW31 		302		/* MS-WIN 3.1	*/
#define		CDOPSLVL_MSW311		303		/* MS-WIN 3.11 (WFWG) */

/*
	software platforms: user-interfaces			(CDUI_*)
*/
#define		CDUI_MSDTTY    		1		/* MS-DOS TTY	*/
#define		CDUI_UNIXTTY   		2		/* Unix	TTY	*/
#define		CDUI_MSW       		3		/* MS-Windows	*/
#define		CDUI_XW        		4		/* X Window	*/

/*
	software platforms:	specific compilers		(CDCOMPILER_*)
*/
#define		CDCOMPILER_MS4		1	/* microsoft C 5.xx	    */
#define		CDCOMPILER_MS5 		2	/* microsoft C 5.xx	    */
#define		CDCOMPILER_MS6 		3	/* microsoft C 6.xx	    */
#define		CDCOMPILER_MS7 		4	/* microsoft C 7.xx	    */
#define		CDCOMPILER_MS8 		5	/* microsoft C 8.xx	    */
#define		CDCOMPILER_UXV 		6	/* standard Unix V compiler */
#define		CDCOMPILER_BSD 		7	/* standard BSD4.x compiler */
#define		CDCOMPILER_XENIX	8	/* standard Xenix compiler  */
#define		CDCOMPILER_SUNOS	9	/* sun-OS compiler	    */

/*
	hardware platforms: machines/architechtures		(CDMACHINE_*)
*/
#define		CDMACHINE_NCR3000	1      		/* NCR 3000	*/
#define		CDMACHINE_SUN360	2      		/* SUN 3/60	*/
#define		CDMACHINE_HP9000	3      		/* HP 9000	*/
#define		CDMACHINE_AVIION	4      		/* DG AVIION	*/
#define		CDMACHINE_NCRTWR	5      		/* NCR TOWER	*/
#define		CDMACHINE_IBMPC		6      		/* IBM PC	*/
#define		CDMACHINE_SPARC		7      		/* Sun SparcStn	*/
					 
/*
	hardware platforms:	byte/word ordering		(CDBYTEORDER_*)
*/
#define		CDBYTEORDER_HILO	0
#define		CDBYTEORDER_LOHI	1

/*
	end of definitions
*/

/* #ifndef COMPDEFS_H_INC */
#endif

/* end of compdefs.h */
