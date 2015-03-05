#include <streamreader.hpp>
#include "xmlgrammar.hpp"

#include <iostream>

namespace xml {

const uint8_t LEFTB = '<';
const uint8_t RIGHTB = '>';
const uint8_t QE = '?';
const uint8_t QT = '"';
const uint8_t EQ = '=';
const uint8_t SPACE = ' ';
const uint8_t TAB = '\t';
const uint8_t SCORE = '-';
const uint8_t SLASH = '/';

namespace rxp = boost::xpressive;

static std::string extract_doc_attr_val(const std::string &src, const rxp::sregex& exp) BOOST_NOEXCEPT_OR_NOTHROW {
	rxp::smatch matcher;
	std::string result;
	if(rxp::regex_search(src.begin(), src.end(), matcher, exp, rxp::regex_constants::format_first_only)) {
		result.append(matcher[2]);
	}
	return result;
}

//StreamReader

StreamReader::StreamReader(const Source& source) BOOST_NOEXCEPT_OR_NOTHROW:
	 src_(source)
{}

SEvent StreamReader::next() throw(xml_stream_error)
{
	return parseNext();
}

SEvent StreamReader::nextTag() throw(xml_stream_error)
{
	return SEvent((Event*)NULL);
}

SEvent StreamReader::parseNext() throw(xml_stream_error) {
	if( !src_.hasNext() ) {
		boost::throw_exception(xml_stream_error("invalid xml structure"));
	}
	uint8_t ch = src_.nextByte();
	switch(ch) {
	case QE:
		return parseStartDocument();
	default:
		return SEvent(NULL);
	}
}

bool StreamReader::hasNext() throw(xml_stream_error)
{
	bool hasNextByte = src_.hasNext();
	uint8_t nextByte = 0;
	while(hasNextByte) {
		 nextByte = src_.nextByte();
		 if(LEFTB == nextByte) {
			return true;
		 }
	}
	return hasNextByte;
}

SEvent StreamReader::parseStartDocument() {
	std::string spec;
	uint8_t nextByte = src_.nextByte();
	while(nextByte != QE) {
		if(!src_.hasNext()) {
			boost::throw_exception(xml_stream_error("invalid xml structure"));
		} else {
			spec.push_back(static_cast<char>(nextByte));
		}
		nextByte = src_.nextByte();
	}
	if(rxp::regex_match(spec.begin(),spec.end(),grm::xml_document_exp())) {
		std::string version = extract_doc_attr_val(spec,grm::doc_ver_exp());
		std::string encoding = extract_doc_attr_val(spec,grm::doc_encd_exp());
		std::string stanaloneV = extract_doc_attr_val(spec,grm::doc_stendalone_exp());
		bool stanalone = !stanaloneV.empty() && rxp::regex_match(stanaloneV,grm::true_exp());
		return boost::shared_ptr<DocumentEvent>(new DocumentEvent(version,encoding,stanalone));
	}
	return SEvent((Event*)NULL);
}


std::string getElementText() throw(xml_stream_error)
{
	return "";
}

}
