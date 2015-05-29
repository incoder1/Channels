#include <streamreader.hpp>

#include <iostream>

namespace xml {

typedef uint8_t token_t;

#define DECLARE_TOKEN(NAME,VAL) static const token_t NAME = VAL;
DECLARE_TOKEN(LEFTB,'<');
DECLARE_TOKEN(RIGHTB,'>');
DECLARE_TOKEN(QE,'?');
DECLARE_TOKEN(QT,'"');
DECLARE_TOKEN(EQ,'=');
DECLARE_TOKEN(SPACE,' ');
DECLARE_TOKEN(TAB,'\t');
DECLARE_TOKEN(SCORE,'-');
DECLARE_TOKEN(SLASH,'/');
#undef DECLARE_TOKEN

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
	if(nextEvent_ == NULL && !hasNext() ) {
		boost::throw_exception(xml_stream_error("End of XML stream, no more data"));
	}
	Event* currentEvent = nextEvent_;
	return SEvent(currentEvent);
}

bool StreamReader::hasNext() throw(xml_stream_error)
{
	nextEvent_ = parseNext();
	return nextEvent_ != NULL;
}

Event* StreamReader::parseNext() throw(xml_stream_error)
{
	Event* result = NULL;
	io::byte_buffer tag = getFullTagDecl();
	if(!tag.empty()) {
		char* tagText = reinterpret_cast<char*>(tag.position().ptr());
		if(matches_expr(tagText, grm::start_doc_exp() ) ) {
			result = tagParsers_[START_DOCUMENT](tagText);
		} else if(matches_expr(tagText, grm::processing_instruction_exp()) ) {
			result = tagParsers_[PROCESSING_INSTRUCTION](tagText);
		}
	}
	return result;
}

io::byte_buffer StreamReader::getFullTagDecl() throw(xml_stream_error)
{
	readBuffer_.clear();
	if(!src_->end()) {
		uint8_t nextByte = src_->nextByte();
		while(nextByte != RIGHTB) {
			if(src_->end()) {
				readBuffer_.put(0);
				readBuffer_.flip();
				std::cout<<reinterpret_cast<char*>(readBuffer_.position().ptr());
				std::cout<<"Endl"<<std::endl;
				boost::throw_exception(xml_stream_error("parse error tag is unbalanced"));
			}
			nextByte = src_->nextByte();
			// make buffer grow exponential
			if(readBuffer_.full()) {
				readBuffer_ = readBuffer_.resize(readBuffer_.capacity() << 1);
			}
			readBuffer_.put(nextByte);
		}
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
