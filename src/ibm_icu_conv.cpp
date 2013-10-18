#include "ibm_icu_conv.hpp"

namespace io {

PConverter CHANNEL_PUBLIC icu_conv(const char* src, const char* dst) throw(charset_exception)
{
}

ICUConverter::ICUConverter() BOOST_NOEXCEPT_OR_NOTHROW:
	Converter(),
	conv_(NULL),
	utf16c_(NULL),
	srcCs_(NULL),
	destCs_(NULL)
{
}

ICUConverter::~ICUConverter()
{
}

void ICUConverter::convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception)
{
}

} // namespace io
