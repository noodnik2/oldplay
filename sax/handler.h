
#ifndef		__MSAX_HANDLER_H__
#define		__MSAX_HANDLER_H__

#include	"attrlist.h"

namespace msax {

	class Handler
	{

	public:
		virtual ~Handler() ;

      		// Receive notification of character data. 
		virtual void characters(const Char[], Int, Int) = 0 ;

      		// Receive notification of the end of a document. 
  		virtual void endDocument() = 0 ;

      		// Receive notification of the end of an element. 
  		virtual void endElement(const String&) = 0 ;

      		// Receive notification of ignorable whitespace in element content. 
  		virtual void ignorableWhitespace(const Char[], Int, Int) = 0 ;

      		// Receive notification of a processing instruction. 
  		virtual void processingInstruction(const String&, const String&) = 0 ;

      		// Receive an object for locating the origin of SAX document events. 
  		virtual void setDocumentLocator(const Locator&) = 0 ;

		// Receive notification of the beginning of a document. 
		virtual void startDocument() = 0 ;

		// Receive notification of the beginning of an element.
  		virtual void startElement(const String&, const AttributeList&) = 0 ;

	} ;

}

#endif
// end of file
