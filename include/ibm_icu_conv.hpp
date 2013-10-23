#ifndef IBM_ICU_CONV_HPP_INCLUDED
#define IBM_ICU_CONV_HPP_INCLUDED

#define DEBUG_TMI 0  /* define to 1 to enable Too Much Information */

#include <cstdio>

#include <convert.hpp>

#include <unicode/utypes.h>   /* Basic ICU data types */
#include <unicode/ucnv.h>     /* C   Converter API    */
#include <unicode/ustring.h>  /* some more string fcns*/
#include <unicode/uchar.h>    /* char names           */
#include <unicode/uloc.h>
#include <unicode/unistr.h>

namespace io {

class ICUEngine {
private:
	mutable ::UConverter* intoUTF16_;
	mutable ::UConverter* fromUTF16_;
	void swapPtr(const ICUEngine& c) {
		intoUTF16_ = c.intoUTF16_;
		fromUTF16_ = c.fromUTF16_;
		c.intoUTF16_ = NULL;
		c.fromUTF16_ = NULL;
	}
public:
	ICUEngine(::UConverter* into, ::UConverter* from) BOOST_NOEXCEPT_OR_NOTHROW:
		intoUTF16_(into),
		fromUTF16_(from)
	{}
	ICUEngine(const ICUEngine& c) {
		swapPtr(c);
	}
	const ICUEngine& operator=(const ICUEngine& c) {
		swapPtr(c);
		return *this;
	}
	~ICUEngine() {
		if(NULL != intoUTF16_) {
			::ucnv_close(intoUTF16_);
		}
		if(NULL != fromUTF16_) {
			::ucnv_close(fromUTF16_);
		}
	}
	UErrorCode toUnicode(const char* src, size_t srcLen, UChar* dst, size_t& aval) const;
	UErrorCode fromUnicode(UChar* src, size_t srcLen, char* dst, size_t& aval) const;
};

/**
 * ! \brief Converts string representing in byte sequence from one code page (charset)
 *  to the another.
 * \param CharType - the byte type character representation. I.e. char, wchar_t, char8_t, char16_t etc.
 */
class CHANNEL_PUBLIC ICUConverter:public Converter {
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
