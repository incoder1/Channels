#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef WIN32_CONV_HPP_INCLUDE
#define WIN32_CONV_HPP_INCLUDE

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <convert.hpp>

#include <Objbase.h>
#include <Mlang.h>

namespace io {

class PMLang {
private:
	IMLangConvertCharset *ptr_;
	inline void copyPtr(const PMLang& oth);
public:
	PMLang() BOOST_NOEXCEPT;
	PMLang(const PMLang& oth) BOOST_NOEXCEPT;
	~PMLang();
	const PMLang& operator=(const PMLang& oth) BOOST_NOEXCEPT;
	inline void reset(IMLangConvertCharset *ptr) BOOST_NOEXCEPT;
	inline IMLangConvertCharset* operator->() const BOOST_NOEXCEPT;
	inline IMLangConvertCharset* operator&() const BOOST_NOEXCEPT;
};

/**
 * ! \brief Microsoft MLang (Internet Explorer component) based converter
 */
class CHANNEL_PUBLIC Win32Converter : public Converter
{
	public:
		Win32Converter(const Charset* srcCt,const Charset* dstCt) BOOST_NOEXCEPT_OR_NOTHROW;
		~Win32Converter();
		virtual const Charset* sourceCharset() const;
		virtual const Charset* destinationCharset() const;
		virtual void convert(const byte_buffer& src, byte_buffer& dest) throw(charset_exception);
	private:
		HMODULE libMLang_;
		bool freeCOM_;
		const Charset* srcCt_;
		const Charset* dstCt_;
		PMLang mLang_;
};

PConverter CHANNEL_PUBLIC win32_converter(const std::string& src, const std::string& dst) throw(charset_exception);

} // namespace io

#endif // WIN32_CONV_HPP_INCLUDE
