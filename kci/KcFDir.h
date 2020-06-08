// kcfdir.h

#ifndef		INC_KCFDIR_H
#define		INC_KCFDIR_H

#include	<windows.h>

class kcFileDirectoryEntry
{

	public:
		kcFileDirectoryEntry() { resetHandle() ; }
		~kcFileDirectoryEntry() { close() ; }

		kcBool findFirst(LPCTSTR lpFileName) ;
		kcBool findNext() ;
		inline kcBool isOpen() const {
			return(m_handle != INVALID_HANDLE_VALUE) ;
		}
		inline kcBool isDirectory() const {
			return(isSpecial(FILE_ATTRIBUTE_DIRECTORY)) ;
		}
		inline kcBool isHidden() const {
			return(isSpecial(FILE_ATTRIBUTE_HIDDEN)) ;
		}
		inline kcBool isSystem() const {
			return(isSpecial(FILE_ATTRIBUTE_SYSTEM)) ;
		}
		inline LPCTSTR c_str() const { return(m_wfdata.cFileName) ; }
		inline LPCTSTR c_strDos() const { return(m_wfdata.cAlternateFileName) ; }
		string fullPathName() const ;
		void close() ;

	protected:
		inline resetHandle() { m_handle= INVALID_HANDLE_VALUE ; }
		kcBool isSpecial(DWORD dwSpecialMask) const ;
		kcBool getPathPart(LPCTSTR lpFileName) ;

	private:
		HANDLE m_handle ;
		WIN32_FIND_DATA m_wfdata ;
		string m_pathPart ;

} ;


// #ifndef		INC_KCFDIR_H
#endif

// end of kcfdir.h
