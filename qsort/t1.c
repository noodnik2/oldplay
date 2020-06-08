#include	<stdio.h>
#include	<memory.h>

/*
*/

struct arrayItem {
	int iValue ;
} ;

typedef struct arrayItem arrayItem_t ;
typedef arrayItem_t *arrayItemPtr_t ;

typedef int (*cmpFn_t)(arrayItemPtr_t, arrayItemPtr_t) ;

static void itemSwap(
	arrayItemPtr_t pArray,
	int idx0,
	int idx1
) {
	arrayItem_t itemTemp ;

					/* NOTE: using ANSI 'C' extension! */
	itemTemp= pArray[idx0] ;
	pArray[idx0]= pArray[idx1] ;
	pArray[idx1]= itemTemp ;
}

void qsort(
	arrayItemPtr_t pArray,
	cmpFn_t cmpFn,
	int idxFrom,
	int idxTo
) {
	int i, j ;
	arrayItemPtr_t pItem ;

	if (idxFrom>= idxTo) return ;

	i= idxFrom ;
	j= idxTo + 1 ;
	pItem= pArray +	idxFrom ;
	while(/*CONSTCOND*/1) {
		do { if ((++i)>= j) break ; } while(cmpFn(pItem, pArray+i)> 0) ;
		do { --j ; }	 	      while(cmpFn(pItem, pArray+j)< 0) ;
		if (i>= j) break ;
		itemSwap(pArray, i, j) ;
	}
	itemSwap(pArray, idxFrom, j) ;
	qsort(pArray, cmpFn, idxFrom, j-1) ;
	qsort(pArray, cmpFn, j+1, idxTo) ;
}

/*
	user code
*/

#define	MAXITEMS		100 

static int cmpFn(arrayItemPtr_t pItem0, arrayItemPtr_t pItem1)
{
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
	qsort(array, cmpFn, 0, l-1) ;

	printf("printing sorted list\n") ;
	for (i= 0; i< l; i++) {
		printf("%d\n", array[i].iValue) ;
	}
	
}

/* end */
