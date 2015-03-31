#ifndef STREAMREADER_HPP_INCUDED
#define STREAMREADER_HPP_INCUDED

#include <stdexcept>

#include <boost/core/ref.hpp>

#include "event.hpp"
#include "xmlsource.hpp"
#include "xmlgrammar.hpp"

namespace xml {

class xml_stream_error:public std::runtime_error {
public:
	xml_stream_error(const std::string& msg) BOOST_NOEXCEPT_OR_NOTHROW:
		std::runtime_error(msg)
	{}
};

typedef boost::function<Event*(char*)> tag_parser_f;

class StreamReader {
public:
	StreamReader(SSource source);
	SEvent next() throw(xml_stream_error);
	bool hasNext() throw(xml_stream_error);
	std::string getElementText() throw(xml_stream_error);
private:
	Event* parseNext() throw(xml_stream_error);
	io::byte_buffer getFullTagDecl() throw(xml_stream_error);
private:
	SSource src_;
	boost::unordered_map<EvenType,tag_parser_f> tagParsers_;
	Event* nextEvent_;
	io::byte_buffer readBuffer_;
};

} // namesapce xml

#endif // STREAMREADER_HPP_INCUDED
