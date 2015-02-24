#include "prchdrs.h"
#include <channels.hpp>

namespace io {

// ReadChannel
ReadChannel::ReadChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

ReadChannel::~ReadChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

//WriteChannel
WriteChannel::WriteChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

WriteChannel::~WriteChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

//ReadWriteChannel
ReadWriteChannel::ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

ReadWriteChannel::~ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

// AsynchhReadChannel
AsynchReadChannel::AsynchReadChannel(const read_callback& callback,byte_buffer& buff) BOOST_NOEXCEPT_OR_NOTHROW:
	callback_(callback_),
	buffer_(buff)
{
}

AsynchReadChannel::~AsynchReadChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
}

} // namespace io
