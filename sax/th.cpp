
#include	"handler.h"

using namespace msax ;

class MyHandler : public Handler
{
	// Receive notification of character data. 
	void characters(const Char[], Int, Int) {
	}

      	// Receive notification of the end of a document. 
  	void endDocument() {
	}

      	// Receive notification of the end of an element. 
  	void endElement(const String&) {
	}

      	// Receive notification of ignorable whitespace in element content. 
  	void ignorableWhitespace(const Char[], Int, Int) {
	}

      	// Receive notification of a processing instruction. 
  	void processingInstruction(const String&, const String&) {
	}

      	// Receive an object for locating the origin of SAX document events. 
  	void setDocumentLocator(const Locator&) {
	}

	// Receive notification of the beginning of a document. 
	void startDocument() {
	}

	// Receive notification of the beginning of an element.
  	void startElement(const String&, const AttributeList&) {
	}

} ;


void main()
{
	MyHandler h ;

	h.startDocument() ;
}


