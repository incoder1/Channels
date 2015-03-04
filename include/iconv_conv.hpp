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

/**
 * ! \brief Converts string representing in byte sequence from one code page (charset)
 *  to the another.
 */
class CHANNEL_PUBLIC IconvConverter:public virtual Converter, public virtual SmallObject {
private:
	IconvConverter(::iconv_t conv, const Charset* srcCt, const Charset* dstCt) BOOST_NOEXCEPT_OR_NOTHROW;
	friend SConverter CHANNEL_PUBLIC iconv_conv(const char*, const char*) throw(charset_exception);
public:

	virtual ~IconvConverter() BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Converting character sequence from source character set into destination charter set
	 * \param src source characters in their bytes sequence representation
	 * \param dest destination characters in their character type representation
	 */
	virtual ssize_t convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception);
private:
	boost::shared_ptr<void> conv_;
};

} // namespace io
#endif // __INCONV_CONV_HPP_INCLUDED
