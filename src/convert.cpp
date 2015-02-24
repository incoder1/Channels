#include "prchdrs.h"
#include "convert.hpp"
namespace io {

// Charset

Charset::Charset(std::size_t id, const char* name, const std::size_t charSize, bool isUnicode) BOOST_NOEXCEPT_OR_NOTHROW:
id_(id),
    name_(name),
    charSize_(charSize),
    unicode_(isUnicode)
{}

std::size_t Charset::id() const BOOST_NOEXCEPT_OR_NOTHROW
{
	return id_;
}

const char* Charset::name() const BOOST_NOEXCEPT_OR_NOTHROW
{
	return name_;
}

std::size_t Charset::charSize() const BOOST_NOEXCEPT_OR_NOTHROW
{
	return charSize_;
}

bool Charset::isUnicode() const BOOST_NOEXCEPT_OR_NOTHROW
{
	return unicode_;
}

bool Charset::equal(const Charset* oth) const BOOST_NOEXCEPT_OR_NOTHROW
{
	bool result = false;
	if(NULL != oth) {
		result = id_ == (oth->id_);
	}
	return result;
}

// Charset constants

// Unicode representation char-sets (the code page is same)
static const Charset UTF_8(65001,"UTF-8",6,true); // can store even 6 bytes characters
static const Charset UTF_16LE(1200,"UTF-16LE",sizeof(uint16_t),true);
static const Charset UTF_16BE(1201,"UTF-16BE",sizeof(uint16_t),true);
static const Charset UTF_32BE(12001,"UTF-32BE",sizeof(uint32_t),true);
static const Charset UTF_32LE(12000,"UTF-32LE",sizeof(uint32_t),true);
static const Charset UTF_7(65000,"UTF-7",sizeof(uint8_t),true);

// Single bytes char sets
static const Charset ASCII(20127,"ASCII", sizeof(char),false);
// ISO standards ASCII compatible ISO/EEC
static const Charset ISO_8859_1(28591,"ISO-8859-1",sizeof(char),false); // Latin 1
static const Charset ISO_8859_2(28592,"ISO-8859-2",sizeof(char),false);
static const Charset ISO_8859_3(28593,"ISO-8859-3",sizeof(char),false);
static const Charset ISO_8859_4(28594,"ISO-8859-4",sizeof(char),false);
static const Charset ISO_8859_5(28595,"ISO-8859-5",sizeof(char),false);
static const Charset ISO_8859_6(28596,"ISO-8859-6",sizeof(char),false);
static const Charset ISO_8859_7(28597,"ISO-8859-7",sizeof(char),false);
static const Charset ISO_8859_8(28598,"ISO-8859-8",sizeof(char),false);
static const Charset ISO_8859_9(28599,"ISO-8859-9",sizeof(char),false);
static const Charset ISO_8859_10(28600,"ISO-8859-10",sizeof(char),false);
static const Charset ISO_8859_11(28601,"ISO-8859-11",sizeof(char),false);
static const Charset ISO_8859_12(28602,"ISO-8859-12",sizeof(char),false);
static const Charset ISO_8859_13(28603,"ISO-8859-13",sizeof(char),false);
static const Charset ISO_8859_14(28604,"ISO-8859-14",sizeof(char),false);
static const Charset ISO_8859_15(28605,"ISO-8859-15",sizeof(char),false);
static const Charset ISO_8859_16(28606,"ISO-8859-16",sizeof(char),false);
/* Cyrillic Unix */
static const Charset KOI8_R(20866,"KOI8-R",sizeof(char),false); // Unix Russian
static const Charset KOI8_U(21866,"KOI8-U",sizeof(char),false); // Unix Ukrainian
static const Charset KOI8_RU(20866,"KOI8-RU",sizeof(char),false); // Unix Belarusian
// Windows national code pages for the alphabet based languages
static const Charset CP_1250(1250,"CP1250",sizeof(char),false); // Windows version of ISO-8859-1 + additional character
static const Charset CP_1251(1251,"CP1251",sizeof(char),false); // Windows version of ISO-8859-5 + additional character
static const Charset CP_1252(1252,"CP1252",sizeof(char),false);
static const Charset CP_1253(1253,"CP1253",sizeof(char),false);
static const Charset CP_1254(1254,"CP1254",sizeof(char),false);
static const Charset CP_1255(1255,"CP1255",sizeof(char),false);
static const Charset CP_1256(1256,"CP1256",sizeof(char),false);
static const Charset CP_1257(1257,"CP1257",sizeof(char),false);
static const Charset CP_1258(1258,"CP1258",sizeof(char),false);


Charsets::Charsets() BOOST_NOEXCEPT_OR_NOTHROW
{
	insert("ASCII",&ASCII);
	insert("UTF-8",&UTF_8);
	insert("UCS-2",&UTF_16LE);
	insert("UCS-2BE",&UTF_16BE);
	insert("UCS-2LE",&UTF_16LE);
	insert("UCS-4",&UTF_32LE);
	insert("UCS-4BE",&UTF_32BE);
	insert("UCS-4LE",&UTF_32LE);
	insert("UTF-16",&UTF_16LE);
	insert("UTF-16BE",&UTF_16BE);
	insert("UTF-16LE",&UTF_16LE);
	insert("UTF-32",&UTF_32LE);
	insert("UTF-32BE",&UTF_32BE);
	insert("UTF32-LE",&UTF_32LE);
	insert("UTF-7", &UTF_7);
	insert("CP1250",&CP_1250);
	insert("CP1251",&CP_1251);
	insert("CP1252",&CP_1252);
	insert("CP1253",&CP_1253);
	insert("CP1254",&CP_1254);
	insert("CP1255",&CP_1255);
	insert("CP1256",&CP_1256);
	insert("CP1257",&CP_1257);
	insert("CP1258",&CP_1258);
	insert("ISO-8859-1", &ISO_8859_1);
	insert("ISO-8859-2", &ISO_8859_2);
	insert("ISO-8859-3", &ISO_8859_3);
	insert("ISO-8859-4", &ISO_8859_4);
	insert("ISO-8859-5", &ISO_8859_5);
	insert("ISO-8859-6", &ISO_8859_6);
	insert("ISO-8859-7", &ISO_8859_7);
	insert("ISO-8859-8", &ISO_8859_8);
	insert("ISO-8859-9", &ISO_8859_9);
	insert("ISO-8859-10", &ISO_8859_10);
	insert("ISO-8859-11", &ISO_8859_11);
	insert("ISO-8859-12", &ISO_8859_12);
	insert("ISO-8859-13", &ISO_8859_13);
	insert("ISO-8859-14", &ISO_8859_14);
	insert("ISO-8859-15", &ISO_8859_15);
	insert("ISO-8859-16", &ISO_8859_16);
	insert("KOI8-R",&KOI8_R);
	insert("KOI8-U",&KOI8_U);
	insert("KOI8-RU",&KOI8_RU);
}

const Charset* Charsets::find(const char* name) const BOOST_NOEXCEPT_OR_NOTHROW
{
	ctmap_t::const_iterator searchRes = nameMap_.find(name);
	return searchRes == nameMap_.end() ? NULL : searchRes->second;
}

const Charset* Charsets::forName(const char* name) BOOST_NOEXCEPT_OR_NOTHROW {
		return instance()->find(name);
}

const Charset* Charsets::utf16le() BOOST_NOEXCEPT_OR_NOTHROW {
	return &UTF_16LE;
}

const Charset* Charsets::utf8() BOOST_NOEXCEPT_OR_NOTHROW {
	return &UTF_8;
}

// Converter
Converter::~Converter() BOOST_NOEXCEPT_OR_NOTHROW
{}

} // namespace io
