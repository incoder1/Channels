#include "ibm_icu_conv.hpp"

namespace io {

const CharsetFactory* ICUConverter::chFactory()
{
	static CharsetFactory factory;
	return &factory;
}

ICUConverter::ICUConverter(const std::string& src,const std::string& dst) throw(charset_exception):
	Converter(),
	conv_(NULL),
	utf16c_(NULL),
	srcCs_(NULL),
	destCs_(NULL)
{
	if(src == dst) {
		throw charset_exception("Source character set is equal destination, no conversation needed");
	}
	srcCs_ = ICUConverter::chFactory()->forName(src);
	if(NULL == srcCs_) {
		throw charset_exception(src+" is not provided by iconv converter");
	}
	destCs_ = ICUConverter::chFactory()->forName(dst);
	if(NULL == srcCs_) {
		throw charset_exception(dst+" is not provided by iconv converter");
	}
	::UErrorCode status;
	conv_ = ::ucnv_open( srcCs_->name().c_str(), &status);
	if(!U_SUCCESS(status)) {
		throw charset_exception("Can't build IBM icu converter");
	}
	utf16c_ = ::ucnv_open("utf-16", &status);
	if(!U_SUCCESS(status)) {
		::ucnv_close(conv_);
		throw charset_exception("Can't build IBM icu converter");
	}
}

ICUConverter::~ICUConverter()
{
	::ucnv_close(utf16c_);
	::ucnv_close(conv_);
}

void ICUConverter::convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception)
{
	void *srcptr = reinterpret_cast<char*>(&src.position());
	void *dstptr = reinterpret_cast<char*>(&dest.position());
	byte_buffer utfbuff = new_byte_byffer(src.length()*2);
}

} // namespace io
