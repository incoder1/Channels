#ifndef CHANNELS_HPP_INCLUDED
#define CHANNELS_HPP_INCLUDED

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


class CHANNEL_PUBLIC ReadChannel:private boost::noncopyable {
protected:
	ReadChannel()
	{}
public:
	virtual std::size_t read(byte_buffer& buffer) throw(io_exception) = 0;
	virtual ~ReadChannel()
	{}
};

typedef boost::shared_ptr<ReadChannel> SReadChannel;

class CHANNEL_PUBLIC WriteChannel:private boost::noncopyable {
protected:
	WriteChannel()
	{}
public:
	virtual std::size_t write(const byte_buffer& buffer) throw(io_exception) = 0;
	virtual ~WriteChannel()
	{}
};

typedef boost::shared_ptr<WriteChannel> SWriteChannel;

class CHANNEL_PUBLIC ReadWriteChannel:public virtual ReadChannel,public virtual WriteChannel {
public:
	enum MoveMethod {FROM_BEGIN = 0 , FROM_CURRENT_POSITION = 1, FROM_END = 2};
private:
	ReadWriteChannel(const ReadWriteChannel&) {
	}
	const ReadWriteChannel& operator=(const ReadWriteChannel&) {
		return *this;
	}
protected:
	ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW:
		ReadChannel(),
		WriteChannel()
	{}
public:
	/**
	 * Moves read/write pointer
	 * \param offset offset in bytes
	 * \param method for moving
	 */
	virtual void seek(std::size_t offset, MoveMethod method) throw(io_exception) = 0;
	virtual ~ReadWriteChannel()
	{}
};

typedef  boost::shared_ptr<ReadWriteChannel> SReadWriteChannel;


} // namespace io

#endif // CHANNELS_HPP_INCLUDED
