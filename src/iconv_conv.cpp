#include "iconv_conv.hpp"

namespace io {


const CharsetFactory* IconvConverter::chFactory()
{
	static CharsetFactory factory;
	return &factory;
}

// IconvConverter
IconvConverter::IconvConverter(const std::string& src,const std::string& dst) throw(charset_exception):
		  Converter(),
          conv_(NULL),
          srcCs_(NULL),
          destCs_(NULL)
{
	if(src == dst) {
		throw charset_exception("Source character set is equal destination, no conversation needed");
	}
	srcCs_ = IconvConverter::chFactory()->forName(src);
	if(NULL == srcCs_) {
		throw charset_exception(src+" is not provided by iconv converter");
	}
	destCs_ = IconvConverter::chFactory()->forName(dst);
	if(NULL == srcCs_) {
		throw charset_exception(dst+" is not provided by iconv converter");
	}

	::iconv_allocation_t* descrpt = new iconv_allocation_t;
	int result = ::iconv_open_into( destCs_->name().c_str(), srcCs_->name().c_str(), descrpt);
	if(result == -1) {
		throw charset_exception("Can't build iconv converter");
	}
	conv_ =  (::iconv_t)descrpt;
}

IconvConverter::~IconvConverter()
{
	::iconv_close(conv_);
}

void IconvConverter::convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception)
{
	char *itptr = reinterpret_cast<char*>(&src.position());
	char *dstptr = reinterpret_cast<char*>(&dest.position());
	size_t srclen = src.length();
	size_t avail = dest.capacity();
	size_t iconvValue = ::iconv(conv_, &itptr, &srclen, &dstptr, &avail);
	if(iconvValue == ((size_t)-1)) {
		switch (errno) {
			/* See "man 3 iconv" for an explanation. */
		case EILSEQ:
			throw charset_exception("Invalid multi-byte sequence");
		case EINVAL:
			throw charset_exception("Incomplete multi-byte sequence");
		case E2BIG:
			throw charset_exception("No more room");
		default:
			throw charset_exception(std::strerror(errno));
		}
	}
	// calc size of char buffer, and move it
	size_t offset = dest.capacity() - avail;
	dest.move(offset);
}

} // namespace io
