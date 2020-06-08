#define	Revision		1

/*

	qsort.c				QSORT C Source
					Copyright (C) 1996, Point Of Sale Ltd.

*/

/*
	#includes
*/

#include	"qsort.h"


/*
	internal function decls
*/

static void itemSwap(
	char *pArray,
	size_t sItemSize,
	int sidx0,
	int sidx1
) ;


/*
	entry points
*/

void qsortQuickSortArray(
	void *pArrayVoid,
	size_t sNItems,
	size_t sItemSize,
	qscmpFn_t cmpFn
) {
	qsortQuickSortSubArray(
		pArrayVoid,
		sItemSize,
		cmpFn,
		0,
		(int) (sNItems - 1)
	)  ;
}

void qsortQuickSortSubArray(
	void *pArrayVoid,
	size_t sItemSize,
	qscmpFn_t cmpFn,
	int idxFrom,
	int idxTo
) {
	int i, j ;
	char *pArray, *pItem ;

	if (idxFrom>= idxTo) return ;

	i= idxFrom ;
	j= idxTo + 1 ;

	pArray= (char *) pArrayVoid ;
	pItem= pArray +	(idxFrom * sItemSize) ;
	while(/*CONSTCOND*/1) {
		do { if ((++i)>= j) break ; } while(
			cmpFn(pItem, pArray + (i * sItemSize))> 0
		) ;
		do { --j ; } while(
			cmpFn(pItem, pArray + (j * sItemSize))< 0
		) ;
		if (i>= j) break ;
		itemSwap(pArray, sItemSize, i, j) ;
	}
	itemSwap(pArray, sItemSize, idxFrom, j) ;
	qsortQuickSortSubArray(pArray, sItemSize, cmpFn, idxFrom, j - 1) ;
	qsortQuickSortSubArray(pArray, sItemSize, cmpFn, j + 1, idxTo) ;
}

/*
	internal functions
*/

static void itemSwap(
	char *pArray,
	size_t sItemSize,
	int idx0,
	int idx1
) {
	size_t i ;
	char cTemp, *pArray0, *pArray1 ;

	pArray0= pArray + (idx0 * sItemSize) ;
	pArray1= pArray + (idx1 * sItemSize) ;
	for (i= 0; i< sItemSize; i++) {
		cTemp= pArray0[i] ;
		pArray0[i]= pArray1[i] ;
		pArray1[i]= cTemp ;
	}
}

/* end of qsort.c */
