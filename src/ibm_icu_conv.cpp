#include "prchdrs.h"
#include "ibm_icu_conv.hpp"

namespace io {


inline void validate_create_conv(const UErrorCode& errCode, const std::string& chName) {
	if(U_FAILURE(errCode)) {
	   std::string msg("Can not build IBM ICU converter for converting");
	   boost::throw_exception(std::runtime_error(msg+chName));
	}
}

static const std::size_t UTF16LE = 1200;

inline bool notUTF16(const Charset* ch) {
	return UTF16LE != ch->id();
}

inline bool isWindowsChName(const Charset* ch) {
	return (ch->id() >= 1250) && (ch->id() <= 1258);
}

// factory for converter
UConverter* openConverter(const io::Charset* ch) throw(std::runtime_error) {
	UConverter *result = NULL;
	if(notUTF16(ch)) {
		std::string chName;
		if(isWindowsChName(ch)) {
			static const char* WIN_CP_FORMAT = "windows-%1%";
			boost::format format(WIN_CP_FORMAT);
			chName.append((format % ch->id()).str());
		} else {
			chName.append(ch->name());
		}
		UErrorCode errCode = U_ZERO_ERROR;
		result = ::ucnv_open(chName.c_str(), &errCode);
		validate_create_conv(errCode, ch->name());
	}
	return result;
}

// ICUEngine
ICUEngine::ICUEngine() BOOST_NOEXCEPT_OR_NOTHROW
{}

ICUEngine::ICUEngine(::UConverter* into, ::UConverter* from) BOOST_NOEXCEPT_OR_NOTHROW:
	intoUTF16_(into,::ucnv_close),
	fromUTF16_(from,::ucnv_close)
{
}

UErrorCode ICUEngine::toUnicode(const char* src, std::size_t srcLen, UChar* dst, std::size_t& aval) const {
		UErrorCode result = U_ZERO_ERROR;
		aval = ::ucnv_toUChars(intoUTF16_.get(), dst, aval, src, srcLen, &result);
		return result;
}

UErrorCode ICUEngine::fromUnicode(UChar* src, std::size_t srcLen, char* dst, std::size_t& aval) const {
		UErrorCode result = U_ZERO_ERROR;
		aval = ::ucnv_fromUChars(fromUTF16_.get(), dst, aval, src, srcLen, &result);
		return result;
}

//ICUConverter
ICUConverter::ICUConverter(const Charset *from, const Charset* to):
	from_(from),
	to_(to)
{
	if(from_->equal(to_)) {
		 boost::throw_exception(std::runtime_error("Source character set is equal destination, no conversation needed"));
	}
	UConverter* intoUnc = openConverter(from_);
	UConverter* fromUnc = openConverter(to_);
	engine_ = ICUEngine(intoUnc, fromUnc);
}

ICUConverter::~ICUConverter()
{
}

inline void validate_into_conv(UErrorCode errCode,const Charset* charset) {
	if(U_FAILURE(errCode)) {
		std::string msg("Can not convert from ");
		msg.append(charset->name());
		msg.append(" into UTF-16LE");
		boost::throw_exception(std::runtime_error(msg));
	}
}

inline void validate_from_conv(UErrorCode errCode,const Charset* charset) {
	if(U_FAILURE(errCode)) {
		std::string msg("Can not convert from UTF-16LE into ");
		msg.append(charset->name());
		boost::throw_exception(std::runtime_error(msg));
	}
}

void ICUConverter::intoUnicode(const byte_buffer& source,byte_buffer& dest)
{
	char *src = reinterpret_cast<char*>(source.position().ptr());
	UChar *dst = reinterpret_cast<UChar*>(dest.position().ptr());
	std::size_t avail = dest.capacity() / sizeof(UChar);
	std::size_t srclen = source.length();
	UErrorCode errCode = engine_.toUnicode(src, srclen, dst, avail);
	validate_into_conv(errCode, to_);
	std::size_t offset = dest.capacity() - (avail*sizeof(UChar));
	dest.move(0 != offset ? offset: dest.capacity() - 1);
}

void ICUConverter::fromUnicode(const byte_buffer& source,byte_buffer& dest)
{
	UChar *src = reinterpret_cast<UChar*>(source.position().ptr());
	char *dst = reinterpret_cast<char*>(dest.position().ptr());
	std::size_t srclen = source.length() / sizeof(UChar);
	std::size_t avail = dest.capacity();
	UErrorCode errCode = engine_.fromUnicode(src, srclen, dst, avail);
	validate_from_conv(errCode, to_);
	std::size_t offset = dest.capacity() - avail;
	dest.move(0 != offset ? offset: dest.capacity() - 1);
}

inline std::size_t ICUConverter::calcBuffSize(const byte_buffer& src) {
	std::size_t result = 0;
	if(to_->charSize() > from_->charSize() || from_ == Charsets::utf8() ){
		result = src.length() * to_->charSize();
	} else {
		result= src.length();
	}
	return result;
}

std::size_t ICUConverter::convert(const byte_buffer& src, byte_buffer& dest)
{
	if(notUTF16(from_)) {
		intoUnicode(src, dest);
	} else if(notUTF16(to_)) {
		fromUnicode(src, dest);
	} else
	// Conversation over Unicode, wouldn't be used
	if(notUTF16(from_) && notUTF16(to_)) {
		byte_buffer unicodeBuff = byte_buffer::heap_buffer(src.length()*sizeof(UChar));
		intoUnicode(src,unicodeBuff);
		unicodeBuff.flip();
		fromUnicode(unicodeBuff, dest);
	}
	dest.flip();
	return dest.length();
}

byte_buffer ICUConverter::convert(const byte_buffer& src)
{
	std::size_t buffSize = calcBuffSize(src);
	byte_buffer dest = byte_buffer::heap_buffer(buffSize);
	convert(src,dest);
	return dest;
}

SConverter CHANNEL_PUBLIC make_converter(const Charset* from, const Charset* to) {
	return boost::make_shared<ICUConverter>(from,to);
}


} // namespace io
