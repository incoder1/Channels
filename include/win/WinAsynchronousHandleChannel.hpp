#ifndef __WIN_ASYNCHRONOUS_HANDLE_CHANNEL_HPP_INCLUDED__
#define __WIN_ASYNCHRONOUS_HANDLE_CHANNEL_HPP_INCLUDED__

#include "asynchronous.hpp"

namespace io {

	class CHANNEL_PUBLIC WinAsynchronousHandleChannel:public AsynchronousChannel {
	public:
		WinAsynchronousHandleChannel(boost::asio::io_service& ios, ::HANDLE hDevice);
		~WinAsynchronousHandleChannel() BOOST_NOEXCEPT_OR_NOTHROW;
		virtual void send(std::size_t position,byte_buffer& buffer,const asynh_handler_f& handler) BOOST_NOEXCEPT_OR_NOTHROW;
	private:
		boost::asio::io_service& ios_;
		::HANDLE hDevice_;
	};

} // namesapce io


#endif // __WIN_ASYNCHRONOUS_HANDLE_CHANNEL_HPP_INCLUDED__
