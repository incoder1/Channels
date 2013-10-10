#ifndef IBM_ICU_CONV_HPP_INCLUDED
#define IBM_ICU_CONV_HPP_INCLUDED

#define DEBUG_TMI 0  /* define to 1 to enable Too Much Information */

#include <boost/unordered_map.hpp>
#include <convert.hpp>

#include <unicode/utypes.h>   /* Basic ICU data types */
#include <unicode/ucnv.h>     /* C   Converter API    */
#include <unicode/ustring.h>  /* some more string fcns*/
#include <unicode/uchar.h>    /* char names           */
#include <unicode/uloc.h>
#include <unicode/unistr.h>

namespace io {

/**
 * ! \brief Converts string representing in byte sequence from one code page (charset)
 *  to the another.
 * \param CharType - the byte type character representation. I.e. char, wchar_t, char8_t, char16_t etc.
 */
class CHANNEL_PUBLIC ICUConverter:public Converter {
private:
	UConverter *conv_;
	UConverter *utf16c_;
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
	ICUConverter(const std::string& sourceCharset,const std::string& destinationCharset) throw(charset_exception);

	virtual const Charset* sourceCharset() const {
		return srcCs_;
	}

	virtual const Charset* destinationCharset() const {
		return destCs_;
	}

	/**
	 * Frees resources allocated by converter
	 */
	virtual ~ICUConverter();

	/**
	 * Converting character sequence from source character set into destination charter set
	 * \param src source characters in their bytes sequence representation
	 * \param dest destination characters in their character type representation
	 */
	virtual void convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception);
};

} // namespace io

#endif // IBM_ICU_CONV_HPP_INCLUDED
