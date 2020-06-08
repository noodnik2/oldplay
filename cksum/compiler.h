/*
	compiler.h

	Currently understood are

		COMPILER==PCMicrosoft   CVERSION==400
					CVERSION==510

		COMPILER==SUNC

		COMPILER==IX386

		COMPILER==SUN386i

		COMPILER==HP9000

		COMPILER==NCRTOWER



	In addition to COMPILER and CVERSION, this header file defines:

		GenericC        exists if meets Generic C Compiler def
		NULL            definition of NULL pointer
		EFMTBAD         exists if %e format broken in compiler

		OPSYS           Name of Operating System
		OPSYSLVL        Level of Operating System


		SZptr		Size of a pointer (2 or 4)
		SZshort         Size of a short (2)
		SZint           Size of an int (2 or 4)
		SZlong          Size of a long  (4)
		SZfloat         Size of a float (4)
		SZdouble        Size of a double (8)

		BYTEORDER       HILO or LOHI

		chau            type of unsigned character
		shint           type of a 2-byte int
		int4		type of a 4-byte int

		sint		"native" signed integer
		uint4		four byte unsigned integer 
		sint4		four byte signed integer
		uint2		two byte unsigned integer
		sint2		two byte signed integer

	This header file may be included multiple times.

	This file to be included AFTER all compiler-manufacturer supplied
	header files.

*/


#ifndef COMPILER

/* -------------------------------------------------------------------- */
#define PCMicrosoft     1
#define SUNC            3
#define IX386           4
#define SUN386i		5
#define HP9000		6
#define NCRTOWER	7
#define	NCRNST		4

#define COMPILER        NCRNST
/* -------------------------------------------------------------------- */

#define OPMSDOS         1
#define OPUNIX          2

#define OPLBSD42        1
#define OPLSYSV         2

#define HILO            1
#define LOHI            2


/* -------------------------------------------------------------------- */

				/* Begin Microsoft C      for PC        */
#if COMPILER==PCMicrosoft
				/* Runs on MSDOS and PCDOS              */
#define OPSYS OPMSDOS
				/* Meets Generic C requirements         */
#define GenericC

				/* Type of unsigned char and integers   */
#define chau char
#define shint short int
#define	int4 long
#define sint int
#define	sint2 int
#define uint2 unsigned
#define sint4 long
#define uint4 unsigned long

				/* %e format broken                     */
#define EFMTBAD
				/* Byte order is LOHI                   */
#define BYTEORDER LOHI

				/* define NULL pointer                  */
#ifndef NULL
#define NULL    0
#endif
				/* define sizes                         */
#define SZptr   2
#define SZshort 2
#define SZint   2
#define SZlong  4
#define SZfloat 4
#define SZdouble 8
#endif
				/* End Microsoft C      for PC          */
/* -------------------------------------------------------------------- */

				/* Begin SUNC Compiler for SUN 3/50     */
#if COMPILER==SUNC
				/* Runs on UNIX 4.2 BSD                 */
#define OPSYS OPUNIX
#define OPSYSLVL OPLBSD42
				/* Meets Generic C requirements         */
#define GenericC

				/* Byte order is HILO                   */
#define BYTEORDER HILO

#define chau unsigned char      /* Type of unsigned char & integers     */
#define shint short int
#define int4 int
#define sint int
#define sint4 int
#define uint4 unsigned
#define sint2 short int
#define uint2 short unsigned

				/* %e format broken                     */
#define EFMTBAD
				/* define NULL pointer                  */
#ifndef NULL
#define NULL    0
#endif
				/* define sizes                         */
#define SZptr   4
#define SZshort 2
#define SZint   4
#define SZlong  4
#define SZfloat 4
#define SZdouble 8
#endif
				/* End SUNC compiler for SUN 3/50       */
/* -------------------------------------------------------------------- */
				/* Begin HPUX Compiler for HP 9000      */
#if COMPILER==HP9000
				/* Runs on UNIX AT&T V                  */
#define OPSYS OPUNIX
#define OPSYSLVL OPLSYSV
				/* Meets Generic C requirements         */
#define GenericC

				/* Byte order is HILO                   */
#define BYTEORDER HILO

#define chau unsigned char      /* Type of unsigned char & integers     */
#define shint short int
#define int4 int
#define sint int
#define sint4 int
#define uint4 unsigned
#define sint2 short int
#define uint2 short unsigned

#define EFMTBAD
				/* define NULL pointer                  */
#ifndef NULL
#define NULL    0
#endif
				/* define sizes                         */
#define SZptr   4
#define SZshort 2
#define SZint   4
#define SZlong  4
#define SZfloat 4
#define SZdouble 8
#endif
				/* End HPUX compiler for HP 9000        */
/* -------------------------------------------------------------------- */
				/* Begin NCRTOWER compiler for NCR      */
#if COMPILER==NCRTOWER
				/* Runs on UNIX AT&T V                  */
#define OPSYS OPUNIX
#define OPSYSLVL OPLSYSV
				/* Meets Generic C requirements         */
#define GenericC

				/* Byte order is HILO                   */
#define BYTEORDER HILO

#define chau unsigned char      /* Type of unsigned char & integers     */
#define shint short int
#define int4 int
#define sint int
#define sint4 int
#define uint4 unsigned
#define sint2 short int
#define uint2 short unsigned

#define EFMTBAD
				/* define NULL pointer                  */
#ifndef NULL
#define NULL    0
#endif
				/* define sizes                         */
#define SZptr   4
#define SZshort 2
#define SZint   4
#define SZlong  4
#define SZfloat 4
#define SZdouble 8
#endif
				/* End NCRTOWER compiler for NCR TOWER  */
/* -------------------------------------------------------------------- */
				/* Begin 386/ix compiler                */
#if COMPILER==IX386
				/* Runs on UNIX SYSTEM V                */
#define OPSYS OPUNIX
#define OPSYSLVL OPLSYSV
				/* Meets Generic C requirements         */
#define GenericC

				/* Byte order is LOHI                   */
#define BYTEORDER LOHI

#define chau unsigned char      /* Type of unsigned char & integers     */
#define shint short int
#define int4 int
#define sint int
#define sint4 int
#define uint4 unsigned
#define sint2 short int
#define uint2 short unsigned

				/* %e format broken                     */
#define EFMTBAD
				/* define NULL pointer                  */
#ifndef NULL
#define NULL    0
#endif
				/* define sizes                         */
#define SZptr   4
#define SZshort 2
#define SZint   4
#define SZlong  4
#define SZfloat 4
#define SZdouble 8
				/* End IX386				*/
#endif


/* -------------------------------------------------------------------- */

				/* Begin SUN386i Compiler		*/
#if COMPILER==SUN386i
				/* Runs on UNIX 4.2 BSD                 */
#define OPSYS OPUNIX
#define OPSYSLVL OPLBSD42
				/* Meets Generic C requirements         */
#define GenericC

				/* Byte order is LOHI                   */
#define BYTEORDER LOHI

#define chau unsigned char      /* Type of unsigned char & integers     */
#define shint short int
#define int4 int
#define sint int
#define sint4 int
#define uint4 unsigned
#define sint2 short int
#define uint2 short unsigned

				/* %e format broken                     */
#define EFMTBAD
				/* define NULL pointer                  */
#ifndef NULL
#define NULL    0
#endif
				/* define sizes                         */
#define SZptr   4
#define SZshort 2
#define SZint   4
#define SZlong  4
#define SZfloat 4
#define SZdouble 8
#endif
				/* End SUN386i Compiler 		*/
/* -------------------------------------------------------------------- */

#endif
				/* End of header file                   */
/* -------------------------------------------------------------------- */
