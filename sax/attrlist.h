
#ifndef	__MSAX_ATTRLIST_INC__
#define	__MSAX_ATTRLIST_INC__

#include	"msax.h"

namespace msax {

	class AttributeList
	{

	public:

		// Return the number of attributes in this list. 
		virtual void getLength() = 0 ;

		// Return the name of an attribute in this list (by position). 
		virtual void getName(Int) = 0 ;

		// Return the type of an attribute in the list (by position). 
		virtual void getType(Int) = 0 ;

		// Return the type of an attribute in the list (by name). 
		virtual void getType(const String&) = 0 ;

		// Return the value of an attribute in the list (by position). 
		virtual void getValue(Int) = 0 ;

		// Return the value of an attribute in the list (by name). 
		virtual void getValue(const String&) = 0 ;
	} ;

}


#endif

// end of file
