#include "win32_conv.hpp"

namespace io {

Win32Converter::Win32Converter(const std::string& srcCt,const std::string& dstCt) BOOST_NOEXCEPT_OR_NOTHROW:
	freeCom_(false),
	mLang_()
{
	freeCom_ = ( S_OK == ::CoInitializeEx(NULL, COINIT_MULTITHREADED) );
	//mLang_.reset();
}

void Win32Converter::convert(const byte_buffer& src, byte_buffer& dest) throw(charset_exception)
{

}

const Charset* Win32Converter::sourceCharset() const
{
	return nullptr;
}

const Charset* Win32Converter::destinationCharset() const
{
	return nullptr;
}

Win32Converter::~Win32Converter()
{
	if(freeCom_) {
		::CoUninitialize();
	}
}

} // namespace io
