#ifndef __WIN_ASYNCHRONOUS_HANDLE_CHANNEL_HPP_INCLUDED__
#define __WIN_ASYNCHRONOUS_HANDLE_CHANNEL_HPP_INCLUDED__

#include "asynchronous.hpp"


namespace io {

	class CHANNEL_PUBLIC WinAsynchronousHandleChannel:public AsyncChannel {
	public:
		WinAsynchronousHandleChannel(boost::asio::io_service& ios,::HANDLE hDevice);
		~WinAsynchronousHandleChannel() BOOST_NOEXCEPT_OR_NOTHROW;
		virtual void send(std::size_t position,byte_buffer& buffer,const handler_f& handler);
		virtual void receive(std::size_t position,std::size_t maxSize,const handler_f& handler);
	private:
		inline void seek(std::size_t pos,::LPOVERLAPPED overlapped);
	private:
		boost::asio::windows::random_access_handle hDevice_;
		boost::asio::io_service& ios_;
	};

} // namesapce io


#endif // __WIN_ASYNCHRONOUS_HANDLE_CHANNEL_HPP_INCLUDED__
