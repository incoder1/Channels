#ifndef __ASYNCHRONOUS_HPP_INCLUDED__
#define __ASYNCHRONOUS_HPP_INCLUDED__

#include "errors.hpp"
#include "bytebuffer.hpp"

#include <boost/function.hpp>
#include <boost/asio.hpp>

namespace io {

	class AsynchronousChannel;

	class AsynchronousCompletitionHandler
	{
	protected:
		const AsynchronousChannel* channel();
	public:
		virtual void handleSend(std::size_t transfered,const byte_buffer& buffer) = 0;
		virtual void handleFailure(boost::system::error_code& err) = 0;
		virtual ~AsynchronousCompletitionHandler() BOOST_NOEXCEPT_OR_NOTHROW = 0;
	};


	class CHANNEL_PUBLIC AsynchronousChannel {
		BOOST_MOVABLE_BUT_NOT_COPYABLE(AsynchronousChannel)
	protected:
		AsynchronousChannel();
	public:
		virtual void send(byte_buffer& buffer) const BOOST_NOEXCEPT_OR_NOTHROW  = 0;
		virtual ~AsynchronousChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
	};

	class CHANNEL_PUBLIC WinAsynchronousHandleChannel:public virtual AsynchronousChannel {
	public:
		WinAsynchronousHandleChannel(boost::asio::io_service& ios,::HANDLE hDevice);
		~WinAsynchronousHandleChannel() BOOST_NOEXCEPT_OR_NOTHROW;
		virtual void send(byte_buffer& buffer) const BOOST_NOEXCEPT_OR_NOTHROW;
	};

} // namesapce io

#endif // __ASYNCHRONOUS_HPP_INCLUDED__
