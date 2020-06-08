/*	Revision:      		1
*/

/*

	qsort.h				QSORT C Header
					Copyright (C) 1996, Point Of Sale Ltd.

*/

#ifndef	QSORT_H_INC
#define	QSORT_H_INC

/*
	sub-#includes
*/

#include	<sys/types.h>


/*
	#defines
*/

#define	QSORTCMP_RC		int
#define	QSORTCMPRC_LESS		-1
#define	QSORTCMPRC_EQUAL	0
#define	QSORTCMPRC_GREATER	1


/*
	typedefs
*/

typedef QSORTCMP_RC (*qscmpFn_t)(const void *pLeftItem, const void *pRightItem) ;


/*
	API functions
*/

void qsortQuickSortSubArray(
	void *pArrayVoid,
	size_t sItemSize,
	qscmpFn_t,
	int sidxFrom,
	int sidxTo
) ;

void qsortQuickSortArray(
	void *pArrayVoid,
	size_t sNItems,
	size_t sItemSize,
	qscmpFn_t
) ;


/* #ifndef QSORT_H_INC */
#endif

/* end of qsort.h */
