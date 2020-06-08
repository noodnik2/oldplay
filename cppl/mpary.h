//	Revision:		71

#ifndef	M_PARY_H_INC
#define	M_PARY_H_INC

/*

	mpary.h			qad-NewARC21 C++ Header: "M_POAry" class
				Copyright (C) 1995, Contahal Ltd.

	
	VERY IMPORTANT NOTE:
	
	The class defined here, "M_POAry", is similar to the "M_Str" class
	in that it manages an array of items that is terminated by a null
	item (an item whose value is zero).

	For this reason, unexpected results will occur if the user attempts
	to assign a zero value to an element of the array!
*/

/*
	sub-#includes
*/
#include	"mpa.h"
#include	<memory.h>

/*
	#definitions
*/
#define	M_POARYMIN_EXPAND		8
#define	M_POARYDATA_PNIL		M_POAryData_pNil()

/*
	Class declarations
*/

/*
	Class "M_POAryData"
*/

class M_POAryData
{
	public:
		~M_POAryData() ;
		M_POAryData() ;

		static M_POAryData *alloc() ;
		static M_POAryData *alloc(unsigned short nalloc) ;

		int length() const ;
		int allocated() const ;
		const M_PO *getm_poa() const ;
		M_SBOOL isEmpty() const ;

		int load(const M_PO *m_poa, int len) ;
		int load(const M_POAryData *pd) ;
		void truncate() ;
		M_PO *getwm_poa() ;
		int insert(const M_PO *m_poa, int len, int pos) ;
		M_PO removeAt(int) ;
		void freeSelf() ;

	private:
		M_POAryData(unsigned short nalloc) ;
		void *operator new(size_t, unsigned short nalloc) ;

		unsigned short m_nlen ;	   // actual length of array
		unsigned short m_nalloc ;  // # of allocated entries
		M_PO m_po[1] ;		   // array starts here: last -> 0
} ;

extern M_POAryData *M_POARYDATA_PNIL ;

inline M_POAryData::~M_POAryData() {}
inline void M_POAryData::truncate() { m_nlen= 0 ; }	// improve!!!
inline int M_POAryData::load(const M_PO *m_poa, int len) {
	return(insert(m_poa, len, m_nlen= 0)) ;
}
inline int M_POAryData::load(const M_POAryData *pd) {
	return(load(pd->getm_poa(), pd->length())) ;
}
inline M_PO *M_POAryData::getwm_poa() { return(&m_po[0]) ; }

inline int M_POAryData::length() const { return(m_nlen) ; }
inline int M_POAryData::allocated() const { return(m_nalloc) ; }
inline const M_PO *M_POAryData::getm_poa() const { return(&m_po[0]) ; }
inline M_SBOOL M_POAryData::isEmpty() const { return((M_SBOOL) (m_nlen == 0)) ; }

/*
	Class "M_POAry"
*/

class M_POAry : public M_Obj
{
	M_CLSDEFS() ;

	public:
		~M_POAry() ;
		M_POAry() ;
		M_POAry(M_PO po) ;
		M_POAry(const M_PO *m_poa) ;
		M_POAry(const M_PO *m_poa, int len) ;
		M_POAry(const M_POAry &oa) ;

				      	// base class method replacements
		void reset() ;
		void resetAndFreeContents() ;
		int compare(M_PCO pco) const ;

					// replaced operators
		int operator == (const M_PO *m_poa) const ;
		int operator != (const M_PO *m_poa) const ;
		int operator >  (const M_PO *m_poa) const ;
		int operator <  (const M_PO *m_poa) const ;
		int operator >= (const M_PO *m_poa) const ;
		int operator <= (const M_PO *m_poa) const ;
		
					// new class methods
		void add(M_PO po) ;
		void add(const M_PO *m_poa) ;
		void add(const M_POAry *pm_poary) ;

		virtual void add(const M_PO *m_poa, int len) ;
		virtual int addentry(M_PO po) ;

		void shrink() ;
		void expand(int n) ;
		void realloc(int n) ;

		int search(M_PCO pco, M_METHODCMP omc) const ;

		M_PO &poAtRef(int idx) ;    // danger: user could set to 0!!
		M_PO poAt(int idx) const ;
		M_PO removeAt(int idx) ;
		int length() const ;
		int allocated() const ;
		int isEmpty() const ;
		void doPerform(M_METHOD om) const ;

		M_SBOOL poIn(M_PCO pco) const ;
		int compareM_POAry(const M_PO *m_poa) const ;

		const M_PO *getm_poa() const ;       // return ptr to M_PO []
		M_PO *getm_poa(M_PO *m_poa) const ;  // copy M_PO [] to *m_poa
		M_PO *getm_poa(M_PO *m_poa, int maxlen) const ;
	
		int putm_poa(const M_PO *m_poa) ;
		int putm_poa(const M_POAry *pos) ;

						// new operators
		int operator !() const ;
		M_PO &operator[](int idx) ;    // danger: user could set to 0!!
		M_PO operator[](int idx) const ;

		M_POAry &operator+= (const M_POAry &os) ;
		M_POAry &operator+= (const M_PO *m_poa) ;
		M_POAry &operator+= (M_PO po) ;

		M_POAry& operator= (const M_POAry &os) ;
		M_POAry& operator= (const M_PO *m_poa) ;
		M_POAry& operator= (M_PO po) ;

		operator const M_PO *() const ;
	
	protected:
		M_PO *getwm_poa() ;		// write-access to ptr array
		int insert(M_PO po, int pos) ;
		int insert(const M_PO *m_poa, int len, int pos) ;

	private:
		int construct(const M_PO *m_poa) ;
		int construct(const M_PO *m_poa, int len) ;
		int reconstruct(const M_PO *m_poa) ;
		int reconstruct(const M_PO *m_poa, int len) ;
		void reload(const M_PO *m_poa) ;

		M_POAryData *m_pd ;
} ;

inline M_POAry::~M_POAry() { M_POAry::reset() ; }
inline M_POAry::M_POAry() : m_pd(M_POARYDATA_PNIL) {}
inline M_POAry::M_POAry(M_PO po) { construct(&po, 1) ; }
inline M_POAry::M_POAry(const M_PO *m_poa) { construct(m_poa) ; }
inline M_POAry::M_POAry(const M_PO *m_poa, int len) { construct(m_poa, len) ; }
inline M_POAry::M_POAry(const M_POAry &oa) { construct(oa.getm_poa(), oa.length()) ; }

inline void M_POAry::shrink() { realloc(m_pd->length()) ; }
inline void M_POAry::expand(int n) { realloc(m_pd->allocated() + n) ; }

inline M_PO *M_POAry::getwm_poa() { return(m_pd->getwm_poa()) ; }
inline const M_PO *M_POAry::getm_poa() const { return(m_pd->getm_poa()) ; }
inline int M_POAry::putm_poa(const M_PO *m_poa) { return(reconstruct(m_poa)) ; }

inline M_PO M_POAry::poAt(int idx) const {
	if (idx>= length()) return(0) ;
	return((getm_poa())[idx]) ;
}

inline M_PO M_POAry::removeAt(int idx) { return(m_pd->removeAt(idx)) ; } 
inline int M_POAry::allocated() const { return(m_pd->allocated()) ; }
inline int M_POAry::length() const { return(m_pd->length()) ; }
inline int M_POAry::isEmpty() const { return(m_pd->isEmpty()) ; }

inline int M_POAry::insert(M_PO po, int pos) { return(insert(&po, 1, pos)) ; }

inline void M_POAry::add(M_PO po) { addentry(po) ; }
inline void M_POAry::add(const M_PO *poa) { add(poa, m_poalen(poa)) ; }

inline void M_POAry::add(const M_POAry *pm_poary) {
	add(pm_poary->getm_poa(), pm_poary->length()) ;
}

inline M_SBOOL M_POAry::poIn(M_PCO pco) const {
	return(m_poafind(getm_poa(), pco, length())>= 0) ;
}

inline int M_POAry::search(M_PCO pco, M_METHODCMP omc) const {
	return(m_poalinsearch(getm_poa(), pco, length(), omc)) ;
}

inline int M_POAry::operator == (const M_PO *m_poa) const {
	return(compareM_POAry(m_poa) == 0) ;
}
	
inline int M_POAry::operator != (const M_PO *m_poa) const {
	return(compareM_POAry(m_poa) != 0) ;
}

inline int M_POAry::operator >  (const M_PO *m_poa) const {
	return(compareM_POAry(m_poa) > 0) ;
}

inline int M_POAry::operator <  (const M_PO *m_poa) const {
	return(compareM_POAry(m_poa) < 0) ;
}

inline int M_POAry::operator >= (const M_PO *m_poa) const {
	return(compareM_POAry(m_poa) >= 0) ;
}

inline int M_POAry::operator <= (const M_PO *m_poa) const {
	return(compareM_POAry(m_poa) <= 0) ;
}

inline M_POAry &M_POAry::operator+= (const M_POAry &os) {
	add(&os) ;
	return(*this) ;
}

inline M_POAry &M_POAry::operator+= (const M_PO *m_poa) {
	add(m_poa) ;
	return(*this) ;
}

inline M_POAry &M_POAry::operator+= (M_PO po) {
	add(po) ;
	return(*this) ;
}

inline M_POAry& M_POAry::operator= (const M_POAry &os) {
	if (&os != this) reconstruct(os.getm_poa(), os.length()) ;
	return(*this) ;
}

inline M_POAry& M_POAry::operator= (const M_PO *m_poa) {
	reconstruct(m_poa) ;
	return(*this) ;
}

inline M_POAry& M_POAry::operator= (M_PO po) {
	reconstruct(&po, 1) ;
	return(*this) ;
}

inline M_PO &M_POAry::operator[](int idx) { return(poAtRef(idx)) ; }
inline M_PO M_POAry::operator[](int idx) const { return(poAt(idx)) ; }

inline int M_POAry::operator !() const { return(isEmpty()) ; }
inline M_POAry::operator const M_PO *() const { return(getm_poa()) ; }

/*
	global inline function definitions
*/

inline M_POAry join(const M_POAry &os1, const M_POAry &os2) {
	M_POAry os(os1) ;
	os.add(os2) ;
	return(os) ;
}

inline M_POAry join(const M_PO *m_poa, const M_POAry &os2) {
	M_POAry os(m_poa) ;
	os.add(os2) ;
	return(os) ;
}

inline M_POAry join(const M_POAry &os1, const M_PO *m_poa) {
	M_POAry os(os1) ;
	os.add(m_poa) ;
	return(os) ;
}

inline M_POAry operator+ (const M_POAry &os1, const M_POAry &os2) {
	return(join(os1, os2)) ;
}

inline M_POAry operator+ (const M_PO *m_poa, const M_POAry &os2) {
	return(join(m_poa, os2)) ;
}

inline M_POAry operator+ (const M_POAry &os1, const M_PO *m_poa) {
	return(join(os1, m_poa)) ;
}

/*
	global function forward declarations
*/
M_POAry subm_poa(const M_POAry& os1, int offset, int len= 0) ;

// #ifndef M_PARY_H_INC
#endif

// end of mpary.h
