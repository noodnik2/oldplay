#define	Revision       		49

/*

	m_poary.cpp		qad-NewARC21 C++ Source: "M_POAry" class
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	#includes
*/

#include	"compiler.h"
#include	"mpary.h"

/*
	Class global declaration
*/

M_CLSDECLARE_DERIVEDWITHDEFCTOR(M_POAry, M_Obj) ;

/*
	module "internal data"
*/

M_POAryData *M_POARYDATA_PNIL
{
	static M_POAryData m_poary_nil ;
	return(&m_poary_nil) ;
}

/*
	Class methods
*/

/*
	Class:	M_POAryData
*/

M_POAryData::M_POAryData()
:	m_nlen(0),
	m_nalloc(0)
{
	m_po[0]= 0 ;
}

M_POAryData::M_POAryData(unsigned short nalloc)
:	m_nlen(0),
	m_nalloc(nalloc)
{
	m_po[0]= 0 ;
}
					      
M_POAryData *M_POAryData::alloc()
{
	return(M_POARYDATA_PNIL) ;
}

M_POAryData *M_POAryData::alloc(unsigned short nalloc)
{
	return new(nalloc) M_POAryData(nalloc) ;
}

void M_POAryData::freeSelf()
{
	if (this != M_POARYDATA_PNIL) delete this ;
}

void *M_POAryData::operator new(size_t sizeBase, unsigned short nalloc)
{
	// we assert that "sizeBase == sizeof(M_POAryData)"
	return new char[sizeBase + (nalloc * sizeof(M_PO))] ;
}

// insert given M_PO list into M_POAryData object: return # M_POs inserted
int M_POAryData::insert(const M_PO *m_poa, int len, int pos)
{
	int i ;
	M_PO *m_poaend ;

	if ((m_nlen + len)> m_nalloc) len= m_nalloc - m_nlen ;
	if (len<= 0) return(0) ;

	// first make room for new list
	m_poaend= &m_po[len] ;
	for (i= (m_nlen-1); i>= pos; i--) {
		m_poaend[i]= m_po[i] ;
	}

	// now copy new list into vacated spot
	m_poaend= &m_po[pos] ;
	for (i= 0; (i< len) && (m_poa[i] != 0); i++) {
		m_poaend[i]= m_poa[i] ;
	}

	// now adjust the end of list pointer and set final 0 pointer
	m_nlen+= i ;
	m_po[m_nlen]= 0 ;

	return(i) ;
}

M_PO M_POAryData::removeAt(int idx)
{
	if ((idx< 0) || (idx>= (int) m_nlen)) return(0) ;
	M_PO po= m_po[idx] ;

	m_nlen-- ;
	for (int i= idx; i< (int) m_nlen; i++) {
		m_po[i]= m_po[i+1] ;
	}
	m_po[m_nlen]= 0 ;
	return(po) ;
}

/*
	Class:	M_POAry
*/

int M_POAry::construct(const M_PO *m_poa, int len)
{
	if (!m_poa || (len<= 0)) {
		m_pd= M_POAryData::alloc() ;
	}
	else {
		m_pd= M_POAryData::alloc(len) ;
		add(m_poa, len) ;
	}
	return(length()) ;
}

int M_POAry::construct(const M_PO *m_poa)
{
	if (!m_poa) return(construct(0, 0)) ;
	return(construct(m_poa, m_poalen(m_poa))) ;
}

int M_POAry::reconstruct(const M_PO *m_poa, int len)
{
	if (!m_poa || (len<= 0)) {
		M_POAry::reset() ;
	}
	else {
		if (len<= m_pd->allocated()) {
			m_pd->truncate() ;		// improve!!
			add(m_poa, len) ;
		}
		else {
			M_POAry::reset() ;
			construct(m_poa, len) ;
		}
	}
	return(length()) ;
}

int M_POAry::reconstruct(const M_PO *m_poa)
{
	if (!m_poa) return(reconstruct(0, 0)) ;
	return(reconstruct(m_poa, m_poalen(m_poa))) ;
}

// reset() - deletes the object pointer array
void M_POAry::reset()
{
	m_pd->freeSelf() ;
	m_pd= M_POAryData::alloc() ;
}

// resetAndFreeContents()
//	deletes the objects pointed to via method "M_Obj::freeSelf()"
//	(which does "delete this" - destructor of M_Obj is virtual)
//	and then calls "reset()" to delete object pointer array
void M_POAry::resetAndFreeContents()
{
	doPerform((M_METHOD) &M_Obj::freeSelf) ;
	reset() ;
}

void M_POAry::realloc(int n)
{
	if (n<= m_pd->length()) return ;		// won't hurt value
	M_POAryData *pd= M_POAryData::alloc(n) ;
	pd->load(m_pd) ;
	m_pd->freeSelf() ;
	m_pd= pd ;
}

M_PO *M_POAry::getm_poa(M_PO *m_poa) const
{
	m_poacpy(m_poa, m_pd->getm_poa()) ;
	return(m_poa) ;
}

M_PO *M_POAry::getm_poa(M_PO *m_poa, int len) const
{
	m_poancpy(m_poa, m_pd->getm_poa(), len) ;
	return(m_poa) ;
}

int M_POAry::putm_poa(const M_POAry *pos)
{
	return(reconstruct(pos->getm_poa(), pos->length())) ;
}

int M_POAry::insert(const M_PO *m_poa, int len, int pos)
{
	if (!m_poa || (len<= 0)) return(0) ;
	if ((length() + len)> allocated()) {
		expand(M_POARYMIN_EXPAND + len - 1) ;
	}
	return(m_pd->insert(m_poa, len, pos)) ;
}

void M_POAry::add(const M_PO *m_poa, int len)
{
	insert(m_poa, len, length()) ;
}

int M_POAry::addentry(M_PO po)
{
	int pos= length() ;
	insert(po, pos) ;
	return(pos) ;
}

void M_POAry::doPerform(M_METHOD om) const
{
	int l= length() ;
	const M_PO *m_poa= m_pd->getm_poa() ;

	for (int i= 0; i< l; i++) {
		m_poa[i]->perform(om) ;
	}
}

int M_POAry::compareM_POAry(const M_PO *m_poa) const
{
	return(m_poacmp(m_pd->getm_poa(), m_poa)) ;
}

int M_POAry::compare(M_PCO pco) const
{
	// assumes object is M_POAry or derived from M_POAry
	return(compareM_POAry(((const M_POAry *)pco)->getm_poa())) ;
}

M_PO &M_POAry::poAtRef(int idx)
{
	if (idx< length()) return((getwm_poa())[idx]) ;
	static M_PO poSafe ;
	return(poSafe) ;		// should throw exception here
}

/*
	global M_POAry functions
*/

#if 1
M_POAry subm_poa(const M_POAry& oa1, int offset, int len)
{
	M_POAry r ;
	int olen= oa1.length() ;
	if (offset< olen) {
		if ((len == 0) || (len> (olen - offset))) {
			len= olen - offset ;
		}
		r= M_POAry(oa1.getm_poa() + offset, len) ;
	}
	return(r) ;
}
#else
M_POAry subm_poa(const M_POAry& oa1, int offset, int len)
{
	int olen= oa1.length() ;
	if (offset>= olen) return(M_POAry()) ;
	if ((len == 0) || (len> (olen - offset))) {
		len= olen - offset ;
	}
	return(M_POAry(oa1.getm_poa() + offset, len)) ;
}
#endif

// end of m_poary.cpp
