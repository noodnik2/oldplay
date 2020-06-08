// kcicmd3.cpp

#include	"main.h"
#include	"kcicmd3.h"
#include	"kcclist.h"


KC_CMDITEMADD(kcInternet3Cmd, internet3, "<url> - retrieves URL") ;

//
//
//

kcUrlConduit2PumpRc kcUrlConduit2ForInternet3::dataPump(
	kcUrlConduit2PumpDataType dataType,
	void *dataPtr,
	DWORD dataLength
) {
	char *dataPtrAsString= (char *) dataPtr ;
	string outString ;

	outString.assign(dataPtrAsString, dataLength) ;
	cout	<< "dataPump("
			<< (int) dataType
			<< "); read("
			<< dataLength
			<< "): \""
			<< outString
			<< "\"\n"
	;
	if (tolower(kcPause("<ENTER for more, or 'n'>\n")) == 'n') {
		return(kcUrlConduit2PumpRc_Stop) ;
	}
	return(kcUrlConduit2PumpRc_Ok) ;
}


//
//
//

kcInternet3Cmd::kcInternet3Cmd(const kcCmdLine &cmdLine)
	: kcCmd(cmdLine), m_channel2Ptr(0), m_conduit2Ptr(0)
{
	m_conduit2Ptr= new kcUrlConduit2ForInternet3 ;
	if (!m_conduit2Ptr) return ;
	m_channel2Ptr= new kcUrlChannel2(m_conduit2Ptr) ;
}

kcInternet3Cmd::~kcInternet3Cmd()
{
	if (m_channel2Ptr) delete m_channel2Ptr ;
	if (m_conduit2Ptr) delete m_conduit2Ptr ;
}

kcCmdRc kcInternet3Cmd::Run()
{
	kcUrlChannel2Rc urlRc ;

	cout << "Running internet3 command: " << m_cmdLine.c_str() << "\n" ;

	if (!m_channel2Ptr) {
		cout << "kcInternet3Cmd object in invalid state.\n" ;
		return(kcCmdRc_Failure) ;
	}

	if (m_cmdLine.wordCount() != 2) {
		cout << "Incorrect number of arguments supplied.\n" ;
		return(kcCmdRc_Failure) ;
	}

	urlRc= m_channel2Ptr->get(m_cmdLine.c_word(1)) ;
	cout << "RC(" << (int) urlRc << "); m_channel2Ptr->get()\n" ;

	return(kcCmdRc_Success) ;
}

// end of kcicmd3.cpp