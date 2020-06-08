/*	Revision:      		4
*/

#ifndef	COMPILER_H_INC
#define	COMPILER_H_INC

/*

	compiler.dos:		Compiler Environment: MS-DOS

	Macros:
		CD_OPSYS	"generic name" of operating system
		CD_OPSLVL	"minumum level" of operating system
		CD_UI		User-Interface
		CD_COMPILER	specific compiler identification
		CD_MACHINE	Specific machine or architechture
		CD_BYTEORDER	Byte ordering within words

	(user must set all macros to configure new compilation environment)

*/

/*
	#include compiler definitions
*/
#include	"compdefs.h"

/*
	Definitions in use for this product:
*/
#define		CD_OPSYS       		CDOPSYS_MSDOS
#define		CD_OPSLVL      		CDOPSLVL_MSD51
#define		CD_UI	       		CDUI_MSDTTY
#define		CD_COMPILER    		CDCOMPILER_MS8
#define		CD_MACHINE     		CDMACHINE_IBMPC
#define		CD_BYTEORDER      	CDBYTEORDER_LOHI

/*
	end of definitions
*/

/* #ifndef COMPILER_H_INC */
#endif

/* end of compiler.msd */
