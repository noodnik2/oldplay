#include	<stdio.h>
#include	<malloc.h>
#include	<memory.h>

/*
	LIST API Code
*/

typedef	int listdata_t ;

struct list {
	struct list *pUp ;
	struct list *pLeft ;
	struct list *pRight ;
	listdata_t v ;
} ;

typedef struct list list_t ;
typedef list_t *listPtr_t ;

typedef int (*cmpFn_t)(listdata_t, listdata_t) ;

static listPtr_t newNode(listdata_t v)
{
	listPtr_t pTree ;

	pTree= (listPtr_t) malloc(sizeof(list_t)) ;
	memset(pTree, '\0', sizeof(list_t)) ;
	printf("insert: %d\n", (int) v) ;
	pTree->v= v ;
	return(pTree) ;
}

static void insertNode(listPtr_t pTree, listdata_t v, cmpFn_t cfn)
{
	listPtr_t pTreeNew ;

	if (cfn(v, pTree->v)<= 0) {
		if (pTree->pLeft) {
			insertNode(pTree->pLeft, v, cfn) ;
			return ;
		}
		pTreeNew= newNode(v) ;
		pTreeNew->pUp= pTree ;
		pTree->pLeft= pTreeNew ;
		return ;
	}
	if (pTree->pRight) {
		insertNode(pTree->pRight, v, cfn) ;
		return ;
	}
	pTreeNew= newNode(v) ;
	pTreeNew->pUp= pTree ;
	pTree->pRight= pTreeNew ;
}

static void traverse(listPtr_t pTree)
{
	if (pTree->pLeft) {
		traverse(pTree->pLeft) ;
	}
	printf("traverse: %d", (int) pTree->v) ;
	if (pTree->pUp) printf(" u=%d", (int) pTree->pUp->v) ;
	if (pTree->pLeft) printf(" l=%d", (int) pTree->pLeft->v) ;
	if (pTree->pRight) printf(" r=%d", (int) pTree->pRight->v) ;
	printf("\n") ;
	if (pTree->pRight) {
		traverse(pTree->pRight) ;
	}
}

/*
	user code
*/

static int cmpFn(listdata_t d0, listdata_t d1)
{
	if (d0< d1) return(-1) ;
	if (d0> d1) return(1) ;
	return(0) ;
}


void main()
{
	int iValue ;
	listPtr_t pTree ;
	char buf[128] ;

	pTree= (listPtr_t) 0 ;
	printf("reading database, building tree\n") ;
	while(gets(buf)) {
		sscanf(buf, "%d", &iValue) ;
		if (pTree) insertNode(pTree, (listdata_t) iValue, cmpFn) ;
		else pTree= newNode((listdata_t) iValue) ;
	}

	printf("outputting tree\n") ;
	traverse(pTree) ;
}

/* end of */
