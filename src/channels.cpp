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

//RandomAccessChannel
RandomAccessChannel::RandomAccessChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

RandomAccessChannel::~RandomAccessChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
}

} // namespace io
