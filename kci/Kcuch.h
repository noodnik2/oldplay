// kcUrlChannel

#ifndef		INC_KCUCH_H
#define		INC_KCUCH_H

#include	<windows.h>
#include	<wininet.h>


//
//
//

enum kcUrlConduitPumpRc {
	kcUrlConduitPumpRc_Ok,
	kcUrlConduitPumpRc_Stop
} ;

enum kcUrlConduitPumpDataType {
	kcUrlConduitPumpDataType_HttpHeader,
	kcUrlConduitPumpDataType_HttpData
} ;

class kcUrlConduit
{
	public:
		virtual ~kcUrlConduit() {}
		virtual kcUrlConduitPumpRc dataPump(
			kcUrlConduitPumpDataType,
			void *dataPtr,
			DWORD dataLength
		)= 0 ;
} ;


//
//
//

enum kcUrlChannelRc {
	kcUrlChannelRc_Success,
	kcUrlChannelRc_Failure,
	kcUrlChannelRc_NotOpen,
	kcUrlChannelRc_NotConnected
} ;

class kcUrlChannel
{
	public:

		kcUrlChannel(kcUrlConduit *conduitPtr) ;
		~kcUrlChannel() ;
		kcUrlChannelRc get(const string &urlString) ;

	private:
		kcUrlChannelRc disconnect() ;
		inline kcBool isOpen() { return(m_hInternetRoot != NULL) ; }
		inline kcBool isConnected() { return(m_hInternetConnect != NULL) ; }
		inline void resetConnect() { m_hInternetConnect= NULL ; }
		inline void resetHandles() { m_hInternetRoot= m_hInternetConnect= NULL ; }
		HINTERNET m_hInternetRoot, m_hInternetConnect ;
		kcUrlConduit *m_conduitPtr ;
} ;

// #ifndef INC_KCUCH_H
#endif

// end of kcuch.h

