// kcicmd2.h

#ifndef		INC_KCICMD2_H
#define		INC_KCICMD2_H

#include	"kccmd.h"
#include	"kcuch.h"

class kcUrlConduitForInternet2 : public kcUrlConduit
{
	public:
		virtual kcUrlConduitPumpRc dataPump(
			kcUrlConduitPumpDataType,
			void *dataPtr,
			DWORD dataLength
		) ;
} ;

class kcInternet2Cmd : public kcCmd
{
	public:
		kcInternet2Cmd(const kcCmdLine &) ;
		~kcInternet2Cmd() ;
		virtual kcCmdRc Run() ;

	private:
		kcUrlConduitForInternet2 *m_conduitPtr ;
		kcUrlChannel *m_channelPtr ;

} ;

// #ifndef		INC_KCICMD2_H
#endif

// end of kcicmd2.h
