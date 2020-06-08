// kccmd.h

#ifndef		INC_KCCMD_H
#define		INC_KCCMD_H

typedef vector<string> kcCmdWordArray ;

class kcCmdLine
{
	public:
		kcCmdLine(const string &cmdLine) ;
		const char *c_str() const { return(m_cmdLine.c_str()) ; }
		inline int wordCount() const { return(m_cmdWordArray.size()) ; }
		const char *c_word(int idx) const {
			if ((idx< 0) || (idx>= m_cmdWordArray.size())) return("") ;
			return(m_cmdWordArray[idx].c_str()) ;
		} 
		kcBool isWord(int idx, const char *pszWord) const {
			return(strcmp(c_word(0), pszWord) == 0) ;
		}

	private:
		string m_cmdLine ;
		kcCmdWordArray m_cmdWordArray ;
} ;


enum kcCmdRc {
	kcCmdRc_Success,
	kcCmdRc_Failure,
	kcCmdRc_Exit
} ;

class kcCmd
{
	public:
		kcCmd(const kcCmdLine &cmdLine) : m_cmdLine(cmdLine) {}
		virtual ~kcCmd() {} ;
		virtual kcCmdRc Run()= 0 ;

	protected:
		kcCmdLine m_cmdLine ;
} ;

// #ifndef		INC_KCCMD_H
#endif

// end of kccmd.h
