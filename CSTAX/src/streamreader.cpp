#include <streamreader.hpp>

#include <iostream>

namespace xml {

namespace rxp = boost::xpressive;

static inline bool matches_expr(char* spec,const grm::regex_t& exp)
{
	return rxp::regex_match(spec,exp);
}

static std::string extract_attr_val(char* src, const grm::regex_t& exp) BOOST_NOEXCEPT_OR_NOTHROW {
	rxp::cmatch matcher;
	std::string result;
	if(rxp::regex_search(src, matcher, exp, rxp::regex_constants::format_first_only)) {
		result = matcher[2].str();
	}
	return result;
}

Event* start_document(char* tag)
{
	Event* result = NULL;
	if(matches_expr(tag,grm::start_doc_exp())) {
		std::string version = extract_attr_val(tag,grm::doc_version_exp());
		std::string encoding = extract_attr_val(tag,grm::doc_encoding_exp());
		std::string stanaloneV = extract_attr_val(tag,grm::doc_stendalone_exp());
		bool stanalone = (!stanaloneV.empty()) && rxp::regex_match(stanaloneV.data(),grm::true_exp());
		result = new DocumentEvent(version,encoding,stanalone);
	}
	return result;
}

Event* processing_instruction(char* tag)
{
	Event* result = NULL;
	if(matches_expr(tag,grm::processing_instruction_exp())) {
		std::string href = extract_attr_val(tag,grm::instr_href_exp());
		std::string type = extract_attr_val(tag, grm::instr_type_exp());
		result = new ProcessingInstructionEvent(type,href);
	}
	return result;
}

//StreamReader

StreamReader::StreamReader(SSource xmlSource):
	src_(xmlSource),
	predicates_(),
	nextEvent_(NULL),
	readBuffer_(io::byte_buffer::heap_buffer(128))
{
	predicates_.push_back(predicate(start_document));
	predicates_.push_back(predicate(processing_instruction));
}

SEvent StreamReader::next()
{
	if(nextEvent_ == NULL && !hasNext() ) {
		boost::throw_exception(xml_stream_error("End of XML stream, no more data"));
	}
	Event* currentEvent = nextEvent_;
	return SEvent(currentEvent);
}

bool StreamReader::hasNext()
{
	nextEvent_ = parseNext();
	return nextEvent_ != NULL;
}

Event* StreamReader::parseNext()
{
	Event* result = NULL;
	io::byte_buffer tag = getFullTagDecl();
	if(!tag.empty()) {
		std::vector<predicate>::iterator it = predicates_.begin();
		while( (NULL == result) && it != predicates_.end()) {
			char* def = reinterpret_cast<char*>(tag.position().ptr());
			result = (*(it++))(def);
		}
	}
	return result;
}

io::byte_buffer StreamReader::getFullTagDecl()
{
	readBuffer_.clear();
	uint8_t nextByte;
	do {
		if(!src_->hasNext()) {
			boost::throw_exception(xml_stream_error("Parse error. Tag is unbalanced"));
		}
		nextByte = src_->nextByte();
		if(readBuffer_.full()) {
			readBuffer_ = readBuffer_.resize(readBuffer_.capacity() << 1);
		}
		readBuffer_.put(nextByte);
	} while(nextByte != '>');
	readBuffer_.put(0);
	readBuffer_.flip();
	return readBuffer_;
}

}
