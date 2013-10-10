#include "iconv_conv.hpp"

namespace io {

// Unicode char sets
static const Charset UTF8(0xFF,"UTF-8",sizeof(char));
static const Charset UTF_16LE(1,"UTF-16LE",sizeof(char));
static const Charset UCS_2(2,"UCS-2",sizeof(char));
static const Charset UCS_2BE(3,"UCS-2BE",sizeof(wchar_t));
static const Charset UCS_2LE(4,"UCS-2LE",sizeof(wchar_t));
static const Charset UCS_4(5,"UCS-4",sizeof(uint32_t));
static const Charset UCS_4BE(6,"UCS-4BE",sizeof(uint32_t));
static const Charset UCS_4LE(7,"UCS-4LE",sizeof(uint32_t));
static const Charset UTF16(8,"UTF-16",sizeof(wchar_t));
static const Charset UTF16_BE(9,"UTF-16BE",sizeof(wchar_t));
static const Charset UTF16_LE(10,"UTF-16LE",sizeof(wchar_t));
static const Charset UTF32(11,"UTF-32",sizeof(uint32_t));
static const Charset UTF32_BE(12,"UTF-32BE",sizeof(uint32_t));
static const Charset UTF32_LE(13,"UTF-32LE",sizeof(uint32_t));
static const Charset UTF7(14,"UTF-7",sizeof(char));

// Single bytes char sets
static const Charset ASCII(0,"ASCII", sizeof(char));
static const Charset ISO_8859_1(16,"ISO-8859-1",sizeof(char));
static const Charset ISO_8859_2(17,"ISO-8859-2",sizeof(char));
static const Charset ISO_8859_3(18,"ISO-8859-3",sizeof(char));
static const Charset ISO_8859_4(19,"ISO-8859-4",sizeof(char));
static const Charset ISO_8859_5(20,"ISO-8859-5",sizeof(char));
static const Charset ISO_8859_6(21,"ISO-8859-6",sizeof(char));
static const Charset ISO_8859_7(22,"ISO-8859-7",sizeof(char));
static const Charset ISO_8859_8(23,"ISO-8859-8",sizeof(char));
static const Charset ISO_8859_9(24,"ISO-8859-9",sizeof(char));
static const Charset ISO_8859_10(25,"ISO-8859-10",sizeof(char));
static const Charset ISO_8859_11(26,"ISO-8859-11",sizeof(char));
static const Charset ISO_8859_12(27,"ISO-8859-12",sizeof(char));
static const Charset ISO_8859_13(28,"ISO-8859-13",sizeof(char));
static const Charset ISO_8859_14(29,"ISO-8859-14",sizeof(char));
static const Charset ISO_8859_15(30,"ISO-8859-15",sizeof(char));
static const Charset ISO_8859_16(31,"ISO-8859-16",sizeof(char));
static const Charset KOI8_R(32,"KOI8-R",sizeof(char));
static const Charset KOI8_U(33,"KOI8-U",sizeof(char));
static const Charset KOI8_RU(34,"KOI8-RU",sizeof(char));
static const Charset CP_1250(1250,"CP1250//IGNORE",sizeof(char));
static const Charset CP_1251(1251,"CP1251//IGNORE",sizeof(char));


inline std::pair< std::string, const Charset*> make_pair(const std::string& n, const Charset* chs)
{
	return std::make_pair(n,chs);
}

IconvCharsetFactory::IconvCharsetFactory() BOOST_NOEXCEPT:
	charSets_()
{
	charSets_.insert(make_pair("UTF-8",&UTF8));
	charSets_.insert(make_pair("UTF-16LE",&UTF_16LE));
	charSets_.insert(make_pair("UCS-2",&UCS_2));
	charSets_.insert(make_pair("UCS-2BE",&UCS_2BE));
	charSets_.insert(make_pair("UCS-2LE",&UCS_2LE));
	charSets_.insert(make_pair("UCS-4",&UCS_4));
	charSets_.insert(make_pair("UCS-4BE",&UCS_4BE));
	charSets_.insert(make_pair("UCS-4LE",&UCS_4LE));
	charSets_.insert(make_pair("UTF-16",&UTF16));
	charSets_.insert(make_pair("UTF-16BE",&UTF16_BE));
	charSets_.insert(make_pair("UTF-16LE",&UTF16_LE));
	charSets_.insert(make_pair("UTF-32",&UTF32));
	charSets_.insert(make_pair("UTF-32BE",&UTF32_BE));
	charSets_.insert(make_pair("UTF32-LE",&UTF32_LE));
	charSets_.insert(make_pair("UTF-7",&UTF7));
	charSets_.insert(make_pair("CP1251",&CP_1251));
}

const Charset* IconvCharsetFactory::forName(const std::string& name) const
{
	Charset *result = NULL;
	hash_table_t::const_iterator res = charSets_.find(name);
	if(res != charSets_.end()) {
		result = const_cast<Charset*>(res->second);
	}
	return result;
}


const IconvCharsetFactory* IconvConverter::chFactory()
{
	static IconvCharsetFactory factory;
	return &factory;
}

// IconvConverter
IconvConverter::IconvConverter(const std::string& src,const std::string& dst) throw(charset_exception):
		  Converter(),
          conv_(NULL),
          srcCs_(NULL),
          destCs_(NULL)
{
	if(src == dst) {
		throw charset_exception("Source character set is equal destination, no conversation needed");
	}

	srcCs_ = IconvConverter::chFactory()->forName(src);
	if(NULL == srcCs_) {
		throw charset_exception(src+" is not provided by iconv converter");
	}
	destCs_ = IconvConverter::chFactory()->forName(dst);
	if(NULL == srcCs_) {
		throw charset_exception(dst+" is not provided by iconv converter");
	}

	::iconv_allocation_t* descrpt = new iconv_allocation_t;
	int result = ::iconv_open_into( destCs_->name().c_str(), srcCs_->name().c_str(), descrpt);
	if(result == -1) {
		throw charset_exception("Can't build iconv converter");
	}
	conv_ =  (::iconv_t)descrpt;
}

IconvConverter::~IconvConverter()
{
	::iconv_close(conv_);
}

void IconvConverter::convert(const byte_buffer& src,byte_buffer& dest) throw(charset_exception)
{
	char *itptr = reinterpret_cast<char*>(&src.position());
	char *dstptr = reinterpret_cast<char*>(&dest.position());
	size_t srclen = src.length();
	size_t avail = dest.capacity();
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
	size_t offset = dest.capacity() - avail;
	dest.move(offset);
}

} // namespace io
