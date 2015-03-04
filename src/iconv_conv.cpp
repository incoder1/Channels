#include "prchdrs.h"
#include "iconv_conv.hpp"

namespace io {

// helpers
inline void validate_conversion(bool condition, const std::string& name) throw(charset_exception)
{
	validate<charset_exception>(condition,name);
}

inline void validate_charset(const Charset* ch, const std::string& name) throw(charset_exception)
{
	validate<charset_exception>(NULL != ch, name + " is not provided by iconv converter");
}

SConverter CHANNEL_PUBLIC iconv_conv(const char* src, const char* dst) throw(charset_exception)
{
	const Charset* srcCt = Charsets::forName(src);
	validate_charset(srcCt, src);
	const Charset* destCt = Charsets::forName(dst);
	validate_charset(destCt, dst);
	validate_conversion(!srcCt->equal(destCt),"Source character set is equal destination, no conversation needed");
	::iconv_t descrpt = ::iconv_open(destCt->name(), srcCt->name());
	validate_conversion(descrpt != (::iconv_t)(-1), "Can not construct iconv engine instance");
	return SConverter(new IconvConverter(descrpt, srcCt, destCt) );
}


// IconvConverter
IconvConverter::IconvConverter(iconv_t conv, const Charset* srcCt, const Charset* dstCt) BOOST_NOEXCEPT_OR_NOTHROW:
		  Converter(srcCt,dstCt),
		  conv_(conv,::iconv_close)
{}

ssize_t IconvConverter::convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception)
{
	char *itptr = reinterpret_cast<char*>(src.position().ptr());
	char *dstptr = reinterpret_cast<char*>(dest.position().ptr());
	std::size_t srclen = src.length();
	std::size_t avail = dest.capacity();
	std::size_t iconvValue = ::iconv(conv_.get(), &itptr, &srclen, &dstptr, &avail);
	if( static_cast<std::size_t>(-1) == iconvValue) {
		switch (errno) {
			/* See "man 3 iconv" for an explanation. */
		case EILSEQ:
			boost::throw_exception(charset_exception("Invalid multi-byte sequence"));
		case EINVAL:
			boost::throw_exception(charset_exception("Incomplete multi-byte sequence"));
		case E2BIG:
			boost::throw_exception(charset_exception("No more room"));
		default:
			boost::throw_exception(charset_exception(std::strerror(errno)));
		}
	}
	// calc size of char buffer, and move it
	ptrdiff_t  offset = (uint8_t*)dstptr - dest.position().ptr();
	if(offset > 0) {
		dest.move(offset);
	}
	dest.flip();
	return offset;
}

IconvConverter::~IconvConverter() BOOST_NOEXCEPT_OR_NOTHROW
{}

} // namespace io
