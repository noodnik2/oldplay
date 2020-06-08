// kcicmd.h

#ifndef		INC_KCICMD_H
#define		INC_KCICMD_H

#include	"kccmd.h"
#include	"kcicon.h"

class kcInternetCmd : public kcCmd
{
	public:
		kcInternetCmd(const kcCmdLine &) ;
		virtual kcCmdRc Run() ;

	private:
		kcInternetChannel m_channel ;

} ;

// #ifndef		INC_KCICMD_H
#endif

// end of kcicmd.h
