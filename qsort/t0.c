#include	<stdio.h>
#include	<memory.h>

/*
*/


typedef int (*cmpFn_t)(char *pLeftItem, char *pRightItem) ;

static void itemSwap(
	char *pArray,
	int iItemSize,
	int idx0,
	int idx1
) {
	int i ;
	char cTemp, *pArray0, *pArray1 ;

	pArray0= pArray + (idx0 * iItemSize) ;
	pArray1= pArray + (idx1 * iItemSize) ;
	for (i= 0; i< iItemSize; i++) {
		cTemp= pArray0[i] ;
		pArray0[i]= pArray1[i] ;
		pArray1[i]= cTemp ;
	}
}

void qsort(
	void *pArrayVoid,
	int iItemSize,
	cmpFn_t cmpFn,
	int idxFrom,
	int idxTo
) {
	int i, j ;
	char *pArray, *pItem ;

	if (idxFrom>= idxTo) return ;

	pArray= (char *) pArrayVoid ;
	i= idxFrom ;
	j= idxTo + 1 ;
	pItem= pArray +	(idxFrom * iItemSize) ;
	while(/*CONSTCOND*/1) {
		do { if ((++i)>= j) break ; } while(
			cmpFn(pItem, pArray + (i * iItemSize))> 0
		) ;
		do { --j ; } while(
			cmpFn(pItem, pArray + (j * iItemSize))< 0
		) ;
		if (i>= j) break ;
		itemSwap(pArray, iItemSize, i, j) ;
	}
	itemSwap(pArray, iItemSize, idxFrom, j) ;
	qsort(pArray, iItemSize, cmpFn, idxFrom, j-1) ;
	qsort(pArray, iItemSize, cmpFn, j+1, idxTo) ;
}

/*
	user code
*/

struct arrayItem {
	int iValue ;
} ;

typedef struct arrayItem arrayItem_t ;
typedef arrayItem_t *arrayItemPtr_t ;

#define	MAXITEMS		100 

static int cmpFn(char *pLeftItem, char *pRightItem)
{
	arrayItemPtr_t pItem0= (arrayItemPtr_t) pLeftItem ;
	arrayItemPtr_t pItem1= (arrayItemPtr_t) pRightItem ;
	if (pItem0->iValue< pItem1->iValue) return(-1) ;
	if (pItem0->iValue> pItem1->iValue) return(1) ;
	return(0) ;
}

void main()
{
	char buf[128] ;
	arrayItem_t array[MAXITEMS] ;
	int i, l ;

	printf("reading database into array\n") ;
	for (i= 0; i< MAXITEMS; i++) {
		if (!gets(buf)) break ;
		sscanf(buf, "%d", &array[i].iValue) ;
		printf("%d\n", array[i].iValue) ;
	}
	if (i == MAXITEMS) {
		printf("NOTE: database truncated to %d items\n", i) ;
	}
	l= i ;

	printf("sorting list\n") ;
	qsort(array, sizeof(arrayItem_t), cmpFn, 0, l-1) ;

	printf("printing sorted list\n") ;
	for (i= 0; i< l; i++) {
		printf("%d\n", array[i].iValue) ;
	}
	
}

/* end */
