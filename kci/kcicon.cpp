// kcicon.cpp

#include	"main.h"
#include	"kcicon.h"

kcInternetChannel::kcInternetChannel()
{
	reset() ;
	m_hInternetRoot= ::InternetOpen(
		"kcicon.cpp",
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0 // INTERNET_FLAG_OFFLINE
	) ;
}

kcInternetChannel::~kcInternetChannel()
{
	if (isOpen()) {
		::InternetCloseHandle(m_hInternetRoot) ;
		if (isConnected()) {
			::InternetCloseHandle(m_hInternetConnect) ;
		}
	}
}

kcInternetChannel::Rc kcInternetChannel::connect(
	const string &urlString
) {
	if (!isOpen()) return(RcNotOpen) ;
	m_hInternetConnect= ::InternetOpenUrl(
		m_hInternetRoot,
		urlString.c_str(),
		NULL,
		0,
		INTERNET_FLAG_RAW_DATA,
		0
	) ;
	return(isConnected()? RcSuccess: RcNotConnected) ;
}
	
kcInternetChannel::Rc kcInternetChannel::disconnect()
{
	if (!isOpen() || !isConnected()) return(RcNotOpen) ;
	::InternetCloseHandle(m_hInternetConnect) ;
	resetConnect() ;
	return(RcSuccess) ;
}

kcInt kcInternetChannel::read(void *outputBuffer, kcInt bufferSizeBytes)
{
	DWORD numBytesRead= 0 ;

	if (!isConnected()) return(-1) ;
	if (bufferSizeBytes< 0) return(-2) ;
	if (
		!::InternetReadFile(
			m_hInternetConnect,
			outputBuffer,
			bufferSizeBytes,
			&numBytesRead
		)
	) {
		cout << "::InternetReadFile returns errorno=" << ::GetLastError() << "\n" ;
		return(-3) ;
	}
	if (numBytesRead> (DWORD) bufferSizeBytes) {
		// throw Error()
		return(-4) ;
	}
	return(numBytesRead) ;
}

// NOTE: assumes sizeof(TCHAR) == sizeof(char)
kcBool kcInternetChannel::readEx(
	LPVOID outputBufferPtr,
	DWORD bufferSizeBytesMax,
	DWORD *bufferSizeBytesOutPtr,
	LPTSTR outputHeaderBufferPtr,
	DWORD headerBufferSizeTcharsMax,
	DWORD *headerBufferSizeTcharsOutPtr
) {
	INTERNET_BUFFERS ibuf ;
	DWORD dwBytesToCopy ;

	if (!isConnected()) return(kcBoolFalse) ;
	memset(&ibuf, '\0', sizeof(ibuf)) ;

	if (
		!::InternetReadFileEx(
			m_hInternetConnect,
			&ibuf,
			IRF_SYNC,
			0
		)
	) {
		cout << "::InternetReadFileEx returns errorno=" << ::GetLastError() << "\n" ;
		return(kcBoolFalse) ;
	}

	if (ibuf.lpvBuffer) {
		dwBytesToCopy= ibuf.dwBufferLength ;	// TCHARs, or bytes???
		if (dwBytesToCopy> bufferSizeBytesMax) {
			dwBytesToCopy= bufferSizeBytesMax ;
		}
		memcpy(outputBufferPtr, ibuf.lpvBuffer, dwBytesToCopy) ;
		(*bufferSizeBytesOutPtr)= dwBytesToCopy ;
	}
	else {
		(*bufferSizeBytesOutPtr)= 0 ;
	}

	if (ibuf.lpcszHeader) {
		dwBytesToCopy= ibuf.dwHeadersLength ;	// assumes sizeof(TCHAR) == 1
		if (dwBytesToCopy> headerBufferSizeTcharsMax) {
			dwBytesToCopy= headerBufferSizeTcharsMax ;
		}
		memcpy(outputHeaderBufferPtr, ibuf.lpcszHeader, dwBytesToCopy) ;
		(*headerBufferSizeTcharsOutPtr)= dwBytesToCopy ;
	}
	else {
		(*headerBufferSizeTcharsOutPtr)= 0 ;
	}

	return(kcBoolTrue) ;
}

kcInt kcInternetChannel::httpInfo(void *outputBuffer, kcInt bufferSizeBytes)
{
	DWORD numBytesRead= bufferSizeBytes ;
	DWORD dwIndex= 0 ;

	if (!isConnected()) return(-1) ;
	if (bufferSizeBytes< 0) return(-2) ;
	if (
		!::HttpQueryInfo(
			m_hInternetConnect,
			HTTP_QUERY_RAW_HEADERS_CRLF,
			outputBuffer,
			&numBytesRead,
			&dwIndex
		)
	) {
		return(-3) ;
	}
	if (numBytesRead>= (DWORD) bufferSizeBytes) {
		// throw Error()
		return(-4) ;
	}
	return(numBytesRead) ;
}

// end of kcicon.cpp