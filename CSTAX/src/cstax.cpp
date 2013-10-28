#include "cstax.hpp"

namespace cstax {

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
	return PXMLEvent((XMLEvent*)NULL);
}

PXMLEvent StreamReader::nextTag() throw(xml_stream_exception)
{
	return PXMLEvent((XMLEvent*)NULL);
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
