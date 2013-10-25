#include "cstax.hpp"

namespace cstax {

// XMLSyntax
XMLSyntax::XMLSyntax() BOOST_NOEXCEPT_OR_NOTHROW {
	uint8_t i;
	for(i=0; i < 26; i++) {
		whitespaces_.insert(WHITESPACES[i]);
	}
	for(i=0; i < 7; i++) {
		syntax_.insert(SYNTAX[i]);
	}
}

// XMLEvent
XMLEvent::~XMLEvent()
{}

ElementEvent::~ElementEvent()
{}

//StreamReader

StreamReader::StreamReader(const XMLSource& source,size_t buffSize) BOOST_NOEXCEPT_OR_NOTHROW:
	 src_(source),
     buffSize_(buffSize)
{}

PXMLEvent StreamReader::next() throw(xml_stream_exception)
{
	return PXMLEvent(NULL);
}

PXMLEvent StreamReader::nextTag() throw(xml_stream_exception)
{
	return PXMLEvent(NULL);
}

bool StreamReader::hasNext() throw(xml_stream_exception)
{
	return false;
}

const wchar_t* getElementText() throw(xml_stream_exception)
{
	return NULL;
}

} // namespace cstax
