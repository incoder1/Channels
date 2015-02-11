#ifndef IBM_ICU_CONV_HPP_INCLUDED
#define IBM_ICU_CONV_HPP_INCLUDED

#define DEBUG_TMI 0  /* define to 1 to enable Too Much Information */

#include <boost/format.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

#include <convert.hpp>

#include <unicode/utypes.h>   /* Basic ICU data types */
#include <unicode/ucnv.h>     /* C   Converter API    */
#include <unicode/ustring.h>  /* some more string fcns*/
#include <unicode/uchar.h>    /* char names           */
#include <unicode/uloc.h>
#include <unicode/unistr.h>

#include <smallobjectpool.hpp>

namespace io {

class ICUEngine {
private:
	boost::shared_ptr<::UConverter> intoUTF16_;
	boost::shared_ptr<::UConverter> fromUTF16_;
public:
	ICUEngine(::UConverter* into, ::UConverter* from) BOOST_NOEXCEPT_OR_NOTHROW;
	UErrorCode toUnicode(const char* src, std::size_t srcLen, UChar* dst, std::size_t& aval) const;
	UErrorCode fromUnicode(UChar* src, std::size_t srcLen, char* dst, std::size_t& aval) const;
};

/**
 * ! \brief Converts string representing in byte sequence from one code page (charset)
 *  to the another.
 * \param CharType - the byte type character representation. I.e. char, wchar_t, char8_t, char16_t etc.
 */
class CHANNEL_PUBLIC ICUConverter:public virtual Converter, public virtual SmallObject {
private:
	ICUEngine engine_;
	void fromUnicode(const byte_buffer& src,byte_buffer& dest) throw(charset_exception);
	void intoUnicode(const byte_buffer& src,byte_buffer& dest) throw(charset_exception);
public:
	/**
	 * Constructs new converter
	 * \param srcCt source char set
	 * \param destCt destination char set
	 * \throw charset_exception if conversation is not possible
	 */
	ICUConverter(ICUEngine engine, const Charset *srcCt, const Charset *dstCt) BOOST_NOEXCEPT_OR_NOTHROW;

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
