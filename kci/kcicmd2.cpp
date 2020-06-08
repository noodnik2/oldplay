// kcicmd2.cpp

#include	"main.h"
#include	"kcicmd2.h"
#include	"kcclist.h"


KC_CMDITEMADD(kcInternet2Cmd, internet2, "<url> - retrieves URL") ;

//
//
//

kcUrlConduitPumpRc kcUrlConduitForInternet2::dataPump(
	kcUrlConduitPumpDataType dataType,
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
		return(kcUrlConduitPumpRc_Stop) ;
	}
	return(kcUrlConduitPumpRc_Ok) ;
}


//
//
//

kcInternet2Cmd::kcInternet2Cmd(const kcCmdLine &cmdLine)
	: kcCmd(cmdLine), m_channelPtr(0), m_conduitPtr(0)
{
	m_conduitPtr= new kcUrlConduitForInternet2 ;
	if (!m_conduitPtr) return ;
	m_channelPtr= new kcUrlChannel(m_conduitPtr) ;
}

kcInternet2Cmd::~kcInternet2Cmd()
{
	if (m_channelPtr) delete m_channelPtr ;
	if (m_conduitPtr) delete m_conduitPtr ;
}

kcCmdRc kcInternet2Cmd::Run()
{
	kcUrlChannelRc urlRc ;

	cout << "Running internet command: " << m_cmdLine.c_str() << "\n" ;

	if (!m_channelPtr) {
		cout << "kcInternet2Cmd object in invalid state.\n" ;
		return(kcCmdRc_Failure) ;
	}

	if (m_cmdLine.wordCount() != 2) {
		cout << "Incorrect number of arguments supplied.\n" ;
		return(kcCmdRc_Failure) ;
	}

	urlRc= m_channelPtr->get(m_cmdLine.c_word(1)) ;
	cout << "RC(" << (int) urlRc << "); m_channelPtr->get()\n" ;

	return(kcCmdRc_Success) ;
}

// end of kcicmd2.cpp