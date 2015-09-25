#include "prchdrs.h"
#include "unixconsole.hpp"
#include "helpers.hpp"

namespace io {


// ConsoleChannel
ConsoleChannel::ConsoleChannel(int fd):
	fd_(fd)
{
}

ConsoleChannel::~ConsoleChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
}

std::size_t ConsoleChannel::read(byte_buffer& buffer)
{
	ssize_t result = ::read(fd_, vpos(buffer), buffer.capacity());
	validate_io(result, "Read console error");
	buffer.move(static_cast<std::size_t>(result));
	return static_cast<std::size_t>(result);
}

std::size_t ConsoleChannel::write(const byte_buffer& buffer)
{
	ssize_t result = ::write(fd_, vpos(buffer), buffer.length());
	validate_io(result, "Write file error");
	return result;
}

} // namesapce io
