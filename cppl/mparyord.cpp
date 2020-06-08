#define	Revision       		24

/*

	mparyord.cpp		qad-NewARC21 C++ Source: "M_POAryOrd" class
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	#includes
*/
#include	"compiler.h"
#include	"mparyord.h"

/*
	Class global declaration
*/
M_CLSDECLARE_DERIVEDWITHDEFCTOR(M_POAryOrd, M_POAry) ;

/*
	Class methods
*/

M_POAryOrd::~M_POAryOrd()
{
}

// constructors using default comparison method "M_POAODMC"
M_POAryOrd::M_POAryOrd()
:	m_i(0),
	m_omc(M_POAODMC)
{
	// nothing to do
}

M_POAryOrd::M_POAryOrd(M_PO po)
:	m_i(0),
	m_omc(M_POAODMC)
{
	addentry(po) ;
}

M_POAryOrd::M_POAryOrd(const M_PO *poa)
:	m_i(0),
	m_omc(M_POAODMC)
{
	M_POAry::add(poa) ;
}

M_POAryOrd::M_POAryOrd(const M_PO *poa, int len)
:	m_i(0),
	m_omc(M_POAODMC)
{
	add(poa, len) ;
}

M_POAryOrd::M_POAryOrd(const M_POAryOrd &oa)
:	m_i(oa.m_i),
	m_omc(oa.m_omc)
{
	add(oa.getm_poa(), oa.length()) ;
}

// constructors using user-supplied comparison method
M_POAryOrd::M_POAryOrd(M_METHODCMP omc)
:	m_i(0),
	m_omc(omc)
{
	// nothing to do
}

M_POAryOrd::M_POAryOrd(M_METHODCMP omc, M_PO po)
:	m_i(0),
	m_omc(omc)
{
	addentry(po) ;
}

M_POAryOrd::M_POAryOrd(M_METHODCMP omc, const M_PO *poa)
:	m_i(0),
	m_omc(omc)
{
	M_POAry::add(poa) ;
}

M_POAryOrd::M_POAryOrd(M_METHODCMP omc, const M_PO *poa, int len)
:	m_i(0),
	m_omc(omc)
{
	add(poa, len) ;
}

M_POAryOrd::M_POAryOrd(M_METHODCMP omc, const M_POAry &oa)
:	m_i(0),
	m_omc(omc)
{
	add(oa.getm_poa(), oa.length()) ;
}

// search routine using list-sort comparison method
int M_POAryOrd::search(M_PCO pco)
{
  	if (m_omc == (M_METHODCMP) 0) return(-2) ;
	return(m_poabinsearch(getm_poa(), pco, length(), m_omc, m_i)) ;
}

// add entry to list
int M_POAryOrd::addentry(M_PO po)
{
	int ipos ;
	m_poabinsearch(getm_poa(), po, length(), m_omc, m_i, ipos) ;
	insert(po, ipos) ;
	return(ipos) ;
}

// add list to list
void M_POAryOrd::add(const M_PO *poa, int len)
{
	for (int i= 0; (i< len) && (poa[i] != 0); i++) {
		addentry(poa[i]) ;
	}
}

// end of mparyord.cpp
