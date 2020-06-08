// kccmd.cpp


#include	"main.h"
#include	"kccmd.h"

kcCmdLine::kcCmdLine(const string &cmdString)
	: m_cmdLine(cmdString)
{
	string wordString ;
	istringstream stringStream(m_cmdLine) ;
	while(stringStream >> wordString) {
		m_cmdWordArray.push_back(wordString) ;
	}
}


// end of kccmd.cpp