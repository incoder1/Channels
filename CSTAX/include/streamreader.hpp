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

class StreamReader {
public:
	StreamReader(SSource source) BOOST_NOEXCEPT_OR_NOTHROW;
	SEvent next() throw(xml_stream_error);
	SEvent nextTag() throw(xml_stream_error);
	bool hasNext() throw(xml_stream_error);
	std::string getElementText() throw(xml_stream_error);
private:
	SEvent parseNext() throw(xml_stream_error);
	bool matchesExpr(const std::string& spec,const grm::regex_t& exp);
	std::string getFullTagDecl() throw(xml_stream_error);
	SEvent parseStartDocument(const std::string& prologText);
private:
	SSource src_;
};

} // namesapce xml

#endif // STREAMREADER_HPP_INCUDED
