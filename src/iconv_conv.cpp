#include "prchdrs.h"
#include "iconv_conv.hpp"

namespace io {


inline void validate_charset(const Charset* ch, const std::string& name) throw(charset_exception)
{
	validate(NULL == ch, name + " is not provided by iconv converter");
}

SConverter CHANNEL_PUBLIC iconv_conv(const char* src, const char* dst) throw(charset_exception)
{
	const Charset* srcCt = Charsets::forName(src);
	validate_charset(srcCt, src);
	const Charset* destCt = Charsets::forName(dst);
	validate_charset(destCt, dst);
	validate(srcCt->equal(destCt),"Source character set is equal destination, no conversation needed");
	::iconv_t descrpt = ::iconv_open(destCt->name(), srcCt->name());
	validate(descrpt == (::iconv_t)(-1), "Can not construct iconv engine instance");
	IconvEngine engine(descrpt);
	return SConverter(new IconvConverter(engine, srcCt, destCt) );
}


// IconvConverter
IconvConverter::IconvConverter(const IconvEngine& engine, const Charset* srcCt, const Charset* dstCt) BOOST_NOEXCEPT_OR_NOTHROW:
		  Converter(srcCt,dstCt),
		  engine_(engine)
{}

void IconvConverter::convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception)
{
	char *itptr = reinterpret_cast<char*>(src.position().ptr());
	char *dstptr = reinterpret_cast<char*>(dest.position().ptr());
	std::size_t srclen = src.length();
	std::size_t avail = dest.capacity();
	std::size_t iconvValue = engine_.conv(&itptr, &srclen, &dstptr, &avail);
	if(iconvValue == ((std::size_t)-1)) {
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
	std::size_t offset = dest.capacity() - avail;
	dest.move(0 != offset ? offset: dest.capacity() - 1);
}

} // namespace io
