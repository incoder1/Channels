#include "network.hpp"


namespace io {

	namespace network {

		NetworkChannel::NetworkChannel():
			ReadChannel(), WriteChannel()
		{}

		NetworkChannel::~NetworkChannel()
		{
		}

		boost::asio::mutable_buffers_1 asio_buffer(byte_buffer& data) {
		  return  boost::asio::mutable_buffers_1(boost::asio::mutable_buffer(&data.position(), data.capacity()));
		}

        boost::asio::const_buffers_1 asio_buffer(const byte_buffer& data) {
        	return boost::asio::const_buffers_1(boost::asio::const_buffer(&data.position(), data.length()));
        }


	} // namespace network
} // namespace io
