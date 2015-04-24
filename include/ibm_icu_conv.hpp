#ifndef IBM_ICU_CONV_HPP_INCLUDED
#define IBM_ICU_CONV_HPP_INCLUDED

#define DEBUG_TMI 0  /* define to 1 to enable Too Much Information */

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/format.hpp>

#include <unicode/utypes.h>   /* Basic ICU data types */
#include <unicode/ucnv.h>     /* C   Converter API    */
#include <unicode/ustring.h>  /* some more string fcns*/
#include <unicode/uchar.h>    /* char names           */
#include <unicode/uloc.h>
#include <unicode/unistr.h>

#include "charsets.hpp"
#include "bytebuffer.hpp"
#include "smallobject.hpp"

namespace io {

class ICUEngine {
private:
	boost::shared_ptr<::UConverter> intoUTF16_;
	boost::shared_ptr<::UConverter> fromUTF16_;
public:
	ICUEngine() BOOST_NOEXCEPT_OR_NOTHROW;
	ICUEngine(::UConverter* into, ::UConverter* from) BOOST_NOEXCEPT_OR_NOTHROW;
	UErrorCode toUnicode(const char* src, std::size_t srcLen, UChar* dst, std::size_t& aval) const;
	UErrorCode fromUnicode(UChar* src, std::size_t srcLen, char* dst, std::size_t& aval) const;
};

/**
 * ! \brief Converts string representing in byte sequence from one code page (charset)
 *  to the another.
 * \param CharType - the byte type character representation. I.e. char, wchar_t, char8_t, char16_t etc.
 */
class CHANNEL_PUBLIC ICUConverter:public object {
	void fromUnicode(const byte_buffer& src,byte_buffer& dest);
	void intoUnicode(const byte_buffer& src,byte_buffer& dest);
	inline std::size_t calcBuffSize(const byte_buffer& src);
public:
	/**
	 * Constructs new converter
	 * \param srcCt source char set
	 * \param destCt destination char set
	 * \throw charset_exception if conversation is not possible
	 */
	ICUConverter(const Charset *srcCt, const Charset *dstCt);

	/**
	 * Frees resources allocated by converter
	 */
	virtual ~ICUConverter();

	/**
	 * Converts character sequence from source character set into destination charter set
	 * \param src source characters in their bytes sequence representation
	 *  \return heap byte buffer with converted characters
	 */
	byte_buffer convert(const byte_buffer& src);

	/**
	 * Converts character sequence from source character set into destination charter set
	 * \param src source characters in their bytes sequence representation
	 * \param dest destination characters in their character type representation
	 * \return converted string length in bytes
	 */
	std::size_t convert(const byte_buffer& src, byte_buffer& dest);

private:
	const Charset* from_;
	const Charset* to_;
	ICUEngine engine_;
};

typedef boost::shared_ptr<ICUConverter> SConverter;

SConverter CHANNEL_PUBLIC make_converter(const Charset* from, const Charset* to);

inline SConverter make_converter(const char *from, const char* to) {
	return make_converter(Charsets::forName(from),Charsets::forName(to));
}

} // namespace io

#endif // IBM_ICU_CONV_HPP_INCLUDED
