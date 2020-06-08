// kcicmd.cpp

#include	"main.h"
#include	"kcicmd.h"
#include	"kcclist.h"


KC_CMDITEMADD(kcInternetCmd, internet, "<url> - retrieves URL") ;

kcInternetCmd::kcInternetCmd(const kcCmdLine &cmdLine)
	: kcCmd(cmdLine)
{
}

kcCmdRc kcInternetCmd::Run()
{
	string outString ;
	char readBuffer[1024] ;
	int readSize ;
	int i ;

	cout << "Running internet command: " << m_cmdLine.c_str() << "\n" ;

	if (m_cmdLine.wordCount() != 2) {
		cout << "Incorrect number of arguments supplied.\n" ;
		return(kcCmdRc_Failure) ;
	}

	if (!m_channel.isOpen()) {
		cout << "not open\n" ;
		return(kcCmdRc_Failure) ;
	}
	cout << "Connecting to: \"" << m_cmdLine.c_word(1) << "\"\n" ;
	if (
		m_channel.connect(m_cmdLine.c_word(1))
	 != kcInternetChannel::RcSuccess
	) {
		cout << "can't connect\n" ;
		return(kcCmdRc_Failure) ;
	}

	cout << "Connected.\n" ;

	cout << "Reading header...\n" ;
	if ((readSize= m_channel.httpInfo(readBuffer, sizeof(readBuffer)))> 0) {
		outString.assign(readBuffer, readSize) ;
		cout << "httpInfo(" << readSize << "): \"" << outString << "\"\n" ;
	}
	kcPause() ;

	cout << "Reading data...\n" ;
#if 1
	for (
		i= 0;
		(readSize= m_channel.read(readBuffer, sizeof(readBuffer)))> 0;
		i++
	) {
		outString.assign(readBuffer, readSize) ;
		cout << i << ". " << "read(" << readSize << "): \"" << outString << "\"\n" ;
	}
#else
	TCHAR readHeaderBuffer[1024] ;
	DWORD bufferLen, headerBufferLen ;
	if (m_channel.readEx(
			readBuffer,
			sizeof(readBuffer),
			&bufferLen,
			readHeaderBuffer,
			sizeof(readHeaderBuffer),
			&headerBufferLen
	)) {
		outString.assign(readHeaderBuffer, headerBufferLen) ;
		cout << i << ". " << "readHeader(" << headerBufferLen << "): \"" << outString << "\"\n" ;
		outString.assign(readBuffer, bufferLen) ;
		cout << i << ". " << "read(" << bufferLen << "): \"" << outString << "\"\n" ;
	}

#endif
	return(kcCmdRc_Success) ;
}

// end of kcicmd.cpp