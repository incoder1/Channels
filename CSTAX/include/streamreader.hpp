#ifndef STREAMREADER_HPP_INCUDED
#define STREAMREADER_HPP_INCUDED

#include <stdexcept>
#include <vector>

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

typedef boost::function<Event*(char*)> predicate;

class StreamReader {
public:
	StreamReader(SSource source);
	SEvent next();
	bool hasNext();
	std::string getElementText();
private:
	Event* parseNext();
	io::byte_buffer getFullTagDecl();
private:
	SSource src_;
	std::vector<predicate> predicates_;
	Event* nextEvent_;
	io::byte_buffer readBuffer_;
};

} // namesapce xml

#endif // STREAMREADER_HPP_INCUDED
