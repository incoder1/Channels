#include "xmlsource.hpp"

namespace xml {

Source::Source(io::PReadChannel source, const io::byte_buffer& readBuffer) BOOST_NOEXCEPT_OR_NOTHROW:
	src_(source),
	readBuff_(readBuffer)
{}


Source::reader Source::newReader() throw(std::runtime_error)
{
	return reader(src_, readBuff_, detectInputCharset());
}

Source::~Source() BOOST_NOEXCEPT_OR_NOTHROW
{
}

}
