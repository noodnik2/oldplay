// kcInternetChannel

#ifndef		INC_KCICON_H
#define		INC_KCICON_H

#include	<windows.h>
#include	<wininet.h>

class kcInternetChannel
{
	public:
		enum Rc {
			RcSuccess,
			RcFailure,
			RcNotOpen,
			RcNotConnected
		} ;
		kcInternetChannel() ;
		~kcInternetChannel() ;
		Rc connect(const string &urlString) ;
		Rc disconnect() ;
		kcInt read(void *outputBuffer, kcInt bufferSizeBytes) ;
		kcBool readEx(
			LPVOID outputBufferPtr,
			DWORD bufferSizeBytesMax,
			DWORD *bufferSizeBytesOutPtr,
			LPTSTR outputHeaderBufferPtr,
			DWORD headerBufferSizeTcharsMax,
			DWORD *headerBufferSizeTcharsOutPtr
		) ;

		kcInt httpInfo(void *outputBuffer, kcInt bufferSizeBytes) ;
		inline kcBool isOpen() { return(m_hInternetRoot != NULL) ; }
		inline kcBool isConnected() { return(m_hInternetConnect != NULL) ; }

	private:
		inline void reset() { m_hInternetRoot= m_hInternetConnect= NULL ; }
		inline void resetConnect() { m_hInternetConnect= NULL ; }
		HINTERNET m_hInternetRoot, m_hInternetConnect ;
} ;

// #ifndef INC_KCICON_H
#endif

// end of kcicon.h

