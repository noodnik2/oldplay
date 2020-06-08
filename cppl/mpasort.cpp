#define	Revision       		3

/*

	m_poasort.cpp		qad-NewARC21 C++ Source: "M_POA" sort fns
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	#includes
*/
#include	"compiler.h"
#include	"mpa.h"

/*
	start of global functions
*/

void m_poasort(M_PO *m_poa, M_METHODCMP omc, int m, int n)
{
	if (m>= n) return ;

	int i= m ;
	int j= n + 1 ;
	M_PO pom= m_poa[m] ;
	while(/*CONSTCOND*/1) {
		do { if ((++i)>= j) break ; } while(((pom->*omc)(m_poa[i]))> 0) ;
		do { --j ; } 		      while(((pom->*omc)(m_poa[j]))< 0) ;
		if (i>= j) break ;
		m_poaswap(m_poa, i, j) ;
	}
	m_poaswap(m_poa, m, j) ;
	m_poasort(m_poa, omc, m, j-1) ;
	m_poasort(m_poa, omc, j+1, n) ;
}

// end of m_poasort.cpp
