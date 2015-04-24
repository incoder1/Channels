#include "prchdrs.hpp"
#include "unixconsole.hpp"

namespace io {

// ConsoleChannel
ConsoleChannel::ConsoleChannel(int fd):
	ReadWriteChannel(),
	object(),
	fd_(fd)
{
}

ConsoleCahannel::~ConsoleCahannel() BOOST_NOEXCEPT_OR_NOTHROW
{
}

std::size_t ConsoleCahannel::read(byte_buffer& buffer)
{
	::ssize_t result = ::read(fd_, vpos(buffer), buffer.capacity());
	validate_io(result, "Read console error");
	buffer.move(static_cast<std::size_t>(result));
	return static_cast<std::size_t>(result);
}

std::size_t ConsoleCahannel::write(const byte_buffer& buffer)
{
	ssize_t result = ::write(fd_, vpos(buffer), buffer.length());
	validate_io(result, "Write file error");
	return result;
}

} // namesapce io
