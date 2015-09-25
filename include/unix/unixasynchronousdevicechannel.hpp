#ifndef __UNIX_ASYNCHRONOUS_DEVICECHANNEL_HPP_INCLUDED__
#define __UNIX_ASYNCHRONOUS_DEVICECHANNEL_HPP_INCLUDED__

#include "asynchronous.hpp"

namespace io {

class UnixAsynchronousDeviceChannel:public AsyncChannel
{
    public:
        explicit UnixAsynchronousDeviceChannel(boost::asio::io_service& ios,int fd);
        virtual ~UnixAsynchronousDeviceChannel() BOOST_NOEXCEPT_OR_NOTHROW;
        virtual void send(std::size_t position,byte_buffer& buffer,const handler_f& handler);
		virtual void receive(std::size_t position,std::size_t maxSize,const handler_f& handler);
    private:
        boost::asio::io_service& ios_;
        int fd_;
};

} // namespace io

#endif // __UNIX_ASYNCHRONOUS_DEVICECHANNEL_HPP_INCLUDED__
