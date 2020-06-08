#define	Revision		2

/*

	list.c				LIST C Source
					Copyright (C) 1996, Point Of Sale Ltd.

*/


/*
	#includes
*/

#include	<malloc.h>
#include	<memory.h>
#include	"list.h"


/*
	internal functions
*/

static void freeItemMemory(void *pItemMemory) ;
static void *getItemMemory(size_t sNBytesMemoryWanted) ;


/*
	internal data
*/

LIST_RC lerrno_global= LISTRC_SUCCESS ;


/*
	entry points
*/

LIST_RC listInit(struct list_initparms *pip)
{
	return(lerrno_global= LISTRC_SUCCESS) ;
}

void listFini()
{
}

LIST_RC listErrorLevel()
{
	return(lerrno_global) ;
}

listItemPtr_t listAddItem(listItemPtr_t pItem, size_t sDLen)
{
	listItemPtr_t pNewItem ;
	size_t sItemTotalSize ;

	sItemTotalSize= sizeof(listItem_t) + sDLen ;
	pNewItem= (listItemPtr_t) getItemMemory(sItemTotalSize) ;
	if (!pNewItem) {
		lerrno_global= LISTRC_NOMEMORY ;
		return((listItemPtr_t) 0) ;
	}

	memset(pNewItem, '\0', sItemTotalSize) ;

	if (pItem) {
		while(pItem->pNext) pItem= pItem->pNext ;
		pItem->pNext= pNewItem ;
	}

	return(pNewItem) ;
}

void *listGetItemData(listItemPtr_t pItem)
{
	return((void *) (pItem + 1)) ;
}

listItemPtr_t listGetNextItem(listItemPtr_t pItem)
{
	return(pItem->pNext) ;
}

void listFree(listItemPtr_t pList)
{
	listItemPtr_t pItem, pNext ;

	if (!pList) return ;
	for (pItem= pList; pItem; pItem= pNext) {
		pNext= pItem->pNext ;
		freeItemMemory(pItem) ;	
	}
}


/*
	internal subroutines
*/

static void *getItemMemory(size_t sNBytesMemoryWanted)
{
	/*
		this should probably be implemented internally,
		since malloc() is notorious for being bad at
		allocating many many small blocks...
	*/
	return(malloc(sNBytesMemoryWanted)) ;
}

static void freeItemMemory(void *pItemMemory)
{
	/*
		this must correspond to whatever "getItemMomory" does...
	*/
	free(pItemMemory) ;
}

/* end of list.c */
