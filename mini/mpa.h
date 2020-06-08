//	Revision:		3

#ifndef	M_PA_H_INC
#define	M_PA_H_INC

/*

	mpa.h	

*/

/*
	sub-#includes
*/
#include	"mobj.h"

/*
	global function forward declarations
*/

int m_poalen(const M_PO*) ;
void m_poacpy(M_PO *m_poa_to, const M_PO *m_poa_from) ;
void m_poancpy(M_PO *m_poa_to, const M_PO *m_poa_from, int maxl) ;
int m_poacmp(const M_PO *m_poa1, const M_PO *m_poa2) ;
int m_poafind(const M_PO*, M_PCO, int nl) ;
int m_poalinsearch(const M_PO*, M_PCO, int nl, M_METHODCMP) ;
int m_poabinsearch(const M_PO*, M_PCO, int nl, M_METHODCMP, int &i) ;
int m_poabinsearch(const M_PO*, M_PCO, int nl, M_METHODCMP, int &is, int &ii) ;
void m_poaswap(M_PO*, int i, int j) ;
void m_poasort(M_PO*, int nl, M_METHODCMP) ;
void m_poasort(M_PO*, M_METHODCMP, int n, int m) ;

/*
	inline functions
*/

inline int m_poabinsearch(
	const M_PO *m_poa,
	M_PCO pco,
	int nlen,
	M_METHODCMP omc
) {
	int ipos ;
	int index= 0 ;
	return(m_poabinsearch(m_poa, pco, nlen, omc, index, ipos)) ;
}

inline int m_poabinsearch(
	const M_PO *a,
	M_PCO o,
	int l,
	M_METHODCMP m,
	int &is
) {
	int ipos ;
	return(m_poabinsearch(a, o, l, m, is, ipos)) ;
}

inline void m_poasort(M_PO *m_poa, int nlen, M_METHODCMP omc) {
	m_poasort(m_poa, omc, 0, nlen-1) ;
}

inline void m_poaswap(M_PO *m_poa, int i, int j)
{
	M_PO poTemp= m_poa[i] ;
	m_poa[i]= m_poa[j] ;
	m_poa[j]= poTemp ;
}

// #ifndef M_PA_H_INC
#endif

// end of mpa.h
