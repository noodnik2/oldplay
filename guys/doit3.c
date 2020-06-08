#include	<stdio.h>

#define	N_PEOPLE	6
#define	N_COUPLES	(N_PEOPLE / 2)

#define	PERSON_ID	int
#define	PAIR_ID		int
#define	PAIR_CNT	int
#define	PERIOD_CNT	int

#define	BOOL_OK		int

static PAIR_CNT nPairs(PERIOD_CNT np)
{
	PAIR_CNT pc ;
	int i ;

	pc= 0 ;
	for (i= 0; i< (np-1); i++) {
		pc+= (i+1) ;
	}
	return(pc) ;
}

static void abortit(char *pszMsg)
{
	fprintf(stderr, "abort: %s\n", pszMsg) ;
	exit(1) ;
}

static BOOL_OK gppair(PAIR_ID pid, PERSON_ID *ppid1, PERSON_ID *ppid2)
{
	PERSON_ID p1, p2 ;
	PAIR_ID pid0 ;
	PAIR_CNT pc0 ;
	int i ;

	pid0= 0 ;
	p1= 0 ;
	p2= 1 ;
	for (pc0= N_PEOPLE - 1; pc0> 0; pc0--) {
		for (i= 0; i< pc0; i++) {
			if (pid0 == pid) {
				(*ppid1)= p1 ;
				(*ppid2)= p2 ;
				return(1) ;
			}
			p2++ ;
			pid0++ ;
		}
		p1++ ;
		p2= p1 + 1 ;
	}
	
	return(0) ;
}

static void ppair(PAIR_ID pid)
{
	PERSON_ID p1, p2 ;

	if (!gppair(pid, &p1, &p2)) abortit("no person") ;
	printf("%c%c\n", p1+'A', p2+'A') ;
}

void main()
{
	PAIR_ID pid ;
	PAIR_CNT pc ;

	pc= nPairs(N_PEOPLE) ;
	printf("nPairs==%d\n", pc) ;
	for (pid= 0; pid< pc; pid++) {
#if 0
		if ((pid % N_COUPLES) == 0) {
			printf("\nperiod: %d\n", (pid / N_COUPLES) + 1) ;
		}
#endif
		ppair(pid) ;
	}

}

