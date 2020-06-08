#include	<stdio.h>
#include	<memory.h>

#include	"qsort.h"

struct arrayItem {
	int iValue ;
} ;

typedef struct arrayItem arrayItem_t ;
typedef arrayItem_t *arrayItemPtr_t ;

#define	MAXITEMS		100 

static QSORTCMP_RC cmpFn(void *pLeftItem, void *pRightItem)
{
	arrayItemPtr_t pItem0= (arrayItemPtr_t) pLeftItem ;
	arrayItemPtr_t pItem1= (arrayItemPtr_t) pRightItem ;
	if (pItem0->iValue< pItem1->iValue) return(QSORTCMPRC_LESS) ;
	if (pItem0->iValue> pItem1->iValue) return(QSORTCMPRC_GREATER) ;
	return(QSORTCMPRC_EQUAL) ;
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

#if 0
	printf("sorting list using qsortQuickSortArray\n") ;
	qsortQuickSortArray(array, l, sizeof(arrayItem_t), cmpFn) ;
#else
	printf("sorting list using qsort(2)\n") ;
	qsort(array, l, sizeof(arrayItem_t), cmpFn) ;
#endif

	printf("printing sorted list\n") ;
	for (i= 0; i< l; i++) {
		printf("%d\n", array[i].iValue) ;
	}
	
}

/* end */
