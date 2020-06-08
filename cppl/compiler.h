/*	Revision:	2
*/

#ifndef	COMPILER_H_INC
#define	COMPILER_H_INC

/*

	compiler.uvx:		qad-NewARC21 Compiler Environment: Unix V/X
				Copyright (C) 1991-1995, Contahal Ltd.

	Macros:
		OPSYS		"generic name" of operating system
		OPSLVL		"minumum level" of operating system
		UINTFC		User-Interface
		COMPILER	specific compiler identification
		MACHINE		Specific machine or architechture
		BYTEORDER	Byte ordering within words

	(user must set all macros to configure new compilation environment)

*/

/*
	#include compiler definitions
*/
#include	"compdefs.h"

/*
	Definitions in use for this product:
*/
#define		OPSYS		OPSYS_UNIX
#define		OPSLVL		OPSLVL_SYSV
#define		UINTFC		UINTFC_XW
#define		COMPILER	COMPILER_UXV
#define		MACHINE		MACHINE_NCR3000
#define		BYTEORDER	BYTEORDER_LOHI

/*
	end of definitions
*/

/* #ifndef COMPILER_H_INC */
#endif

/* end of compiler.uvx */
