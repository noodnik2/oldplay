#define	Revision	2

/*

	m_poa.cpp			qad-NewARC21 C++ Source: "M_POA" functions
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

int m_poalen(const M_PO *m_poa)
{
	int i ;
	for (i= 0; (m_poa[i] != 0); i++) ;
	return(i) ;
}

void m_poacpy(M_PO *m_poa_to, const M_PO *m_poa_from)
{
	while((*m_poa_from) != 0) {
		(*m_poa_to++)= (*m_poa_from++) ;
	}
	(*m_poa_to)= 0 ;
}

void m_poancpy(M_PO *m_poa_to, const M_PO *m_poa_from, int maxlen)
{
	for (int i= 0; (i< maxlen) && ((*m_poa_from) != 0); i++) {
		(*m_poa_to++)= (*m_poa_from++) ;
	}
	(*m_poa_to)= 0 ;
}

int m_poacmp(const M_PO *m_poa1, const M_PO *m_poa2)
{
	int ic ;

	while(((*m_poa1) != 0) && ((*m_poa2) != 0)) {
		ic= (*m_poa1)->compare(*m_poa2) ;
		if (ic != 0) return(ic) ;
		m_poa1++ ;
		m_poa2++ ;
	}
	if (((*m_poa1) == 0) && ((*m_poa2) == 0)) return(0) ;	// ==
	if ((*m_poa1) == 0) return(-1) ;		// m_poa1 < m_poa2
	return(1) ;					// m_poa1 > m_poa2
}

// end of m_poa.cpp
