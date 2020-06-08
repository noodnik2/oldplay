#define	Revision       		5

/*

	m_poasrch.cpp		qad-NewARC21 C++ Source: "M_POA" search fns
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	#includes
*/
#include	"compiler.h"
#include	"mpa.h"

/*
	global functions
*/

int m_poafind(const M_PO *m_poa, M_PCO pco, int nlen)
{
	for (int i= 0; i< nlen; i++) {	
		if (m_poa[i] == pco) return(i) ;
	}
	return(-1) ;
}

int m_poalinsearch(const M_PO *m_poa, M_PCO pco, int nlen, M_METHODCMP omc)
{
	for (int i= 0; i< nlen; i++) {	
		if ((m_poa[i]->*omc)(pco) == 0) return(i) ;
	}
	return(-1) ;
}

int m_poabinsearch(
	const M_PO *m_poa,
	M_PCO pco,
	int nlen,
	M_METHODCMP omc,
	int &index,
	int &ipos
) {
	const M_PO *m_poa0= m_poa ;
	int index0= index ;
	int nlen0= nlen ;
	int ic ;

	if (nlen0<= 0) {
		index= ipos= 0 ;
		return(-1) ;
	}
	if (index0>= nlen0) index0= 0 ;
	while((ic= (m_poa0[index0]->*omc)(pco)) != 0) {
		if (nlen0<= 1) {
			index= (m_poa0 - m_poa) + index0 ;
			ipos= index + (ic< 0) ;
			return(-1) ;
		}
		if (ic> 0) {		// pco is smaller than current item
			nlen0= index0 ;	// so we must go backwards
			index0/= 2 ;
		} else {		// pco is greater than current item
			m_poa0+= index0 ;	// so we must move forwards
			nlen0-= index0 ;
			index0= nlen0 / 2 ;
		}
	}
	index= (m_poa0 - m_poa) + index0 ;
	return(index) ;
}

// end of m_poasrch.cpp
