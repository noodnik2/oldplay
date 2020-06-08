#define	Revision       		9

/*

	mparysrt.cpp		qad-NewARC21 C++ Source: "M_POArySrt" class
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	#includes
*/
#include	"mparysrt.h"

/*
	Class global declaration
*/
M_CLSDECLARE_DERIVEDWITHDEFCTOR(M_POArySrt, M_POAry) ;

/*
	Class methods
*/

M_POArySrt::~M_POArySrt()
{
	// nothing to do
}

M_POArySrt::M_POArySrt() : m_omc(0)
{
	// nothing to do
}

M_POArySrt::M_POArySrt(M_PO po)
:	m_omc(0),
	M_POAry(po)
{
	// nothing to do
}

M_POArySrt::M_POArySrt(const M_PO *m_poa)
:	m_omc(0),
	M_POAry(m_poa)
{
	// nothing to do
}

M_POArySrt::M_POArySrt(const M_PO *m_poa, int len)
:	m_omc(0),
	M_POAry(m_poa, len)
{
	// nothing to do
}

M_POArySrt::M_POArySrt(const M_POArySrt &oa)
:	m_omc(0),
	M_POAry(oa)
{
	// nothing to do
}

void M_POArySrt::sort(M_METHODCMP omc)
{
	m_poasort(getwm_poa(), length(), omc) ;
	m_omc= omc ;
}

int M_POArySrt::search(M_PCO pco)
{
	if (!isSorted()) return(-1) ;
	return(m_poabinsearch(getm_poa(), pco, length(), m_omc)) ;
}

void M_POArySrt::add(const M_PO *m_poa, int len)
{
	m_omc= (M_METHODCMP) 0 ;
	M_POAry::add(m_poa, len) ;
}

int M_POArySrt::addentry(M_PO po)
{
	m_omc= (M_METHODCMP) 0 ;
	return(M_POAry::addentry(po)) ;
}

// end of mparysrt.cpp
