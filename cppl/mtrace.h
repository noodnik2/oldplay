//	Revision:		85

#ifndef	M_TRACE_H_INC
#define	M_TRACE_H_INC

/*	

	mtrace.h		qad-NewARC21 C++ Header: "M_Trace" class
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	sub-#includes
*/

#ifndef	M_NOTRACE
#include	"mthrow.h"
#include	"a21flogt.h"
#include	"musrdef.h"
#include	<stdarg.h>
#endif

/*
	public macros
*/

// trace flags

#define	M_TF_NONE		0
#define	M_TF_ALL		~0

#define	M_TF_PROLOG	0x0001		// function entry
#define	M_TF_EPILOG	0x0002		// function exit
#define	M_TF_GLOBAL    	0x0004		// global module messages
#define	M_TF_DEBUG     	0x0008		// normal print
#define	M_TF_DETAIL	0x0010		// extra detail
#define	M_TF_UNUSED1	0x0020
#define	M_TF_ERROR 	0x0040 		// errors
#define	M_TF_WARNING	0x0080 		// warnings
#define	M_TF_INPUT  	0x0100 		// args
#define	M_TF_OUTPUT 	0x0200 		// return vals
#define	M_TF_UNUSED2	0x0400
#define	M_TF_UNUSED3	0x0800
#define	M_TF_USER1	0x1000
#define	M_TF_USER2	0x2000
#define	M_TF_USER3	0x4000
#define	M_TF_USER4	0x8000

// end of trace flags 

#ifdef	M_NOTRACE
#define	M_TRACE(n)
#define	M_TRACEOBJ(n)
#define	M_TDIAG(d)
#define	M_TCOND(d, c)
#else
#define	M_TRACE(n)      	M_Trace M_TFN(0, M_TUNIT, n)
#define	M_TRACEOBJ(n)      	M_Trace M_TFN(this, M_TUNIT, n)
#define	M_TDIAG(d)		M_TFN.##d
#define	M_TCOND(f, c)		if (M_TFN.isflag(f)) { c ; }
#endif

/*
	private macros
*/

#ifndef	M_NOTRACE
#define	M_TFN			otracefn_fn
#define	M_TUNIT			otraceunit_unit()
#define	M_TRACE_PMGR		M_TraceMgr::GetGlobalObject()

/*
	local types, forward class definitions
*/
typedef M_SFLAGS M_TRACEFLAGS ;

/*
	forward classes
*/
class	M_Trace ;

/*
	Class declarations
*/

/*
	Class "M_TraceMgr"
*/
class M_TraceMgr
{
	public:
		~M_TraceMgr() ;
		M_TraceMgr(int column0= 0) ;
		M_TraceMgr(
			const char *pszTFn,
			M_SFLAGS tsff= A21LF_NONE,
			M_TRACEFLAGS otf= M_TF_ALL,
			int column0= 0
		) ;

		static M_TraceMgr *GetGlobalObject() ;

		void makelm(PA21LMID pa21lmid, const char *pszName) const ;
		int getcolumn() const ;
		void setcolumn(int column) ;
		void puts(M_Trace *pot, M_TRACEFLAGS otf, const char *psz) ;
		void lineout(const char *pline) ;
		void vprintf(
			M_Trace *pot,
			M_TRACEFLAGS otf,
			const char *pszFmt,
			va_list vl
		) ;
		void setname(const char *) ;
		void reset() ;
		void open() ;
		void reopen(
			const char *pszTFn,
			M_SFLAGS tsff= A21LF_NONE,
			M_TRACEFLAGS otf= M_TF_ALL,
			int column0= 0
		) ;
		void close() ;

	private:
		void prefix(M_Trace *pot) ;

		static int m_a21fnum ;
		int m_column ;
		M_TRACEFLAGS m_otf ;
		M_SFLAGS m_tsff ;
		char *m_pszName ;
		M_SBOOL m_bHaveLog ;
} ;

/*
	Class "M_TraceUnit"
*/
class M_TraceUnit
{
	public:
		~M_TraceUnit() ;
		M_TraceUnit(const char *pszName, M_TRACEFLAGS otf, int r) ;
		const char *getname() const ;
		M_TRACEFLAGS getotf() const ;
		LPA21LMID getlpm() const ;

	private:
		A21LMID m_lm ;
		M_TRACEFLAGS m_otf ;
} ;

/*
	Class "M_Trace"
*/
class M_Trace
{
	public:
		~M_Trace() ;
		M_Trace(
			const void *pObjThis,
			M_TraceUnit &otu,
			const char *pszName= 0,
			int indent= 1
		) ;
		
		int getcolumn() const ;
		M_TraceUnit &getotu() const { return(m_otu) ; }
		const char *getname() const ;

		void puts(M_TRACEFLAGS otf, const char *psz) ;
		void vprintf(
			M_TRACEFLAGS otf,
			const char *pszFmt,
			va_list vl
		) ;
		void printf(M_TRACEFLAGS otf, const char *pszFmt, ...) ;

		void puts(const char *psz) ;
		void vprintf(const char *pszFmt, va_list vl) ;
		void printf(const char *pszFmt, ...) ;
		
		M_SBOOL isflag(M_TRACEFLAGS otf) const ;
		M_TRACEFLAGS getotf() const ;
		void close() ;
		const void *GetObjThis() const ;

	private:
		const char *m_pszName ;
		M_TraceUnit &m_otu ;
		int m_column ;
		const void *m_pObjThis ;

} ;

/*
	methods implemented in-line
*/

inline const char *M_Trace::getname() const { return(m_pszName) ; }
inline int M_Trace::getcolumn() const { return(m_column) ; }
inline M_TRACEFLAGS M_Trace::getotf() const { return(m_otu.getotf()) ; }
inline M_SBOOL M_Trace::isflag(M_TRACEFLAGS otf) const {
	return(	(M_SBOOL)((getotf() & otf) != 0) ) ;
}
inline const void *M_Trace::GetObjThis() const { return(m_pObjThis) ; }

/*
	per-module setup (conditional upon user-#defined macros)
*/
#ifdef	M_TRACE_MODULEID

#ifndef	M_TRACE_FLAGVAL
#define	M_TRACE_FLAGVAL		M_TF_ALL
#endif

#ifndef	Revision
#define	Revision		0
#endif

static M_TraceUnit &M_TUNIT
{
	static M_TraceUnit tu(M_TRACE_MODULEID, M_TRACE_FLAGVAL, Revision) ;
	return(tu) ;
}

#endif

// #ifndef M_NOTRACE
#endif

// #ifndef M_TRACE_H_INC
#endif

// end of mtrace.h
