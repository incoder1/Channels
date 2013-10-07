#include "cstax.hpp"

namespace cstax {

// XMLSyntax
XMLSyntax::XMLSyntax() NOEXCEPT
{
	uint8_t i;
	for(i=0; i < 26; i++) {
		whitespaces_.insert(WHITESPACES[i]);
	}
	for(i=0; i < 7; i++) {
		syntax_.insert(SYNTAX[i]);
	}
}

const wchar_t XMLSyntax::WHITESPACES[26] = {
	L'\u0009',L'\u000A',L'\u000B',L'\u000C',L'\u000D',
	L'\u0020',L'\u0085',L'\u00A0',L'\u2000',L'\u2001',
	L'\u2002',L'\u1680',L'\u180E',L'\u2003',L'\u2004',
	L'\u2005',L'\u2006',L'\u2007',L'\u2008',L'\u2009',
	L'\u200A',L'\u2028',L'\u2029',L'\u202F',L'\u205F',
	L'\u3000'
};

const wchar_t XMLSyntax::SYNTAX[7] = {
	L'<',L'>',L'?',L'=',L'\'',L'"',L'&'
};

// XMLEvent
XMLEvent::~XMLEvent()
{}

ElementEvent::~ElementEvent()
{}

} // namespace cstax
