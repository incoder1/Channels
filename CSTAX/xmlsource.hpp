#ifndef XMLSOURCE_HPP_INCLUDED
#define XMLSOURCE_HPP_INCLUDED

#include <readwrite.hpp>


namespace xml {

class Source
{
	private:
		io::PReadChannel src_;
		io::byte_buffer readBuff_;
		io::PConverter detectInputCharset() throw(std::runtime_error);
	public:
		// Parser uses UTF-8, i.e. 1 byte character strings.
		// Because XML syntax uses only Latin1 it is Ok.
		typedef io::Reader<std::string> reader;
		explicit Source(io::PReadChannel source, const io::byte_buffer& readBuffer) BOOST_NOEXCEPT_OR_NOTHROW;
		reader newReader() throw(std::runtime_error);
		virtual ~Source() BOOST_NOEXCEPT_OR_NOTHROW;
	private:
};

} // namespace xml

#endif // XMLSOURCE_HPP_INCLUDED
