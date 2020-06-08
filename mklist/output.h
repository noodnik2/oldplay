/*	Revision:      		4
*/

/*

	output.h			OUTPUT C Header
					Copyright (C) 1996, Point Of Sale Ltd.

*/

#ifndef	OUTPUT_H_INC
#define	OUTPUT_H_INC


/*
	#defines
*/

#define	OUTPUT_RC		int
#define	OUTPUTRC_SUCCESS	0
#define	OUTPUTRC_BADARGS	12
#define	OUTPUTRC_FAILURE	99
#define	OUTPUTRC_CANTBE		999


/*
	structs, types
*/

typedef void (*outputDeliverItemFn_t)(
	long lExprValue,
	long lSortValue,
	const void *pUserLine,
	void *pUserData
) ;

struct output_initparms {
	int iDebugLevel ;
	outputDeliverItemFn_t pfnDeliver ;
	void *pUserData ;
} ;


/*
	function decls
*/

OUTPUT_RC outputInit(struct output_initparms *) ;
void outputFini(void) ;

OUTPUT_RC outputAddItem(
	long lExprValue,
	long lSortValue,
	const void *pUserLine,
	int iLenUserLine
) ;
OUTPUT_RC outputDumpItems(void) ;


/* #ifndef OUTPUT_H_INC */
#endif

/* end of output.h */
