// kcclist.cpp

#include	"main.h"
#include	"kcclist.h"


//
//
//

kcCmdItemList &getCmdItemList()
{
	static kcCmdItemList cmdItemList ;
	return(cmdItemList) ;
}


//
//
//

kcCmdItem::kcCmdItem(
	kcCmdCreateFn cmdCreateFn,
	const char *pszName,
	const char *pszDesc
)
	: m_cmdCreateFn(cmdCreateFn), m_pszName(pszName), m_pszDesc(pszDesc)
{
	KC_CMDITEMLIST.addItemPtr(this) ;
}

kcCmdItemList::kcCmdItemList()
{
}

const kcCmdItem *kcCmdItemList::getItemPtr(int i) const
{
	if ((i< 0) || (i>= numItems())) return(0) ;
	return(m_cmdItemPtrArray[i]) ;
}


// end of kcclist.cpp