/*	Revision:      		3
*/

/*

	list.h				LIST C Header
					Copyright (C) 1996, Marty Ross

*/

#ifndef	LIST_H_INC
#define	LIST_H_INC


/*
	sub-#includes
*/

#include	<sys/types.h>


/*
	#defines
*/

#define	LIST_RC			int
#define	LISTRC_SUCCESS		0
#define	LISTRC_NOMEMORY		13

#define	LIST_DL			int
#define	LISTDL_ALWAYS		0
#define	LISTDL_ERROR		1
#define	LISTDL_DETAIL		2
#define	LISTDL_INFORM		3

#define	LISTCMP_RC		int
#define	LISTCMPRC_LESS		-1
#define	LISTCMPRC_EQUAL		0
#define	LISTCMPRC_GREATER	1


/*
	types, structures
*/

struct list_initparms {
	LIST_DL iDebugLevel ;
} ;

struct listItem_hdr {
	struct listItem_hdr *pNext ;
} ;

typedef struct listItem_hdr listItem_t ;
typedef listItem_t *listItemPtr_t ;


/*
	API functions
*/

LIST_RC listInit(struct list_initparms *) ;
LIST_RC listErrorLevel() ;
void listFini(void) ;

listItemPtr_t listAddItem(listItemPtr_t, size_t sDLen) ;
void *listGetItemData(listItemPtr_t pItem) ;
listItemPtr_t listGetNextItem(listItemPtr_t pItem) ;
void listFree(listItemPtr_t) ;


/* #ifndef LIST_H_INC */
#endif

/* end of list.h */
