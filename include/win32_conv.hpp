#ifndef WIN32_CONV_HPP_INCLUDE
#define WIN32_CONV_HPP_INCLUDE

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <convert.hpp>

#include <Mlang.h>

namespace io {

class PMLang {
private:
	IMLangConvertCharset *ptr_;
public:
	PMLang() BOOST_NOEXCEPT:
		ptr_(NULL)
	{}
	void reset(IMLangConvertCharset *ptr) {
		if(NULL == ptr_) {
			ptr_ = ptr;
		}
	}
	const IMLangConvertCharset* operator->() BOOST_NOEXCEPT {
		return ptr_;
	}
	~PMLang() {
		ptr_->Release();
	}
};

/**
 * ! \brief Microsoft MLang (Internet Explorer component) based converter
 */
class CHANNEL_PUBLIC Win32Converter : public Converter
{
	public:
		Win32Converter(const std::string& srcCt,const std::string& dstCt) BOOST_NOEXCEPT_OR_NOTHROW;
		~Win32Converter();
		virtual const Charset* sourceCharset() const;
		virtual const Charset* destinationCharset() const;
		virtual void convert(const byte_buffer& src, byte_buffer& dest) throw(charset_exception);
	private:
		bool freeCom_;
		PMLang mLang_;
};

} // namespace io

#endif // WIN32_CONV_HPP_INCLUDE
