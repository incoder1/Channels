#ifndef __INCONV_CONV_HPP_INCLUDED
#define __INCONV_CONV_HPP_INCLUDED

#include <string>
#include <iconv.h>

#include "bytebuffer.hpp"
#include "charbuffers.hpp"
#include "charset.hpp"

namespace io {

typedef charset<const char*> charset_t;

// List of character sets supported by libconv

const charset_t ASCII("ASCII");
const charset_t UTF8("UTF-8");
const charset_t UTF_16LE("UTF-16LE");
const charset_t UCS_2("UCS-2");
const charset_t UCS_2BE("UCS-2BE");
const charset_t UCS_2LE("UCS-2LE");
const charset_t UCS_4("UCS-4");
const charset_t UCS_4BE("UCS-4BE");
const charset_t UCS_4LE("UCS-4LE");
const charset_t UTF16("UTF-16");
const charset_t UTF16_BE("UTF-16BE");
const charset_t UTF16_LE("UTF-16LE");
const charset_t UTF32("UTF-32");
const charset_t UTF32_BE("UTF-32BE");
const charset_t UTF32_LE("UTF-32LE");
const charset_t UTF7("UTF-7");


const charset_t ISO_8859_1("ISO-8859-1");
const charset_t ISO_8859_2("ISO-8859-2");
const charset_t ISO_8859_3("ISO-8859-3");
const charset_t ISO_8859_4("ISO-8859-4");
const charset_t ISO_8859_5("ISO-8859-5");
const charset_t ISO_8859_6("ISO-8859-6");
const charset_t ISO_8859_7("ISO-8859-7");
const charset_t ISO_8859_8("ISO-8859-8");
const charset_t ISO_8859_9("ISO-8859-9");
const charset_t ISO_8859_10("ISO-8859-10");
const charset_t ISO_8859_11("ISO-8859-11");
const charset_t ISO_8859_12("ISO-8859-12");
const charset_t ISO_8859_13("ISO-8859-13");
const charset_t ISO_8859_14("ISO-8859-14");
const charset_t ISO_8859_15("ISO-8859-15");
const charset_t ISO_8859_16("ISO-8859-16");
const charset_t KOI8_R("KOI8-R");
const charset_t KOI8_U("KOI8-U");
const charset_t KOI8_RU("KOI8-RU");
const charset_t CP_1250("CP-1250");
const charset_t CP_1251("CP-1251");
const charset_t CP_1252("CP-1252");
const charset_t CP_1253("CP-1253");
const charset_t CP_1254("CP-1254");
const charset_t CP_1257("CP-1257");
const charset_t CP_850("CP-850");
const charset_t CP_866("CP-866");
const charset_t CP_1131("CP-866");
const charset_t MAC_ROMAN("Mac-Roman");
const charset_t MAC_CENTRAL_EUROPE("Mac-CentralEurope");
const charset_t MAC_ICELAND("Mac-Iceland");
const charset_t MAC_CROATIAN("Mac-Croatian");
const charset_t MAC_ROMANIA("Mac-Romania");
const charset_t MAC_CYRILLIC("Mac-Cyrillic");
const charset_t MAC_UKRAINE("Mac-Ukraine");
const charset_t MAC_GREEK("Mac-Greek");
const charset_t MAC_TURKISH("Mac-Turkish");
const charset_t MACINTOSH("Macintosh");
const charset_t MAC_HEBREW("Mac-Hebrew");
const charset_t MAC_ARABIC("Mac-Arabic");

/**
 * ! \ Abstraction for converting string representing in byte sequence from one code page (charset)
 *  to the another.
 * <p>
 *  This implementation uses libconv POSIX standard library (commonly GNU iconv) for char-set conversation.
 * </p>
 * \param CharType - the byte type character representation. I.e. char, wchar_t, char8_t, char16_t etc.
 */
template<typename ChartType>
class conveter {
private:
	::iconv_t conv_;
public:

	/**
	 * Constructs new converter
	 * \param srcCs source char set
	 * \param destCs destination char set
	 * \throw runtime_error if conversation is not possible
	 */
	conveter(const charset_t& srcCs, const charset_t& destCs) throw(std::runtime_error):
		conv_(NULL)
	{
		::iconv_allocation_t* descrpt = new iconv_allocation_t;
		int result = ::iconv_open_into( destCs.id(), srcCs.id(), descrpt);
		if(result == -1) {
			throw std::runtime_error("Can't build iconv converter");
		}
		conv_ =  (::iconv_t)descrpt;
	}

	/**
	 * Frees resources allocated by converter
	 */
	~conveter() {
		::iconv_close(conv_);
	}

	/**
	 * Converting character sequence from source character set into destination charter set
	 * \param src source characters in their bytes sequence representation
	 * \param dest destination character buffer
	 */
	void convert(const byte_buffer& src,char_buffer<ChartType>& dest) throw(charset_exception) {
		byte_buffer::iterator it = src.position();
		char *itptr = reinterpret_cast<char*>(it.ptr());
		char *dstptr = reinterpret_cast<char*>(dest.position().ptr());
		size_t srclen = src.length();
        size_t avail = sizeof(ChartType) * dest.capacity();
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
		size_t offset = (sizeof(ChartType) * dest.capacity() - avail) /sizeof(ChartType);
		dest.move(offset);
	}

};

} // namespace io
#endif // __INCONV_CONV_HPP_INCLUDED
