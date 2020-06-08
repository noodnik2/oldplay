#define	Revision       		13

/*

	mcls.cpp

*/

/*
	#includes
*/

#include	"mclstbl.h"
#include	"mthrow.h"

/*
	class methods
*/

M_Cls::~M_Cls()
{
	// nothing to do
}

M_Cls::M_Cls(
	const char *pmyname,
	M_CLSCTORFN pfnconstruct,
	int nsize
)
:	m_pszClassName(pmyname),
	m_nClassSize(nsize),
	m_pfnConstruct(pfnconstruct),
	m_pbClass(0)
{
	addclass() ;
}

M_Cls::M_Cls(
	const char *pmyname,
	M_CLSCTORFN pfnconstruct,
	int nsize,
	M_Cls *pbaseclass
)
:	m_pszClassName(pmyname),
	m_nClassSize(nsize),
	m_pfnConstruct(pfnconstruct),
	m_pbClass(pbaseclass)
{
	addclass() ;
}

M_SBOOL M_Cls::isa(const M_Cls *pClass) const
{
	const M_Cls *pMyClassNext ;

	for (
		pMyClassNext= this ;
		pMyClassNext != 0;
		pMyClassNext= pMyClassNext->m_pbClass
	) {
		if (pMyClassNext->is(pClass)) return(M_CTRUE) ;
	}
     	return(M_CFALSE) ;
}

void *M_Cls::construct(const char *pszClassName)
{
	M_Cls *pClass ;
	
	pClass= findclass(pszClassName) ;
	if (pClass == (M_Cls *) 0) {
		return((void *) 0) ;
	}
	return(pClass->construct()) ;
}

void M_Cls::addclass()
{
	M_CLSTBLGLOBAL_PTR->add(this) ;
}

M_Cls *M_Cls::findclass(const char *pszClassName)
{
	return(M_CLSTBLGLOBAL_PTR->of(pszClassName)) ;
}

// end of mcls.cpp
