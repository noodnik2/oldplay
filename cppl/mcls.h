//	Revision:      		31

#ifndef	M_CLS_H_INC
#define	M_CLS_H_INC

/*

	mcls.h 			qad-NewARC21 C++ Header: Class "M_Cls"
				Copyright (C) 1994-1995, Contahal Ltd.


	This file provides methods for class identity.

*/

/*
	sub-#includes
*/

#include	"musrdef.h"
#include	"mdefs.h"

/*
	typedefs
*/

typedef void *(*M_CLSCTORFN)() ;

/*
	class definitions
*/

class	M_Cls
{
	public:
		~M_Cls() ;
		M_Cls(const char *, M_CLSCTORFN, int, M_Cls *) ;
		M_Cls(const char *, M_CLSCTORFN, int) ;	   
		M_SBOOL is(const M_Cls *) const ;
		M_SBOOL isa(const M_Cls *) const ;
		static void *construct(const char *pszClassName) ;
		static M_Cls *findclass(const char *pszClassName) ;
		int classsize()	const { return(m_nClassSize) ; } ;
		const char *classname() const { return(m_pszClassName) ; } ;
		void *construct() const { return(m_pfnConstruct()) ; } ;

	private:
		void addclass() ;
		const char *m_pszClassName ;	// class name
		int m_nClassSize ;		// class size
		M_CLSCTORFN m_pfnConstruct ;	// ptr to default ctor fn
		M_Cls *m_pbClass ;	   	// ptr to base class
} ;

/*
	methods implemented in-line
*/

inline M_SBOOL M_Cls::is(const M_Cls *pClass) const {
	return((M_SBOOL) (this == pClass)) ;
}

/*
	macro #definitions
*/

/*
	internal macros not used by user
*/
#define	M_CLSMAX_CLASSES		256

// M_CLSCLS_ID(c) returns M_Cls * for class "c"
#define	M_CLSCLS_ID(c)	M_Cls_##c##_ID()

#define	M_CLSDEF_REF(c) extern M_Cls *	M_CLSCLS_ID(c)

#define	M_CLSDEF_ACCESS(c, newfn)				       		\
void *		c ## c () { return((void *) newfn) ; }	       		\
const M_Cls *c::m_cls_id() const {					\
	M_CLSDEF_REF(c) ;						\
	return(M_CLSCLS_ID(c)) ;					\
}

/*
	user macros
*/

// M_CLSDEFS() -- to appear in body of each user class definition
#define	M_CLSDEFS()				       	       		\
	public:				       	       	       		\
		virtual const M_Cls *m_cls_id() const

//
// DEFxxx macros -- to appear once in application for each user class
//

// M_CLSDECLARE_BASE(c) -- define class 'c' as topmost class in hierarchy
#define	M_CLSDECLARE_BASE(c)					       		\
M_CLSDEF_ACCESS(c, new c) ;						       	\
M_Cls *M_CLSCLS_ID(c) { 						\
	static M_Cls c##Cls(#c, c##c, sizeof(c)) ;	       		\
	return(&c##Cls) ;				       		\
}

// M_CLSDECLARE_DERIVEDWITHDEFCTOR(c, b) -- define class 'c' with base class 'b'
#define	M_CLSDECLARE_DERIVEDWITHDEFCTOR(c,b)					       		\
M_CLSDEF_ACCESS(c, new c) ;					       		\
M_Cls *M_CLSCLS_ID(c) { 						\
	M_CLSDEF_REF(b) ;					       	\
	static M_Cls c##Cls(#c, c##c, sizeof(c), M_CLSCLS_ID(b)) ;	\
	return(&c##Cls) ;				       		\
}

// M_CLSDECLARE_DERIVED(c, b) -- define "abstract" class 'c' with base class 'b'
// (an "abstract" class here is a class without a default constructor)
#define	M_CLSDECLARE_DERIVED(c,b)				       		\
M_CLSDEF_ACCESS(c, 0) ;					       		\
M_Cls *M_CLSCLS_ID(c) { 						\
	M_CLSDEF_REF(b) ;					       	\
	static M_Cls c##Cls(#c, c##c, sizeof(c), M_CLSCLS_ID(b)) ;	\
	return(&c##Cls) ;				       		\
}

//
// run-time class identification service macros
//

#define	GetClsID()		m_cls_id()
#define	IsCls(c)		GetClsID()->is(M_CLSCLS_ID(c))
#define	IsClsA(c)		GetClsID()->isa(M_CLSCLS_ID(c))
#define	GetClsName()		GetClsID()->classname()
#define	GetClsSize()		GetClsID()->classsize()
#define	NewClsObject()		GetClsID()->construct()

// #ifndef M_CLS_H_INC
#endif

// end of mcls.h
