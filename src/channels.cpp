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

//AsynchChannel
AsynchChannel::AsynchChannel(const completion_handler_f& recvHnd,const completion_handler_f& sendHnd):
	recvHandler_(recvHnd),
	sendHandler_(sendHnd)
{}

AsynchChannel::~AsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

void AsynchChannel::handleReceive(std::size_t transfered,const byte_buffer& buff) const
{
	recvHandler_(transfered,buff);
}

void AsynchChannel::handleSend(std::size_t transfered,const byte_buffer& buff) const
{
	sendHandler_(transfered,buff);
}

//AsynhDispatcher
AsynhDispatcher::AsynhDispatcher()
{}

AsynhDispatcher::~AsynhDispatcher() BOOST_NOEXCEPT_OR_NOTHROW
{}

} // namespace io
