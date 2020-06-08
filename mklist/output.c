#define	Revision		1

/*

	output.c				OUTPUT C Source
					Copyright (C) 1996, Point Of Sale Ltd.

*/


/*
	#includes
*/

#include	<memory.h>
#include	"output.h"
#include	"list.h"
#include	"qsort.h"


/*
	internal structs, types
*/

struct output_sortitem {
	long *pDataLong ;
} ;


/*
	internal data
*/

struct output_initparms oip ;
static listItemPtr_t pListBase, pListLast ;
static int iNumItems ;


/*
	internal functions
*/

static QSORTCMP_RC compareItems(const void *p0, const void *p1) ;


/*
	entry points
*/

OUTPUT_RC outputInit(struct output_initparms *poip)
{
	LIST_RC lrc ;
	struct list_initparms lip ;

	if (!poip) return(OUTPUTRC_BADARGS) ;
	if (!poip->pfnDeliver) return(OUTPUTRC_BADARGS) ;
	memcpy(&oip, poip, sizeof(struct output_initparms)) ;

	memset(&lip, '\0', sizeof(lip)) ;
	lrc= listInit(&lip) ;
	if (lrc != LISTRC_SUCCESS) return(OUTPUTRC_FAILURE) ;

	pListBase= pListLast= (listItemPtr_t) 0 ;
	iNumItems= 0 ;
	return(OUTPUTRC_SUCCESS) ;
}

void outputFini()
{
	listFree(pListBase) ;
	listFini() ;
}

OUTPUT_RC outputAddItem(
	long lExprValue,
	long lSortValue,
	const void *pUserLine,
	int iLenUserLine
) {
	size_t sLine ;
	long *pDataLong ;

	pListLast= listAddItem(
		pListLast,
		sizeof(long) + sizeof(long) + (pUserLine? iLenUserLine: 0)
	) ;
	if (!pListLast) return(OUTPUTRC_FAILURE) ;

	if (!pListBase) pListBase= pListLast ;

	pDataLong= (long *) listGetItemData(pListLast) ;
	pDataLong[0]= lSortValue ;
	pDataLong[1]= lExprValue ;

	if (pUserLine) memcpy(pDataLong + 2, pUserLine, iLenUserLine) ;

	iNumItems++ ;
	return(OUTPUTRC_SUCCESS) ;
}

OUTPUT_RC outputDumpItems()
{
	struct output_sortitem *pSortArray ;
	listItemPtr_t pItem ;
	int i ;

	/*
		Allocate an array to store pointers to all the items
	*/
	pSortArray= (struct output_sortitem *) malloc(
		iNumItems * sizeof(struct output_sortitem)
	) ;
	if (!pSortArray) {
		return(OUTPUTRC_FAILURE) ;
	}

	/*
		Loop across the list; fill up the array
	*/
	pItem= pListBase ;
	for (i= 0; i< iNumItems; i++) {
		if (!pItem) return(OUTPUTRC_CANTBE) ;
		pSortArray[i].pDataLong= (long *) listGetItemData(pItem) ;
		pItem= listGetNextItem(pItem) ;
	}

	/*
		Sort the list by the "lSortValue" that's in the first word
	*/
	qsortQuickSortArray(
		pSortArray,
		iNumItems,
		sizeof(struct output_sortitem),
		compareItems	
	) ;

	/*
		Call the user's callback to deliver the
		list items in sorted order.
	*/
	for (i= 0; i< iNumItems; i++) {
		oip.pfnDeliver(
			pSortArray[i].pDataLong[1],
			pSortArray[i].pDataLong[0],
			(const void *) (pSortArray[i].pDataLong + 2),
			oip.pUserData
		) ;
	}
	
	return(OUTPUTRC_SUCCESS) ;
}


/*
	internal functions
*/

static QSORTCMP_RC compareItems(const void *p0, const void *p1)
{
	struct output_sortitem *pItem0= (struct output_sortitem *) p0 ;
	struct output_sortitem *pItem1= (struct output_sortitem *) p1 ;

	if (pItem0->pDataLong[0]< pItem1->pDataLong[0]) {
		return(QSORTCMPRC_LESS) ;
	}
	if (pItem0->pDataLong[0]> pItem1->pDataLong[0]) {
		return(QSORTCMPRC_GREATER) ;
	}
	return(QSORTCMPRC_EQUAL) ;
}

/* end of output.c */
