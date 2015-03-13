#ifndef CHANNELS_HPP_INCLUDED
#define CHANNELS_HPP_INCLUDED

#include <channels_config.h>

#include <stdexcept>

#include <boost/config.hpp>
#include <boost/system/error_code.hpp>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/move/move.hpp>

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
	virtual std::size_t position() = 0;
	virtual std::size_t forward(std::size_t offset) throw (io_exception) = 0;
	virtual std::size_t backward(std::size_t offset) throw (io_exception) = 0;
	virtual std::size_t fromBegin(std::size_t offset) throw (io_exception) = 0;
	virtual std::size_t fromEnd(std::size_t offset) throw (io_exception) = 0;
	virtual ~ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

typedef boost::shared_ptr<ReadWriteChannel> SReadWriteChannel;

class CHANNEL_PUBLIC AsynchReadChannel {
BOOST_MOVABLE_BUT_NOT_COPYABLE(AsynchReadChannel)
public:
	typedef boost::function<void(boost::system::error_code&,std::size_t,const byte_buffer)> read_callback;
	AsynchReadChannel(const read_callback& callback,byte_buffer& buff) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~AsynchReadChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
protected:
	inline byte_buffer buffer() const {
		return buffer_;
	}
private:
	read_callback callback_;
	byte_buffer buffer_;
};


/// Transfers data from source read channel to the destination channel using memory buffer
/// \param src source channel
/// \param dst destination buffer
/// \param buffer memory block for copying date
inline void transfer(const SReadChannel& src,const SWriteChannel& dst, byte_buffer& buffer) {
	while(src->read(buffer) > 0) {
		buffer.flip();
		dst->write(buffer);
		buffer.clear();
	}
}

} // namespace io

#endif // CHANNELS_HPP_INCLUDED
