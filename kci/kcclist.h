// kcclist.h

#ifndef		INC_KCCLIST_H
#define		INC_KCCLIST_H

#include	"kccmd.h"

//
//
//

#define	KC_CMDITEMADD(cmdClass, cmdName, cmdDesc)				\
	static kcCmd *new_##cmdClass(const kcCmdLine &cmdLine)	{	\
	return new cmdClass(cmdLine) ;								\
}																\
static kcCmdItem cmdName##CmdItem(new_##cmdClass, #cmdName, cmdDesc)

#define	KC_CMDITEMLIST	getCmdItemList()


//
//
//

class kcCmdItemList ;
extern kcCmdItemList &KC_CMDITEMLIST ;


//
//
//

typedef kcCmd *(*kcCmdCreateFn)(const kcCmdLine &) ;


//
//
//

class kcCmdItem ;
class kcCmdItemList
{
	public:
		kcCmdItemList() ;
		inline void addItemPtr(const kcCmdItem *iPtr) {
			m_cmdItemPtrArray.push_back(iPtr) ;
		}
		inline int numItems() const { return(m_cmdItemPtrArray.size()) ; }
		const kcCmdItem *getItemPtr(int i) const ;
		const kcCmdItem *operator [](int i) const { return(getItemPtr(i)) ; }

	private:
		vector<const kcCmdItem *> m_cmdItemPtrArray ;
} ;

class kcCmdItem
{
	public:
		kcCmdItem(kcCmdCreateFn, const char *pszName, const char *pszDesc) ;
		const char *name() const { return(m_pszName) ; }
		const char *desc() const { return(m_pszDesc) ; }
		kcCmdCreateFn createFn() const { return(m_cmdCreateFn) ; }

	private:
		const char *m_pszName, *m_pszDesc ;
		kcCmdCreateFn m_cmdCreateFn ;

	private:
} ;

// #ifndef		INC_KCCLIST_H
#endif

// end of kcclist.h
