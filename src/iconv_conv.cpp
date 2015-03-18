#include "prchdrs.h"
#include "iconv_conv.hpp"

namespace io {

// helpers
inline void validate_conversion(bool condition, const std::string& name) throw(std::runtime_error)
{
	validate<std::runtime_error>(condition,name);
}

inline void validate_charset(const Charset* ch, const std::string& name) throw(std::runtime_error)
{
	validate<std::runtime_error>(NULL != ch, name + " is not provided by iconv converter");
}


// IconvConverter
IconvConverter::IconvConverter(const Charset* from, const Charset* to):
	from_(from),
	to_(to)
{
	validate_conversion(!from_->equal(to_),"Source character set is equal destination, no conversation needed");
	::iconv_t descrpt = ::iconv_open(to_->name(),from_->name());
	validate_conversion(descrpt != (::iconv_t)(-1), "Can not construct iconv engine instance");
	conv_.reset(descrpt,::iconv_close);
}

IconvConverter::~IconvConverter() BOOST_NOEXCEPT_OR_NOTHROW
{}

byte_buffer IconvConverter::convert(const byte_buffer& src) throw(std::bad_alloc,std::runtime_error)
{
	std::size_t buffSize = 0;
	if(to_->charSize() > from_->charSize() || from_ == Charsets::utf8() ){
		buffSize = src.length() * to_->charSize();
	} else {
		buffSize = src.length();
	}
	byte_buffer dest = byte_buffer::heap_buffer(buffSize);
	char *itptr = (char*)src.position().ptr();
	char *dstptr = (char*)dest.position().ptr();
	std::size_t srclen = src.length();
	std::size_t avail = dest.remain();
	std::size_t iconvValue = ::iconv(conv_.get(), &itptr, &srclen, &dstptr, &avail);
	if( static_cast<std::size_t>(-1) == iconvValue) {
		switch (errno) {
			/* See "man 3 iconv" for an explanation. */
		case EILSEQ:
			boost::throw_exception(std::runtime_error("Invalid multi-byte sequence"));
		case EINVAL:
			boost::throw_exception(std::runtime_error("Incomplete multi-byte sequence"));
		case E2BIG:
			boost::throw_exception(std::runtime_error("No more room"));
		default:
			boost::throw_exception(std::runtime_error(std::strerror(errno)));
		}
	}
	// calc size of char buffer, and move it
	if(avail > 0) {
		dest.clear();
		ptrdiff_t offset = (uint8_t*)dstptr - dest.position().ptr();
		dest.move((std::size_t)offset);
	} else {
		dest.move(dest.end());
	}
	dest.flip();
	return dest;
}


} // namespace io
