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
ReadWriteChannel::ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW:
	ReadChannel(),
	WriteChannel()
{}

ReadWriteChannel::~ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

// AsynchhReadChannel
AsynchReadChannel::AsynchReadChannel(const read_callback& callback) BOOST_NOEXCEPT_OR_NOTHROW:
	object(),
	callback_(callback)
{
}


void AsynchReadChannel::handleRead(boost::system::error_code& err, std::size_t read,const byte_buffer& data) const {
	callback_(err,read,data);
}

AsynchReadChannel::~AsynchReadChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
}

} // namespace io
