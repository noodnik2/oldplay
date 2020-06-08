// kcicmd3.h

#ifndef		INC_KCICMD3_H
#define		INC_KCICMD3_H

#include	"kccmd.h"
#include	"kcuch2.h"

class kcUrlConduit2ForInternet3 : public kcUrlConduit2
{
	public:
		virtual kcUrlConduit2PumpRc dataPump(
			kcUrlConduit2PumpDataType,
			void *dataPtr,
			DWORD dataLength
		) ;
} ;

class kcInternet3Cmd : public kcCmd
{
	public:
		kcInternet3Cmd(const kcCmdLine &) ;
		~kcInternet3Cmd() ;
		virtual kcCmdRc Run() ;

	private:
		kcUrlConduit2ForInternet3 *m_conduit2Ptr ;
		kcUrlChannel2 *m_channel2Ptr ;

} ;

// #ifndef		INC_KCICMD3_H
#endif

// end of kcicmd3.h
