#include "convert.hpp"
#include <iostream>

namespace io {


// Charset

Charset::Charset(size_t id, const char* name, const size_t charSize) BOOST_NOEXCEPT_OR_NOTHROW:
id_(id),
    name_(name),
    charSize_(charSize)
{}

const size_t Charset::id() const
{
	return id_;
}

const char* Charset::name() const
{
	return name_;
}

const size_t Charset::charSize() const
{
	return charSize_;
}

bool Charset::equal(const Charset* oth) const
{
	bool result = false;
	if(NULL != oth) {
		result = id_ == (oth->id_);
	}
	return result;
}

// Charset constants

// Unicode representation char-sets (the code page is same)
static const Charset UTF_8(65001,"UTF-8",sizeof(uint8_t));
static const Charset UTF_16LE(1200,"UTF-16LE",sizeof(uint16_t));
static const Charset UTF_16BE(1201,"UTF-16BE",sizeof(uint16_t));
static const Charset UTF_32BE(12001,"UTF-32BE",sizeof(uint32_t));
static const Charset UTF_32LE(12000,"UTF-32LE",sizeof(uint32_t));
static const Charset UTF_7(65000,"UTF-7",sizeof(uint8_t));

// Single bytes char sets
static const Charset ASCII(20127,"ASCII", sizeof(char));
// ISO standards ASCII compatible ISO/EEC
static const Charset ISO_8859_1(28591,"ISO-8859-1",sizeof(char)); // Latin 1
static const Charset ISO_8859_2(28592,"ISO-8859-2",sizeof(char));
static const Charset ISO_8859_3(28593,"ISO-8859-3",sizeof(char));
static const Charset ISO_8859_4(28594,"ISO-8859-4",sizeof(char));
static const Charset ISO_8859_5(28595,"ISO-8859-5",sizeof(char));
static const Charset ISO_8859_6(28596,"ISO-8859-6",sizeof(char));
static const Charset ISO_8859_7(28597,"ISO-8859-7",sizeof(char));
static const Charset ISO_8859_8(28598,"ISO-8859-8",sizeof(char));
static const Charset ISO_8859_9(28599,"ISO-8859-9",sizeof(char));
static const Charset ISO_8859_10(28600,"ISO-8859-10",sizeof(char));
static const Charset ISO_8859_11(28601,"ISO-8859-11",sizeof(char));
static const Charset ISO_8859_12(28602,"ISO-8859-12",sizeof(char));
static const Charset ISO_8859_13(28603,"ISO-8859-13",sizeof(char));
static const Charset ISO_8859_14(28604,"ISO-8859-14",sizeof(char));
static const Charset ISO_8859_15(28605,"ISO-8859-15",sizeof(char));
static const Charset ISO_8859_16(28606,"ISO-8859-16",sizeof(char));
/* Cyrillic Unix */
static const Charset KOI8_R(20866,"KOI8-R",sizeof(char)); // Unix Russian
static const Charset KOI8_U(21866,"KOI8-U",sizeof(char)); // Unix Ukrainian
static const Charset KOI8_RU(20866,"KOI8-RU",sizeof(char)); // Unix Belarusian
// Windows national code pages for the alphabet based languages
static const Charset CP_1250(1250,"CP1250",sizeof(char)); // Windows version of ISO-8859-1 + additional character
static const Charset CP_1251(1251,"CP1251",sizeof(char)); // Windows version of ISO-8859-5 + additional character
static const Charset CP_1252(1252,"CP1252",sizeof(char));
static const Charset CP_1253(1253,"CP1253",sizeof(char));
static const Charset CP_1254(1254,"CP1254",sizeof(char));
static const Charset CP_1255(1255,"CP1255",sizeof(char));
static const Charset CP_1256(1256,"CP1256",sizeof(char));
static const Charset CP_1257(1257,"CP1257",sizeof(char));
static const Charset CP_1258(1258,"CP1258",sizeof(char));

// Charset factory
inline std::pair< std::string, const Charset*> make_pair(const std::string& n, const Charset* chs)
{
	return std::make_pair(n,chs);
}

CharsetFactory::CharsetFactory() BOOST_NOEXCEPT_OR_NOTHROW:
charSets_()
{
	charSets_.insert(make_pair("ASCII",&ASCII));
	charSets_.insert(make_pair("UTF-8",&UTF_8));
	charSets_.insert(make_pair("UTF-16LE",&UTF_16LE));
	charSets_.insert(make_pair("UCS-2",&UTF_16LE));
	charSets_.insert(make_pair("UCS-2BE",&UTF_16BE));
	charSets_.insert(make_pair("UCS-2LE",&UTF_16LE));
	charSets_.insert(make_pair("UCS-4",&UTF_32LE));
	charSets_.insert(make_pair("UCS-4BE",&UTF_32BE));
	charSets_.insert(make_pair("UCS-4LE",&UTF_32LE));
	charSets_.insert(make_pair("UTF-16",&UTF_16LE));
	charSets_.insert(make_pair("UTF-16BE",&UTF_16BE));
	charSets_.insert(make_pair("UTF-16LE",&UTF_16LE));
	charSets_.insert(make_pair("UTF-32",&UTF_32LE));
	charSets_.insert(make_pair("UTF-32BE",&UTF_32BE));
	charSets_.insert(make_pair("UTF32-LE",&UTF_32LE));
	charSets_.insert(make_pair("UTF-7", &UTF_7));
	charSets_.insert(make_pair("CP1250",&CP_1250));
	charSets_.insert(make_pair("CP1251",&CP_1251));
	charSets_.insert(make_pair("CP1252",&CP_1252));
	charSets_.insert(make_pair("CP1253",&CP_1253));
	charSets_.insert(make_pair("CP1254",&CP_1254));
	charSets_.insert(make_pair("CP1255",&CP_1255));
	charSets_.insert(make_pair("CP1256",&CP_1256));
	charSets_.insert(make_pair("CP1257",&CP_1257));
	charSets_.insert(make_pair("CP1258",&CP_1258));

	charSets_.insert(make_pair("ISO-8859-1", &ISO_8859_1));
	charSets_.insert(make_pair("ISO-8859-2", &ISO_8859_2));
	charSets_.insert(make_pair("ISO-8859-3", &ISO_8859_3));
	charSets_.insert(make_pair("ISO-8859-4", &ISO_8859_4));
	charSets_.insert(make_pair("ISO-8859-5", &ISO_8859_5));
	charSets_.insert(make_pair("ISO-8859-6", &ISO_8859_6));
	charSets_.insert(make_pair("ISO-8859-7", &ISO_8859_7));
	charSets_.insert(make_pair("ISO-8859-8", &ISO_8859_8));
	charSets_.insert(make_pair("ISO-8859-9", &ISO_8859_9));
	charSets_.insert(make_pair("ISO-8859-10", &ISO_8859_10));
	charSets_.insert(make_pair("ISO-8859-11", &ISO_8859_11));
	charSets_.insert(make_pair("ISO-8859-12", &ISO_8859_12));
	charSets_.insert(make_pair("ISO-8859-13", &ISO_8859_13));
	charSets_.insert(make_pair("ISO-8859-14", &ISO_8859_14));
	charSets_.insert(make_pair("ISO-8859-15", &ISO_8859_15));
	charSets_.insert(make_pair("ISO-8859-16", &ISO_8859_16));
	charSets_.insert(make_pair("KOI8-R",&KOI8_R));
	charSets_.insert(make_pair("KOI8-U",&KOI8_U));
	charSets_.insert(make_pair("KOI8-RU",&KOI8_RU));
}

const Charset* CharsetFactory::forName(const char* name) const
{
	hash_table_t::const_iterator res = charSets_.find(std::string(name));
	if(res != charSets_.end()) {
		return res->second;
	}
	return NULL;
}

// Converter
Converter::~Converter()
{
}

} // namespace io
