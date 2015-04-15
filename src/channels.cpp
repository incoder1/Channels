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

// AsyncResult
AsyncResult::AsyncResult()
{}

AsyncResult::~AsyncResult() BOOST_NOEXCEPT_OR_NOTHROW
{}

// AsynchReadChannel
AsynchReadChannel::AsynchReadChannel()
{
}

AsynchReadChannel::~AsynchReadChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

// AsynchWriteChannel
AsynchWriteChannel::AsynchWriteChannel()
{}

AsynchWriteChannel::~AsynchWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
}

// AssynchReadWriteChannel
AsynchReadWriteChannel::AsynchReadWriteChannel():
	AsynchReadChannel(),
	AsynchWriteChannel()
{
}

AsynchReadWriteChannel::~AsynchReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
}

} // namespace io
