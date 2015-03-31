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

static std::string extract_attr_val(char* src, const grm::regex_t& exp) BOOST_NOEXCEPT_OR_NOTHROW {
	rxp::cmatch matcher;
	std::string result;
	if(rxp::regex_search(src, matcher, exp, rxp::regex_constants::format_first_only)) {
		result = matcher[2].str();
	}
	return result;
}

static inline bool matches_expr(char* spec,const grm::regex_t& exp)
{
	return rxp::regex_match(spec,exp);
}


Event* parse_start_document(char* prologText)
{
	std::string version = extract_attr_val(prologText,grm::doc_version_exp());
	std::string encoding = extract_attr_val(prologText,grm::doc_encoding_exp());
	std::string stanaloneV = extract_attr_val(prologText,grm::doc_stendalone_exp());
	bool stanalone = (!stanaloneV.empty()) && rxp::regex_match(stanaloneV.data(),grm::true_exp());
	return new DocumentEvent(version,encoding,stanalone);
}

Event* parse_processing_instruction(char* text)
{
	std::string href = extract_attr_val(text,grm::instr_href_exp());
	std::string type = extract_attr_val(text, grm::instr_type_exp());
	return new ProcessingInstructionEvent(type,href);
}


//StreamReader

StreamReader::StreamReader(SSource xmlSource):
	src_(xmlSource),
	tagParsers_(),
	nextEvent_(NULL),
	readBuffer_(io::byte_buffer::heap_buffer(128))
{
	tagParsers_.insert(std::make_pair(START_DOCUMENT,parse_start_document));
	tagParsers_.insert(std::make_pair(PROCESSING_INSTRUCTION, parse_processing_instruction));
}

SEvent StreamReader::next() throw(xml_stream_error)
{
	Event* currentEvent = NULL;
	if(!nextEvent_) {
		if(!hasNext()) {
			boost::throw_exception(xml_stream_error("end of XML stream reached"));
		}
	}
	std::swap(currentEvent,nextEvent_);
	return SEvent(currentEvent);
}

bool StreamReader::hasNext() throw(xml_stream_error) {
	nextEvent_ = parseNext();
	return nextEvent_ != NULL;
}

Event* StreamReader::parseNext() throw(xml_stream_error)
{
	Event* result = NULL;
	io::byte_buffer tag = getFullTagDecl();
	char* tagText = reinterpret_cast<char*>(tag.position().ptr());
	if(matches_expr(tagText, grm::start_doc_exp() ) ) {
		result = tagParsers_[START_DOCUMENT](tagText);
	} else if(matches_expr(tagText, grm::processing_instruction_exp()) ) {
		result = tagParsers_[PROCESSING_INSTRUCTION](tagText);
	}
	return result;
}

io::byte_buffer StreamReader::getFullTagDecl() throw(xml_stream_error)
{
	readBuffer_.clear();
	if(src_->hasNext()) {
		uint8_t nextByte;
		do {
			nextByte = src_->nextByte();
			if(!src_->hasNext()) {
				boost::throw_exception(xml_stream_error("invalid xml structure"));
			} else {
				// make buffer grow exponential
				if(readBuffer_.full()) {
					readBuffer_ = readBuffer_.resize(readBuffer_.capacity() << 1);
				}
				readBuffer_.put(nextByte);
			}
		} while(nextByte != RIGHTB);
	}
	readBuffer_.put(0);
	readBuffer_.flip();
	return readBuffer_;
}

std::string getElementText() throw(xml_stream_error)
{
	return "";
}

}
