// kcuch2.cpp

#include	"main.h"
#include	"kcuch2.h"

VOID CALLBACK kcUrlChannel2CallbackFn(
    HINTERNET hInternet,
    DWORD dwContext,
    DWORD dwInternetStatus,
    LPVOID lpvStatusInformation,
    DWORD dwStatusInformationLength
) {
	kcUrlChannel2 *pChannel= (kcUrlChannel2 *) dwContext ;
	if (!pChannel) return ;
	pChannel->sysCallback(
		kcUrlChannel2CallbackInfo(
			hInternet,
			dwContext,
			dwInternetStatus,
			lpvStatusInformation,
			dwStatusInformationLength
		)
	) ;
}

void kcUrlChannel2::sysCallback(const kcUrlChannel2CallbackInfo &ci)
{
	cout	<< "kcUrlChannel2::sysCallback(" ;

	switch(ci.m_dwInternetStatus) {

#define	EXPAND(x)	case x: cout << #x ; break

		EXPAND(INTERNET_STATUS_CONNECTED_TO_SERVER) ;
		EXPAND(INTERNET_STATUS_CLOSING_CONNECTION) ;
//		EXPAND(INTERNET_STATUS_CONNECTED_TO_SERVER) ;
		EXPAND(INTERNET_STATUS_CONNECTING_TO_SERVER) ;
		EXPAND(INTERNET_STATUS_CONNECTION_CLOSED) ;
		EXPAND(INTERNET_STATUS_CTL_RESPONSE_RECEIVED) ;
		EXPAND(INTERNET_STATUS_HANDLE_CLOSING) ;
		EXPAND(INTERNET_STATUS_HANDLE_CREATED) ;
		EXPAND(INTERNET_STATUS_INTERMEDIATE_RESPONSE) ;
		EXPAND(INTERNET_STATUS_NAME_RESOLVED) ; 
		EXPAND(INTERNET_STATUS_PREFETCH) ;
		EXPAND(INTERNET_STATUS_RECEIVING_RESPONSE) ;
		EXPAND(INTERNET_STATUS_REDIRECT) ;
		EXPAND(INTERNET_STATUS_REQUEST_COMPLETE) ;
		EXPAND(INTERNET_STATUS_REQUEST_SENT) ; 
		EXPAND(INTERNET_STATUS_RESOLVING_NAME) ;
		EXPAND(INTERNET_STATUS_RESPONSE_RECEIVED) ;
		EXPAND(INTERNET_STATUS_SENDING_REQUEST) ;
		EXPAND(INTERNET_STATUS_STATE_CHANGE) ;

		default: cout << ci.m_dwInternetStatus ; break ;

	}

	cout	<< ") ;\n" ;
}

kcUrlChannel2::kcUrlChannel2(kcUrlConduit2 *conduit2Ptr)
	: m_conduit2Ptr(conduit2Ptr), m_isb(INTERNET_INVALID_STATUS_CALLBACK)
{
	resetHandles() ;
	m_hInternetRoot= ::InternetOpen(
		"kcuch2.cpp",
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		INTERNET_FLAG_ASYNC
	) ;
	if (!isOpen()) return ;
	m_isb= ::InternetSetStatusCallback(
		m_hInternetRoot,
		(INTERNET_STATUS_CALLBACK) kcUrlChannel2CallbackFn
	) ;
}

kcUrlChannel2::~kcUrlChannel2()
{
	if (isOpen()) {
		if (m_isb != INTERNET_INVALID_STATUS_CALLBACK) {
			::InternetSetStatusCallback(m_hInternetRoot, m_isb) ;
		}
		::InternetCloseHandle(m_hInternetRoot) ;
		if (isConnected()) {
			::InternetCloseHandle(m_hInternetConnect) ;
		}
	}
}

kcUrlChannel2Rc kcUrlChannel2::get(
	const string &urlString
) {

	if (m_isb == INTERNET_INVALID_STATUS_CALLBACK) {
		return(kcUrlChannel2Rc_NoCallback) ;
	}

	if (!isOpen()) {
		return(kcUrlChannel2Rc_NotOpen) ;
	}
	
	m_hInternetConnect= ::InternetOpenUrl(
		m_hInternetRoot,
		urlString.c_str(),
		NULL,
		0,
		INTERNET_FLAG_RAW_DATA,
		(DWORD) ((void *) this)
	) ;
	if (!isConnected()) return(kcUrlChannel2Rc_NotConnected) ;

	return(kcUrlChannel2Rc_Success) ;
}

void kcUrlChannel2::getComplete()
{ 
	char inputBuffer[512] ;
	DWORD numBytesRead= sizeof(inputBuffer) ;
	DWORD dwIndex= 0 ;

	if (
		!::HttpQueryInfo(
			m_hInternetConnect,
			HTTP_QUERY_RAW_HEADERS_CRLF,
			inputBuffer,
			&numBytesRead,
			&dwIndex
		)
	) {
		disconnect() ;
		return ;
	}

	if (m_conduit2Ptr) {
		if (
			m_conduit2Ptr->dataPump(
				kcUrlConduit2PumpDataType_HttpHeader,
				inputBuffer,
				numBytesRead
			) != kcUrlConduit2PumpRc_Ok
		) {
			disconnect() ;
			return ;
		}
	}

	return ;
}

#if 0
	while(/*CONSTCOND*/1) {
		if (
			!::InternetReadFile(
				m_hInternetConnect,
				inputBuffer,
				sizeof(inputBuffer),
				&numBytesRead
			)
		) {
			disconnect() ;
			return(kcUrlChannel2Rc_Failure) ;
		}
		if (numBytesRead == 0) break ;
		if (m_conduit2Ptr) {
			if (
				m_conduit2Ptr->dataPump(
					kcUrlConduit2PumpDataType_HttpData,
					inputBuffer,
					numBytesRead
				) != kcUrlConduit2PumpRc_Ok
			) {
				break ;
			}
		}
	}

	disconnect() ;
	return(kcUrlChannel2Rc_Success) ;
}
#endif
	
kcUrlChannel2Rc kcUrlChannel2::disconnect()
{
	if (!isOpen() || !isConnected()) return(kcUrlChannel2Rc_NotOpen) ;
	::InternetCloseHandle(m_hInternetConnect) ;
	resetConnect() ;
	return(kcUrlChannel2Rc_Success) ;
}


// end of kcuch2.cpp