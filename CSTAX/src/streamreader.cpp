#include "streamreader.hpp"

#include <iostream>

namespace xml {

static const uint8_t LEFTB = '<';
static const uint8_t RIGHTB = '>';
static const uint8_t QE = '?';
static const uint8_t QT = '"';
static const uint8_t EQ = '=';
static const uint8_t SPACE = ' ';
static const uint8_t TAB = '\t';
static const uint8_t SCORE = '-';


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
	std::string version(spec.begin(),spec.end());
	return boost::shared_ptr<DocumentEvent>(new DocumentEvent(version,""));
}

std::string getElementText() throw(xml_stream_error)
{
	return "";
}

}
