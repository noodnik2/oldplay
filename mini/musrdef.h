/*	Revision:     		12
*/

#ifndef	M_USRDEF_H_INC
#define	M_USRDEF_H_INC

/*
	musrdef.h

	respects macros defined by compiler.h

	Mxxx		Modifier 'xxx'
	Cxxx		Constant 'xxx'
	Sxxx		Scalar type 'xxx'
	Oxxx		M_Object type 'xxx'

*/

/*
	sub-#includes
*/

#include	"compiler.h"

/*
	#definitions
*/

#define	M_CFALSE		(0)
#define	M_CTRUE		(!M_CFALSE)

#if	MACHINE==MACHINE_IBMPC
#define	M_MFAR	_far
#define	M_MNEAR	_near
#else
#define	M_MFAR
#define	M_MNEAR
#endif

/*
	typedefs
*/

typedef char M_MFAR *		M_SLPSTR ;
typedef const char M_MFAR *	M_SLPCSTR ;
typedef unsigned long 		M_SULONG ;
typedef unsigned long 		M_SDWORD ;
typedef unsigned long		M_SFLAGS ;
typedef unsigned short 		M_SWORD ;
typedef unsigned int 		M_SUINT ;
typedef long 			M_SLONG ;
typedef short			M_SINT ;
typedef int 			M_SBOOL ;
typedef int 			M_SPID ; 		// process-id
// NOTE: "M_SPID" should probably be a "pid_t" but we don't want the #include

/* #ifdef M_USRDEF_H_INC */
#endif

/* end of musrdef.h */
