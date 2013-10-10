#ifndef __INCONV_CONV_HPP_INCLUDED
#define __INCONV_CONV_HPP_INCLUDED

#include <string>
#include <cstring>
#include <cerrno>
#include <cstdio>


#include <convert.hpp>
#include <iconv.h>

namespace io {

/**
 * ! \brief Converts string representing in byte sequence from one code page (charset)
 *  to the another.
 * \param CharType - the byte type character representation. I.e. char, wchar_t, char8_t, char16_t etc.
 */
class CHANNEL_PUBLIC IconvConverter:public Converter {
private:
	::iconv_t conv_;
	const Charset* srcCs_;
	const Charset* destCs_;
	static const CharsetFactory* chFactory();
public:
	/**
	 * Constructs new converter
	 * \param srcCs source char set
	 * \param destCs destination char set
	 * \throw charset_exception if conversation is not possible
	 */
	IconvConverter(const std::string& sourceCharset,const std::string& destinationCharset) throw(charset_exception);

	virtual const Charset* sourceCharset() const {
		return srcCs_;
	}

	virtual const Charset* destinationCharset() const {
		return destCs_;
	}

	/**
	 * Frees resources allocated by converter
	 */
	virtual ~IconvConverter();

	/**
	 * Converting character sequence from source character set into destination charter set
	 * \param src source characters in their bytes sequence representation
	 * \param dest destination characters in their character type representation
	 */
	virtual void convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception);
};

inline PConverter iconv_conv(const std::string& sourceCharset,const std::string& destinationCharset)
{
	return io::PConverter(new io::IconvConverter(sourceCharset, destinationCharset));
}

} // namespace io
#endif // __INCONV_CONV_HPP_INCLUDED
