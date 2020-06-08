//	Revision:      		27

#ifndef	M_OBJ_H_INC
#define	M_OBJ_H_INC

/*

	mobj.h

*/

/*
	sub-#includes
*/
#include	"mcls.h"

/*
	forward class declarations
*/
class M_Obj ;
class M_Archive ;

/*
	Class Declaration
*/

class M_Obj
{
	M_CLSDEFS() ;

	public:
		typedef void (M_Obj::*M_METHOD)() ;
		void perform(M_METHOD om) ;
		void perform(M_METHOD om, M_SLONG lparam) ;
		void perform(M_METHOD om, M_SLONG lparam1, M_SLONG lparam2) ;

		M_PO deepCopy() ;      			// remove this??
		M_PO copy() ;	       			// remove this??
		virtual M_SBOOL copyTo(M_PO po) ;	// remove this??

		virtual void freeSelf() ;		// does "delete this"
		virtual void reset() ;			// resets to "empty"

		virtual M_Obj &Clone() const ;		// new: 27.5.95
		virtual ~M_Obj() ;			// new: 27.5.95

		virtual void getFrom(M_Archive &oa) ;
		virtual void putTo(M_Archive &oa) ;
		virtual M_HASH hash() const ;
		virtual M_SBOOL isEqual(M_PCO pco) const ;
		virtual int compare(M_PCO pco) const ;
		virtual void error(const char *pszEmsg) ;

		int operator <= (M_Obj &o) ;
		int operator >= (M_Obj &o) ;
		int operator <  (M_Obj &o) ;
		int operator >  (M_Obj &o) ;
		int operator == (M_Obj &o) ;
		int operator != (M_Obj &o) ;
} ;

/*
	typedefs
*/
typedef void (M_Obj::*M_METHOD)() ;
typedef void (M_Obj::*M_METHOD0)() ;
typedef void (M_Obj::*M_METHOD1)(M_SLONG lparam) ;
typedef void (M_Obj::*M_METHOD2)(M_SLONG lparam1, M_SLONG lparam2) ;
typedef int (M_Obj::*M_METHODCMP)(M_PCO pco) const ;

// #ifndef M_OBJ_H_INC
#endif

// end of mobj.h
