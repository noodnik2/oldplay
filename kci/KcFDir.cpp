// KcFDir.cpp

#include	"main.h"
#include	"kcfdir.h"


kcBool kcFileDirectoryEntry::isSpecial(DWORD dwSpecialMask) const
{
	if (isOpen()) {
		if (m_wfdata.dwFileAttributes & dwSpecialMask) {
			return(kcBoolTrue) ;
		}
	}
	return(kcBoolFalse) ;
}

void kcFileDirectoryEntry::close()
{
	if (isOpen()) {
		::FindClose(m_handle) ;
	}
	resetHandle() ;
}

string kcFileDirectoryEntry::fullPathName() const
{
	string result ;

	if (m_pathPart.size()> 0) {
		result.append(m_pathPart) ;
	}
	result.append(c_str()) ;
	return(result) ;
}

kcBool kcFileDirectoryEntry::getPathPart(LPCTSTR lpctFileName)
{
	m_pathPart.erase() ;
	if (!lpctFileName) return(kcBoolFalse) ;
	for (int i= strlen(lpctFileName); i> 0; i--) {
		if (
			(lpctFileName[i-1] == '/')
		 || (lpctFileName[i-1] == '\\')
		 || (lpctFileName[i-1] == ':')
		 ) {
			break ;
		}
	}
	if (i> 0) m_pathPart.assign(lpctFileName, i) ;
	return(kcBoolTrue) ;
}

kcBool kcFileDirectoryEntry::findFirst(LPCTSTR lpFileName)
{
	close() ;
	if (!getPathPart(lpFileName)) return(kcBoolFalse) ;
	m_handle= ::FindFirstFile(lpFileName, &m_wfdata) ;
	return(isOpen()) ;
}

kcBool kcFileDirectoryEntry::findNext()
{
	if (!isOpen()) return(kcBoolFalse) ;
	if (::FindNextFile(m_handle, &m_wfdata) != TRUE) {
		close() ;
		return(kcBoolFalse) ;
	}
	return(kcBoolTrue) ;
}


// end of KcFDir.cpp
