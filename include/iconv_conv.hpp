#ifndef __INCONV_CONV_HPP_INCLUDED
#define __INCONV_CONV_HPP_INCLUDED

#include <string>
#include <cstring>
#include <cerrno>
#include <cstdio>

#include <convert.hpp>
#include <iconv.h>

#include <smallobjectpool.hpp>

namespace io {

class IconvEngine {
private:
	boost::shared_ptr<void> conv_;
public:
	explicit IconvEngine(::iconv_t conv) BOOST_NOEXCEPT_OR_NOTHROW:
		conv_(conv,::iconv_close)
	{}
	inline std::size_t conv(char** src, std::size_t *srclen, char **dstptr, std::size_t *avail) const
	{
		return ::iconv(conv_.get(), src, srclen, dstptr, avail);
	}
};

/**
 * ! \brief Converts string representing in byte sequence from one code page (charset)
 *  to the another.
 * \param CharType - the byte type character representation. I.e. char, wchar_t, char8_t, char16_t etc.
 */
class CHANNEL_PUBLIC IconvConverter:public virtual Converter, public virtual SmallObject {
private:
	IconvEngine engine_;
public:
	IconvConverter(const IconvEngine& engine, const Charset* srcCt, const Charset* dstCt) BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Converting character sequence from source character set into destination charter set
	 * \param src source characters in their bytes sequence representation
	 * \param dest destination characters in their character type representation
	 */
	virtual void convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception);
};

} // namespace io
#endif // __INCONV_CONV_HPP_INCLUDED
