#include "prchdrs.h"
#include "unixasynchronousdevicechannel.hpp"

namespace io {

UnixAsynchronousDeviceChannel::UnixAsynchronousDeviceChannel(boost::asio::io_service& ios, int fd):
    ios_(ios),
    fd_(fd)
{}

void UnixAsynchronousDeviceChannel::send(std::size_t position,byte_buffer& buffer,const handler_f& handler)
{
}

void UnixAsynchronousDeviceChannel::receive(std::size_t position,std::size_t maxSize,const handler_f& handler)
{
}

UnixAsynchronousDeviceChannel::~UnixAsynchronousDeviceChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
    ::close(fd_);
}

} // namesapce io
