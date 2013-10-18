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

#if !defined(_MSC_VER)
const wchar_t XMLSyntax::WHITESPACES[26] = {
	L'\u0009',L'\u000A',L'\u000B',L'\u000C',L'\u000D',L'\u0020',
	L'\u0085',L'\u00A0',L'\u2000',L'\u2001',L'\u2002',L'\u1680',
	L'\u180E',L'\u2003',L'\u2004',L'\u2005',L'\u2006',L'\u2007',
	L'\u2008',L'\u2009',L'\u200A',L'\u2028',L'\u2029',L'\u202F',
	L'\u205F',L'\u3000'
};
#else
const wchar_t XMLSyntax::WHITESPACES[26] = {
	0x0009,0x000A,0x000B,0x000C,0x000D,0x0020,
	0x0085,0x00A0,0x2000,0x2001,0x2002,0x1680,
	0x180E,0x2003,0x2004,0x2005,0x2006,0x2007,
	0x2008,0x2009,0x200A,0x2028,0x2029,0x202F,
	0x205F,0x3000
};
#endif // Non MS compiler

const wchar_t XMLSyntax::SYNTAX[7] = {
	L'<',L'>',L'?',L'=',L'\'',L'"',L'&'
};

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
	ubuff buff = io::new_char_buff<wchar_t>(buffSize_);
	while( src_.read(buff) > 0 ) {
		buff.flip();
		//std::stack<std::wstring> stack;
		for(ubuff::iterator it = buff.begin(); it != buff.last(); ++it) {
			// skip white spaces
			if(*it != L'<') {
				continue;
			}
		}
	}
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
