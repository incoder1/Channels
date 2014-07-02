#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <channels.hpp>

#ifdef _WIN32
#	include "winver.h"
#endif // _WIN32

#ifdef __MINGW32__
extern "C" BOOL WINAPI UnregisterWaitEx(HANDLE,HANDLE);
#endif // Mingw32

#include <boost/asio.hpp>

namespace io {

	namespace network {

	class NetworkChannel:public virtual ReadChannel, public virtual WriteChannel
	{
	#ifdef BOOST_NO_DELETED_FUNCTIONS
	private:  // emphasize the following members are private
		ReadWriteChannel( const ReadChannel&);
		ReadWriteChannel& operator=( const ReadChannel& );
	#else
		ReadWriteChannel( const ReadWriteChannel& ) = delete;
		ReadWriteChannel& operator=( const ReadWriteChannel& ) = delete;
	#endif
	public:
		NetworkChannel();
		virtual ~NetworkChannel() = 0;
	};

	boost::asio::mutable_buffers_1 asio_buffer(byte_buffer& data);
	boost::asio::const_buffers_1 asio_buffer(const byte_buffer& data);

	template<class Socket>
	class basic_socket_channel:public NetworkChannel {
	private:
		boost::shared_ptr<Socket> socket_;
	public:
		basic_socket_channel(boost::shared_ptr<Socket> socket) BOOST_NOEXCEPT_OR_NOTHROW:
			NetworkChannel(),
			socket_(socket)
		{}
		virtual ~basic_socket_channel()
		{}
		virtual size_t read(byte_buffer& buffer) throw(io_exception)
		{
			boost::system::error_code error;
			size_t result = socket_->read_some(asio_buffer(buffer), error);
			if( (error != boost::asio::error::eof) && error ) {
				boost::throw_exception(io_exception(std::string("Read socket exception")+error.message()));
			}
			buffer.move(result);
			return result;
		}
		virtual size_t write(const byte_buffer& buffer) throw(io_exception) {
			boost::system::error_code error;
			try {
				return socket_->write_some(asio_buffer(buffer));
			} catch(std::exception& exc) {
				throw io_exception(exc.what());
			}
		}
	};

	typedef basic_socket_channel<
		boost::asio::basic_stream_socket<boost::asio::ip::tcp>
		> TCPSocketChannel;

	} // namespace network

} // namespace io

#endif // NETWORK_HPP_INCLUDED
