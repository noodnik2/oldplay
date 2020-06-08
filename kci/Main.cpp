#define	Msrc_Revision		1

/*

*/

#include	"main.h"
#include	"kccmd.h"
#include	"kcicmd.h"
#include	"kcicmd2.h"
#include	"kcfdir.h"
#include	"kcclist.h"

//
//
//

string kcGetLine(char *pszPrompt)
{
	string inputString ;
	if (pszPrompt) cout << pszPrompt ;
#if 1
	char szBuff[255] ;
	gets(szBuff) ;
	inputString= szBuff ;
#else
	getline(cin, inputString) ;
#endif
	return(inputString) ;
}

kcKey kcPause(char *pszPrompt)
{
	string inputString= kcGetLine(pszPrompt) ;
	if (inputString.length()< 1) return((kcKey) 0) ;
	return((kcKey) inputString[0]) ;
}


//
//
//

class kcInvalidCmd : public kcCmd
{
	public:
		kcInvalidCmd(const kcCmdLine &cmdLine) : kcCmd(cmdLine) {}
		virtual kcCmdRc Run() {
			if (m_cmdLine.wordCount()> 0) {
				cout << "invalid command: " << m_cmdLine.c_str() << "\n" ;
			}
			return(kcCmdRc_Success) ;
		}
} ;

class kcExitCmd : public kcCmd
{
	public:
		kcExitCmd(const kcCmdLine &cmdLine) : kcCmd(cmdLine) {}
		virtual kcCmdRc Run() {
			cout << "exiting\n" ;
			return(kcCmdRc_Exit) ;
		}

} ;

KC_CMDITEMADD(kcExitCmd, exit, "- exits CLI") ;


//
//
//

class kcTestCmd : public kcCmd
{
	public:
		kcTestCmd(const kcCmdLine &cmdLine) : kcCmd(cmdLine) {} 
		virtual kcCmdRc Run() ;
} ;

kcCmdRc kcTestCmd::Run()
{
	return(kcCmdRc_Success) ;
}


KC_CMDITEMADD(kcTestCmd, test, "- test command") ;



//
//
//

class kcHelpCmd : public kcCmd
{
	public:
		kcHelpCmd(const kcCmdLine &cmdLine) : kcCmd(cmdLine) {} 
		virtual kcCmdRc Run() ;
} ;

KC_CMDITEMADD(kcHelpCmd, help, "- prints help") ;

kcCmdRc kcHelpCmd::Run()
{
	int i ;

	cout << "help: prints CLI commands:\n" ;
	cout << "\n" ;

	for (i= 0; KC_CMDITEMLIST[i]; i++) {
		if (KC_CMDITEMLIST[i]->name()) {
			cout << KC_CMDITEMLIST[i]->name() ;
			cout << " " ;
			if (KC_CMDITEMLIST[i]->desc()) {
				cout << KC_CMDITEMLIST[i]->desc() ;
			}
			cout << "\n" ;
		}
	}
	cout << "\n" ;

	return(kcCmdRc_Success) ;
}


//
//
//

class kcDirCmd : public kcCmd
{
	public:
		kcDirCmd(const kcCmdLine &cmdLine) : kcCmd(cmdLine) {} 
		virtual kcCmdRc Run() ;
		void traverse(LPCTSTR lpctcSearch) const ;
} ;

KC_CMDITEMADD(kcDirCmd, dir, "<filepath-spec> - prints file directory") ;

void kcDirCmd::traverse(LPCTSTR lpctSearch) const
{
	kcFileDirectoryEntry kcfde ;
	string fileName ;
	for (
		kcfde.findFirst(lpctSearch) ;
		kcfde.isOpen() ;
		kcfde.findNext()
	) {
		fileName= kcfde.c_str() ;
		if (kcfde.isHidden()) continue ;
		if (fileName == "." || fileName == "..") continue ;
		if (kcfde.isDirectory()) {
			traverse(
				(kcfde.fullPathName() + string("/*.*")).c_str()
			) ;
			continue ;
		}
		cout << "dir_entry: \"" << kcfde.fullPathName() << "\"\n" ;
	}
}

kcCmdRc kcDirCmd::Run()
{
	if (m_cmdLine.wordCount() != 2) {
		cout << "Invalid number of arguments\n" ;
		return(kcCmdRc_Failure) ;
	}
	traverse(m_cmdLine.c_word(1)) ;
	return(kcCmdRc_Success) ;
}


//
//
//

enum kcCliRc {
	kcCliRc_Success,
	kcCliRc_Failure
} ;

class kcCli
{
	public:
		kcCliRc Run() ;

	private:
		kcCmd *GetCmd() const ;
} ;

kcCmd *kcCli::GetCmd() const
{
	kcCmdLine kcl(kcGetLine("$ ")) ;
	if (kcl.wordCount()> 0) {
		for (int i= 0; KC_CMDITEMLIST[i]; i++) {
			if (kcl.isWord(0, KC_CMDITEMLIST[i]->name())) {
				return((KC_CMDITEMLIST[i]->createFn())(kcl)) ;
			}
		}
	}
	return(new kcInvalidCmd(kcl)) ;
}

kcCliRc kcCli::Run()
{
	kcCmd *cmdPtr ;
	kcCmdRc cmdRc ;

	while((cmdPtr= GetCmd()) != (kcCmd *) NULL) {
		cmdRc= cmdPtr->Run() ;
		delete cmdPtr ;
		if (cmdRc == kcCmdRc_Exit) return(kcCliRc_Success) ;
	}

	cerr << "abort: command allocation failure\n" ;
	return(kcCliRc_Failure) ;
}



//
//	entry points
//

void main(int argc, char *argv[])
{
	kcCli cli ;

	cout	<< "kiosk CLI - "
			<< "0.0." << Msrc_Revision
			<< "\n"
			<< "Copyright (C) 2000, Libraary.com\n"
			<< "(type 'help' for help)\n"
			<< "\n"
	;

	cli.Run() ;

	exit(0) ;
	/*NOTREACHED*/
}


// end of file
