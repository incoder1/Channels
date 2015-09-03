#include "prchdrs.h"
#include <channels.hpp>

namespace io {

// ReadChannel
ReadChannel::ReadChannel()
{}

ReadChannel::~ReadChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

//WriteChannel
WriteChannel::WriteChannel()
{}

WriteChannel::~WriteChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

//ReadWriteChannel
ReadWriteChannel::ReadWriteChannel():
	ReadChannel(),
	WriteChannel()
{}

ReadWriteChannel::~ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

//RandomAccessChannel
RandomAccessChannel::RandomAccessChannel():
	ReadWriteChannel()
{}

RandomAccessChannel::~RandomAccessChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
}

// AsynchChannel
AsynchChannel::AsynchChannel(const completition_handler_f& sendHandler):
	sendHandler_(sendHandler)
{}

AsynchChannel::~AsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

void AsynchChannel::handleSend(const boost::system::error_code& code,std::size_t transfered,byte_buffer& buffer) const
{
	sendHandler_(code,transfered,buffer);
}

} // namespace io
