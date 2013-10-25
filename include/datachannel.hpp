#ifndef DATACHANNEL_HPP_INCLUDED
#define DATACHANNEL_HPP_INCLUDED

#include <channels.hpp>

namespace io {

class CHANNEL_PUBLIC DataChannel: public ReadWriteChannel {
private:
	byte_buffer data_;
public:
	DataChannel(void *data, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~DataChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t read(byte_buffer& buffer) throw(io_exception);
	virtual std::size_t write(const byte_buffer& buffer) throw(io_exception);
	virtual void seek(std::size_t offset, MoveMethod method) throw(io_exception);
};

} // namespace io

#endif // DATACHANNEL_HPP_INCLUDED
