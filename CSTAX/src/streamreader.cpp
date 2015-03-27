#include <streamreader.hpp>

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

StreamReader::StreamReader(SSource xmlSource) BOOST_NOEXCEPT_OR_NOTHROW:
	 src_(xmlSource)
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
	std::string decl = getFullTagDecl();
	if( matchesExpr( decl, grm::start_doc_exp() ) ) {
		return parseStartDocument(decl);
	}
	return SEvent((Event*)NULL);
}

bool StreamReader::matchesExpr(const std::string& spec,const grm::regex_t& exp)
{
	return rxp::regex_match(spec.begin(),spec.end(),exp);
}

std::string StreamReader::getFullTagDecl() throw(xml_stream_error) {
	std::string result("<");
	uint8_t nextByte = src_->nextByte();
	while(nextByte != RIGHTB) {
		if(!src_->hasNext()) {
			boost::throw_exception(xml_stream_error("invalid xml structure"));
		} else {
			result.push_back(static_cast<char>(nextByte));
		}
		nextByte = src_->nextByte();
	}
	result.push_back('>');
	return result;
}

bool StreamReader::hasNext() throw(xml_stream_error)
{
	bool hasNextByte = src_->hasNext();
	uint8_t nextByte = 0;
	while(hasNextByte) {
		 nextByte = src_->nextByte();
		 if(LEFTB == nextByte) {
			return true;
		 }
	}
	return hasNextByte;
}


SEvent StreamReader::parseStartDocument(const std::string& prologText) {
	std::string version = extract_doc_attr_val(prologText,grm::doc_version_exp());
	std::string encoding = extract_doc_attr_val(prologText,grm::doc_encoding_exp());
	std::string stanaloneV = extract_doc_attr_val(prologText,grm::doc_stendalone_exp());
	bool stanalone = !stanaloneV.empty() && rxp::regex_match(stanaloneV,grm::true_exp());
	SDocumentEvent ev = boost::make_shared<DocumentEvent>(version,encoding,stanalone);
	return ev->shared_from_this();
}


std::string getElementText() throw(xml_stream_error)
{
	return "";
}

}
