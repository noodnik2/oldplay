#define	Revision       		30

/*

	mobj.cpp		qad-NewARC21 C++ Source: "M_Obj" class
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	#includes
*/

#include	"compiler.h"
#include	"mobj.h"
#include	"mthrow.h"

#include	<memory.h>

/*
	global data allocations
*/

M_CLSDECLARE_BASE(M_Obj) ;

/*
	class methods
*/

M_Obj::~M_Obj()
{
	// nothing to do
}

void M_Obj::reset()
{
	// nothing to do
}

void M_Obj::freeSelf()
{
	reset() ;		// call derived "reset()"
	delete this ;
}

M_Obj &M_Obj::Clone() const
{
	M_Throw::FatalError("Clone() not implemented for object") ;
	/*NOTREACHED*/
	return(*((M_Obj *) 0)) ;
}

M_PO M_Obj::deepCopy()
{
	error("deepCopy not implemented") ;
	return(0) ;
}

M_PO M_Obj::copy()
{
	M_Obj *po= (M_PO) NewClsObject() ;
	if (!copyTo(po)) {
		po->freeSelf() ;
		return(0) ;
	}
	return(po) ;
}

M_SBOOL M_Obj::copyTo(M_PO po)
{
	if (GetClsID() != po->GetClsID()) {
		error("attempt to copyTo: conflicting types") ;
		return(M_CFALSE) ;
	}
	int size= po->GetClsSize() ;	// sizeof(target class) ;
	memcpy((void *) po, (void *) this, size) ;
	return(M_CTRUE) ;
}

void M_Obj::getFrom(M_Archive &oa)
{
	error("getFrom: no archive") ;
}

void M_Obj::putTo(M_Archive &oa)
{
	error("putTo: no archive") ;
}

void M_Obj::perform(M_METHOD om)
{
	(this->*om)() ;
}

void M_Obj::perform(M_METHOD om, M_SLONG lparam)
{
	(this->*((M_METHOD1)om))(lparam) ;
}

void M_Obj::perform(M_METHOD om, M_SLONG lparam1, M_SLONG lparam2)
{
	(this->*((M_METHOD2)om))(lparam1, lparam2) ;
}

M_SBOOL M_Obj::isEqual(M_PCO pco) const
{
	return((M_SBOOL) (pco == this)) ;
}

M_HASH M_Obj::hash() const
{
	return((M_HASH) this) ;
}

int M_Obj::compare(M_PCO pco) const
{
	if (this> pco) return(1) ;		// I am greater than him
	if (this< pco) return(-1) ;		// I am smaller than him
	return(0) ;				// He and I are equal
}

void M_Obj::error(const char *pszEmsg)
{
	M_Throw::FatalError(pszEmsg) ;
	/*NOTREACHED*/
}

int M_Obj::operator <= (M_Obj &o)
{
	return(compare(&o)<= 0) ;
}

int M_Obj::operator >= (M_Obj &o)
{
	return(compare(&o)>= 0) ;
}

int M_Obj::operator <  (M_Obj &o)
{
	return(compare(&o)< 0) ;
}

int M_Obj::operator >  (M_Obj &o)
{
	return(compare(&o)> 0) ;
}

int M_Obj::operator == (M_Obj &o)
{
	return(isEqual(&o)) ;
}

int M_Obj::operator != (M_Obj &o)
{
	return(!isEqual(&o)) ;
}

// end of mobj.cpp
