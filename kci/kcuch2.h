// kcUrlChannel2

#ifndef		INC_KCUCH2_H
#define		INC_KCUCH2_H

#include	<windows.h>
#include	<wininet.h>


//
//
//

enum kcUrlConduit2PumpRc {
	kcUrlConduit2PumpRc_Ok,
	kcUrlConduit2PumpRc_Stop
} ;

enum kcUrlConduit2PumpDataType {
	kcUrlConduit2PumpDataType_HttpHeader,
	kcUrlConduit2PumpDataType_HttpData
} ;

class kcUrlConduit2
{
	public:
		virtual ~kcUrlConduit2() {}
		virtual kcUrlConduit2PumpRc dataPump(
			kcUrlConduit2PumpDataType,
			void *dataPtr,
			DWORD dataLength
		)= 0 ;
} ;


//
//
//

struct kcUrlChannel2CallbackInfo
{

	kcUrlChannel2CallbackInfo(
		HINTERNET hInternet,
		DWORD dwContext,
		DWORD dwInternetStatus,
		LPVOID lpvStatusInformation,
		DWORD dwStatusInformationLength
	) :
		m_hInternet(hInternet),
		m_dwContext(dwContext),
		m_dwInternetStatus(dwInternetStatus),
		m_lpvStatusInformation(lpvStatusInformation),
		m_dwStatusInformationLength(dwStatusInformationLength)
	{
	}

    HINTERNET m_hInternet ;
    DWORD m_dwContext ;
    DWORD m_dwInternetStatus ;
    LPVOID m_lpvStatusInformation ;
    DWORD m_dwStatusInformationLength ;

} ;

enum kcUrlChannel2Rc {
	kcUrlChannel2Rc_Success,
	kcUrlChannel2Rc_Failure,
	kcUrlChannel2Rc_NotOpen,
	kcUrlChannel2Rc_NotConnected,
	kcUrlChannel2Rc_NoCallback
} ;

class kcUrlChannel2
{
	public:

		kcUrlChannel2(kcUrlConduit2 *conduit2Ptr) ;
		~kcUrlChannel2() ;
		kcUrlChannel2Rc get(const string &urlString) ;

	private:

		friend VOID CALLBACK kcUrlChannel2CallbackFn(
			HINTERNET hInternet,
			DWORD dwContext,
			DWORD dwInternetStatus,
			LPVOID lpvStatusInformation,
			DWORD dwStatusInformationLength
		) ;

		void sysCallback(const kcUrlChannel2CallbackInfo &) ;
		void getComplete() ;
		kcUrlChannel2Rc disconnect() ;
		inline kcBool isOpen() { return(m_hInternetRoot != NULL) ; }
		inline kcBool isConnected() { return(m_hInternetConnect != NULL) ; }
		inline void resetConnect() { m_hInternetConnect= NULL ; }
		inline void resetHandles() { m_hInternetRoot= m_hInternetConnect= NULL ; }
		HINTERNET m_hInternetRoot, m_hInternetConnect ;
		kcUrlConduit2 *m_conduit2Ptr ;
		INTERNET_STATUS_CALLBACK m_isb ;
} ;

// #ifndef INC_KCUCH2_H
#endif

// end of kcuch2.h

