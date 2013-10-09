#ifndef __INCONV_CONV_HPP_INCLUDED
#define __INCONV_CONV_HPP_INCLUDED

#include <cstring>
#include <string>
#include <cerrno>
#include <cstdio>

#include <convert.hpp>
#include <iconv.h>

#include <boost/unordered_map.hpp>

namespace io {

class CHANNEL_PUBLIC IconvCharsetFactory {
private:
	typedef boost::unordered_map<std::string,const Charset*> hash_table_t;
	hash_table_t charSets_;
	IconvCharsetFactory();
public:
	static const IconvCharsetFactory* instance() {
		static IconvCharsetFactory _instance;
		return &_instance;
	}
	const Charset* forName(const std::string& name) const;
};

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
public:
	/**
	 * Constructs new converter
	 * \param srcCs source char set
	 * \param destCs destination char set
	 * \throw charset_exception if conversation is not possible
	 */
	IconvConverter(const Charset* srcCs, const Charset* destCs) throw(charset_exception);

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

} // namespace io
#endif // __INCONV_CONV_HPP_INCLUDED
