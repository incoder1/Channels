#ifndef CHANNELS_NETWORK_HPP_INCLUDED
#define CHANNELS_NETWORK_HPP_INCLUDED

#include "smallobject.hpp"
#include "channels.hpp"

#if  defined(__MINGW32__) && !defined(__MINGW64__)
#	include <winsock2.h>
extern "C" BOOL WINAPI UnregisterWaitEx(HANDLE,HANDLE);
#endif // Mingw32

#include <boost/asio.hpp>

namespace io {

namespace net {

	  template<class Socket>
	  class basic_socket_channel:public ReadWriteChannel {
		public:
			basic_socket_channel(boost::shared_ptr<Socket> socket) BOOST_NOEXCEPT_OR_NOTHROW:
				socket_(socket)
			{}
			virtual std::size_t read(byte_buffer& buffer) {
				boost::system::error_code error;
				size_t result = socket_->read_some(asio_buffer(buffer), boost::ref(error));
				if( (error != boost::asio::error::eof) && error) {
					boost::throw_exception(io_exception("Read socket error "+error.message()));
				} else if(error == boost::asio::error::eof) {
					return 0;
				}
				buffer.move(result);
				return result;
			}
			virtual std::size_t write(const byte_buffer& buffer)  {
				boost::system::error_code error;
				std::size_t result = socket_->write_some(asio_buffer(buffer), boost::ref(error));
				if(error) {
					boost::throw_exception(io_exception("Write socket error "+error.message()));
				}
				return result;
			}
			virtual ~basic_socket_channel() BOOST_NOEXCEPT_OR_NOTHROW {
				socket_->shutdown(Socket::shutdown_receive);
				socket_->close();
			}
		protected:
			inline boost::asio::mutable_buffers_1 asio_buffer(byte_buffer& data) {
				return  boost::asio::mutable_buffers_1(boost::asio::mutable_buffer(data.position().ptr(), data.capacity()));
			}
			inline boost::asio::const_buffers_1 asio_buffer(const byte_buffer& data) {
				return boost::asio::const_buffers_1(boost::asio::const_buffer(data.position().ptr(), data.length()));
			}
		private:
			boost::shared_ptr<Socket> socket_;
	  };

	typedef net::basic_socket_channel<
		boost::asio::basic_stream_socket<boost::asio::ip::tcp>
	> TCPSocketChannel;

	typedef net::basic_socket_channel<
		boost::asio::basic_datagram_socket<boost::asio::ip::udp>
	> UDPSocketChannel;

	DECLARE_PTR_T(TCPSocketChannel);
	DECLARE_PTR_T(UDPSocketChannel);

} // namespace net

} // namespace io
#endif // CHANNELS_NETWORK_HPP_INCLUDED
