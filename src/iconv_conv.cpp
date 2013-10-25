#include "iconv_conv.hpp"

namespace io {


inline void validate_charset(const Charset* ch, const std::string& name) throw(charset_exception)
{
	validate(NULL == ch, name + " is not provided by iconv converter");
}

PConverter CHANNEL_PUBLIC iconv_conv(const char* src, const char* dst) throw(charset_exception)
{
	static CharsetFactory chFactory;
	const Charset* srcCt = chFactory.forName(src);
	validate_charset(srcCt, src);
	const Charset* destCt = chFactory.forName(dst);
	validate_charset(destCt, dst);
	validate(srcCt->equal(destCt),"Source character set is equal destination, no conversation needed");
	::iconv_allocation_t* descrpt = new iconv_allocation_t;
	int result = ::iconv_open_into(destCt->name(), srcCt->name(), descrpt);
	validate(result == -1, "Can not construct iconv engine instance");
	IconvEngine engine((::iconv_t)descrpt);
	return PConverter(new IconvConverter(engine, srcCt, destCt) );
}


// IconvConverter
IconvConverter::IconvConverter(const IconvEngine& engine, const Charset* srcCt, const Charset* dstCt) BOOST_NOEXCEPT_OR_NOTHROW:
		  Converter(srcCt,dstCt),
		  engine_(engine)
{}

void IconvConverter::convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception)
{
	char *itptr = reinterpret_cast<char*>(&src.position());
	char *dstptr = reinterpret_cast<char*>(&dest.position());
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
