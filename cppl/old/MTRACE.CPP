#define	Revision		85
#define	M_TRACE_MODULEID	"mtrace"

/*

	otrace.cpp		qad-NewARC21 C++ Source: "M_Trace" class
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	#includes
*/

#include	"mtrace.h"
#include	"a21flog.h"

#include	<stdarg.h>
#include	<string.h>

#ifndef	M_NOTRACE

/*
	Class "M_TraceMgr":	static data
*/

int M_TraceMgr::m_a21fnum= A21LFID_GUI ;

/*
	Class "M_TraceMgr":	methods
*/

M_TraceMgr::~M_TraceMgr()
{
	reset() ;
}

M_TraceMgr::M_TraceMgr(int column0)
:	m_pszName(0),
	m_tsff(0),
	m_otf(M_TF_NONE),
	m_column(column0),
	m_bHaveLog(M_CFALSE)
{
	// nothing to do
}

M_TraceMgr::M_TraceMgr(
	const char *pszName,
	M_SFLAGS tsff,
	M_TRACEFLAGS otf,
	int column0
)
:	m_pszName(0),
	m_tsff(tsff),
	m_otf(otf),
	m_column(column0),
	m_bHaveLog(M_CFALSE)
{
	setname(pszName) ;
	open() ;
}

void M_TraceMgr::reset()
{
	close() ;
	setname(0) ;
}

void M_TraceMgr::setname(const char *pszName)
{
	if (m_pszName) {				// free old name if have one
		delete [] m_pszName ;
		m_pszName= 0 ;
	}
	if (pszName) {				// allocate new name if have
		int len= strlen(pszName) ;
		m_pszName= new char[len+1] ;
		strcpy(m_pszName, pszName) ;
	}
}

void M_TraceMgr::reopen(
	const char *pszName,
	M_SFLAGS tsff,
	M_TRACEFLAGS otf,
	int column0
) {
	reset() ;
	m_tsff= tsff ;
	m_otf= otf ;
	m_column= column0 ;
	setname(pszName) ;
	open() ;
}

void M_TraceMgr::close()
{
	if (m_bHaveLog) {
		lineout("close") ;
		a21flog_close(m_a21fnum) ;
		m_bHaveLog= M_CFALSE ;
	}
}

void M_TraceMgr::open()
{
	if (!m_bHaveLog && m_pszName) {		// must have name && not open
		if (
			a21flog_open(m_pszName, m_a21fnum, (int) m_tsff)
		     == HA21LOG_ERROR
		) {
			M_Throw::FatalError("can't open trace log") ;
			/*NOTREACHED*/
		}
		m_bHaveLog= M_CTRUE ;
		lineout("open") ;
	}
}

void M_TraceMgr::lineout(const char *pline)
{
	A21LMID lmid ;
	makelm(&lmid, M_TRACE_MODULEID) ;
	a21flog_printf(&lmid, "%s\n", pline) ;
}

void M_TraceMgr::makelm(PA21LMID pa21lmid, const char *pszName) const
{
	pa21lmid->fnum= m_a21fnum ;
	if (!pszName) {
		pa21lmid->prefix[0]= '\0' ;
		return ;
	}
	strncpy(pa21lmid->prefix, pszName, A21LDV_PFXLEN-2) ;
	strcat(pa21lmid->prefix, ": ") ;
}

int M_TraceMgr::getcolumn() const
{
	return(m_column) ;
}

void M_TraceMgr::setcolumn(int n)
{
	m_column= n ;
}

void M_TraceMgr::prefix(M_Trace *pot)
{
	LPA21LMID lpm= (pot->getotu()).getlpm() ;
	const char *pszPfx= pot->getname() ;
	a21flog_setlmgn(lpm, pot->getcolumn()) ;
	M_SBOOL bPrefixed= M_CFALSE ;
	if (pot->GetObjThis()) {
		a21flog_printf(lpm, "[%lX] ", (M_SDWORD) pot->GetObjThis()) ;
		bPrefixed= M_CTRUE ;
	}
	if (pszPfx) {
		a21flog_puts(lpm, pszPfx) ;
		bPrefixed= M_CTRUE ;
	}
	if (bPrefixed) {
		a21flog_puts(lpm, ": ") ;
	}
}

#define	OK2PRINT (m_bHaveLog && ((otf & m_otf & otu.getotf()) != 0))

void M_TraceMgr::puts(
	M_Trace *pot,
	M_TRACEFLAGS otf,
	const char *psz
) {
	M_TraceUnit &otu= pot->getotu() ;

	if (OK2PRINT) {
		prefix(pot) ;
		a21flog_puts(otu.getlpm(), psz) ;
	}
}

void M_TraceMgr::vprintf(
	M_Trace *pot,
	M_TRACEFLAGS otf,
	const char *pszFmt,
	va_list vl
) {
	M_TraceUnit &otu= pot->getotu() ;

	if (OK2PRINT) {
		prefix(pot) ;
		a21flog_vprintf(otu.getlpm(), pszFmt, vl) ;
	}
}

/*
	Class "M_TraceUnit":		methods
*/

M_TraceUnit::~M_TraceUnit()
{
	if ((m_otf & M_TF_GLOBAL) != 0) {
		a21flog_tabto(&m_lm, 16) ;
		a21flog_printf(&m_lm, "stop\n") ;
	}
}

M_TraceUnit::M_TraceUnit(
	const char *pszName,
	M_TRACEFLAGS otf,
	int r
)
:	m_otf(otf)
{
	M_TRACE_PMGR->makelm(&m_lm, pszName) ;
	if ((m_otf & M_TF_GLOBAL) != 0) {
		a21flog_tabto(&m_lm, 16) ;
		a21flog_printf(
			&m_lm,
			"start: flags=0x%lX, Revision=%d\n",
			(long) otf,
			r
		) ;
	}
}

const char *M_TraceUnit::getname() const
{
	return(m_lm.prefix) ;
}
 
M_TRACEFLAGS M_TraceUnit::getotf() const
{
	return(m_otf) ;
}
 
LPA21LMID M_TraceUnit::getlpm() const {
	return((LPA21LMID) &m_lm) ;
}

/*
	Class "M_Trace":		methods
*/

M_Trace::~M_Trace()
{
	puts(M_TF_EPILOG, "exited\n") ;
	M_TRACE_PMGR->setcolumn(m_column) ;
}

M_Trace::M_Trace(
	const void *pObjThis,
	M_TraceUnit &otu,
	const char *pszName,
	int indent
)
:	m_otu(otu),
	m_pszName(pszName),
	m_pObjThis(pObjThis)
{
	m_column= M_TRACE_PMGR->getcolumn() ;
	M_TRACE_PMGR->setcolumn(m_column + indent) ;
	puts(M_TF_PROLOG, "entered\n") ;
}

void M_Trace::close()
{
	M_TRACE_PMGR->close() ;
}

#undef	POBJ

void M_Trace::puts(M_TRACEFLAGS otf, const char *psz)
{
	M_TRACE_PMGR->puts(this, otf, psz) ;
}

void M_Trace::vprintf(
	M_TRACEFLAGS otf,
	const char *pszFmt,
	va_list vl
) {
	M_TRACE_PMGR->vprintf(this, otf, pszFmt, vl) ;
}

void M_Trace::printf(M_TRACEFLAGS otf, const char *pszFmt, ...)
{
	va_list vl ;

	va_start(vl, pszFmt) ;
	vprintf(otf, pszFmt, vl) ;
}

void M_Trace::puts(const char *psz)
{
	puts(M_TF_DEBUG, psz) ;
}

void M_Trace::vprintf(const char *pszFmt, va_list vl)
{
	vprintf(M_TF_DEBUG, pszFmt, vl) ;
}

void M_Trace::printf(const char *pszFmt, ...)
{
	va_list vl ;
	va_start(vl, pszFmt) ;
	vprintf(M_TF_DEBUG, pszFmt, vl) ;
}

// #ifndef M_NOTRACE
#endif

// end of otrace.cpp
