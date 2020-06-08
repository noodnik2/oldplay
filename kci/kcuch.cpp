// kcuch.cpp

#include	"main.h"
#include	"kcuch.h"

kcUrlChannel::kcUrlChannel(kcUrlConduit *conduitPtr)
	: m_conduitPtr(conduitPtr)
{
	resetHandles() ;
	m_hInternetRoot= ::InternetOpen(
		"kcuch.cpp",
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0 // INTERNET_FLAG_OFFLINE
	) ;
}

kcUrlChannel::~kcUrlChannel()
{
	if (isOpen()) {
		::InternetCloseHandle(m_hInternetRoot) ;
		if (isConnected()) {
			::InternetCloseHandle(m_hInternetConnect) ;
		}
	}
}

kcUrlChannelRc kcUrlChannel::get(
	const string &urlString
) {

	if (!isOpen()) return(kcUrlChannelRc_NotOpen) ;
	
	m_hInternetConnect= ::InternetOpenUrl(
		m_hInternetRoot,
		urlString.c_str(),
		NULL,
		0,
		INTERNET_FLAG_RAW_DATA,
		0
	) ;
	if (!isConnected()) return(kcUrlChannelRc_NotConnected) ;

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
		return(kcUrlChannelRc_Failure) ;
	}
	if (m_conduitPtr) {
		if (
			m_conduitPtr->dataPump(
				kcUrlConduitPumpDataType_HttpHeader,
				inputBuffer,
				numBytesRead
			) != kcUrlConduitPumpRc_Ok
		) {
			disconnect() ;
			return(kcUrlChannelRc_Success) ;
		}
	}

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
			return(kcUrlChannelRc_Failure) ;
		}
		if (numBytesRead == 0) break ;
		if (m_conduitPtr) {
			if (
				m_conduitPtr->dataPump(
					kcUrlConduitPumpDataType_HttpData,
					inputBuffer,
					numBytesRead
				) != kcUrlConduitPumpRc_Ok
			) {
				break ;
			}
		}
	}

	disconnect() ;
	return(kcUrlChannelRc_Success) ;
}
	
kcUrlChannelRc kcUrlChannel::disconnect()
{
	if (!isOpen() || !isConnected()) return(kcUrlChannelRc_NotOpen) ;
	::InternetCloseHandle(m_hInternetConnect) ;
	resetConnect() ;
	return(kcUrlChannelRc_Success) ;
}


// end of kcuch.cpp