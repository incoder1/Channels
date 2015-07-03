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
AsynchChannel::AsynchChannel(const copletition_handler& handler)
	handler_(handler)
{}

AsynchChannel::~AsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

BOOST_FORCEINLINE void AsynchChannel::handle(std::size_t transfered,const byte_buffer& buff) const {
	handler_(transfered,buff);
}

//AsynhDispatcher
AsynhDispatcher::AsynhDispatcher()
{}

AsynhDispatcher::~AsynhDispatcher() BOOST_NOEXCEPT_OR_NOTHROW
{}

} // namespace io
