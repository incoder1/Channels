#include "ibm_icu_conv.hpp"

namespace io {

inline void validate_charset(const Charset* ch, const std::string& name) throw(charset_exception) {
	validate(NULL == ch, name + " is not provided by IBM ICU converter");
}

inline void validate_create_conv(const UErrorCode& errCode, const std::string& chName) {
	validate(U_FAILURE(errCode), "Can not build IBM ICU converter for converting "+chName);
}

static const std::size_t UTF16LE = 1200;

inline bool notUTF16(const Charset* ch) {
	return UTF16LE != ch->id();
}

inline bool isWindowsChName(const Charset* ch) {
	return (ch->id() >= 1250) && (ch->id() <= 1258);
}

UConverter* openConverter(const io::Charset* ch) throw(io::charset_exception) {
	UConverter *result = NULL;
	if(notUTF16(ch)) {
		const char* chName;
		if(isWindowsChName(ch)) {
			char winName[13];
			winName[12] = 0;
			std::sprintf(winName,"windows-%i",ch->id());
			chName = winName;
		} else {
			chName = ch->name();
		}
		UErrorCode errCode = U_ZERO_ERROR;
		result = ::ucnv_open(chName , &errCode);
		validate_create_conv(errCode, ch->name());
	}
	return result;
}

SConverter CHANNEL_PUBLIC icu_conv(const char* src, const char* dst) throw(charset_exception)
{
	static CharsetFactory chFactory;
	const Charset* srcCt = chFactory.forName(src);
	validate_charset(srcCt, src);
	const Charset* destCt = chFactory.forName(dst);
	validate_charset(destCt, dst);
	validate(srcCt->equal(destCt),"Source character set is equal destination, no conversation needed");
	UConverter* intoUnc = openConverter(srcCt);
	UConverter* fromUnc = openConverter(destCt);
	ICUEngine engine(intoUnc, fromUnc);
	return SConverter(new ICUConverter(engine, srcCt, destCt));
}

// ICUEngine
UErrorCode ICUEngine::toUnicode(const char* src, std::size_t srcLen, UChar* dst, std::size_t& aval) const {
		UErrorCode result = U_ZERO_ERROR;
		aval = ::ucnv_toUChars(intoUTF16_, dst, aval, src, srcLen, &result);
		return result;
}

UErrorCode ICUEngine::fromUnicode(UChar* src, std::size_t srcLen, char* dst, std::size_t& aval) const {
		UErrorCode result = U_ZERO_ERROR;
		aval = ::ucnv_fromUChars(fromUTF16_, dst, aval, src, srcLen, &result);
		return result;
}

//ICUConverter
ICUConverter::ICUConverter(ICUEngine engine, const Charset *srcCt, const Charset *dstCt) BOOST_NOEXCEPT_OR_NOTHROW:
	Converter(srcCt,dstCt),
	engine_(engine)
{
}

ICUConverter::~ICUConverter()
{
}

inline void validate_into_conv(UErrorCode errCode,const Charset* charset) {
	if(U_FAILURE(errCode)) {
		std::string msg("Can not convert from ");
		msg.append(charset->name());
		msg.append(" into UTF-16LE");
		boost::throw_exception(charset_exception(msg));
	}
}

inline void validate_from_conv(UErrorCode errCode,const Charset* charset) {
	if(U_FAILURE(errCode)) {
		std::string msg("Can not convert from UTF-16LE into ");
		msg.append(charset->name());
		boost::throw_exception(charset_exception(msg));
	}
}

void ICUConverter::intoUnicode(const byte_buffer& source,byte_buffer& dest) throw(charset_exception)
{
	char *src = reinterpret_cast<char*>(&source.position());
	UChar *dst = reinterpret_cast<UChar*>(&dest.position());
	std::size_t avail = dest.capacity() / sizeof(UChar);
	std::size_t srclen = source.length();
	UErrorCode errCode = engine_.toUnicode(src, srclen, dst, avail);
	validate_into_conv(errCode, srcCharset());
	std::size_t offset = dest.capacity() - (avail*sizeof(UChar));
	dest.move(0 != offset ? offset: dest.capacity() - 1);
}

void ICUConverter::fromUnicode(const byte_buffer& source,byte_buffer& dest) throw(charset_exception)
{
	UChar *src = reinterpret_cast<UChar*>(&source.position());
	char *dst = reinterpret_cast<char*>(&dest.position());
	std::size_t srclen = source.length() / sizeof(UChar);
	std::size_t avail = dest.capacity();
	UErrorCode errCode = engine_.fromUnicode(src, srclen, dst, avail);
	validate_from_conv(errCode, destCharset());
	std::size_t offset = dest.capacity() - avail;
	dest.move(0 != offset ? offset: dest.capacity() - 1);
}

void ICUConverter::convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception)
{
	if(notUTF16(srcCharset())) {
		intoUnicode( src, dest);
	}
	if(notUTF16(destCharset())) {
		fromUnicode(src, dest);
	}
	// Conversation over Unicode, will not be invoked in most cases
	if(notUTF16(srcCharset()) && notUTF16(destCharset())) {
		byte_buffer unicodeBuff = new_byte_byffer(src.length()*sizeof(UChar));
		intoUnicode(src,unicodeBuff);
		unicodeBuff.flip();
		fromUnicode(unicodeBuff, dest);
	}
}

} // namespace io
