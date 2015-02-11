#ifndef CHANNELS_HPP_INCLUDED
#define CHANNELS_HPP_INCLUDED

#include <boost/config.hpp>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

#include <channels_config.h>
#include <bytebuffer.hpp>

namespace io {

class io_exception: public std::runtime_error {
public:
	explicit io_exception(const std::string& what):
		std::runtime_error(what)
	{}
};


class CHANNEL_PUBLIC ReadChannel {

BOOST_MOVABLE_BUT_NOT_COPYABLE(ReadChannel)

protected:
	ReadChannel() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	virtual std::size_t read(byte_buffer& buffer) throw(io_exception) = 0;
	virtual ~ReadChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

typedef boost::shared_ptr<ReadChannel> SReadChannel;

class CHANNEL_PUBLIC WriteChannel {

BOOST_MOVABLE_BUT_NOT_COPYABLE(WriteChannel)

protected:
	WriteChannel() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	virtual std::size_t write(const byte_buffer& buffer) throw(io_exception) = 0;
	virtual ~WriteChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

typedef boost::shared_ptr<WriteChannel> SWriteChannel;

//typedef enum {FROM_BEGIN = 0 , FROM_CURRENT_POSITION = 1, FROM_END = 2} MoveMethod;

class CHANNEL_PUBLIC ReadWriteChannel:public virtual ReadChannel,public virtual WriteChannel {
BOOST_MOVABLE_BUT_NOT_COPYABLE(ReadWriteChannel)
protected:
	ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	virtual uint64_t position() = 0;
	virtual uint64_t forward(uint64_t offset) throw (io_exception) = 0;
	virtual uint64_t backward(uint64_t offset) throw (io_exception) = 0;
	virtual uint64_t fromBegin(uint64_t offset) throw (io_exception) = 0;
	virtual uint64_t fromEnd(uint64_t offset) throw (io_exception) = 0;
	virtual ~ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

typedef boost::shared_ptr<ReadWriteChannel> SReadWriteChannel;

/// Transfers data from source read channel to the destination channel using memory buffer
/// \param src source channel
/// \param dst destination buffer
/// \param buffer memory block for copying date
inline void transfer(const SReadChannel& src, const SWriteChannel& dst, const byte_buffer& buffer) {
	byte_buffer buff(buffer);
	while(src->read(buff) > 0) {
		buff.flip();
		dst->write(buff);
		buff.clear();
	}
}



} // namespace io

#endif // CHANNELS_HPP_INCLUDED
